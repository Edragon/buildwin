/*****************************************************************************
* Module    : UI
* File      : ui_clock.c
* Author    : Hanny
* Function  : 时钟用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

#if IS_LCD_DISPLAY

#include "lcd.h"
#include "task_clock.h"
#include "user_lunar_algorithm.h"

#pragma constseg="TASK_CLOCK_CONST"
string IAR_CONST tbl_clock_week[CLOCK_WEEK_NUM] ={
    STR_SATURDAY,
    STR_SUNDAY,
    STR_MONDAY,
    STR_TUESDAY,
    STR_WEDNESDAY,
    STR_THURSDAY,
    STR_FRIDAY,
};
#endif

//时钟界面显示
#if IS_LEDSEG_7PIN_DISPLAY
#pragma location="TASK_CLOCK_SEG"
void task_clock_show_display(void)
{
    if((t_clock.sub_sta == STA_SHOW_CLOCK) || (task_ctl.work_sta == TASK_ALARM))
    {
        if(IRTCON & BIT(3))
        {
            IRTCON &= ~BIT(3);
            //u32 rtc_time = irtcc_read_rtc(READ_RTC);
            //printf("clock:%08lx\n",rtc_time);
            make_time(irtcc_read_rtc(READ_RTC));
        }
    }
    ledseg_disp(MENU_RTC_TIME);
}

//时钟界面显示
#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        task_clock_show_display();
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

#elif IS_LCD_DISPLAY

#pragma location="TASK_CLOCK_SEG"
void task_clock_display_clock_1()
{
    s8 shift;   // 英文模式下的显示偏移
    make_time(irtcc_read_rtc(READ_RTC));

    disp_num(t_clock.show_year + 2000, 0+12, 0+3, DN_RIGHT | DN_ZERO | 4);
    //disp_menu(STR_YEAR_CALENDAR, 0+31+1+12, 0, 0+3, 0);
#if(FONT_SEL == SMALL_FONT)
    disp_num(t_clock.show_month + 1,0+31+1+12+2+12-2, 0+3, DN_RIGHT | DN_ZERO | 2);
#else
    disp_num(t_clock.show_month + 1,0+31+1+12+5+12-2, 0+3, DN_RIGHT | DN_ZERO | 2);
#endif
    //disp_menu(STR_MONTH_CALENDAR, 0+31+1+12+1+16+1+12, 0, 0+3, 0);
#if(FONT_SEL == SMALL_FONT)
    disp_num(t_clock.show_day + 1, 0+32+12+1+16+12+2+12-2, 0+3, DN_RIGHT | DN_ZERO | 2);
#else
    disp_num(t_clock.show_day + 1, 0+32+12+1+16+12+5+12-2, 0+3, DN_RIGHT | DN_ZERO | 2);
#endif
    //disp_menu(STR_DAY_CALENDAR, 0+32+12+1+16+12+1+16+1+12, 0, 0+3, 0);

    disp_draw(RES_LINE_BMP, 0, 2);
    // 判断语言是英文还是中文
    if(sys_ctl.lang_id == 1) {
        shift = -3;
        disp_text((char *)"/", 0+31+1+12+3, 0+3, 0);
        disp_text((char *)"/", 0+31+1+12+1+16+1+12+3, 0+3, 0);
    }else {
        shift = 8;
        disp_menu(STR_YEAR_CALENDAR, 0+31+1+12, 0, 0+3, 0);
        disp_menu(STR_MONTH_CALENDAR, 0+31+1+12+1+16+1+12, 0, 0+3, 0);
        disp_menu(STR_DAY_CALENDAR, 0+32+12+1+16+12+1+16+1+12, 0, 0+3, 0);
    }
    //line 3~5
    disp_num(t_time.hour, 5+shift, 3+3, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, 5+14+1+shift, 3+3);
    disp_num(t_time.minute, 5+14+1+4+1+shift, 3+3, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, 5+16+1+4+1+14+1+shift, 3+3);
    disp_num(t_time.second, 5+16+1+4+1+14+1+4+1+shift, 3+3, DN_RIGHT | DN_ZERO | 2);
    disp_invert(5+shift, 3+3, 16+1+4+1+14+1+4+1+15, 2);
    disp_menu(tbl_clock_week[t_clock.show_wday], 5+16+1+4+1+14+1+4+1+14+6+shift, 0, 3+3, 0);

    disp_draw(RES_LINE_BMP, 0, 5);
}

#pragma location="TASK_CLOCK_SEG"
void task_clock_show_display(void)
{
    u32 rtc_time = irtcc_read_rtc(READ_RTC);
    if (t_clock.disp_rtc != rtc_time) {
        t_clock.disp_rtc = rtc_time;
        t_clock.disp_flag = 1;
    }
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_clock.disp_flag = 1;             //设置更新标志
    }
    if (!t_clock.disp_flag) {
        return;
    }
    t_clock.disp_flag = 0;

    disp_clear();
    task_clock_display_clock_1();
    if(sys_ctl.alarm_switch)
    {
        disp_draw(RES_MAIN_RTC0_BMP, 60, 0);
    }
    if(sys_ctl.time_fm_on)
    {
        disp_draw(RES_MAIN_RADIO0_BMP, 80, 0);
    }
    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    disp_update();
}

//闹钟界面显示
#pragma location="TASK_CLOCK_SEG"
void task_clock_alarm_display(void)
{
    if (!t_clock.disp_flag) {
        return;
    }
    t_clock.disp_flag = 0;

    disp_clear();
    disp_draw(RES_ICON_ALM_BMP, 0, 0);
#if ALM_TYPE
    switch(sys_ctl.alarm_mode) {
    case ALARM_CYC_7D:

        disp_menu(STR_ALARM_CYC_7D, 20, 0, 0, 0);
        break;
    case ALARM_CYC_5D:
        disp_menu(STR_ALARM_CYC_5D, 20, 0, 0, 0);
        break;
    }
#else
    if(sys_ctl.alarm_switch)
        disp_menu(STR_ALARM_ON, 20, 0, 0, 0);
    else
        disp_menu(STR_ALARM_OFF, 20, 0, 0, 0);
#endif
    task_clock_display_hour_min();
    if (STA_SET_ALARM == t_clock.sub_sta) {
        task_clock_display_clock_invert();
    }
    disp_update();
}

/*
#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
    switch (t_clock.sub_sta) {
    case STA_SHOW_CLOCK:
        task_clock_show_display();
#if !SYSTEM_TIME_SET
    case STA_SET_CLOCK:
//        task_clock_set_display();
        break;
#endif
#if ALARM_EN
    case STA_SHOW_ALARM:
#if !SYSTEM_TIME_SET
    case STA_SET_ALARM:
#endif
//       task_clock_alarm_display();
        break;
#endif
    }
}
*/
#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
    switch (t_clock.sub_sta) {
    case STA_SHOW_CLOCK:
        task_clock_show_display();
#if !SYSTEM_TIME_SET
    case STA_SET_CLOCK:
        task_clock_set_display();
        break;
#endif
#if ALARM_EN
    case STA_SHOW_ALARM:
#if !SYSTEM_TIME_SET
    case STA_SET_ALARM:
#endif
        task_clock_alarm_display();
        break;
#endif
    }
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
#else
#pragma location="TASK_CLOCK_SEG"
void task_clock_show_display(void)
{
}
#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
}
#endif

