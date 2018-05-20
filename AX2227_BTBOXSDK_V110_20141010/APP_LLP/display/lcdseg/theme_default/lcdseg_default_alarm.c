/*****************************************************************************
 * Module    : Theme
 * File      : lcdseg_default_alarm.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 闹钟任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_ALARM == LCDSEG_THEME_DEFAULT)
#include "task_alarm.h"

//任务显示处理
#pragma location="TASK_ALARM_SEG"
void task_alarm_display(void)
{

}


#endif
