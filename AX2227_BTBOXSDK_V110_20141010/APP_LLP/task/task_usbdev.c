/*****************************************************************************
 * Module    : Task
 * File      : task_usbdev.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 读卡器任务
 *****************************************************************************/
#include "include.h"
#if TASK_USBDEV_EN
#include "task.h"
#include "task_usbdev.h"

//USB Audio音量调节
#define USB_AUDIO_STOP        0x80
#define USB_AUDIO_PREFILE     0x20
#define USB_AUDIO_NEXTFILE    0x10
#define USB_AUDIO_PLAYPAUSE   0x08
#define USB_AUDIO_MUTE        0x04
#define USB_AUDIO_VOLDOWN     0x02
#define USB_AUDIO_VOLUP       0x01
#define USB_AUDIO_NONE        0x00

enum {
    OPT_NONE,
    OPT_READ,
    OPT_WRITE,
};

IAR_XDATA_A type_usbdev_task t_usbdev @ "USBDEV_XDATA";

extern IAR_BIT_A bool usb_dev_flag;

//任务初始化
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_enter(void)
{
    led_mode_off();
    led_idle();
    set_sys_clk(SYS_48M);
    usb_dev_flag = 1;
    if (!device_activate(DEVICE_PC)) {
        task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
        if(task_ctl.work_sta != TASK_USBDEV)
        {
            return;
        }
    }
    draw_clear();
    task_ctl.work_sta = TASK_USBDEV;

#if USB_DEVICE_WARNING_MUSIC
    dac_enable();
    mp3_res_play_wait(RES_MP3_PC_MODE);
#endif

    //加载USB CardReader程序
    load_code(DCODE_USBDEV, 3);

    mp3en_exit();
    mp3en_source_init(AUDIO_MIC);
    AGCCON0 = 0x17;
    memset(mp3_adc_buf, 0x00, 0xd80);

    dac_enable();
    usb_audio_set_volume(sys_ctl.volume);       //设置默认音量

    if (!usb_dev_init(USBDEV_ENUM_TYPE)) {
        task_ctl.work_sta = TASK_EXIT;      //设备初始化失败，返回主菜单
        return;
    }

    music_unmute();

    t_usbdev.opt_status = OPT_NONE;
    t_usbdev.disp_opt = 0xff;                     //更新显示
    t_usbdev.disp_volume = 0xff;
    t_usbdev.first_key = 0;
    t_usbdev.vol_flag = 0;
#if USBAUDIO_FREQ_SHOW
    music_freq_init(FREQ_DAC);
#if LCD_THEME
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
#endif
}

//任务退出
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_exit(void)
{
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
    usb_dev_exit();
    usb_dev_flag = 0;
    dac_disable();
}

//任务事务处理
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_event(void)
{
    WATCHDOG_CLR();
    if (!device_activate(DEVICE_PC)) {
#if MP3BT_DECT_EN
    #if MP3BT_DECT_REUSE_EN
        MP3BT_DECT_INIT();
    #endif
        if(MP3BT_DECT_IS_BT()){
            task_ctl.work_sta = TASK_BT;
        }
        else{
            task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
        }
        MP3BT_DECT_END();
#else
        task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
#endif
        return;
    }
    if (!t_usbdev.first_key) {                  //自动发送下一曲按键
        t_usbdev.first_key = usb_hid_set(USB_AUDIO_NEXTFILE);
    }
    if (t_usbdev.vol_flag && ocx_show_cnt >= 60) {
        t_usbdev.vol_flag = 0;
        t_usbdev.disp_update = 1;
    }
    usb_dev_event();                        //执行USB设备事务
    t_usbdev.opt_status = usb_get_opt();        //获取当前的读写状态
#if USBAUDIO_FREQ_SHOW
    if (music_freq_get()) {//频谱计算完成
        /*for(u8 i=0;i<16;i++)
        {
            printf("%04x ",freq_buf[i]);
        }
        printf("\n");*/
        music_freq_kick();//kick下一次频谱计算
    }
#endif
}

