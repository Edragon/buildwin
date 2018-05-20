/*****************************************************************************
* Module    : UI
* File      : ui_aux_eq.c
* Author    : Hanny
* Function  : 音源输入任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//扩音器任务显示处理
#if IS_LEDSEG_7PIN_DISPLAY
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

#elif IS_LCD_DISPLAY
#pragma location="TASK_AUX_SEG"
void task_aux_eq_display(void)
{
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_auxeq.updata_flag = 1;             //设置更新标志
    }
    if (!t_auxeq.updata_flag) {
        return;
    }
    t_auxeq.updata_flag = 0;
    //line_clear();
/*
    // 清除"请稍候"字样
    line_clear();
    line_update(2);
    line_update(4);
*/
    // 画麦克风大图标
    draw_img(RES_RECORD_LINEIN_BMP, 3, 3);
    line_clear();

    if (t_auxeq.play_sta == AUXEQ_STA_PAUSE) {
        disp_draw(RES_ICON_REC_BMP, 0, 0);
    }else if (t_auxeq.play_sta == AUXEQ_STA_PLAY) {
        disp_draw(RES_STATUS1_BMP, 0, 0);
    }

    disp_draw(RES_PLAY_EQ_0_BMP + sys_ctl.eq_num, 16+13+29, 0);
    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }

    line_update(0);
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
    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;
    case KU_PLAY:
        auxeq_mute_control();
        if(t_auxeq.play_sta == AUXEQ_STA_PAUSE){
            t_auxeq.play_sta = AUXEQ_STA_PLAY;
            t_auxeq.updata_flag = 1;
        }else{
            t_auxeq.play_sta = AUXEQ_STA_PAUSE;
            t_auxeq.updata_flag = 1;
        }
        break;

        //音量加减
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
	case KH_PREV_VOL_DOWN:
        show_volume();
        t_auxeq.updata_flag = 1;
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
	case KH_NEXT_VOL_UP:
        show_volume();
        t_auxeq.updata_flag = 1;
        break;

    case KU_NEXT:
        user_set_eq(sys_ctl.eq_num+1);
        t_auxeq.updata_flag = 1;
        break;

    case KU_PREV:
        if(sys_ctl.eq_num){
            user_set_eq(sys_ctl.eq_num-1);
        }else{
            user_set_eq(MUSIC_EQ_NUM-1);
        }
        t_auxeq.updata_flag = 1;
        break;

    default:
        deal_msg(msg);
        break;
    }
}
#endif
