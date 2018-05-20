/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_clock.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 时钟任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_CLOCK == LCD_THEME_DEFAULT)
#if 1
#include "task.h"
#include "task_clock.h"

void Conversion_lunar(u8 year,u8 month,u8 day);

/*#pragma constseg="LIST_CONTENT"
string IAR_CONST tbl_clock_cyc_items[CLOCK_CYC_ITEMS_NUM] ={
    STR_ALARM_CYCLE,            //title 闹钟周期
    STR_ALARM_CYC_7D,           //每天
    STR_ALARM_CYC_5D,           //工作日
};*/

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

#pragma constseg="TASK_CLOCK_CONST"
string IAR_CONST tbl_lunar_month[LUNAR_MONTH_NUM] ={
    STR_LUNAR_MONTH_1,
    STR_LUNAR_MONTH_2,
    STR_LUNAR_MONTH_3,
    STR_LUNAR_MONTH_4,
    STR_LUNAR_MONTH_5,
    STR_LUNAR_MONTH_6,
    STR_LUNAR_MONTH_7,
    STR_LUNAR_MONTH_8,
    STR_LUNAR_MONTH_9,
    STR_LUNAR_MONTH_10,
    STR_LUNAR_MONTH_11,
    STR_LUNAR_MONTH_12
};

#pragma constseg="TASK_CLOCK_CONST"
string IAR_CONST tbl_lunar_day[LUNAR_DAY_NUM] ={
    STR_LUNAR_DAY_1, STR_LUNAR_DAY_2, STR_LUNAR_DAY_3,
    STR_LUNAR_DAY_4, STR_LUNAR_DAY_5, STR_LUNAR_DAY_6,
    STR_LUNAR_DAY_7, STR_LUNAR_DAY_8, STR_LUNAR_DAY_9,
    STR_LUNAR_DAY_10, STR_LUNAR_DAY_11, STR_LUNAR_DAY_12,
    STR_LUNAR_DAY_13, STR_LUNAR_DAY_14, STR_LUNAR_DAY_15,
    STR_LUNAR_DAY_16, STR_LUNAR_DAY_17, STR_LUNAR_DAY_18,
    STR_LUNAR_DAY_19, STR_LUNAR_DAY_20, STR_LUNAR_DAY_21,
    STR_LUNAR_DAY_22, STR_LUNAR_DAY_23, STR_LUNAR_DAY_24,
    STR_LUNAR_DAY_25, STR_LUNAR_DAY_26, STR_LUNAR_DAY_27,
    STR_LUNAR_DAY_28, STR_LUNAR_DAY_29, STR_LUNAR_DAY_30
};

#pragma constseg=default

void task_clock_display_clock_invert(void);


//显示时与分
#pragma location="TASK_CLOCK_SEG"
void task_clock_display_hour_min(void)
{
    disp_num(t_time.hour, 24, 2, DN_BIG | DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_BIGNUM_COLON_BMP, 24+28+1, 2);
    disp_num(t_time.minute, 24+28+1+12+1, 2, DN_BIG| DN_RIGHT | DN_ZERO | 2);
}

//显示年月日
#pragma location="TASK_CLOCK_SEG"
void task_clock_display_year_month_day(void)
{
    disp_num(t_time.year + 2000, 0, 6, DN_RIGHT | DN_ZERO | 4);
    disp_text((void *)"-",0+31, 6, 1);
    disp_num(t_time.month + 1,0+31+4+1, 6, DN_RIGHT | DN_ZERO | 2);
    disp_text((void *)"-",0+31+4+1+16, 6, 1);
    disp_num(t_time.day + 1, 0+32+4+1+16+4+1, 6, DN_RIGHT | DN_ZERO | 2);
}

//显示时钟的设置状态
#pragma location="TASK_CLOCK_SEG"
void task_clock_set_display(void)
{
    if (!t_clock.disp_flag) {
        return;
    }
    t_clock.disp_flag = 0;

    disp_clear();

    disp_menu(STR_SYS_TIME,0,0, 0,ME_CENTER);
    task_clock_display_hour_min();
#if DATE_EN
    task_clock_display_year_month_day();
#endif
    task_clock_display_clock_invert();
    disp_update();
}

