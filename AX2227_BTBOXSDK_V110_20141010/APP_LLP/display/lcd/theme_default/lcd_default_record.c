/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_music.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 音乐播放任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_RECORD == LCD_THEME_DEFAULT)
#include "task.h"
#include "task_record.h"
void task_fm_record_display(void);

#pragma constseg="TASK_RECORD_CONST"
IAR_CONST struct {
    u16 bitrate;
    u8 icon;
} tbl_bitrate_icon[3] = {
    {64, RES_ICON_MP3_064_BMP},
    {96, RES_ICON_MP3_096_BMP},
    {128, RES_ICON_MP3_128_BMP},
};
#pragma constseg=default

//获取Bitrate的图标
#pragma location="TASK_RECORD_SEG"
u8 task_record_get_bitrate_icon(void)
{
    u8 i;
    for (i=0; i!=3; i++) {
        if (tbl_bitrate_icon[i].bitrate == mp3en_ctrl.bitrate) {
            break;
        }
    }
    return tbl_bitrate_icon[i].icon;
}

//显示顶部时间
#pragma location="TASK_RECORD_SEG"
void task_record_disp_time(void)
{
}

//显示底部状态
#pragma location="TASK_RECORD_SEG"
void task_record_disp_status(void)
{
}
//显示中间的图标
#pragma location="TASK_RECORD_SEG_1"
void task_record_disp_icon(void)
{
    line_clear();
    line_update(2);
    line_update(4);
    if(task_ctl.work_sta == TASK_AUX)
        draw_img(RES_RECORD_LINEIN_BMP, 3, 2);
    if(task_ctl.work_sta == TASK_MIC)
        draw_img(RES_RECORD_REC0_BMP, 45, 2);
}
//公共显示处理
#pragma location="TASK_RECORD_SEG"
void task_record_display_common(void)
{
}

#if AUX_FREQ_SHOW
//显示频谱
#pragma location="TASK_RECORD_SEG"
void task_record_display_disp_freq(void)
{
    if (!freq_show_update) {
        return;
    }
    freq_show_update = 0;

    if (music_freq_get()) {
        line_clear();
        disp_freq_line(0);
        music_freq_kick();
        line_update(6);
    }
}
#endif

//录音任务显示处理
#pragma location="TASK_RECORD_SEG_1"
void task_record_display_status(void)
{
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_rec.update_flag = 1;             //设置更新标志
    }
    if (!t_rec.update_flag ) {
        return;
    }
    t_rec.update_flag = 0;

    line_clear();
    //显示播放状态
   // if(t_rec.rec_sta == REC_STA_STOP)
    {
        disp_draw(RES_ICON_STOP_BMP, 0, 0);
    }
    /*else
    {
        disp_draw(RES_STATUS1_BMP, 0, 0);
    } */
    disp_draw(task_record_get_bitrate_icon(), 18, 0);

    //显示时间
    /*hour = t_rec.rec_time / 3600;
    min = (t_rec.rec_time / 60) % 60;
    sec = t_rec.rec_time % 60;*/
    disp_medium_time(0, 0, 0, 45, 0);
    if (sys_ctl.bat_val >= 1) {
      disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    line_update_clear(0);
    task_record_disp_icon();
}

//录音任务显示处理
#pragma location="TASK_RECORD_SEG"
void task_record_display(void)
{

}

//录音中任务显示处理，只能使用小显存
#pragma location="TASK_RECORD_SEG"
void task_recording_display_status(void)
{
    u8 hour, min, sec;
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_rec.update_flag = 1;             //设置更新标志
    }
    if (!t_rec.update_flag) {
        if(ocx_show_cnt < 5)
        {
            return;
        }
    }
    t_rec.update_flag = 0;

    line_clear();
    //显示播放状态
    //disp_draw(RES_ICON_REC_BMP, 0, 0);
    if(ocx_show_cnt >= 5)
    {
        move_ctl.move_flag = !move_ctl.move_flag;
        ocx_show_cnt = 0;
    }
    if(move_ctl.move_flag)
    {
        disp_draw(RES_ICON_REC_BMP, 0, 0);
    }
    disp_draw(task_record_get_bitrate_icon(), 18, 0);

    //显示时间
    hour = t_rec.rec_time / 3600;
    min = (t_rec.rec_time / 60) % 60;
    sec = t_rec.rec_time % 60;
    disp_medium_time(hour, min, sec, 45, 0);
    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    line_update(0);
}

#pragma location="TASK_RECORD_SEG"
void task_recording_display(void)
{
    if(t_rec.rec_src == RECORD_FM)
        task_fm_record_display();
    else
    {
        task_recording_display_status();
#if AUX_FREQ_SHOW
        task_record_display_disp_freq();
#endif
    }
}

//MIC任务显示处理
#pragma location="TASK_MIC_SEG"
void task_mic_display(void)
{
     task_record_display_status();
#if AUX_FREQ_SHOW
    task_record_display_disp_freq();
#endif
}
#pragma location="TASK_MIC_SEG"
void task_aux_display(void)
{
    task_mic_display();
}
#endif
