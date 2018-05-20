/*****************************************************************************
* Module    : UI
* File      : ui_comm.c
* Author    : Hanny
* Function  : ���񹫹��û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C
//#pragma location="POWER_CHECK_SEG"
__near_func void set_time_poweroff_check(void) {
    bool tmp_ietm2 = IE_TM2;
    IE_TM2 = 0;
    if (sys_ctl.poweroffcnt != 0 && sys_ctl.poweroffcnt != 0xffff) {
        sys_ctl.poweroffcnt--;
    }
    if (sys_ctl.poweroffcnt == 0) {
        task_ctl.work_sta = TASK_POWEROFF;
    }
    IE_TM2 = tmp_ietm2;
}

//��������Ĺ�����Ϣ������
__near_func void deal_msg(u8 msg)
{
    switch (msg) {
    case KU_MODE:
        //ģʽ�л�
        task_ctl.work_sta = TASK_EXIT;
        break;

   case Q_BT_CONN_REQ:
#if TASK_BT_EN
        task_ctl.work_sta = TASK_BT;
#endif
        break;

        //�����˵������˳������˵�
    case KL_MENU:

#if BT_CALL_PRIVATE
    case KU_HSF:
#else
    case KL_HSF:
#endif
        if (sys_ctl.bt_sta >= BT_STA_INCOMING)
        {
            break;      //ͨ�������������ģʽ����ģʽ
        } else
        {
#if MODE_CHANGE_BLUE_ON
            led_mode_on();
#endif
            task_ctl.work_sta = TASK_EXIT;
            break;
        }

#if(SOFT_POWER_ON_OFF) //ʹ�����ػ�����
    case KL_POWER:
    case KL_PLAY_POWER:
    #if BT_AUTO_PLAY
        if(sys_ctl.bt_sta == BT_STA_PLAYING){
            sys_ctl.bt_auto_play_flag = 1;
            sys_param_write8(PARAM_BT_AUTO_PLAY, sys_ctl.bt_auto_play_flag);
            param_write(Link_BdAddr, PARAM_REMOTE_DEV_ADDR, 6);
        }
        else{
            sys_ctl.bt_auto_play_flag = 0;
            sys_param_write8(PARAM_BT_AUTO_PLAY, sys_ctl.bt_auto_play_flag);
        }
    #endif
        sys_ctl.soft_poweroff_flag = 1;
        task_ctl.work_sta = TASK_POWEROFF;
        break;
#endif

#if IR_HARD_POWERDOWN_EN
    case KU_POWER:
        sys_ctl.ir_power_flag = 1;
        //printf("IR_POWER_DOWN\n");
        break;
#endif

    //1����Ϣ
    case QSYSTEM_1S:

#if ALARM_EN
        task_alarm_check();
#endif
#if FM_TIME_EN
        task_fm_check();
#endif
#if TASK_HOLD_EN
        task_sleep_check();
#endif

#if IS_LCD_DISPLAY
        task_backlight_check();     // ������ʱ����
        task_poweroff_check();
        set_time_poweroff_check();  // ��ʱ�ػ����
#endif
        //task_backlight_check();
/*
        task_poweroff_check();

#if LED_STATUS_BLINK
        if(sys_ctl.led_status_blink_flag){  //�͵�ʱ״̬������
            LED_STATUS_TOG();
        }
#endif*/
        break;

#if (HARD_IR_EN || SOFT_IR_EN)
    case T_KEY_MUTE:
#if FM_VOUT_THROUGH_MCU
        if(sys_ctl.mute_flag){
            sys_unmute();
        } else{
            sys_mute();
        }
        break;
#else
        if(sys_ctl.mute_flag){
            if(task_ctl.work_sta == TASK_FM) {
                sys_ctl.mute_flag = 0;
                speaker_unmute();//FMģʽ�������������������ֱ��mute���ż���
            } else {
                sys_unmute();
            }
        } else{
            if(task_ctl.work_sta == TASK_FM) {
                sys_ctl.mute_flag = 1;
                speaker_mute();
            } else {
                sys_mute();
            }
        }
        break;
#endif
#endif
    }
}


#endif
