/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_menu.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD��������Ĭ������ - ���ֲ���
 *****************************************************************************/
#include "include.h"
#include "task_music.h"

#if (THEME_SELECT_MUSIC == LEDSEG_THEME_DEFAULT)

//��������������ʾ
#pragma location="TASK_MUSIC_SEG"
void task_music_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        if(u_msc.pause)
            ledseg_disp(MENU_MUSIC_PAUSE);
        else
            ledseg_disp(MENU_MUSIC_PLAY);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

#endif
