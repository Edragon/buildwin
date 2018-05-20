/*****************************************************************************
* Module    : UI
* File      : ui_fm.c
* Author    : Hanny
* Function  : FM�����û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C

//������ʾ����
#if IS_LEDSEG_5C7S_DISPLAY
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_FREQ);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

//¼����������ʾ����
#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_display(void)
{
    //ledseg_ocx_event();
    if (ledseg_ocx.show_time && ocx_show_cnt >= ledseg_ocx.show_time) {
        ledseg_ocx.status = LEDSEG_OCX_NULL;
    }
    if (ledseg_ocx.status != ledseg_ocx.disp_status) {
        ledseg_ocx.disp_status = ledseg_ocx.status;
    }

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_RECORDING);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else

#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display(void)
{
}

#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_display(void)
{
}
#endif

//������Ϣ����
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1)){
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        fm_play_continue();
        flush_msg();
        return;
    }
#endif

    switch(msg) {
        //�Զ���̨
    case KL_PLAY:
        if(t_fm.seek_sta == SEEK_AUTO){
            task_fm_auto_stop();        //ֹͣ�Զ���̨
            task_fm_auto_play();
            led_fm_play();
        } else {
            task_fm_auto_start();       //��ʼ�Զ���̨
            t_fm.seek_sta = SEEK_AUTO;
            sys_ctl.sleep_cnt = 0xffff;
            led_fm_scan();
        }
        break;

    case KU_PLAY:   //����̨ʱ,ֹͣ��̨. �ڲ���ʱΪPP
        if(t_fm.seek_sta == SEEK_AUTO){
            task_fm_auto_stop();        //ֹͣ�Զ���̨
            task_fm_auto_play();
        } else {  //����/��ͣ
            if(t_fm.mute && sys_ctl.volume){     //����
                t_fm.mute = 0;
                sys_ctl.sleep_cnt = 0xffff;
#if FM_VOUT_THROUGH_MCU
                sys_unmute();
                amux_play();
#else
                fm_set_vol(sys_ctl.volume);
                speaker_unmute();
#endif
            }
            else{               //��ͣ
                t_fm.mute = 1;
                sys_ctl.sleep_cnt = STANDBY_TIME;
#if FM_VOUT_THROUGH_MCU
                amux_stop();
#else
                speaker_mute();
                fm_set_vol(0);
#endif
            }
        }
        break;

        //��һ������һ��
    case KU_PREV:
        if(t_fm.seek_sta == SEEK_STOP) {
            task_fm_prevch();
            sys_param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
            sys_param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
        }
        //printf("channel %d: %dMHz\n",t_fm.ch_sel,t_fm.freq);
        sys_ctl.sleep_cnt = 0xffff;
        break;

    case KU_NEXT:
        if(t_fm.seek_sta == SEEK_STOP) {
            task_fm_nextch();
            sys_param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
            sys_param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
        }
        //printf("t_fm.freq: %d\n",t_fm.freq);
        //printf("channel %d: %dMHz\n",t_fm.ch_sel,t_fm.freq);
        sys_ctl.sleep_cnt = 0xffff;
        break;

        //��������
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
#if FM_SET_VOL_CHANGE_BANK
        user_set_volume(user_volume_dec(sys_ctl.volume));
#else
        user_set_volume_fm(user_volume_dec_fm(sys_ctl.volume));
#endif
        show_volume();
#if WARNING_VOL_MAXMIN
        if(!sys_ctl.volume){
            fm_play_vol_music();
            t_fm.mute = 1;
        }
#endif
        if(sys_ctl.volume){
            t_fm.mute = 0;
            sys_ctl.sleep_cnt = 0xffff;
#if FM_VOUT_THROUGH_MCU
            sys_unmute();
            amux_play();
#else
            fm_set_vol(sys_ctl.volume);
            speaker_unmute();
#endif
        }
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
#if FM_SET_VOL_CHANGE_BANK
        user_set_volume(user_volume_inc(sys_ctl.volume));
#else
        user_set_volume_fm(user_volume_inc_fm(sys_ctl.volume));
#endif
        show_volume();
#if WARNING_VOL_MAXMIN
        if(sys_ctl.volume == VOLUME_MAX){
            fm_play_vol_music();
        }
#endif
        t_fm.mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
#if FM_VOUT_THROUGH_MCU
        sys_unmute();
        amux_play();
#else
        fm_set_vol(sys_ctl.volume);
        speaker_unmute();
#endif
        break;

#if FM_RECORD_EN
        //��ʼ¼��
    case KU_HSF:
        if(t_fm.seek_sta == SEEK_AUTO){     //�Զ���̨ʱ������¼��
            break;
        }
        if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
            tasksub_fm_record();
            if(t_fm_rec.play_flag) {
                tasksub_fm_recplay();//���Ÿո�¼�µ��ļ�
            }
        } else {
            show_no_device();
        }
        break;
#endif

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
        task_fm_select_freq_chan(msg - T_KEY_NUM_0);
        break;
    case KL_SCAN:
    case KU_SCAN:
        if(t_fm.seek_sta == SEEK_AUTO){ //�̰�������SCAN������ֹͣ��̨
            task_fm_auto_stop();        //ֹͣ�Զ���̨
            task_fm_auto_play();
            sys_ctl.sleep_cnt = 0xffff;
        }
        else if(msg == KL_SCAN){        //����SCAN����ʼ��̨
            task_fm_auto_start();       //��ʼ�Զ���̨
            t_fm.seek_sta = SEEK_AUTO;
        }
        break;
#endif
    default:
        deal_msg(msg);
        break;
    }
}

//����¼����������Ϣ����
#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_deal_msg(u8 msg)
{
    switch(msg) {
        //��ͣ¼��
    case KU_PLAY:
    case KU_PLAY_POWER:
        task_fm_record_pause();
        break;

        //ֹͣ¼��
    case KU_MODE:
    case KU_HSF:
        t_fm_rec.play_flag = 1;
        task_fm_record_stop();
        break;

    case QSYSTEM_1S:
        task_fm_record_sync();
        break;
        //���ù�����Ϣ����
    default:
        break;
    }
}

//¼������������Ϣ����
#pragma location="TASK_MUSIC_SEG"
void tasksub_fm_recplay_deal_msg(u8 msg)
{
    switch(msg) {
    case KU_PLAY:
    case KU_MODE:
        music_stop();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_ctl.volume) {
            sys_unmute();
        }
#endif
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_ctl.volume) {
            sys_unmute();
        }
#endif
        break;
    }
}
#endif
