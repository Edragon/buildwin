/*****************************************************************************
* Module    : UI
* File      : ui_usbdev.c
* Author    : Hanny
* Function  : ���������û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C
// ����progressbox
extern IAR_XDATA_A struct {
    u8 disp_value;                      //��ǰ��ʾ��ѡ��������ж��Ƿ���Ҫˢ��
    u8 value;                           //��ǰ��ѡ����
    u8 in_value;                        //�����ԭ��ֵ
    u8 min;                             //��Сֵ
    u8 max;                             //���ֵ
    u8 on_change_flag;                  //�Ƿ���Ӧ�����¼�
    u8 on_click_flag;                   //�Ƿ���Ӧ̧���¼�
    u8 key_mode;                        //��Ӧ���¼�����������
    u8 ok_flag;                         //�Ƿ���ȷ����
    string title;                       //��������
    u8 show_time;                       //��ʾʱ��
    bool (*on_change)(u8 value);        //���ýӿڣ���Ӧ�����¼�������false��ΪԤ�裬����true���ʾ���趨
    bool (*on_click)(u8 value);         //���ýӿڣ���Ӧ̧���¼�
} t_prog;

//������ʾ����
#if IS_LEDSEG_7PIN_DISPLAY
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
#elif IS_LCD_DISPLAY

//�����ؼ���ʾ�ӿ�
#pragma location="TASK_USBDEV_LCD"
void volume_display(void)
{
    line_clear();
    disp_menu(STR_VOLUME_SET,1,0, 0,ME_CENTER);
    line_update_clear(0);
    disp_num(sys_ctl.volume, 56, 0, DN_RIGHT | 2);
    line_update_clear(2);
    u8 x = (sys_ctl.volume << 7) / USB_AUDIO_VOLMAX;
    x = 29 + (x * 65 >> 7);
    disp_draw(RES_OCX_BAR_BMP, 29, 0);
    disp_draw(RES_OCX_INDEX_BMP, x, 0);
    line_update(4);
}

// usb������ר����Ϣ����
#pragma location="DISPLAY_MEM"
void usb_progressbar_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //ȷ��
    case KU_OK:
        t_prog.ok_flag = 1;
        break;

        //ȡ��
#if THEME_SELECT==THEME_LCD_DEFAULT
    case KU_PLAY_POWER:
#else
    case KU_BACK:
#endif
        t_prog.ok_flag = 0xff;
        break;

        //�����������˵�
    case KL_MENU:
#if THEME_SELECT==THEME_LCD_DEFAULT
    case KL_PLAY_POWER:
#endif
        task_ctl.work_sta = TASK_EXIT;
        break;

        //�����ӣ�������
    case K_PREV:
    case KL_PREV:
    case KH_PREV:
        task_usbdev_vol_down();
        break;

    case K_NEXT:
    case KL_NEXT:
    case KH_NEXT:
        task_usbdev_vol_up();
        break;

        //���ù�����Ϣ����
    default:
        ocx_msg(msg);
        break;
    }
}

// usbר�ý�����
#pragma location="DISPLAY_MEM"
void usb_progressbox(u8 show_time)
{
    void progressbar_event(void);
    t_prog.ok_flag = 0;
    t_prog.show_time = show_time;
    ocx_show_cnt = 0;
    while (!t_prog.ok_flag) {
        progressbar_event();
        usb_progressbar_deal_msg();
        volume_display();
        if (task_ctl.work_sta == TASK_EXIT) {
            t_prog.ok_flag = 0xfe;
        }
    }
}

//������ʾ����
#pragma location="TASK_USBDEV_LCD"
void task_usbdev_display_status(void)
{
    if(!t_usbdev.vol_flag)
    {
        if (t_usbdev.disp_opt != t_usbdev.opt_status) {     //�ж��費��Ҫ����״̬
            t_usbdev.disp_opt = t_usbdev.opt_status;
            t_usbdev.disp_update = 1;
        }
        if (sys_ctl.disp_bat_val != sys_ctl.bat_val) {     //�ж��費��Ҫ����״̬
            sys_ctl.disp_bat_val = sys_ctl.bat_val;
            t_usbdev.disp_update = 1;
        }
    }
    else
    {
        if (t_usbdev.disp_volume != sys_ctl.volume) {
            t_usbdev.disp_volume = sys_ctl.volume;
            t_usbdev.disp_update = 1;
        }
    }
    if (!t_usbdev.disp_update) {
        return;
    }
    t_usbdev.disp_update = 0;
    line_clear();
    //disp_draw(RES_ICON_VOL1_BMP, 2, 0);
    //disp_num(sys_ctl.volume, 20, 0, DN_RIGHT | DN_ZERO | 2);
    if(!t_usbdev.vol_flag)
    {
        if (sys_ctl.bat_val >= 1) {
            disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
            line_update(0);
        }
    }
    if(t_usbdev.vol_flag)
    {
        volume_display();
    }
    else
    {
        draw_img(RES_MASS_IDLE_BMP+t_usbdev.disp_opt, 0, 2);
    }
    line_clear();
    line_update(6);
}

#pragma location="TASK_USBDEV_LCD"
void task_usbdev_display(void)
{
    task_usbdev_display_status();
#if USBAUDIO_FREQ_SHOW
    task_usbdev_disp_freq();
#endif
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
    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;
        //��һ������һ��
    case KU_PREV:
    case KU_PREV_VOL_DOWN:
        usb_hid_set(USB_AUDIO_PREFILE);
        break;
    case KU_NEXT:
    case KU_NEXT_VOL_UP:
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
    case KH_PREV_VOL_DOWN:
        task_usbdev_vol_down();
        show_volume();
        break;
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
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

