/*****************************************************************************
 * Module    : Task
 * File      : task_clock.h
 * Author    : Hanny
 * Function  : 时钟任务的相关定义
 *****************************************************************************/
#ifndef TASK_CLOCK_H
#define TASK_CLOCK_H

#define CLOCK_CYC_ITEMS_NUM     3
#define CLOCK_WEEK_NUM          7
#define LUNAR_MONTH_NUM         12
#define LUNAR_DAY_NUM           30

#define DAYS_FOUR_YEAR          1461UL
#define DAYS_ONE_YEAR           365UL
#define SECS_PER_HOUR           3600UL

#define SECS_PER_DAY            86400UL     	//每天有多少秒
#define SECS_PER_HOUR           3600UL        	//每小时有多少秒
#define SECS_PER_MINUTE         60UL          	//每分钟有多少秒
#define YEAR_DAYS               365UL
#define FOURYEAR_DAYS           ((YEAR_DAYS * 4) + 1)

//子状态
enum {
    STA_SHOW_CLOCK,             //时钟界面
    STA_SET_CLOCK,              //时钟设置界面
    STA_SHOW_ALARM,             //闹钟界面
    STA_SET_ALARM,              //闹钟设置界面
    STA_SHOW_SETTING,           //设置菜单
};

 //调整标志
enum {
#if IS_LCD_DISPLAY
    STA_SET_TIME_YEAR,
    STA_SET_TIME_MONTH,
    STA_SET_TIME_DAY,
#endif
    STA_SET_TIME_HOUR,
    STA_SET_TIME_MIN,
    STA_SET_TIME_LAST = STA_SET_TIME_MIN,
};


typedef struct {
    u8 sub_sta;
    u8 set_sta;         //调整标志
    u8 index;           //当前菜单的位置
    u8 disp_flag;       //显示更新标志
    u32 disp_rtc;       //上次显示的RTC时间
    u8 setting_flag;    //正在设置时间标志
#if TASK_SHOWTIME_EN
    u8 back_sec;        //一段时间无操作后返回时间界面
#endif
    u8 show_year;       //日历显示的年份
    u8 show_month;      //日历显示的月份，长按上下曲可以调整
    u8 show_day;        //日历显示的天，短按上下曲可以调整
    u8 show_wday;
} type_clock_task;

extern type_clock_task t_clock;

void task_clock_display(void);
u8 task_show_time_get_month_day(u8 month, u8 year);
u8 task_show_time_get_weekday(u8 show_year, u8 show_month, u8 show_day);
void task_clock_alm_music_set(void);

#endif
