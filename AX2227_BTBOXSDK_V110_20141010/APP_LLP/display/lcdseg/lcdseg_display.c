/*****************************************************************************
 * Module    : Display
 * File      : lcdseg_display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD断码屏显示函数
*****************************************************************************/
#include "include.h"
#if LCDSEG_THEME

u8 disp_freq_show_buf[16];      //记录上次的显示值

//等待界面
#pragma location="DRAW_IMG_SEG1"
void draw_wait(void)
{
}

//清屏
void draw_clear(void)
{
}


#endif
