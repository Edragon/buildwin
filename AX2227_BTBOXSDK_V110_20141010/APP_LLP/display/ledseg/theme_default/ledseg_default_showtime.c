/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_showtime.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 扩音器任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_SHOWTIME == LEDSEG_THEME_DEFAULT)
#include "task.h"

#pragma location="TASK_SHOW_TIME_SEG"
void task_show_time_display(void)
{
    u8 rtc_buf[5];

    irtcc_read_rtc(rtc_buf);
    make_time(get_time(rtc_buf));

    ledseg_disp(MENU_RTC_TIME);
}
#endif