typedef struct {
    s8 year;
    u8 month;
    u8 day;
} type_t_lunar_time;
extern type_t_lunar_time t_lunar_time;
//显示时钟，show_second为是否显示秒
#pragma location="TASK_CLOCK_SEG"
void task_clock_display_clock(u8 show_second)
{
    u8 rtc_buf[5];
    irtcc_read_rtc(rtc_buf);
    make_time(get_time(rtc_buf));

    //line 3~5
    task_clock_display_hour_min();
    if (show_second) {
        disp_num(t_time.second, 20+30+2+12+2+30+2, 3, DN_RIGHT | DN_ZERO | 2);
    }
    //line 6~7
    task_clock_display_year_month_day();
    disp_menu(tbl_clock_week[t_time.wday], 128-14*3-13, 0, 6, 0);
#if ALARM_EN
    /*if(sys_ctl.alarm_switch){
        disp_draw(RES_ICON_ALM_BMP, 0, 0);
        make_time(next_alarm);

        disp_num(t_time.month + 1, 20, 0, DN_SMALL | DN_RIGHT | DN_ZERO | 2);
        disp_num(t_time.day + 1, 20+14, 0, DN_SMALL | DN_RIGHT | DN_ZERO | 2);

        disp_num(t_time.hour, 20+14+14+14, 0, DN_SMALL | DN_RIGHT | DN_ZERO | 2);
        disp_draw(RES_SMALLNUM_COLON_BMP, 20+14+14+14+14+1, 0);
        disp_num(t_time.minute, 20+14+14+14+14+1+4+1, 0, DN_SMALL | DN_RIGHT | DN_ZERO | 2);
    }*/
#endif
}

