/*****************************************************************************
 * Module    : Task
 * File      : task_usbdev.c
 * Author    : Hanny
 * Function  : 读卡器任务
 *****************************************************************************/
#include "include.h"

#if TASK_USBDEV_EN
#include "task.h"
#include "task_usbdev.h"

void task_usbdev_vol_down(void);
void task_usbdev_vol_up(void);
type_task_usbdev t_usbdev;
extern u8 usb_audiobuf[194];
#include SET_USER_PATH(CFG_UI_USBDEV_DIR, /ui/ui_usbdev.c)

//判断播放状态，0:暂停  1:播放
#pragma location="TASK_USBDEV_SEG"
u8 task_hid_status(void)
{
    u8 ret = 0;
    static u8 cnt = 0,mute_flag = 0;
    static u32 u_pow = 0; //buf能量检测

    if(cnt < 10){
        if(u_pow == voice_power((u8*)usb_audiobuf, 97)){
            mute_flag++;
        }
        u_pow = voice_power((u8*)usb_audiobuf, 97);//更新能量值
        cnt++;
    }else{
        cnt = 0;
        if(mute_flag > 8){ //检测到能量大于8次没有变化=暂停，mute住系统
            if(!sys_ctl.mute_flag){
                sys_mute();
            }
            ret = 0;
        }else{  //播放状态，系统可能被mute住，解mute
            if(sys_ctl.mute_flag){
                sys_unmute();
            }
                ret = 1;
        }
        mute_flag = 0;
    }
    return ret; //cnt = 10 才会返回正确的值
}

//任务初始化
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_enter(void)
{
    led_mode_off();
    led_idle();
    set_sys_clk(SYS_48M);
    set_lcd_buf(LCD_FULL_MODE);
    usb_dev_flag = 0;
    if (!device_activate(DEVICE_PC)) {
        task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
        msgbox(STR_DISCONNECTED_PC, NULL, MSGBOX_TIME);
        if(task_ctl.work_sta != TASK_USBDEV)
        {
            return;
        }
    }
#if BT_ALL_TASK
    bt_close();
#endif
    usb_dev_flag = 1;
    draw_clear();
    task_ctl.work_sta = TASK_USBDEV;
    dac_enable();
#if WARNING_TASK_USBDEV
    mp3_res_play_wait(RES_MP3_PC_MODE);
#endif
    //加载USB CardReader程序
    load_code(0x40, 3);
    memset(adc_buf, 0, USBMIC_SAMPLES*2);
    pcm_init(S_SPR_48000);
    AUCON10 |= BIT(7);
    FIFO_SPEED = 0;
    adc_init(SEL_USBMIC);
    usb_audio_set_volume(sys_ctl.volume);       //设置默认音量
    if (!usb_dev_init(USBDEV_ENUM_TYPE)) {
        task_ctl.work_sta = TASK_EXIT;      //设备初始化失败，返回主菜单
        msgbox(STR_USB_DEVICE_ERROR, NULL, MSGBOX_TIME);
        return;
    }
#if MIC_DIRECT_OUT_EN
    mic_direct_out_enter(MIC_CHANNEL_SEL,11);
#endif
    pcm_play();
    t_usbdev.opt_status = OPT_NONE;
    t_usbdev.disp_opt = 0xff;                     //更新显示
    t_usbdev.disp_volume = 0xff;
    t_usbdev.first_key = 0;
    t_usbdev.vol_flag = 0;
}

//任务退出
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_exit(void)
{
    if(sys_ctl.mute_flag){ //防止暂停时系统被mute住
        sys_unmute();
    }
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
    usb_dev_exit();
    adc_stop();
    pcm_stop();
	adc_exit();
#if MIC_DIRECT_OUT_EN
    mic_direct_out_exit();
#endif
    dac_disable();
#if BT_ALL_TASK
    if(usb_dev_flag)
    {
        user_bt_init();
        bt_connect();                                           //进入蓝牙任务，发起回连
        sys_ctl.bt_sta = bt_status_get();
        sys_ctl.connected = (sys_ctl.bt_sta >= BT_STA_CONNECTED) ? 1 : 0;
    }
#endif
    usb_dev_flag = 0;
}

//任务事务处理
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_event(void)
{
    WATCHDOG_CLR();
    task_hid_status();
    if (!device_activate(DEVICE_PC)) {
#if MP3BT_DECT_EN
        if(MP3BT_DECT_IS_BT()){
            task_ctl.work_sta = TASK_BT;
        }
        else{
            task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
        }
#else
        task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
#endif
        return;
    }
    if (!t_usbdev.first_key) {                  //自动发送下一曲按键
        t_usbdev.first_key = usb_hid_set(USB_AUDIO_NEXTFILE);
    }
    usb_dev_event();                        //执行USB设备事务
    t_usbdev.opt_status = usb_get_opt();        //获取当前的读写状态
}

#pragma location="TASK_USBDEV_SEG"
void task_usbdev_vol_up(void)
{
    usb_hid_set(USB_AUDIO_VOLUP);
    t_usbdev.disp_update = 1;
    ocx_show_cnt = 0;
}
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_vol_down(void)
{
    usb_hid_set(USB_AUDIO_VOLDOWN);
    t_usbdev.disp_update = 1;
    ocx_show_cnt = 0;
}


#pragma constseg="TASK_USBDEV_SEG_CONST"
IAR_CONST char str_show_taskusbdev[] = "task_usbdev\n";
#pragma constseg=default

//任务主流程
#pragma location="TASK_USBDEV_SEG"
void task_usbdev(void)
{u16 usb_audio_get_vol(u8 vol_type);
    u8 msg;
    printf(str_show_taskusbdev);
    task_usbdev_enter();
    while (task_ctl.work_sta == TASK_USBDEV) {
        msg = get_msg();
        task_usbdev_event();
        task_usbdev_deal_msg(msg);
        task_usbdev_display();
    }
    task_usbdev_exit();
}
#endif
