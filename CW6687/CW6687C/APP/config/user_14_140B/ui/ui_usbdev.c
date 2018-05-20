/*****************************************************************************
* Module    : UI
* File      : ui_usbdev.c
* Author    : Hanny
* Function  : 音乐任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//任务显示处理
#if IS_LEDSEG_5C7S_DISPLAY
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_PC);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_display(void)
{
}
#endif

//任务消息处理
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_deal_msg(u8 msg)
{
    switch(msg) {
        //上一曲，下一曲
    case KU_PREV:
        usb_hid_set(USB_AUDIO_PREFILE);
        break;
    case KU_NEXT:
        usb_hid_set(USB_AUDIO_NEXTFILE);
        break;

        //播放、暂停
    case KU_PLAY:
        usb_hid_set(USB_AUDIO_PLAYPAUSE);
        break;

        //音量调节
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
        task_usbdev_vol_down();
        show_volume();
        break;
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
        task_usbdev_vol_up();
        show_volume();
        break;

        //静音
    case KL_PLAY:
        usb_hid_set(USB_AUDIO_MUTE);
        break;

        //一秒消息
    case QSYSTEM_1S:
      	t_usbdev.disp_update = 1;

        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}
#endif

