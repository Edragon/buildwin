/*****************************************************************************
* Module    : UI
* File      : ui_bt.c
* Author    : Hanny
* Function  : 音乐任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//音乐任务下显示
#if IS_LEDSEG_5C7S_DISPLAY
#pragma location="TASK_MUSIC_SEG"
void task_music_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        if(t_msc.pause)
            ledseg_disp(MENU_MUSIC_PAUSE);
        else
            ledseg_disp(MENU_MUSIC_PLAY);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_MUSIC_SEG"
void task_music_display(void)
{
}
#endif

//音乐任务消息处理
#pragma location="TASK_MUSIC_SEG"
void task_music_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1)){
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        u8 pause_flag = t_msc.pause;
        music_init();
        task_music_play_init();
        music_jump(&pt);    //恢复播放
        music_play();
        t_msc.pause = pause_flag;
        if(pause_flag){   //没有暂停时才恢复播放
            music_pause();
        }
        if(!sys_ctl.volume) {
            sys_mute();
        }
        flush_msg();
        return;
    }
#endif

    if (msg == NULL) {
        return;
    }
    switch(msg) {

    /*case KU_HSF:
        //task_music_next_folder();
        task_music_select_dir(1);
        break;
    case T_KEY_STOP:
        task_music_select_dir(0);
        break;*/


        //上一曲，下一曲
    case KU_PREV:
#if AB_REPEAT_EN
        if(t_msc.repeat_flag)
        {
            if(t_msc.repeat_flag == 2)
            {
                t_msc.repeat_flag = 1;
                t_msc.disp_status_flag = 1;
            }
            else if(t_msc.repeat_flag == 3)
            {
                t_msc.repeat_flag = 1;
                t_msc.disp_status_flag = 1;
                t_msc.play_sta = PLAYER_NORMAL;
                music_get_point(&t_msc.point.pb);
                music_jump(&t_msc.point.pb);
            }
            break;
        }
#endif
        DBG_TASK_MUSIC("Prev\n");
        task_music_prev();
        break;
    case KU_NEXT:
#if AB_REPEAT_EN
        if(t_msc.repeat_flag == 1)
        {
            t_msc.repeat_flag = 2;
            t_msc.disp_status_flag = 1;
            task_music_set_a();//更新A点
            break;
        }
        else if(t_msc.repeat_flag == 2)
        {
            task_music_repeat_play();
            t_msc.repeat_flag = 3;
            t_msc.disp_status_flag = 1;
            break;
        }
        else if(t_msc.repeat_flag == 3)
        {
            t_msc.repeat_flag = 1;
            t_msc.disp_status_flag = 1;
            t_msc.play_sta = PLAYER_NORMAL;
            music_get_point(&t_msc.point.pb);
            music_jump(&t_msc.point.pb);
            break;
        }
#endif
        DBG_TASK_MUSIC("Next\n");
        task_music_next();
        break;

        //播放、暂停
    case KU_PLAY:
        if (music_start_pause()) {
            t_msc.pause = 0;                //更新显示状态
            sys_ctl.sleep_cnt = 0xffff;
#if AB_REPEAT_EN
            if (t_msc.play_sta == PLAYER_NORMAL) {
                task_music_set_a();         //正常播放模式下，重设A点
            }
#endif
            DBG_TASK_MUSIC("play\n");
            led_music_play();
        } else {
            t_msc.pause = 1;
            sys_ctl.sleep_cnt = STANDBY_TIME;        //进入休眠模式
            DBG_TASK_MUSIC("pause\n");
            led_idle();
        }
        t_msc.disp_status_flag = 1;
        break;

        //快进快退，复读模式下不支持
    case KL_PREV:
    case KH_PREV:
        if (t_msc.play_sta == PLAYER_NORMAL) {
            DBG_TASK_MUSIC("P");
            t_msc.speed = 0;                            //快进快退恢复会清空变速信息
            music_fast(MUSIC_FAST_BACKWORD, 1);            //快退
#if AB_REPEAT_EN
            task_music_set_a();
#endif
        }

        break;
    case KL_NEXT:
    case KH_NEXT:
        if (t_msc.play_sta == PLAYER_NORMAL) {
            DBG_TASK_MUSIC("N");
           // t_msc.speed = 0;                            //快进快退恢复会清空变速信息
           // music_set_speed(tbl_speed_order[t_msc.speed]);
            /*//不想快进快退自动切换下一曲，则打开此段
            if(t_msc.disp_music_time.min == t_msc.cur_time.min) {
                if((t_msc.disp_music_time.sec - t_msc.cur_time.sec) ==1) {
                    if(!t_msc.pause) {
                        music_fast_end();
                        if (!music_start_pause()) {
                            t_msc.pause = 1;
                            return;
                        }
                    }
                }
            }*/
            music_fast(MUSIC_FAST_FORWORD, 1);             //快进
#if AB_REPEAT_EN
            task_music_set_a();
#endif
        }
        break;
    case KLU_PREV:
    case KLU_NEXT:
        if (t_msc.play_sta == PLAYER_NORMAL) {
            DBG_TASK_MUSIC("E");
            music_fast_end();                           //快进快退结束
#if AB_REPEAT_EN
            task_music_set_a();                         //更新A点
#endif
            //music_set_speed(tbl_speed_order[t_msc.speed]);
          //  t_msc.speed = 0;                            //快进快退恢复会清空变速信息
           // music_set_speed(tbl_speed_order[t_msc.speed]);
        }
        break;
  /*  case KLU_PREV:
         task_music_prev_dir();
         break;
    case KLU_NEXT:
         task_music_next_dir();
         break;*/

        //音量加减
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
#if WARNING_VOL_MAXMIN
        if(!sys_ctl.volume){
            sys_unmute();
            dac_fade_out();
            music_play_vol_music();
        }
