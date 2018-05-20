/*****************************************************************************
* Module    : UI
* File      : ui_usbdev.c
* Author    : Hanny
* Function  : ���������û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C

//������ʾ����
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

//������Ϣ����
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_deal_msg(u8 msg)
{
    switch(msg) {
        //��һ������һ��
    case KU_PREV:
        usb_hid_set(USB_AUDIO_PREFILE);
        break;
    case KU_NEXT:
        usb_hid_set(USB_AUDIO_NEXTFILE);
        break;

        //���š���ͣ
    case KU_PLAY:
        usb_hid_set(USB_AUDIO_PLAYPAUSE);
        break;

        //��������
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

        //����
    case KL_PLAY:
        usb_hid_set(USB_AUDIO_MUTE);
        break;

        //һ����Ϣ
    case QSYSTEM_1S:
      	t_usbdev.disp_update = 1;

        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}
#endif