//显示时钟的反选
#pragma location="TASK_CLOCK_SEG"
void task_clock_display_clock_invert(void)
{
    switch (t_clock.set_sta) {
#if DATE_EN
    case STA_SET_TIME_YEAR:
        disp_invert(0+14, 6, 15, 2);                        //年
        break;
    case STA_SET_TIME_MONTH:
        disp_invert(0+31+4+1, 6, 15, 2);                    //月
        break;
    case STA_SET_TIME_DAY:
        disp_invert(0+31+4+1+16+4+1, 6, 15, 2);             //日
        break;
#endif
    case STA_SET_TIME_HOUR:
        disp_invert(24, 2, 30, 3);                          //时
        break;
    case STA_SET_TIME_MIN:
        disp_invert(24+28+1+12+1, 2, 30, 3);                //分
        break;
    }
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
    if(sys_ctl.alarm_switch)
    {
        disp_menu(STR_ALARM_ON, 20, 0, 0, 0);
    }
    else
        disp_menu(STR_ALARM_OFF, 20, 0, 0, 0);
    task_clock_display_hour_min();
    if (STA_SET_ALARM == t_clock.sub_sta) {
        task_clock_display_clock_invert();
    }
    disp_update();
}
#pragma location="TASK_CLOCK_SEG"
void show_time_hour_min_sec(void)
{
    disp_num(t_time.hour, 6, 2, DN_BIG | DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_BIGNUM_COLON_BMP, 6+28+1, 2);
    disp_num(t_time.minute, 6+28+1+12+1, 2, DN_BIG| DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_BIGNUM_COLON_BMP, 6+28+1+12+1+28+1, 2);
    disp_num(t_time.second, 6+28+1+12+1+28+1+12+1, 2, DN_BIG| DN_RIGHT | DN_ZERO | 2);
}
#pragma location="TASK_CLOCK_SEG"
void task_clock_display_clock_1(void)
{
    /*u8 rtc_buf[5];
    irtcc_read_rtc(rtc_buf);
    make_time(get_time(rtc_buf));*/
#if 0
#if 0      //

    disp_num(t_time.year + 2000, 0, 0, DN_RIGHT | DN_ZERO | 4);
    disp_menu(STR_YEAR_CALENDAR, 0+31+1, 0, 0, 0);
#if(FONT_SEL == SMALL_FONT)
    disp_num(t_time.month + 1,0+31+1+12+2, 0, DN_RIGHT | DN_ZERO | 2);
#else
    disp_num(t_time.month + 1,0+31+1+12+5, 0, DN_RIGHT | DN_ZERO | 2);
#endif
    disp_menu(STR_MONTH_CALENDAR, 0+31+1+12+1+16+1, 0, 0, 0);
#if(FONT_SEL == SMALL_FONT)
    disp_num(t_time.day + 1, 0+32+12+1+16+12+2, 0, DN_RIGHT | DN_ZERO | 2);
#else
    disp_num(t_time.day + 1, 0+32+12+1+16+12+5, 0, DN_RIGHT | DN_ZERO | 2);
#endif
    disp_menu(STR_DAY_CALENDAR, 0+32+12+1+16+12+1+16+1, 0, 0, 0);

    disp_draw(RES_LINE_BMP, 0, 2);

    //line 3~5
    disp_num(t_time.hour, 5, 3, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, 5+14+1, 3);
    disp_num(t_time.minute, 5+14+1+4+1, 3, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, 5+16+1+4+1+14+1, 3);
    disp_num(t_time.second, 5+16+1+4+1+14+1+4+1, 3, DN_RIGHT | DN_ZERO | 2);
    disp_invert(5, 3, 16+1+4+1+14+1+4+1+15, 2);
    disp_menu(tbl_clock_week[t_time.wday], 5+16+1+4+1+14+1+4+1+14+6, 0, 3, 0);

    disp_draw(RES_LINE_BMP, 0, 5);

#if USE_LUNAR
    Conversion_lunar(t_time.year, t_time.month + 1, t_time.day + 1);
    disp_num(t_lunar_time.year + 2000, 0, 6, DN_RIGHT | DN_ZERO | 4);
    disp_menu(tbl_lunar_month[t_lunar_time.month - 1], 0+31, 0, 6, 0);
    disp_menu(tbl_lunar_day[t_lunar_time.day - 1], 0+31+50, 0, 6, 0);
#endif

#else

    disp_num(t_clock.show_year + 2000, 0, 0, DN_RIGHT | DN_ZERO | 4);
    disp_menu(STR_YEAR_CALENDAR, 0+31+1, 0, 0, 0);
#if(FONT_SEL == SMALL_FONT)
    disp_num(t_clock.show_month + 1,0+31+1+12+2, 0, DN_RIGHT | DN_ZERO | 2);
#else
    disp_num(t_clock.show_month + 1,0+31+1+12+5, 0, DN_RIGHT | DN_ZERO | 2);
#endif
    disp_menu(STR_MONTH_CALENDAR, 0+31+1+12+1+16+1, 0, 0, 0);
#if(FONT_SEL == SMALL_FONT)
    disp_num(t_clock.show_day + 1, 0+32+12+1+16+12+2, 0, DN_RIGHT | DN_ZERO | 2);
#else
    disp_num(t_clock.show_day + 1, 0+32+12+1+16+12+5, 0, DN_RIGHT | DN_ZERO | 2);
#endif
    disp_menu(STR_DAY_CALENDAR, 0+32+12+1+16+12+1+16+1, 0, 0, 0);

    disp_draw(RES_LINE_BMP, 0, 2);

    //line 3~5
    disp_num(t_time.hour, 5, 3, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, 5+14+1, 3);
    disp_num(t_time.minute, 5+14+1+4+1, 3, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, 5+16+1+4+1+14+1, 3);
    disp_num(t_time.second, 5+16+1+4+1+14+1+4+1, 3, DN_RIGHT | DN_ZERO | 2);
    disp_invert(5, 3, 16+1+4+1+14+1+4+1+15, 2);
    disp_menu(tbl_clock_week[t_clock.show_wday], 5+16+1+4+1+14+1+4+1+14+6, 0, 3, 0);

    disp_draw(RES_LINE_BMP, 0, 5);

#if USE_LUNAR
    Conversion_lunar(t_clock.show_year, t_clock.show_month + 1, t_clock.show_day + 1);
    disp_num(t_lunar_time.year + 2000, 0, 6, DN_RIGHT | DN_ZERO | 4);
    disp_menu(tbl_lunar_month[t_lunar_time.month - 1], 0+31, 0, 6, 0);
    disp_menu(tbl_lunar_day[t_lunar_time.day - 1], 0+31+50, 0, 6, 0);
#endif
#endif
#else
    show_time_hour_min_sec();
#endif
}

//时钟界面显示
#pragma location="TASK_CLOCK_SEG"
void task_clock_show_display(void)
{
    u8 rtc_buf[5];
    irtcc_read_rtc(rtc_buf);
    u32 rtc_time = get_time(rtc_buf);
    if (t_clock.disp_rtc != rtc_time) {
        t_clock.disp_rtc = rtc_time;
        t_clock.disp_flag = 1;
        make_time(rtc_time);
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
    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    disp_update();
}
#endif
//时钟界面显示
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
    case STA_SET_ALARM:
        task_clock_alarm_display();
        break;
#endif
    }
}

#endif
