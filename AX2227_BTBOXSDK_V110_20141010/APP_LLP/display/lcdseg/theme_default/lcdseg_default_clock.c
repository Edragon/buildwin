/*****************************************************************************
 * Module    : Theme
 * File      : lcdseg_default_clock.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 时钟任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_CLOCK == LCDSEG_THEME_DEFAULT)
#include "task.h"
#include "task_clock.h"

//时钟界面显示
#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
}
#endif
