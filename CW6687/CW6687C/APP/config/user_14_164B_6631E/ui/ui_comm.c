/*****************************************************************************
* Module    : UI
* File      : ui_comm.c
* Author    : Hanny
* Function  : 任务公供用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//所有任务的公共消息处理函数
__near_func void deal_msg(u8 msg)
{
    switch (msg) {
    case KU_MODE:
        //模式切换
        task_ctl.work_sta = TASK_EXIT;
        break;

   case Q_BT_CONN_REQ:
#if TASK_BT_EN
        task_ctl.work_sta = TASK_BT;
#endif
        break;

        //长按菜单键，退出到主菜单
    case KL_MENU:

#if BT_CALL_PRIVATE
    case KU_HSF:
#else
    case KL_HSF:
#endif
        if (sys_ctl.bt_sta >= BT_STA_INCOMING)
        {
            break;      //通话过程中与测试模式不切模式
        } else
        {
#if MODE_CHANGE_BLUE_ON
            led_mode_on();
#endif
            task_ctl.work_sta = TASK_EXIT;
            break;
        }

#if(SOFT_POWER_ON_OFF) //使用软开关机功能
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

    //1秒消息
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
        //task_backlight_check();
/*
        task_poweroff_check();

#if LED_STATUS_BLINK
        if(sys_ctl.led_status_blink_flag){  //低电时状态灯慢闪
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
                speaker_unmute();//FM模式下若不经过主控输出则直接mute功放即可
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
