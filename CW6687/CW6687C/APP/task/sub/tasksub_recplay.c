/*****************************************************************************
 * Module    : TaskSub
 * File      : tasksub_recplay.c
 * Author    : Hanny
 * Function  : 录音任务 - 录音播放
 *****************************************************************************/
#ifdef TASK_SUB

//任务初始化
void tasksub_recplay_enter(void)
{
    adc_exit();
    if (task_ctl.work_sta == TASK_AUX) {
        amux_stop();
    }
    ocx_clear();
    draw_wait();
    led_music_play();
   // dac_enable();
    user_change_volume(sys_ctl.volume);     //数字音量

    dac_enable();
    sys_unmute();
    t_rec.aux_mute = 0;

    t_msc.disp_music_time.sec = 0xff;
    t_msc.disp_flag = 1;
    t_msc.music_type = TYPE_WAV;
    t_msc.music_total = 1;
    t_msc.music_num = 1;
    //播放信息初始化
    t_msc.pause = 0;
    if (play_record_file()) {
        music_play();
    } else {
        //printf("file Not valid!\n");
        music_stop();
    }
    ocx_init(tasksub_recplay_event, NULL);       //设置OCX需要响应的消息及事件
}

//任务退出
void tasksub_recplay_exit(void)
{
    music_stop();
    dac_disable();
    ocx_clear();
    if (task_ctl.work_sta == TASK_MIC) {
        adc_init(SEL_RECMIC);
    } else {
        adc_init(SEL_RECAUX);
    }
}

#pragma location="TASK_MUSIC_SEG"
void tasksub_recplay_loop(void)
{
    u8 msg;
    while ((task_ctl.work_sta == TASK_MIC  || task_ctl.work_sta == TASK_AUX) && music_get_status() > STATUS_PLAY_STOPPING) {
        tasksub_recplay_event();
        while (msg = get_msg()) {
            tasksub_recplay_deal_msg(msg);
        }
        task_music_display();
    }
}

//任务主流程
void tasksub_recplay(void)
{
    tasksub_recplay_enter();
    tasksub_recplay_loop();
    tasksub_recplay_exit();
}
#endif
