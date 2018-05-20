/*****************************************************************************
* Module    : UI
* File      : ui_speaker.c
* Author    : Hanny
* Function  : 扩音器任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//扩音器任务显示处理
#if IS_LEDSEG_5C7S_DISPLAY
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_SPEAKER);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display(void)
{
}
#endif

//任务消息处理
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_deal_msg(u8 msg)
{
    u8 mix_step = t_speaker.mix_step;

#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1)){ //恢复以前的声音
        speaker_continue();
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        flush_msg();
        return;
    }
#endif
    switch(msg) {

    case K_PREV:
    case KL_PREV:
    case KH_PREV:
        mix_step -= 2;
    case K_NEXT:
    case KL_NEXT:
    case KH_NEXT:
        mix_step++;
        if (mix_step <= ECHO_MAX) {
            t_speaker.mix_step = mix_step;
        }
        //printf("step=%02x\n", t_speaker.mix_step);
        mix_step_set(t_speaker.mix_step);
        show_num(t_speaker.mix_step);
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
#if WARNING_VOL_MAXMIN
        if(VOLUME_MAX == sys_ctl.volume){
            speaker_play_vol_music();
        }
#endif
        if(sys_ctl.mute_flag) {
            if(sys_ctl.volume) {
                sys_unmute();
            }
        }
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
#if WARNING_VOL_MAXMIN
        if(!sys_ctl.volume){
            speaker_play_vol_music();
        }
#endif
        if(sys_ctl.volume) {
            sys_unmute();
        }
        break;

    default:
        deal_msg(msg);
        break;
    }
}

#endif