//时钟界面消息处理
#pragma location="TASK_CLOCK_SEG"
void task_clock_deal_msg(u8 msg)
{
   //u32 cur_time ;
    switch(msg) {
#if ALARM_EN
    case KU_PLAY:
        t_time.hour = sys_ctl.alarm_hour;
        t_time.minute = sys_ctl.alarm_min;
        t_clock.sub_sta = STA_SHOW_ALARM;       //显示闹钟
        t_clock.disp_flag = 1;
        //cur_time =  irtcc_read_rtc(READ_RTC);
        //next_alarm = irtcc_read_rtc(READ_ALM);
        //printf("cur=0x%lX\n",cur_time);
        //printf("nxaxx=0x%lX\n",next_alarm);
        break;
#endif
/*
    case KU_PLAY_POWER:
        if(sys_ctl.alarm_switch){
            user_set_alarm_switch(0);
        }else{
            user_set_alarm_switch(1);
        }
        break;
*/
#if !SYSTEM_TIME_SET
    case KL_PLAY:
        t_clock.set_sta = 0;
        t_clock.sub_sta = STA_SET_CLOCK;        //显示设置时间菜单
        t_clock.setting_flag = 0;
        show_setting_clock();
        break;
#endif

#if IS_LCD_DISPLAY
    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;
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
        t_time.second = 0;
        set_rtc_time();                         //把设定的时间写进RTC
#if ALARM_EN
        task_alarm_set();                       //重设闹钟
#endif
        t_clock.sub_sta = STA_SHOW_CLOCK;       //显示时钟
        break;

        //选择设置项
    case KU_PLAY:
        t_clock.set_sta++;
        if (t_clock.set_sta > STA_SET_TIME_LAST) {
            t_clock.set_sta = 0;
        }
        break;
/*
    case KU_PLAY_POWER:
        if(sys_ctl.alarm_switch){
            user_set_alarm_switch(0);
        }else{
            user_set_alarm_switch(1);
        }
        break;
*/
        //调整
    case K_TIME_UP:
    case KL_TIME_UP:
    case KH_TIME_UP:
    case K_PREV_VOL_DOWN:
    case KL_PREV_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        task_clock_set_dec();
        t_clock.setting_flag = 1;
        show_setting_clock();
        break;

        //调整
    case K_TIME_DOWN:
    case KL_TIME_DOWN:
    case KH_TIME_DOWN:
    case K_NEXT_VOL_UP:
    case KL_NEXT_VOL_UP:
    case KH_NEXT_VOL_UP:
        task_clock_set_inc();
        t_clock.setting_flag = 1;
        show_setting_clock();
        break;

        //取消调整
    //case KU_HSF:
    case KU_PLAY_POWER:
        t_clock.sub_sta = STA_SHOW_CLOCK;       //显示时钟
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
    case KU_PLAY_POWER:
        t_clock.sub_sta = STA_SHOW_CLOCK;       //显示时钟
        t_clock.disp_flag = 1;
        break;
/*
    case KU_PLAY_POWER:
        if(sys_ctl.alarm_switch){
            user_set_alarm_switch(0);
        }else{
            user_set_alarm_switch(1);
        }
        break;

    case KL_PLAY:
        //printf("set al\n");
        t_clock.set_sta = 0;
        t_clock.sub_sta = STA_SET_ALARM;
        t_clock.setting_flag = 0;
        show_setting_alarm();
        break;
*/
    case KU_MENU:
        task_ctl.work_sta = TASK_EXIT;
        break;
    default:
        deal_msg(msg);
    }
}

