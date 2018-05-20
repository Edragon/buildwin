/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_speaker.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 音乐播放任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_SPEAKER == LEDSEG_THEME_DEFAULT)
#include "task.h"

#if AUX_FREQ_SHOW
//显示频谱
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display_disp_freq(void)
{
   /* if (!freq_show_update) {
        return;
    }
    freq_show_update = 0;

    if (music_freq_get()) {
        line_clear();
        disp_freq_line(0);
        music_freq_kick();
        line_update(6);
    }*/
}
#endif

//录音任务显示处理
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display_status(void)
{
   /* if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_spk.updata_flag = 1;             //设置更新标志
    }
    if (!t_spk.updata_flag) {
        return ;
    }
    t_spk.updata_flag = 0;

    line_clear();

    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    disp_menu(STR_MAIN_LOUD_SPEAKER,0,0, 0,ME_CENTER);
    line_update_clear(0);
    line_update(2);
    line_update(4);
   // draw_img(RES_PLAY_SPEAKER0_BMP+t_spk.icon_index, 20, 2);*/
}

//录音任务显示处理
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display(void)
{
  /*  task_speaker_display_status();
#if AUX_FREQ_SHOW
    task_speaker_display_disp_freq();
#endif*/

    if(!ledseg_ocx_event()) {
        return;
    }

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_MIX);
        break;
    case LEDSEG_OCX_MIX_STEP:
        ledseg_disp(MENU_MIX_STEP);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

#endif

