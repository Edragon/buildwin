/*****************************************************************************
* Module    : UI
* File      : ui_bt.c
* Author    : Hanny
* Function  : ���������û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C

#pragma constseg="BT_RFC_TX_CONST"
IAR_CONST char at_spp0_ptr[] = "PREV"; //only the pointer form is workable to send vol into RfcommSendATCommand(), (why?) arthur
IAR_CONST char at_spp1_ptr[] = "NEXT";
#pragma constseg=default

#pragma constseg="BT_DISP_PARAM"
IAR_CONST char phone_pre[] = "Phone:";          // �绰����ǰ׺
IAR_CONST char phone_call[] = "Incoming Call";  // �绰û��ʾ֮ǰ����ʾ
IAR_CONST char phone_calling[] = "Calling...";  // ͨ����
IAR_CONST char call_ended[] = "Call Ended";     // ͨ����������ʾ
#pragma constseg=default

__no_init __root u8 bt_name_unicode[64] @"BtPaired"; //  ��������UNICODE���뻺����
extern u8 flag_dial;
extern u8 bt_user_feature;
void mp3res_play_btmode(u8 music_name);
bool send_hshfvol_hook(void);
void task_bt_deal_msg_10s(void);
void task_bt_deal_notice_msg(u8 msg);

void tasksb_bt_test_play(void);
void tasksb_bt_test_hsf(void);
void tasksb_bt_test_prev(void);
void tasksb_bt_test_next(void);
void tasksb_bt_test_voldown(void);
void tasksb_bt_test_volup(void);
void tasksub_bt_menu(void);
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

#pragma location="TASK_BT_SEG"
void task_bt_hid_deal_msg(u8 msg)
{
    switch (msg) {
    case KU_PLAY:
#if BT_HID_MANU
        bt_hid_photo();
#else
        if(bt_status_get() == BT_STA_CONNECTED){
            bt_hid_photo();
        }else if(bt_status_get() == BT_STA_IDLE){
            user_bt_init();
            bt_connect();
        }
#endif
        break;
    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;
#if BT_HID_MANU
	case KL_NEXT:
        //printf("kl\n");
		t_bt.hid_enable = 0;
        bt_hid_disconnect();
        if (bt_hid_wait_disconnect()) {
		    mp3res_play_btmode(RES_MP3_VOL_MAXMIN);//RES_MP3_PH_DISCONNECT
            //printf("hid nc\n");
        }
        break;
#endif

#if TASK_BTHID_EN
    case KL_PLAY:
        //�����Ͽ�����
        bt_disconnect();
        break;
#endif

    case Q_BT_CONN_DONE:                        //����������
    case Q_BT_DISCONN_DONE:                     //�����ѶϿ�
    case Q_BT_PLAY:                             //������ʼ����
    case Q_BT_STOP:                             //����ֹͣ����
    case Q_BT_INCOMING:                         //��������
    case Q_BT_CALL:                             //����ͨ��
    case Q_BT_CONN_LOST:                        //��������Զ����Ͽ�
    case Q_VOL_DOWN:                            //����������
    case Q_VOL_UP:                              //����������
        task_bt_deal_notice_msg(msg);
        break;

    case QSYSTEM_10S:
        task_bt_deal_msg_10s();
        break;

    case QSYSTEM_1S:
        deal_msg(msg);
        break;

    default:
        deal_msg(msg);
        break;
    }
}

//�û�������Ϣ����
#pragma location="TASK_BT_SEG"
void task_bt_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1)){
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        bt_play_continue();
        if(!sys_ctl.volume) {
            sys_mute();
        }
        flush_dispensable_msg();
        return;
    }
#endif

    switch (msg) {
#if IS_LCD_DISPLAY
        //����Ŀ¼����
    case KU_MENU:
    case KU_PLAY_POWER:
        tasksub_bt_menu();
        break;

    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;
#endif
    case KU_PLAY:
        //��������/��ͣ
        bt_pause_play();
        break;

    case KL_PLAY:
    case KL_VOL_UP:
    case KL_VOL_DOWN:
        //�ز��绰
        if (!flag_dial) {
            bt_redial();
        }
        break;

    case KU_PREV:
    case KU_PREV_VOL_DOWN:
        //������һ��
        bt_prev();
        break;

    case KU_NEXT:
    case KU_NEXT_VOL_UP:
        //������һ��
        bt_next();
        break;
#if !IS_LCD_DISPLAY
    case KL_PLAY:
        //�����Ͽ�����
        bt_disconnect();
        //hsf_send_atcmd((u8 *)&call,  sizeof(call));
        //change_bt_name("change_bt_name");
        //bt_connect_appointed_dev(rand(4));
        //printf_remote_dev_bt_info();
        //set_pincode(1,"1234");
        //hsf_send_atcmd(call_phone, sizeof(call_phone));
        break;
#endif
    case KL_PREV:
        //SPP���Ͳ���1
        bt_send_spp((char *)at_spp0_ptr);
        break;

#if BT_HID_MANU
    case KL_NEXT:
        if (sys_ctl.bt_sta >= BT_STA_CONNECTED){
            t_bt.hid_enable = 1;
            bt_hid_connect();
            delay_5ms(2);
            if (bt_hid_wait_connect()) {
                delay_5ms(20);
                mp3res_play_btmode(RES_MP3_KEY);//	mp3_res_play_wait(RES_MP3_PH);//RES_MP3_PH_DISCONNECT
            } else {
                delay_5ms(20);
                t_bt.hid_enable = 0;
                mp3res_play_btmode(RES_MP3_VOL_MAXMIN);
            }
		}
		else {
		    t_bt.hid_enable = 0;
		}
        break;
#else
    case KL_NEXT:
        //bt_send_spp((char *)at_spp1_ptr);
        bt_hid_photo();
        break;
#endif

    case KU_VOL_DOWN:
          //printf("ph-");
        //������
#if IS_LCD_DISPLAY
        show_volume();
        t_bt.updata_flag = 1;
#else
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
#endif
#if WARNING_VOL_MAXMIN
        if(!sys_ctl.volume){
            sys_unmute();
            dac_fade_out();
            bt_play_vol_music();
        }
#else
        if(!sys_ctl.volume){
            sys_mute();
        }
#endif
        break;

    case KU_VOL_UP:
        //printf("nh-");
        //������
#if IS_LCD_DISPLAY
        show_volume();
        t_bt.updata_flag = 1;
#else
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
#endif
#if WARNING_VOL_MAXMIN
        if(VOLUME_MAX == sys_ctl.volume){
            bt_play_vol_music();
        }
#endif
        //sys_unmute();
#if WARNING_VOL_MAXMIN
        if(VOLUME_MAX == sys_ctl.volume){
            bt_play_vol_music();
        }
#endif
        break;

    case KU_EQ:
        //EQ
        user_set_eq(sys_ctl.eq_num+1);
        show_eq();
        break;

    case Q_BT_CONN_DONE:                        //����������
    case Q_BT_DISCONN_DONE:                     //�����ѶϿ�
    case Q_BT_PLAY:                             //������ʼ����
    case Q_BT_STOP:                             //����ֹͣ����
    case Q_BT_INCOMING:                         //��������
    case Q_BT_CALL:                             //����ͨ��
    case Q_BT_CONN_LOST:                        //��������Զ����Ͽ�
    case Q_VOL_DOWN:                            //����������
    case Q_VOL_UP:                              //����������
        task_bt_deal_notice_msg(msg);
        break;

#if (HARD_IR_EN || SOFT_IR_EN)
    case T_KEY_NUM_0:
    case T_KEY_NUM_1:
    case T_KEY_NUM_2:
    case T_KEY_NUM_3:
    case T_KEY_NUM_4:
    case T_KEY_NUM_5:
    case T_KEY_NUM_6:
    case T_KEY_NUM_7:
    case T_KEY_NUM_8:
    case T_KEY_NUM_9:
        //call_fj[7] = msg - T_KEY_NUM_0 + '0';
        //hsf_send_atcmd(call_fj, sizeof(call_fj));
        //bt_connect_appointed_dev(msg - T_KEY_NUM_0);
        //set_pincode(0,"0000");
        break;

    case T_KEY_EQ:
        user_set_eq(sys_ctl.eq_num+1);
        show_eq();
        break;
#endif
    case KU_MODE:
        task_ctl.work_sta = TASK_EXIT;
        break;

    case QSYSTEM_10S:
        task_bt_deal_msg_10s();
        break;

    case QSYSTEM_1S:
        //bt_rssi_update();     //���ڲ�ѯ�ź�ǿ��
        //printf("%02x ", bt_rssi_get());
#if IS_LCD_DISPLAY
        if(sys_ctl.bt_sta == BT_STA_PLAYING) {
            t_bt.play_time++;
            t_bt.disp_status_flag = 1;
        }
#endif
        deal_msg(msg);
        break;

    default:
        deal_msg(msg);
        break;
    }
}

void task_bt_state(void)
{
    if(t_bt.disp_bt_sta != sys_ctl.bt_sta)
    {
        //printf("s:%02x\n", sys_ctl.bt_sta);
        t_bt.disp_bt_sta = sys_ctl.bt_sta;
        t_bt.disp_dev_bat = 0xff;
        //WARNING_MSG("State: %02x\n", BT_STATE);
        switch (t_bt.disp_bt_sta)
        {
        case BT_STA_IDLE:
            led_bt_idle();
            //SWUart_TX("\nNC\n");	
            break;
        case BT_STA_CONNECTED:
            led_bt_connect();
            //SWUart_TX("\nCO\n");
            break;
        case BT_STA_PLAYING:
            led_bt_play();
            if(!dac_e.digit){
                btdec_play(); //���µ���
            }
            //SWUart_TX("\nPL\n");	
            break;
        case BT_STA_OUTGOING:			
            led_bt_play();
            //SWUart_TX("\nOT\n");
            break;
        case BT_STA_PHONE:
            led_bt_play();
            //SWUart_TX("\nIN\n");
            break;
        }
    }
}

//�û���ʾ����
#if IS_LEDSEG_7PIN_DISPLAY
#pragma location="DISP_LEDSEG_SEG"
void task_bt_display(void)
{
    ledseg_ocx_event();
    task_bt_state();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_BT);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#elif IS_LCD_DISPLAY
//�����ؼ���ʾ�ӿ�
#pragma location="TASK_BT_SEG_1"
void bt_volume_display(void)
{
    line_clear();
    line_update(6);
    disp_menu(STR_VOLUME_SET,1,0, 0,ME_CENTER);
    line_update_clear(0);
    disp_num(sys_ctl.hshf_vol, 56, 0, DN_RIGHT | 2);
    line_update_clear(2);
    u8 x = (sys_ctl.hshf_vol << 7) / VOLUME_MAX;
    x = 29 + (x * 65 >> 7);
    disp_draw(RES_OCX_BAR_BMP, 29, 0);
    disp_draw(RES_OCX_INDEX_BMP, x, 0);
    line_update(4);
}

// bt������ר����Ϣ����
#pragma location="TASK_BT_SEG_1"
void bt_progressbar_deal_msg(u8 show_time)
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

        //�����ӣ�������
    case K_PREV:
    case KL_PREV:
    case KH_PREV:
        user_set_hshfvol(user_volume_dec(sys_ctl.hshf_vol));
        send_hshfvol_hook();
        break;

    case K_NEXT:
    case KL_NEXT:
    case KH_NEXT:
        user_set_hshfvol(user_volume_inc(sys_ctl.hshf_vol));
        send_hshfvol_hook();
        break;

        //���ù�����Ϣ����
    default:
        ocx_msg(msg);
        return;
    }
    t_prog.show_time = show_time;
    ocx_show_cnt = 0;
}

// btר�ý�����
#pragma location="TASK_BT_SEG_1"
void bt_progressbox(u8 show_time)
{
    void progressbar_event(void);
    t_prog.ok_flag = 0;
    t_prog.show_time = show_time;
    ocx_show_cnt = 0;
    while (!t_prog.ok_flag) {
        progressbar_event();
        bt_progressbar_deal_msg(show_time);
        bt_volume_display();
        if (task_ctl.work_sta == TASK_EXIT) {
            t_prog.ok_flag = 0xfe;
        }
    }
}

#pragma location="BT_DISP"
u8 bt_name_len(u8 *str)
{
    u8 width=0;
    while (1) {
        if (*str == 0 && *(str+1) == 0) {
            break;
        } else if (*str & BIT(7) || *(str+1) != 0) {
#if(FONT_SEL == SMALL_FONT)
            width += 13;
#else
            width += 15;
#endif
        } else {
#if(FONT_SEL == SMALL_FONT)
            width += 7;
#else
            width += 8;
#endif
            //width += char_width(*str);
        }
        str += 2;

    }
    return width;
}

#pragma location="BT_DISP"
void bt_name_to_unicode(u8 *str)
{
    u8 j;
    u8 i;
    u8 utf8_size;
    u8 ch[2];
    u8 t1 = 0;
    u8 t2 = 0;
    u8 t3 = 0;
    i=0;
    j =0;
    while (str[i] != 0 && i<32) {
        ch[0] = str[i++];
        utf8_size = utf8_char_size(ch[0]);
        switch (utf8_size) {
        case 1:
            ch[1] = 0;
            break;
        case 2:
            t1 = ch[0] & (0x1f);//��λ�ĺ�5λ����ȥ����ͷ����110�����־λ��
            t2 = str[i++] & (0x3f);//��λ�ĺ�6λ����ȥ����ͷ����10�����־λ��
            ch[0] = t2 | ((t1 & (0x03)) << 6);
            ch[1] = t1 >> 2;//�����䱣������λ
            break;
        case 3:
            t1 = ch[0] & (0x1f);
            t2 = str[i++] & (0x3f);
            t3 = str[i++] & (0x3f);
            ch[0] = ((t2 & (0x03)) << 6) | t3;
            ch[1] = (t1 << 4) | (t2 >> 2);
            break;
        }
        if(ch[0] == 0) {
            break;
        } else {
            bt_name_unicode[j++] = ch[0];
            bt_name_unicode[j++] = ch[1];
        }
    }
    bt_name_unicode[j]=0;
}

#pragma location="BT_DISP"
void task_bt_disp_name(u8 line)
{
    if(move_ctl.ms_flag==1) {
        move_ctl.ms_flag=0;
        move_ctl.ms3_flag++;
        if(move_ctl.ms3_flag==3) {
            move_ctl.ms3_flag=0;

            if(move_ctl.move_flag) {
                if(move_ctl.name_x+(s16)move_ctl.name_wide>106) {
                    move_ctl.name_x--;
                } else {
                    move_ctl.name_x=0;
                }
            }
            line_clear();
            if(bt_name_unicode[0] == 0 && bt_name_unicode[0] == 0) {
                disp_menu(STR_BT_CONNECTED, 0, 0, 0, ME_CENTER);
            } else {
                disp_text_u((char *)bt_name_unicode, move_ctl.name_x, 0, 1);
            }
            line_update(line);
        }
    }
}

#pragma location="BT_DISP"
void task_bt_disp_hostname(void)
{
    if(!t_bt.hostname_change) {
        return;
    }
    t_bt.hostname_change = 0;
    if(t_bt.disp_bt_sta < BT_STA_CONNECTED) {    //  ����δ����
        memset(bt_name_unicode,0,sizeof(bt_name_unicode));
        line_clear();
        //��ʾ"������"
        if(t_bt.disp_bt_sta == BT_STA_CONNECTING) {
            disp_menu(STR_BT_CONNECTING, 0, 0, 0, ME_CENTER);
        //��ʾ"����δ����"
        }else {
            disp_menu(STR_BT_NOT_LINK, 0, 0, 0, ME_CENTER);
        }
        line_update(3);
    }else {
        line_clear();
        if(t_bt.bt_paired_update == 0) {
            disp_menu(STR_BT_CONNECTED, 0, 0, 0, ME_CENTER);
            line_update(3);
        }
        line_clear();
        line_update(5);
    }
}

#pragma location="BT_DISP"
void task_bt_display_status(void)
{
    static u8 play_status_icon;
    u8 hour, min, sec;
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_bt.disp_status_flag = 1;             //���ø��±�־
    }
    if (t_bt.disp_bt_sta != sys_ctl.bt_sta) {
        t_bt.disp_bt_sta = sys_ctl.bt_sta;
        t_bt.disp_status_flag = 1;             //���ø��±�־
        t_bt.hostname_change = 1;             //���ø��±�־
    }
    if (!t_bt.disp_status_flag) {
        return;
    }
    t_bt.disp_status_flag = 0;

    line_clear();
    //��ʾ����ͼ��
    disp_draw(RES_MAIN_BT0_BMP, 0, 0);
    if(task_ctl.work_sta == TASK_BT){
        //��ʾ����״̬
        if(t_bt.disp_bt_sta == BT_STA_CONNECTED) {
            play_status_icon = RES_STATUS1_BMP;       //��ʾ��ͣ״̬
        }
        else if(t_bt.disp_bt_sta == BT_STA_PLAYING) {
            play_status_icon = RES_STATUS0_BMP;        //��ʾ����״̬
        }
        else {
            play_status_icon = RES_ICON_STOP_BMP;
        }
        disp_draw(play_status_icon, 10, 0);
        //��ʾ��ЧEQ
        play_status_icon = RES_PLAY_EQ_0_BMP + sys_ctl.eq_num;
        disp_draw(play_status_icon, 26, 0);
        //��ʾʱ��
        hour = t_bt.play_time / 3600;
        min = (t_bt.play_time / 60) % 60;
        sec = t_bt.play_time % 60;
        disp_medium_time(hour, min, sec, 45, 0);
    }
    //��ʾ����
    if (sys_ctl.bat_val >= 1) {
        if(IS_DCIN()){
            disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
        }else {
            disp_draw(RES_ICON_BAT0_BMP + ((sys_ctl.bat_val + 1) >>1) - 1, 108, 0);
        }
    }
    line_update(0);
}

#pragma location="BT_DISP"
void task_bt_display(void)
{
    if (t_bt.updata_flag) {
        t_bt.updata_flag = 0;
        t_bt.disp_status_flag = 1;
        t_bt.hostname_change = 1;
        move_ctl.ms_flag = 1;
        draw_clear();                                       //�������
    }
    task_bt_display_status();
    task_bt_disp_hostname();
}

#pragma location="BT_DISP"
void task_bt_phone_num_display(void)
{
    if(!t_bt.phone_num_disp) {
        return;
    }
    if(!get_num_len()) {
        return;
    }
    t_bt.phone_num_disp = 0;
    u8 i,pre_or_name_len;
    pre_or_name_len = sizeof(phone_pre);

    u8 *phone_num = get_num_buf();
    u8 phone_num_len = get_num_len();
    if(phonebook_total_num){
        for(u8 j=0; j<phone_num_len; j++){
            bt_tmp[j] = phone_num[j] + '0';
        }
        u16 check_result = check_contacts(bt_tmp,phone_num_len);
        if(check_result == 0xffff){//printf("ck_none\n");
            memcpy(bt_tmp, (void const *)phone_pre, sizeof phone_pre);
        } else {//printf("ck_ok\n");
            spi_phonebook_read(phonebook_name_number, check_result);
            pre_or_name_len = utf8_to_utf16(bt_tmp, phonebook_name_number, 40);
            bt_tmp[pre_or_name_len] = ':';
            bt_tmp[pre_or_name_len+1] = 0x00;
            pre_or_name_len += 2;
            for(i=0; i<phone_num_len*2; i+=2) {
                bt_tmp[pre_or_name_len+i] = phone_num[i/2] + '0';
                bt_tmp[pre_or_name_len+i+1] = 0x00;
            }
            bt_tmp[pre_or_name_len+phone_num_len*2] = '\0';
            bt_tmp[pre_or_name_len+phone_num_len*2+1] = '\0';

            u8 disp_len = 0;    //name and phone number display length
            for(u8 j=1; j<pre_or_name_len+phone_num_len*2; j+=2){
                if(bt_tmp[j]){
                    disp_len += 2;
                } else {
                    disp_len++;
                }
            }
            line_clear();
            disp_text_u((char *)bt_tmp, (disp_len<18)? (128-disp_len*7)/2 : 0 ,0 ,1);
            line_update(3);
            return;
        }
    } else {//printf("no_ck\n");
        memcpy(bt_tmp, (void const *)phone_pre, sizeof phone_pre);
    }//printf("pre_name:%s\n",bt_tmp);printf_array(bt_tmp,pre_or_name_len);

    for(i=0; i<phone_num_len; i++) {
        bt_tmp[pre_or_name_len+i-1] = phone_num[i] + '0';
    }
    bt_tmp[pre_or_name_len+i-1] = '\0';
    line_clear();
    disp_text((char *)bt_tmp, TX_CENTER, 0, 1);
    line_update(3);
}

#pragma location="BT_DISP"
void task_bt_phone_calling_display(void)
{
    void task_bt_call_ended();

    if(get_num_len()) {
        t_bt.phone_num_disp = 1;
        return;
    }
    if(!t_bt.calling_disp) {
        return;
    }
    if(bt_in_call()) {
        t_bt.calling_disp = 0;
        return;
    }


    if(t_bt.incoming_call_disp == 0) {
        task_bt_call_ended();
        return;
    }

    t_bt.calling_disp = 0;
    memcpy(bt_tmp, (void const *)phone_call, sizeof phone_call);
    line_clear();
    disp_text((char *)bt_tmp, TX_CENTER, 0, 1);
    line_update(3);
}

#pragma location="BT_DISP"
void task_bt_call_display(void)
{
    if (t_bt.updata_flag) {
        t_bt.updata_flag = 0;
        t_bt.disp_status_flag = 1;
        t_bt.phone_num_disp = 1;
        t_bt.calling_disp = 1;
        move_ctl.ms_flag = 1;
        draw_clear();                                       //�������
    }else {
        return;
    }
    //memcpy(bt_tmp, (void const *)phone_pre, sizeof phone_pre);
    task_bt_display_status();
    task_bt_phone_calling_display();
    task_bt_phone_num_display();

    if(bt_in_call()) {
        memcpy(bt_tmp, (void const *)phone_calling, sizeof phone_calling);
        line_clear();
        disp_text((char *)bt_tmp, TX_CENTER, 0, 1);
        line_update(5);
    }
}

#pragma location="BT_DISP"
void task_bt_call_ended(void)
{
    line_clear();
    line_update(5);
    memcpy(bt_tmp, (void const *)call_ended, sizeof call_ended);
    disp_text((char *)bt_tmp, TX_CENTER, 0, 1);
    line_update(3);
}
#else
#pragma location="DISP_LEDSEG_SEG"
void task_bt_display(void)
{
	 task_bt_state();
}
#endif

/*****************************************************************************
 * Module    : ���� - ���� �µ�UI����
 *****************************************************************************/