#pragma location="TASK_USBDEV_SEG"
void task_usbdev_vol_up(void)
{
    if(t_usbdev.vol_flag)
    {
        usb_hid_set(USB_AUDIO_VOLUP);
        t_usbdev.disp_update = 1;
        ocx_show_cnt = 0;
    }
}
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_vol_down(void)
{
    if(t_usbdev.vol_flag)
    {
        usb_hid_set(USB_AUDIO_VOLDOWN);
        t_usbdev.disp_update = 1;
        ocx_show_cnt = 0;
    }
}
//任务消息处理
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_deal_msg(u8 msg)
{
    if(((msg & 0x1f)!= T_KEY_VOL_DOWN) && ((msg & 0x1f)!= T_KEY_VOL_UP) && (msg != QSYSTEM_1S) && msg)
        t_usbdev.vol_flag = 0;
    switch(msg) {
        //上一曲，下一曲
#if USBAUDIO_VOLUME_MODE
    case KU_PREV:
    case KU_PREV_S10:
        usb_hid_set(USB_AUDIO_PREFILE);
        break;
    case KU_NEXT:
    case KU_NEXT_S10:
        usb_hid_set(USB_AUDIO_NEXTFILE);
        break;
        //播放、暂停
    case KU_PLAY:
    case KU_PLAY_S10:
        if(t_usbdev.vol_flag)
        {
            t_usbdev.vol_flag = 0;
        }
        usb_hid_set(USB_AUDIO_PLAYPAUSE);
        break;

        //音量调节
    case K_USBVOL_DOWN:
    case KL_USBVOL_DOWN:
    case KH_USBVOL_DOWN:
    case KL_PREV_S10:
    case KH_PREV_S10:
        if(!t_usbdev.vol_flag)
        {
            t_usbdev.vol_flag = 1;
            ocx_show_cnt = 0;
            t_usbdev.disp_update = 1;
        }
        else
            t_usbdev.vol_flag = 2;
        task_usbdev_vol_down();
        break;
    case K_USBVOL_UP:
    case KL_USBVOL_UP:
    case KH_USBVOL_UP:
    case KL_NEXT_S10:
    case KH_NEXT_S10:
        if(!t_usbdev.vol_flag)
        {
            t_usbdev.vol_flag = 1;
            ocx_show_cnt = 0;
            t_usbdev.disp_update = 1;
        }
        else
            t_usbdev.vol_flag = 2;
        task_usbdev_vol_up();
        break;

#else
    case KU_PREV:
    case KU_PREV_S10:
        if(!t_usbdev.vol_flag)
        {
            usb_hid_set(USB_AUDIO_PREFILE);
        }
        else
        {
            task_usbdev_vol_down();
        }
        break;
    case KU_NEXT:
    case KU_NEXT_S10:
        if(!t_usbdev.vol_flag)
        {
            usb_hid_set(USB_AUDIO_NEXTFILE);
        }
        else
        {
            task_usbdev_vol_up();
        }
        break;
        //播放、暂停
    case KU_PLAY:
    case KU_PLAY_S10:
        if(t_usbdev.vol_flag)
        {
            t_usbdev.vol_flag = 0;
        }
        else
        {
            usb_hid_set(USB_AUDIO_PLAYPAUSE);
        }
        break;
    //case K_PREV:
    case KL_PREV:
    case KH_PREV:
    case KL_PREV_S10:
    case KH_PREV_S10:
        task_usbdev_vol_down();
        break;

    //case K_NEXT:
    case KL_NEXT:
    case KH_NEXT:
    case KL_NEXT_S10:
    case KH_NEXT_S10:
         task_usbdev_vol_up();
        break;

        //音量调节
    case K_VOL_DOWN:
    case K_VOL_UP:
        t_usbdev.vol_flag = 1;
        ocx_show_cnt = 0;
        t_usbdev.disp_update = 1;
       // usb_hid_set(USB_AUDIO_VOLDOWN);
        break;

#endif

        //静音
    case KL_PLAY:
        usb_hid_set(USB_AUDIO_MUTE);
        break;

        //一秒消息
    case QSYSTEM_1S:
      	t_usbdev.disp_update = 1;
        //调用公共消息函数
    default:
        deal_msg(msg);          //调用时可响应模式键切换模式，不调用时只有断开连接才能退出USBDEV
        break;
    }
}

#pragma constseg="TASK_USBDEV_SEG_CONST"
IAR_CONST char str_show_taskusbdev[] = "task_usbdev\n";
#pragma constseg=default

//任务主流程
#pragma location="TASK_USBDEV_SEG"
void task_usbdev(void)
{
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
