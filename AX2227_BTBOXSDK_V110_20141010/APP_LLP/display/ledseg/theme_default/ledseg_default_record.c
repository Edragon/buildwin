/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_music.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 音乐播放任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_RECORD == LEDSEG_THEME_DEFAULT)
#include "task.h"
#include "task_record.h"

#if RECORD_EN
//录音任务显示处理
#pragma location="TASK_RECORD_SEG"
void task_recording_display(void)
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
#endif

#if TASK_MIC_EN
//MIC任务显示处理
#pragma location="TASK_RECORD_SEG_1"
void task_mic_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_MIC);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#endif

//AUX任务显示处理
#pragma location="TASK_RECORD_SEG_1"
void task_aux_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_LINEIN);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

#endif