#pragma location="TASK_BT_RING_SEG"
void task_bt_ring_deal_msg(u8 msg)
{
    switch (msg) {
    case KU_PLAY:
        //�����绰
        bt_pickup();
        break;

    case KL_PLAY:
        //�Ҷϵ绰
        bt_hangup();
        break;
    }
}

/*****************************************************************************
 * Module    : ���� - ͨ�� �µ�UI����
 *****************************************************************************/
#pragma location="TASK_BT_CALL_SEG"
void task_bt_call_deal_msg(u8 msg)
{
    void tasksub_bt_call_event(void);
    u8 call_status;
    switch (msg) {

    case KU_NEXT:   //1.�ֻ�����������֮���л�����
    case KL_NEXT:
        bt_sco_switch();  //���ߵ��� bt_sco_disconnect/bt_sco_connectҲ����ʵ�ִ˹���
        break;

    case KU_PLAY:
        call_status = bt_get_call_status();
        //printf("c_sta: %d\n", call_status);
        if(call_status == 2) {
            bt_pickup();                //������2·ͨ��
        } else {
            bt_hangup();                //�Ҷϵ绰
        }
        break;

    case KL_PLAY:
        call_status = bt_get_call_status();
        //printf("c_sta: %d\n", call_status);
        if(call_status == 2) {
            bt_hangup();                //�ܽӵ�2·ͨ��
        } else if(call_status == 4) {
            bt_callswap();              //�л���·ͨ��
        }
        break;
#if IS_LCD_DISPLAY
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
        ocx_init(tasksub_bt_call_event, NULL);
        bt_progressbox(PROGRESS_TIME);     //������������������ʾ1.5��
        t_bt.updata_flag = 1;
#else

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        //������
        user_set_hshfvol(user_volume_dec(sys_ctl.hshf_vol));
        send_hshfvol_hook();
        show_volume();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
        //������
        user_set_hshfvol(user_volume_inc(sys_ctl.hshf_vol));
        send_hshfvol_hook();
        show_volume();
        break;
#endif
        //�ֻ��˵�ϵͳ����
    case Q_VOL_DOWN:
#if IS_LCD_DISPLAY
        bt_volume_display();
#else
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
#endif
        break;
    case Q_VOL_UP:
#if IS_LCD_DISPLAY
        bt_volume_display();
#else
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
#endif
        break;

#if !BT_CALL_PRIVATE
        //�ֻ��˵�ͨ������
    case Q_HSHF_VOL_CHANGE:
#if IS_LCD_DISPLAY
        ocx_init(tasksub_bt_call_event, NULL);
        bt_progressbox(PROGRESS_TIME);     //������������������ʾ1.5��
        t_bt.updata_flag = 1;
#else
        show_volume();
#endif
        break;
#endif
    }
}

