/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_clock.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ʱ��������ʾ����
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_CLOCK == LEDSEG_THEME_DEFAULT)
#include "task.h"
#include "task_clock.h"

//ʱ�ӽ�����ʾ
#pragma location="TASK_CLOCK_SEG"
void task_clock_show_display(void)
{
    ledseg_disp(MENU_RTC_TIME);
}

//ʱ�ӽ�����ʾ
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