//闹钟设置界面消息处理
#pragma location="TASK_CLOCK_SEG"
void task_clock_alarmset_deal_msg(u8 msg)
{
    switch(msg) {
        //确认设置
    case KL_PLAY:
        user_set_alarm_switch(1);
        user_set_alarm_time(t_time.hour, t_time.minute);
        task_alarm_set();
        t_time.hour = sys_ctl.alarm_hour;
        t_time.minute = sys_ctl.alarm_min;
        t_clock.sub_sta = STA_SHOW_ALARM;       //显示闹钟
        t_clock.disp_flag = 1;
        break;

    case KU_PLAY_POWER:
        if(sys_ctl.alarm_switch){
            user_set_alarm_switch(0);
        }else{
            user_set_alarm_switch(1);
        }
        break;

       //选择设置项
  /*  case KU_NEXT:
    case KU_PREV:
        t_clock.set_sta = (t_clock.set_sta == STA_SET_TIME_HOUR) ? STA_SET_TIME_MIN : STA_SET_TIME_HOUR;
        t_clock.disp_flag = 1;
        break;*/
    case KU_PLAY:
        t_clock.set_sta = (t_clock.set_sta == STA_SET_TIME_HOUR) ? STA_SET_TIME_MIN : STA_SET_TIME_HOUR;
        break;
        //调整
    case K_TIME_UP:
    case KL_TIME_UP:
    case KH_TIME_UP:
        task_clock_set_dec();
        t_clock.setting_flag = 1;
        show_setting_alarm();
        break;

    case K_TIME_DOWN:
    case KL_TIME_DOWN:
    case KH_TIME_DOWN:
        task_clock_set_inc();
        t_clock.setting_flag = 1;
        show_setting_alarm();
        break;

        //取消调整
    case KU_HSF:
        t_time.hour = sys_ctl.alarm_hour;
        t_time.minute = sys_ctl.alarm_min;
        t_clock.sub_sta = STA_SHOW_ALARM;       //显示时钟
        t_clock.setting_flag = 0;
        break;

    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;
    }
}
#endif

#endif
