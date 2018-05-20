/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_speaker.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 扩音器任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_SPEAKER == LCD_THEME_DEFAULT)
#include "task.h"
extern bool task_mix_step_set(u8 step);
#if AUX_FREQ_SHOW
//显示频谱
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display_disp_freq(void)
{
    if (!freq_show_update) {
        return;
    }
    freq_show_update = 0;

    if (music_freq_get()) {
        line_clear();
        disp_freq_line(0);
        music_freq_kick();
        line_update_clear(6);
    }
}
#endif

//录音任务显示处理
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display_status(void)
{
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_spk.updata_flag = 1;             //设置更新标志
    }
    if (!t_spk.updata_flag) {
        return ;
    }
    t_spk.updata_flag = 0;

    draw_clear();

    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    disp_menu(STR_MAIN_LOUD_SPEAKER,0,0, 0,ME_CENTER);
    line_update_clear(0);
   // draw_img(RES_PLAY_SPEAKER0_BMP+t_spk.icon_index, 20, 2);
}
//录音任务显示处理
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display(void)
{
    task_speaker_display_status();
#if AUX_FREQ_SHOW
    task_speaker_display_disp_freq();
#endif
}
#pragma location="TASK_SPEAKER_SEG"
void lcd_speaker_step_set(void)
{
    u8 show_time = 35;
    u8 task_temp = task_ctl.work_sta;
    u8 disp_step = 0xff;
    ocx_show_cnt = 0;
    draw_clear();
    line_clear();
    disp_menu(STR_MIX_STEP_SET,0,0, 0,ME_CENTER);
    line_update_clear(0);
    while (1) {
        if (!comm_event()) {
            return;
        }
        if (show_time && ocx_show_cnt >= show_time) {
            return;
        }
        u8 msg = get_msg();
        switch(msg) {
            //确定
        case KU_PLAY:
            return;
        case K_PREV:
        case KL_PREV:
        case KH_PREV:
            ocx_show_cnt = 0;
            task_mix_step_set(user_mixstep_dec(t_spk.step));
            break;
        case K_NEXT:
        case KL_NEXT:
        case KH_NEXT:
            ocx_show_cnt = 0;
            task_mix_step_set(user_mixstep_inc(t_spk.step));
            break;
        case QSYSTEM_1S:
            deal_msg(msg);
            break;
        default:
            if(((msg & 0x1f)!= T_KEY_PREV) && ((msg & 0x1f)!=  T_KEY_NEXT) && msg)
            {
                put_msg(msg);
                return;
            }
            else
            {
                deal_msg(msg);
                break;
            }
        }
        if(disp_step != t_spk.step)
        {
            u8 num_h = t_spk.step/10 + RES_BIGNUM_0_BMP;
            draw_img(num_h, 50, 3);
            num_h = t_spk.step%10 + RES_BIGNUM_0_BMP;
            draw_img(num_h, 50+18, 3);
            disp_step = t_spk.step;
        }
        if (task_ctl.work_sta != task_temp)
        {
            return;
        }
    }
}
#endif
