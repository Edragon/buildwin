/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_clock.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 时钟任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_CLOCK == LEDSEG_THEME_DEFAULT)
#include "task.h"
#include "task_clock.h"

//时钟界面显示
#pragma location="TASK_CLOCK_SEG"
void task_clock_show_display(void)
{
    ledseg_disp(MENU_RTC_TIME);
}

//时钟界面显示
#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
    ledseg_ocx_event();
    if(t_clock.sub_sta == STA_SHOW_CLOCK) {
        u8 rtc_buf[5];
        irtcc_read_rtc(rtc_buf);
        make_time(get_time(rtc_buf));
    }
    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        task_clock_show_display();
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#endif
