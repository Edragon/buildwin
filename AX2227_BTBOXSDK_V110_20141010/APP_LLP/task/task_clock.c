/*****************************************************************************
* Module    : Task
* File      : task_clock.c
* Author    : Caiyue_Chen, Yinglang_Li, Hanny
* Email     :
* Function  : 日历
*****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_clock.h"

#if TASK_RTC_EN

//task clock controller
IAR_XDATA_A type_clock_task t_clock @ "CLOCK_XDATA";

void task_clock_event(void);


//任务初始化
#pragma location="TASK_CLOCK_SEG_1"
void task_clock_enter(void)
{
    u8 rtc_buf[5];
    set_sys_clk(SYS_12M);
    task_ctl.work_sta = TASK_CLOCK;

    irtcc_read_rtc(rtc_buf);
    make_time(get_time(rtc_buf));
    t_clock.disp_flag = 1;
    t_clock.sub_sta = STA_SHOW_CLOCK;
    t_clock.set_sta = 0;
#if DATE_EN
    t_clock.show_year = t_time.year;
    t_clock.show_month = t_time.month;
    t_clock.show_day = t_time.day;
    t_clock.show_wday = task_show_time_get_weekday(t_clock.show_year, t_clock.show_month, t_clock.show_day);
#endif

#if KEY_VOICE_EN
    dac_fade_in(1);
#endif

#if CLOCK_MODE_WARNING_MUSIC
    dac_enable();
    mp3_res_play_wait(RES_MP3_CLOCK_MODE);
    dac_disable();
#endif
#if LCD_THEME
    set_lcd_buf(LCD_FULL_MODE);
#endif
}

//任务退出
#pragma location="TASK_CLOCK_SEG_1"
void task_clock_exit(void)
{
}

//设置项++
#pragma location="TASK_CLOCK_SEG_1"
void task_clock_set_inc(void)
{
#if DATE_EN
    u8 month_day;
#endif
    switch (t_clock.set_sta) {
#if DATE_EN
    case STA_SET_TIME_YEAR:
        t_time.year++;
        if (t_time.year > 99) {             //year最大只能到99
            t_time.year = 0;
        }
        month_day = task_clock_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = month_day - 1;
        }
        break;
    case STA_SET_TIME_MONTH:
        t_time.month++;
        if (t_time.month > 11) {
            t_time.month = 0;
        }
        month_day = task_clock_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = month_day - 1;
        }
        break;
    case STA_SET_TIME_DAY:
        t_time.day++;
        month_day = task_clock_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = 0;
        }
        break;
#endif
    case STA_SET_TIME_HOUR:
        t_time.hour++;
        if (t_time.hour > 23) {
            t_time.hour = 0;
        }
        break;
    case STA_SET_TIME_MIN:
        t_time.minute++;
        if (t_time.minute > 59) {
            t_time.minute = 0;
        }
        break;
    }
}

//设置项--
#pragma location="TASK_CLOCK_SEG_1"
void task_clock_set_dec(void)
{
#if DATE_EN
    u8 month_day;
#endif
    switch (t_clock.set_sta) {
#if DATE_EN
    case STA_SET_TIME_YEAR:
        if (t_time.year == 0) {             //year最大只能到99
            t_time.year = 100;
        }
        t_time.year--;
        month_day = task_clock_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = month_day - 1;
        }
        break;
    case STA_SET_TIME_MONTH:
        if (t_time.month == 0) {
            t_time.month = 12;
        }
        t_time.month--;
        month_day = task_clock_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = month_day - 1;
        }
        break;
    case STA_SET_TIME_DAY:
        month_day = task_clock_get_month_day();
        if (t_time.day == 0) {
            t_time.day = month_day;
        }
        t_time.day--;
        break;
#endif
    case STA_SET_TIME_HOUR:
        if (t_time.hour == 0) {
            t_time.hour = 24;
        }
        t_time.hour--;
        break;
    case STA_SET_TIME_MIN:
        if (t_time.minute == 0) {
            t_time.minute = 60;
        }
        t_time.minute--;
        break;
    }
}

//时钟界面消息处理
#pragma location="TASK_CLOCK_SEG"
void task_clock_deal_msg(u8 msg)
{
    switch(msg) {
#if ALARM_EN
    case KU_PLAY:
    case KU_PLAY_S10:
        t_time.hour = sys_ctl.alarm_hour;
        t_time.minute = sys_ctl.alarm_min;
        t_clock.sub_sta = STA_SHOW_ALARM;       //显示闹钟
        t_clock.disp_flag = 1;
        break;
#endif

    case KL_PLAY:
    case KL_PLAY_S10:
        t_clock.set_sta = 0;
        t_clock.sub_sta = STA_SET_CLOCK;        //显示设置菜单
        t_clock.disp_flag = 1;
        break;

#if DATE_EN                                //日历翻页
    case K_TIME_DOWN:
        task_show_time_day_inc();
        break;

    case K_TIME_UP:
        task_show_time_day_dec();
        break;

    case KL_TIME_DOWN:
    case KH_TIME_DOWN:
        task_show_time_month_inc();
        break;

    case KL_TIME_UP:
    case KH_TIME_UP:
        task_show_time_month_dec();
        break;
#endif

    case QSYSTEM_1S:
        deal_msg(msg);
        break;
    default:
        deal_msg(msg);
        break;
    }
}

//时钟设置界面消息处理
#pragma location="TASK_CLOCK_SEG"
void task_clock_set_deal_msg(u8 msg)
{
    switch(msg) {
        //确认设置
    case KL_PLAY:
    case KL_PLAY_S10:
        t_time.second = 0;
        set_rtc_time();                         //把设定的时间写进RTC
#if ALARM_EN
        task_alarm_set();                       //重设闹钟
#endif
        t_clock.sub_sta = STA_SHOW_CLOCK;       //显示时钟
        t_clock.disp_flag = 1;
        break;

        //选择设置项
    case KU_PLAY:
    case KU_PLAY_S10:
        t_clock.set_sta++;
        if (t_clock.set_sta > STA_SET_TIME_LAST) {
            t_clock.set_sta = 0;
        }
        t_clock.disp_flag = 1;
        break;
        //调整
    case K_TIME_UP:
    case KL_TIME_UP:
    case KH_TIME_UP:
        task_clock_set_dec();
        t_clock.disp_flag = 1;
        break;

        //调整
    case K_TIME_DOWN:
    case KL_TIME_DOWN:
    case KH_TIME_DOWN:
        task_clock_set_inc();
        t_clock.disp_flag = 1;
        break;

        //取消调整
    case KU_HSF:
        t_clock.sub_sta = STA_SHOW_CLOCK;       //显示时钟
        t_clock.disp_flag = 1;
        break;

    default:
        deal_msg(msg);
        break;
    }
}

#if ALARM_EN
//闹钟界面消息处理
#pragma location="TASK_CLOCK_SEG"
void task_clock_alarm_deal_msg(u8 msg)
{
    switch(msg) {
    case KU_PLAY:
    case KU_PLAY_S10:
        t_clock.sub_sta = STA_SHOW_CLOCK;       //显示时钟
        t_clock.disp_flag = 1;
        break;

    case KU_HSF:
        if(sys_ctl.alarm_switch) {
            sys_ctl.alarm_switch = 0;
        } else {
            user_set_alarm_switch(1);
            user_set_alarm_time(t_time.hour, t_time.minute);
            task_alarm_set();
        }
        t_clock.disp_flag = 1;
        show_alarm_status();
        break;

    case KL_PLAY:
    case KL_PLAY_S10:
        t_clock.set_sta = 0;
        t_clock.sub_sta = STA_SET_ALARM;
        t_clock.disp_flag = 1;
        break;

    case QSYSTEM_1S:
        deal_msg(msg);
        break;

    default:
        deal_msg(msg);
        break;
    }
}

//闹钟设置界面消息处理
#pragma location="TASK_CLOCK_SEG"
void task_clock_alarmset_deal_msg(u8 msg)
{
    switch(msg) {
        //确认设置
    case KL_PLAY:
    case KL_PLAY_S10:
        user_set_alarm_switch(1);
        user_set_alarm_time(t_time.hour, t_time.minute);
        task_alarm_set();
        t_clock.sub_sta = STA_SHOW_ALARM;       //显示闹钟
        t_clock.disp_flag = 1;
        break;

        //选择设置项
  /*  case KU_NEXT:
    case KU_PREV:
        t_clock.set_sta = (t_clock.set_sta == STA_SET_TIME_HOUR) ? STA_SET_TIME_MIN : STA_SET_TIME_HOUR;
        t_clock.disp_flag = 1;
        break;*/
    case KU_PLAY:
    case KU_PLAY_S10:
        t_clock.set_sta = (t_clock.set_sta == STA_SET_TIME_HOUR) ? STA_SET_TIME_MIN : STA_SET_TIME_HOUR;
        t_clock.disp_flag = 1;
        break;
        //调整
    case K_TIME_UP:
    case KL_TIME_UP:
    case KH_TIME_UP:
        task_clock_set_dec();
        t_clock.disp_flag = 1;
        break;

    case K_TIME_DOWN:
    case KL_TIME_DOWN:
    case KH_TIME_DOWN:
        task_clock_set_inc();
        t_clock.disp_flag = 1;
        break;

        //取消调整
    case KU_HSF:
        t_time.hour = sys_ctl.alarm_hour;
        t_time.minute = sys_ctl.alarm_min;
        t_clock.sub_sta = STA_SHOW_ALARM;       //显示时钟
        t_clock.disp_flag = 1;
        break;

    default:
        deal_msg(msg);
        break;
    }
}
#endif

//任务主流程
#pragma location="TASK_CLOCK_SEG"
void task_clock(void)
{
    task_clock_enter();
    while (task_ctl.work_sta == TASK_CLOCK) {
        u8 msg = get_msg();
        comm_event();

#if LOWPOWER_WARNING
        if(sys_ctl.lowpower_warning_flag == 1){
            sys_ctl.lowpower_warning_flag = 2;
            dac_enable();
            mp3_res_play_wait(RES_MP3_PLEASE_CHARGE);
            dac_disable();
        }
#endif
        switch (t_clock.sub_sta) {
        case STA_SHOW_CLOCK:
            task_clock_deal_msg(msg);
            break;
        case STA_SET_CLOCK:
            task_clock_set_deal_msg(msg);
            break;
#if ALARM_EN
        case STA_SHOW_ALARM:
            task_clock_alarm_deal_msg(msg);
            break;
        case STA_SET_ALARM:
            task_clock_alarmset_deal_msg(msg);
            break;
#endif
        }
        task_clock_display();
    }
    task_clock_exit();
}

#endif