/*****************************************************************************
 * Module    : ���� - ���Ժ� �µ�UI����
 *****************************************************************************/
#pragma location="TASK_BT_CALL_SEG"
void task_bt_test_deal_msg(u8 msg)
{
#if TASK_BTHID_EN
    if (task_hid_flag) {
        if ((msg != KU_MODE) && (msg != KU_PLAY) && (msg != KL_PLAY_POWER)) {
            return;
        }
    }
#endif
    switch (msg) {

    case KU_MODE:  //���Ժ�����ģʽ
        bt_test_flag = true;
        if(sys_ctl.bt_sta >= BT_STA_INCOMING){
          adc_stop();  //�ر�ADC,��ֹХ��
          pcm_stop();
          adc_exit();
          Test_Mode_Flag =0;
          bt_init();   //����ĳЩ����û�壬����λ
        }
        task_ctl.work_sta = TASK_EXIT;
        break;

    case KU_PLAY:
    case KU_PLAY_POWER:
        //���Բ��ż�
        tasksb_bt_test_play();
        break;

    case KU_HSF:
        //����ͨ����
        tasksb_bt_test_hsf();
        break;

    case KU_PREV:
    case KU_PREV_VOL_DOWN:
        //������һ��
        tasksb_bt_test_prev();
        break;

    case KU_NEXT:
    case KU_NEXT_VOL_UP:
        //������һ��
        tasksb_bt_test_next();
        break;

    case K_VOL_DOWN:
        //����������
        tasksb_bt_test_voldown();
        break;

    case K_VOL_UP:
        //����������
        tasksb_bt_test_volup();
        break;

#if(SOFT_POWER_ON_OFF)
    case KL_PLAY_POWER:
        mp3res_play_btmode(RES_MP3_POWER_OFF);
        power_down_enter();
        break;
#endif
    }
}

/*****************************************************************************
 * Module    : �����������ܵ�UI����
 *****************************************************************************/
#pragma location="TASK_BT_LOST_SEG"
void task_bt_lost_deal_msg(u8 msg)
{
    switch (msg) {

    //case KU_HSF://�̰�HSF��MODE�����˳�����ģʽ
    //case KU_MODE:
    case KU_MENU:
    case KU_PLAY_POWER:
    case KU_PLAY:
        task_ctl.work_sub = TASK_BT_NULL;
        break;

    case QSYSTEM_1S:      //�������ܣ�1Sһ����ʾ��
        //printf("bt_lost 1S warning\n");
        mp3res_play_btmode(RES_MP3_BT_DISCONNECT);
        break;

#if(SOFT_POWER_ON_OFF)
    case KL_PLAY_POWER:
        mp3res_play_btmode(RES_MP3_POWER_OFF);
        power_down_enter();
        break;
#endif
    }
}
#endif
