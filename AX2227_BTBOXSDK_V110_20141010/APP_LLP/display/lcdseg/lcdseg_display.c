/*****************************************************************************
 * Module    : Display
 * File      : lcdseg_display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD��������ʾ����
*****************************************************************************/
#include "include.h"
#if LCDSEG_THEME

u8 disp_freq_show_buf[16];      //��¼�ϴε���ʾֵ

//�ȴ�����
#pragma location="DRAW_IMG_SEG1"
void draw_wait(void)
{
}

//����
void draw_clear(void)
{
}


#endif
