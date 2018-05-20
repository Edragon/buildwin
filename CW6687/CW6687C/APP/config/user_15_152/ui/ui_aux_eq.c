/*****************************************************************************
* Module    : UI
* File      : ui_aux_eq.c
* Author    : Hanny
* Function  : 音源输入任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//扩音器任务显示处理
#if IS_LEDSEG_5C7S_DISPLAY
#pragma location="TASK_AUX_SEG"
void task_aux_eq_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_AUX_EQ);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_AUX_SEG"
void task_aux_eq_display(void)
{
}
#endif

//任务消息处理
#pragma location="TASK_AUX_SEG"
void task_aux_eq_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1)){ //恢复以前的声音
        aux_eq_continue();
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        flush_msg();
        return;
    }
#endif

    switch(msg) {
    case KU_PLAY:
    case KU_PLAY_POWER:
        auxeq_mute_control();
        break;

        //音量加减
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
	case KH_PREV_VOL_DOWN:
        auxeq_vol_down();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
	case KH_NEXT_VOL_UP:
        auxeq_vol_up();
        break;

    case KU_HSF:
        user_set_eq(sys_ctl.eq_num+1);
        show_eq();
        break;

    default:
        deal_msg(msg);
        break;
    }
}
#endif
