/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_alarm.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 闹钟任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_ALARM == LCD_THEME_DEFAULT)
#include "task_alarm.h"
void show_time_hour_min_sec(void);
//任务显示处理
#pragma location="TASK_ALARM_SEG"
void task_alarm_display(void)
{
    if(!t_alarm.disp_flag)
       return;
    t_alarm.disp_flag = 0;
    disp_clear();
    disp_draw(RES_ICON_ALM_BMP, 0, 0);
    show_time_hour_min_sec();
    disp_update();
}


#endif
