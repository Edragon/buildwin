/*****************************************************************************
 * Module    : Task
 * File      : task_usbdev.c
 * Author    : Hanny
 * Function  : ����������
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

//�жϲ���״̬��0:��ͣ  1:����
#pragma location="TASK_USBDEV_SEG"
u8 task_hid_status(void)
{
    u8 ret = 0;
    static u8 cnt = 0,mute_flag = 0;
    static u32 u_pow = 0; //buf�������

    if(cnt < 10){
        if(u_pow == voice_power((u8*)usb_audiobuf, 97)){
            mute_flag++;
        }
        u_pow = voice_power((u8*)usb_audiobuf, 97);//��������ֵ
        cnt++;
    }else{
        cnt = 0;
        if(mute_flag > 8){ //��⵽��������8��û�б仯=��ͣ��muteסϵͳ
            if(!sys_ctl.mute_flag){
                sys_mute();
            }
            ret = 0;
        }else{  //����״̬��ϵͳ���ܱ�muteס����mute
            if(sys_ctl.mute_flag){
                sys_unmute();
            }
                ret = 1;
        }
        mute_flag = 0;
    }
    return ret; //cnt = 10 �Ż᷵����ȷ��ֵ
}

//�����ʼ��
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_enter(void)
{
    led_mode_off();
    led_idle();
    set_sys_clk(SYS_48M);
    set_lcd_buf(LCD_FULL_MODE);
    usb_dev_flag = 0;
    if (!device_activate(DEVICE_PC)) {
        task_ctl.work_sta = TASK_EXIT;      //�豸��Ч���������˵�
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
    //����USB CardReader����
    load_code(0x40, 3);
    memset(adc_buf, 0, USBMIC_SAMPLES*2);
    pcm_init(S_SPR_48000);
    AUCON10 |= BIT(7);
    FIFO_SPEED = 0;
    adc_init(SEL_USBMIC);
    usb_audio_set_volume(sys_ctl.volume);       //����Ĭ������
    if (!usb_dev_init(USBDEV_ENUM_TYPE)) {
        task_ctl.work_sta = TASK_EXIT;      //�豸��ʼ��ʧ�ܣ��������˵�
        msgbox(STR_USB_DEVICE_ERROR, NULL, MSGBOX_TIME);
        return;
    }
#if MIC_DIRECT_OUT_EN
    mic_direct_out_enter(MIC_CHANNEL_SEL,11);
#endif
    pcm_play();
    t_usbdev.opt_status = OPT_NONE;
    t_usbdev.disp_opt = 0xff;                     //������ʾ
    t_usbdev.disp_volume = 0xff;
    t_usbdev.first_key = 0;
    t_usbdev.vol_flag = 0;
}

//�����˳�
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_exit(void)
{
    if(sys_ctl.mute_flag){ //��ֹ��ͣʱϵͳ��muteס
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
        bt_connect();                                           //�����������񣬷������
        sys_ctl.bt_sta = bt_status_get();
        sys_ctl.connected = (sys_ctl.bt_sta >= BT_STA_CONNECTED) ? 1 : 0;
    }
#endif
    usb_dev_flag = 0;
}

//����������
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
            task_ctl.work_sta = TASK_EXIT;      //�豸��Ч���������˵�
        }
#else
        task_ctl.work_sta = TASK_EXIT;      //�豸��Ч���������˵�
#endif
        return;
    }
    if (!t_usbdev.first_key) {                  //�Զ�������һ������
        t_usbdev.first_key = usb_hid_set(USB_AUDIO_NEXTFILE);
    }
    usb_dev_event();                        //ִ��USB�豸����
    t_usbdev.opt_status = usb_get_opt();        //��ȡ��ǰ�Ķ�д״̬
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

//����������
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
