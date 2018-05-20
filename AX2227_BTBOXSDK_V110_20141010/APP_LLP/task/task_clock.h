/*****************************************************************************
 * Module    : Task
 * File      : task_clock.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 时钟任务的相关定义
 *****************************************************************************/
#ifndef TASK_CLOCK_H
#define TASK_CLOCK_H

#define CLOCK_CYC_ITEMS_NUM     3
#define CLOCK_WEEK_NUM          7
#define LUNAR_MONTH_NUM         12
#define LUNAR_DAY_NUM           30


//子状态
enum {
    STA_SHOW_CLOCK,             //时钟界面
    STA_SET_CLOCK,              //时钟设置界面
    STA_SHOW_ALARM,             //闹钟界面
    STA_SET_ALARM,              //闹钟设置界面
};

 //调整标志
enum {
#if DATE_EN
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
#if DATE_EN
    u8 show_year;       //日历显示的年份
    u8 show_month;      //日历显示的月份，长按上下曲可以调整
    u8 show_day;        //日历显示的天，短按上下曲可以调整
    u8 show_wday;
#endif
    u32 disp_rtc;
} type_clock_task;

extern type_clock_task t_clock;

void task_clock_display(void);
#endif
