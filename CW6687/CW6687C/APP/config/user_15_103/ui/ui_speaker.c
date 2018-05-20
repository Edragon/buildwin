/*****************************************************************************
* Module    : UI
* File      : ui_speaker.c
* Author    : Hanny
* Function  : 扩音器任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

bool task_mix_step_set(u8 step);
#pragma constseg="PROGRESS_CONTENT"
extern IAR_CONST type_progress_param tbl_pg_mixstep = {
    STR_MIX_STEP_SET, &task_mix_step_set, NULL
};
#pragma constseg=default

struct {
    u8 updata_flag;                     //显示更新标志
    u8 step;                          //调节混响叠加间距
} t_spk;

//扩音器任务显示处理
#if IS_LEDSEG_7PIN_DISPLAY
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
#elif IS_LCD_DISPLAY
#pragma location="TASK_SPEAKER_SEG"
void speaker_picture_display(void)
{
    if(!t_speaker.picture_update_flag)
        return;
    t_speaker.picture_update_flag = 0;
    line_update(2);
    line_update(4);
    line_update(6);
    draw_img(RES_MAIN_SPEARKER3_BMP, 48, 3);
}

#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display(void)
{
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_speaker.update_flag = 1;             //设置更新标志
    }
    if (!t_speaker.update_flag) {
        return ;
    }
    t_speaker.update_flag = 0;

    line_clear();

    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    disp_menu(STR_MAIN_LOUD_SPEAKER,0,0, 0,ME_CENTER);
    line_update_clear(0);
    speaker_picture_display();
}
#else
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display(void)
{
}
#endif

#if IS_LCD_DISPLAY
#pragma location="TASK_SETTING_SEG"
bool task_mix_step_set(u8 step)
{
    if (step == t_speaker.mix_step) {
        return false;                             //没有更改
    }
    if (t_speaker.mix_step <= ECHO_MAX) {
        t_speaker.mix_step = step;
        mix_step_set(step);
    }
    return true;
}

//设置混响级数
void task_speaker_mixstep_set(void)
{
    progressbox(t_speaker.mix_step, 0, ECHO_MAX, (u32)&tbl_pg_mixstep, NULL, PROGRESS_TIME);
    t_speaker.update_flag = 1;
    t_speaker.picture_update_flag = 1;
}

//任务消息处理
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_deal_msg(u8 msg)
{

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
    case K_NEXT:
        task_speaker_mixstep_set();
        break;
    case K_VOL_DOWN:
    case K_VOL_UP:
        show_volume();
        t_speaker.update_flag = 1;
        t_speaker.picture_update_flag = 1;
        break;
    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;              //返回主菜单
        break;
        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}
#else
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
    case KU_PREV_VOL_DOWN:
        mix_step -= 2;
    case K_NEXT:
    case KL_NEXT:
    case KH_NEXT:
    case KU_NEXT_VOL_UP:
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
    case KH_NEXT_VOL_UP:
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
    case KH_PREV_VOL_DOWN:
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

#endif
