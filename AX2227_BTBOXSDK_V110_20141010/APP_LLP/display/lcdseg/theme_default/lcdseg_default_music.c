/*****************************************************************************
 * Module    : Theme
 * File      : lcdseg_default_menu.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD断码屏的默认主题 - 音乐播放
 *****************************************************************************/
#include "include.h"

#if (THEME_SELECT_MUSIC == LCDSEG_THEME_DEFAULT)

//音乐任务下无显示
#pragma location="TASK_MUSIC_SEG"
void task_music_display(void)
{
}

#endif
