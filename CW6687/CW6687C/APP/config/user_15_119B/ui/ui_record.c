/*****************************************************************************
* Module    : UI
* File      : ui_record.c
* Author    : Hanny
* Function  : 录音任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//录音任务显示处理
#if IS_LEDSEG_7PIN_DISPLAY
#pragma location="TASK_MIC_SEG"
void task_record_display(void)
{
    ledseg_ocx_event();
    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        if (task_ctl.work_sta == TASK_MIC) {
            ledseg_disp(MENU_MIC);
        } else if (task_ctl.work_sta == TASK_AUX) {
            ledseg_disp(MENU_AUX);
        }
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

//录音中任务显示处理
#pragma location="TASK_RECORD_SEG"
void tasksub_record_display(void)
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
#pragma location="TASK_MIC_SEG"
void task_record_display(void)
{
}
#pragma location="TASK_RECORD_SEG"
void tasksub_record_display(void)
{
}
#endif

//录音任务消息处理
#pragma location="TASK_MIC_SEG"
void task_record_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1) \
       && (task_ctl.work_sta == TASK_AUX)) //恢复以前的声音
    {
        aux_continue();
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        flush_msg();
        return;
    }
#endif
    switch(msg) {
    case KU_PLAY:
    case KU_PLAY_POWER:
        if (task_ctl.work_sta == TASK_AUX) {
            aux_mute_control();     //控制MUTE
            break;
        }
        //开始录音
    case KU_HSF:
        if (!t_rec.aux_record_flag && (task_ctl.work_sta != TASK_MIC)) {
            break;
        }
        if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
            tasksub_recording();
            if(t_rec.play_flag) {
                tasksub_recplay();//播放刚刚录下的文件
                task_record_init();
            }
        } else {
            show_no_device();
        }
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
        aux_vol_up();
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        aux_vol_down();
        break;

    case KU_MODE:
        if (t_rec.rec_sta != REC_STA_PAUSE) {
            task_ctl.work_sta = TASK_EXIT;
            break;
        }
        task_record_stop();
        tasksub_recplay();//播放刚刚录下的文件
        task_record_init();
        break;

        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}

//正在录音中任务消息处理
#pragma location="TASK_RECORD_SEG"
void tasksub_record_deal_msg(u8 msg)
{
    switch(msg) {
        //暂停录音
    case KU_PLAY:
    case KU_PLAY_POWER:
        task_record_pause();
        break;

        //停止录音
    case KU_MODE:
    case KU_HSF:
        t_rec.play_flag = 1;
        task_record_stop();
        break;

    case QSYSTEM_1S:
        t_rec.rec_time++;                           //录音计时
        if (t_rec.sync_cnt++ < REC_SYNC_CNT) {
            break;
        }
        t_rec.sync_cnt = 0;                     //同步
        if (!sync_file()) {
            //printf("file sync Fail!\n");
            adc_stop();
        }
        break;
        //调用公共消息函数
    default:
        break;
    }
}

//录音播放任务消息处理
#pragma location="TASK_MUSIC_SEG"
void tasksub_recplay_deal_msg(u8 msg)
{
    switch(msg) {
    case KU_PLAY:
    case KU_PLAY_POWER:
    case KU_MODE:
        music_stop();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
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
    case KH_PREV_VOL_DOWN:
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
