#include "include.h"
#include "task_clock.h"

//#define SECS_PER_DAY            86400
#define DAYS_FOUR_YEAR          1461
#define DAYS_ONE_YEAR           365
#define SECS_PER_HOUR           3600

#if DATE_EN

#pragma constseg="TASK_CLOCK_CONST"
IAR_CONST u8 day_per_month[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
#pragma constseg=default


#pragma location="TASK_CLOCK_SEG"
u8 task_show_time_get_weekday(u8 show_year, u8 show_month, u8 show_day)
{
    u16 days;
    u8 four_year, year;
    u8 month;
    year = show_year;
    four_year = year / 4;
    year = year % 4;
    days = four_year * DAYS_FOUR_YEAR;
    days += year * DAYS_ONE_YEAR;
    for (month=0; month!=show_month; month++) {
        days += day_per_month[month];
    }
    if (year !=0 || month >= 2) {
        days++;
    }
    days += show_day;
    return (days%7);
}

//获取本月天数
#pragma location="TASK_CLOCK_SEG"
u8 task_clock_get_month_day(void)
{
    u8 ajust = 0;
    if (t_time.month == 1 && t_time.year % 4 == 0) {
        //闰年二月。由于年的有效时间为2000 ~ 2099，所以省略400的判断
        ajust = 1;
    }
    return (day_per_month[t_time.month] + ajust);
}

//获取本月天数
#pragma location="TASK_CLOCK_SEG"
u8 task_show_time_get_month_day(u8 month, u8 year)
{
    u8 ajust = 0;
    if (month == 1 && year % 4 == 0) {
        //闰年二月。由于年的有效时间为2000 ~ 2099，所以省略400的判断
        ajust = 1;
    }
    return (day_per_month[month] + ajust);
}

#pragma location="TASK_CLOCK_SEG_1"
void task_show_time_day_inc()
{
    u8 month_day;

    t_clock.show_day++;
    month_day = task_show_time_get_month_day(t_clock.show_month, t_clock.show_year);
    if (t_clock.show_day >= month_day) {
        t_clock.show_day = 0;
        t_clock.show_month++;
        if (t_clock.show_month > 11) {
            t_clock.show_month = 0;
            t_clock.show_year++;
            if (t_clock.show_year > 99) {             //year最大只能到99
                t_clock.show_year = 0;
            }
        }
    }
    t_clock.show_wday = task_show_time_get_weekday(t_clock.show_year, t_clock.show_month, t_clock.show_day);
    t_clock.disp_flag = 1;
}

#pragma location="TASK_CLOCK_SEG_1"
void task_show_time_day_dec()
{
    u8 month_day;

    if (t_clock.show_day == 0) {
        if (t_clock.show_month == 0) {
            if (t_clock.show_year == 0) {
                t_clock.show_year = 100;
            }
            t_clock.show_year--;
            t_clock.show_month = 12;
        }
        t_clock.show_month--;
        month_day = task_show_time_get_month_day(t_clock.show_month, t_clock.show_year);
        t_clock.show_day = month_day;
    }
    t_clock.show_day--;
    t_clock.show_wday = task_show_time_get_weekday(t_clock.show_year, t_clock.show_month, t_clock.show_day);

    t_clock.disp_flag = 1;
}

#pragma location="TASK_CLOCK_SEG_1"
void task_show_time_month_inc()
{
    u8 month_day;

    t_clock.show_month++;
    if (t_clock.show_month > 11) {
        t_clock.show_month = 0;
        t_clock.show_year++;
        if (t_clock.show_year > 99) {             //year最大只能到99
            t_clock.show_year = 0;
        }
    }
    month_day = task_show_time_get_month_day(t_clock.show_month, t_clock.show_year);
    if (t_clock.show_day >= month_day) {
        t_clock.show_day = month_day - 1;
    }
    t_clock.show_wday = task_show_time_get_weekday(t_clock.show_year, t_clock.show_month, t_clock.show_day);
    t_clock.disp_flag = 1;
}

#pragma location="TASK_CLOCK_SEG_1"
void task_show_time_month_dec()
{
    u8 month_day;

    if (t_clock.show_month == 0) {
        if (t_clock.show_year == 0) {
            t_clock.show_year = 100;
        }
        t_clock.show_year--;
        t_clock.show_month = 12;
    }
    t_clock.show_month--;
    month_day = task_show_time_get_month_day(t_clock.show_month, t_clock.show_year);
    if (t_clock.show_day >= month_day) {
        t_clock.show_day = month_day - 1;
    }
    t_clock.show_wday = task_show_time_get_weekday(t_clock.show_year, t_clock.show_month, t_clock.show_day);
    t_clock.disp_flag = 1;
}
#endif