#else
        if(!sys_ctl.volume){
            sys_mute();
        }
#endif
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        sys_unmute();
#if WARNING_VOL_MAXMIN
        if(sys_ctl.volume == VOLUME_MAX){
            music_play_vol_music();
        }
#endif
        break;

#if AB_REPEAT_EN
        //AB复读
   /*case KU_SET_A:
        task_music_set_a();
        t_msc.disp_status_flag = 1;
        break;
    case KU_REPEAT:
        task_music_repeat_play();
        t_msc.disp_status_flag = 1;
        break;*/
#endif
        //短按菜单键，进入目录导航
    case KU_MODE:
#if AB_REPEAT_EN
        if(t_msc.repeat_flag)
        {
            t_msc.repeat_flag = 0;
            music_get_point(&t_msc.point.pb);
            music_jump(&t_msc.point.pb);
            t_msc.play_sta = PLAYER_NORMAL;
            t_msc.disp_status_flag = 1;
            break;
        }
#endif
        if(t_msc.mode_flag) {
            task_ctl.work_sta = TASK_EXIT;
        } else {
            if (music_next_device()) {
#if BREAK_POINT_EN
                t_msc.break_flag = 1;
                task_music_get_num();
#endif
                t_msc.file_change = 1;          //需要打开一个新文件
            }
            else
            {
                task_ctl.work_sta = TASK_EXIT;
            }
        }
        t_msc.mode_flag = 1;
        break;
    case KU_LOOP:
        if(sys_ctl.mode_play < MODE_PLAY_NUM - 1){
            sys_ctl.mode_play ++;
        }
        else{
            sys_ctl.mode_play = 0;
        }
        sys_param_write8(PARAM_MODE_PLAY, sys_ctl.mode_play);
        show_mode_play();
        break;

    case KU_EQ:
#if BT_SPP_EQ_EN
        user_def_eq();
#else
        user_set_eq(sys_ctl.eq_num+1);
        show_eq();
#endif
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
        task_music_select_numsic(msg - T_KEY_NUM_0);
        break;

    case T_KEY_LOOP:
        if(sys_ctl.mode_play < MODE_PLAY_NUM - 1){
            sys_ctl.mode_play ++;
        }
        else{
            sys_ctl.mode_play = 0;
        }
        sys_param_write8(PARAM_MODE_PLAY, sys_ctl.mode_play);
        show_mode_play();
        //printf("PM: %d\n",sys_ctl.mode_play);
        break;
    case T_KEY_EQ:
#if BT_SPP_EQ_EN
        user_def_eq();
#else
        user_set_eq(sys_ctl.eq_num+1);
        show_eq();
#endif
        break;

    case T_KEY_USBSD:
        if(is_other_device()) {
            task_music_auto_next_device();
        }
        break;
#endif

#if BT_SPP_MSC_EN
    case Q_SPP_MSC_GET_LFN:
        spp_msc_get_lfn(t_msc.music_total);
        break;

    case Q_SPP_MSC_GET_TOTAL:
        spp_tx_msc_total(t_msc.music_total);
        break;

    case Q_SPP_MSC_GET_NUM:
        spp_tx_msc_num(t_msc.music_num);
        break;
#endif

    case QSYSTEM_1S:
#if MUSIC_LRC_EN
        if (t_msc.lrc_flag && t_msc.lrc_flag != LRC_FILENAME_TIME) {
            t_msc.lrc_flag++;
            if (t_msc.lrc_flag == LRC_FILENAME_TIME) {
                if (music_lrc_first_time() <= music_get_mtime()) {
                    t_msc.lrc_change = music_get_lrc(music_lrc_first_time());
                } else {
                    t_msc.lrc_flag = LRC_FILENAME_TIME - 1;
                }
            }
        }
#endif
#if BREAK_POINT_EN
        if(!t_msc.music_voice_flag)
        {
            music_get_point(&t_msc.break_point);
            task_music_break_save();
        }
#endif
        deal_msg(msg);
        break;

    default:
        deal_msg(msg);
        break;
    }
}
#endif
