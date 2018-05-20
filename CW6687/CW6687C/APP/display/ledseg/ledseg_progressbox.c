/*****************************************************************************
 * Module    : Display
 * File      : ledseg_msgbox.c
 * Author    : Hanny
 * Function  : 进度条控件框，用于各项数值配置等
 *****************************************************************************/
#include "include.h"
#if IS_LEDSEG_DISPLAY
#include "task.h"
#pragma location="PROGRESS_SEG"
u8 progressbox(u8 value, u8 min, u8 max, u32 param, u8 mode, u8 show_time)
{
    return 0;
}
#endif
