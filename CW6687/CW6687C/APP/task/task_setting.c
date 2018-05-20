/*****************************************************************************
 * Module    : Task
 * File      : task_setting.c
 * Author    : Hanny
 * Function  : 系统设置
 *****************************************************************************/
#include "include.h"

#if TASK_SETTING_EN
#include "task.h"
#include "api_spi.h"

//task template controller
IAR_XDATA_A struct {
    u8 index;                           //当前菜单的位置
    u8 sub_sta;                         //系统设置的子状态
#if SYSTEM_TIME_SET
    u8 tool_index;
    u8 tool_sub_sta;
    u8 set_sta;                         //设置系统时间的步骤
    u8 disp_flag;
#endif
} t_set @ "SYS_XDATA";

#define FRE_MAX                    1080
#define FRE_MIN                    875
enum {
    STA_SETTING_FREE,             //其他状态
    STA_SETTING_SET_CLOCK,        //时钟设置界面
    STA_SETTING_TOOL,             //工具界面
};

//工具下闹钟设置子状态
enum {
    STA_SETTING_TOOL_FREE,       //其他状态
    STA_SETTING_ALARM_TIME,       //设置闹钟时间
    STA_SETTING_FM_SET,           //定时FM设置
};

#if SYSTEM_TIME_SET
 //调整标志
enum {
    STA_SET_TIME_YEAR,
    STA_SET_TIME_MONTH,
    STA_SET_TIME_DAY,
    STA_SET_TIME_HOUR,
    STA_SET_TIME_MIN,
    STA_SET_TIME_LAST = STA_SET_TIME_MIN,
};
#endif


#ifndef RECORD_SET_EN
#define RECORD_SET_EN               0
#endif

//系统设置菜单
#if SYSTEM_TIME_SET
#if RECORD_SET_EN
#define SETTING_ITEMS_NUM           8
#else
#define SETTING_ITEMS_NUM           7
#endif
#if FM_TIME_EN
#define TOOL_ITEMS_NUM              4
#else
#define TOOL_ITEMS_NUM              3
#endif
#else
#if RECORD_SET_EN
#define SETTING_ITEMS_NUM           7
#else
#define SETTING_ITEMS_NUM           6
#endif
#if FM_TIME_EN
#define TOOL_ITEMS_NUM              3
#else
#define TOOL_ITEMS_NUM              2
#endif
#endif
#define BACKLIGHT_ITEMS_NUM         (BACKLIGHT_NUM + 1)
#define AUTOPOWER_ITEMS_NUM         6
#define REC_ITEMS_NUM               4
#define FM_TIME_ITEMS_NUM           4

bool contrast_set(u8 value);
extern u32 calc_word_addr(u32 res_addr);

#pragma constseg="PROGRESS_CONTENT"
IAR_CONST type_progress_param tbl_pg_contrast = {
    STR_MENU_CONTRAST, &contrast_set, NULL
};

#pragma constseg="LIST_CONTENT"
string IAR_CONST tbl_setting_items[SETTING_ITEMS_NUM] = {

    //STR_MENU_SYSTEM,
#if SYSTEM_TIME_SET
    STR_SYS_TIME,
#endif
    STR_MENU_BACKLIGHT,
    STR_LANGUAGE_SET,
    STR_SYS_TOOL,
    STR_MENU_CONTRAST,
//  STR_MENU_AUTOPOWER,
#ifndef RECORD_SET_EN
    STR_RECORD_SET,
#endif
    STR_MENU_FIRMWARE,
    STR_MENU_FACTORY,
};

enum {
#if SYSTEM_TIME_SET
    ITEM_SYS_TIME,
#endif
    ITEM_BACKLIGHT, ITEM_LANGUAGE, ITEM_TOOL, ITEM_CONTRAST,
    //ITEM_AUTOPOWER,
#if RECORD_SET_EN
    ITEM_RECORDSET,
#endif
    ITEM_VERSION, ITEM_FACTORY
};

#if FM_TIME_EN
string IAR_CONST tbl_fmtime_items1[FM_TIME_ITEMS_NUM] = {
    STR_FM_TIMEING_OFF,
    STR_FM_TIME,
    STR_FM_FRE_SET,
    STR_MENU_EXIT,
};
string IAR_CONST tbl_fmtime_items2[FM_TIME_ITEMS_NUM] = {
    STR_FM_TIMEING_ON,
    STR_FM_TIME,
    STR_FM_FRE_SET,
    STR_MENU_EXIT,
};
enum {
    ITEM_FM_SWITCH, ITEM_FM_TIME, ITEM_FM_FRE
};
#endif

string IAR_CONST tbl_tool_items[TOOL_ITEMS_NUM] = {
    STR_MENU_AUTOPOWER,
#if SYSTEM_TIME_SET
    STR_ALARM_SET,
#endif
#if FM_TIME_EN
    STR_FM_TIME_SET,
#endif
    STR_MENU_EXIT,
};
enum {
    ITEM_AUTOPOWER,
#if SYSTEM_TIME_SET
    ITEM_ALARM_SET,
#endif
#if FM_TIME_EN
    ITEM_FM_TIME_SET
#endif
};
//背光时间选择菜单
string IAR_CONST tbl_backlight_items[BACKLIGHT_ITEMS_NUM] = {
    STR_BACKLIGHT_SET,
    STR_BACKLIGHT_ALWAYS,
    STR_BACKLIGHT_5S,
    STR_BACKLIGHT_10S,
    STR_BACKLIGHT_15S,
    STR_BACKLIGHT_20S,
    STR_BACKLIGHT_OFF,
};

//定时关机选择菜单
string IAR_CONST tbl_autopower_items[AUTOPOWER_ITEMS_NUM] = {
    STR_MENU_AUTOPOWER,
    STR_AUTOPOWER_OFF,
    STR_AUTOPOWER_15M,
    STR_AUTOPOWER_30M,
    STR_AUTOPOWER_45M,
    STR_AUTOPOWER_60M,
};

//录音质量设置
string IAR_CONST tbl_record_set[REC_ITEMS_NUM] = {
    STR_RECORD_SET,
    STR_RECORD_LOW,
    STR_RECORD_MID,
    STR_RECORD_HIGH,
};
#pragma constseg=default

#if SYSTEM_TIME_SET
#pragma constseg="TASK_SETTING_CONST"
IAR_CONST u8 day_per_month_setting[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
#pragma constseg=default

#define ALARM_ITEMS_NUM_SETTING        4
#pragma constseg="LIST_CONTENT"
string IAR_CONST tbl_alarmon_items_setting_on[ALARM_ITEMS_NUM_SETTING] ={
    STR_ALARM_ON_SETTING,              //闹钟(开)
    STR_TIME_SET,                      //闹钟时间
    STR_ALARM_CYCLE,                   //闹钟周期
    STR_ALARM_MUSIC                    //闹钟音乐
};

#pragma constseg="LIST_CONTENT"
string IAR_CONST tbl_alarmon_items_setting_off[ALARM_ITEMS_NUM_SETTING] ={
    STR_ALARM_OFF_SETTING,              //闹钟(开)
    STR_TIME_SET,                      //闹钟时间
    STR_ALARM_CYCLE,                   //闹钟周期
    STR_ALARM_MUSIC                    //闹钟音乐
};
#pragma constseg=default

#endif

//任务初始化
#pragma location="TASK_SETTING_SEG"
void task_setting_enter(void)
{
    set_sys_clk(SYS_12M);
    task_ctl.work_sta = TASK_SETTING;

    t_set.index = 0;
    //sys_ctl.lang_id = 0;
}

//任务退出
#pragma location="TASK_SETTING_SEG"
void task_setting_exit(void)
{
}

//设置对比度
#pragma location="TASK_SETTING_SEG"
bool contrast_set(u8 value)
{
    lcd_contrast(value);
    return false;
}


#pragma location="TASK_SETTING_SEG"
bool task_setting_device_init(void)
{
    if (!device_is_actived(fs_cur_dev())) {
        return false;
    }
    if (!fs_is_mounted()) {
        return false;
    }

    return true;
}
IAR_XDATA_A string lang_buffer[40] @ "FLIST_BUF";
#pragma location="TASK_SETTING_SEG"
void task_setting_language()
{
   // u32 res_addr;
   // u32 icon_addr;
   // u8 lan_num;
   // res_addr = calc_word_addr(0);
  //  sys_ctl.lang_num = spi_read16(res_addr+12);
    //icon_addr = spi_read32(res_addr+4);
    //spi_read(lang_buffer, icon_addr, 1);
    lang_buffer[0] = STR_LANGUAGE_SET;
    for(u8 k=1;k<sys_ctl.lang_num+1;k++)
    {
        lang_buffer[k] = lang_buffer[0]+k;
    }
}


//显示频率
#pragma location="TASK_SETTING_SEG_1"
void task_set_disp_freq(u16 d_freq, u8 x, u8 line)
{

    u8 num0, num1, num2, num3;
    num0 = d_freq / 1000;
    num1 = (d_freq / 100) % 10;
    num2 = (d_freq / 10) % 10;
    num3 = d_freq % 10;
    if (num0) {
       disp_draw(RES_MEDIUMNUM_0_BMP+num0, x, line);
    }
    if (num0 || num1) {
        disp_draw(RES_MEDIUMNUM_0_BMP+num1, x+7, line);
    }
    disp_draw(RES_MEDIUMNUM_0_BMP+num2, x+14, line);
    lcd_buf[x+128+23] = 0x18;
    lcd_buf[x+128+24] = 0x18;
    disp_draw(RES_MEDIUMNUM_0_BMP + num3, x+26, line);
}
#pragma location="TASK_SETTING_SEG_1"
void task_set_fm_fre(void)
{
    u16 dis_freq = 0;
    u16 freq = param_read16(PARAM_FM_TIME_FREQ);
    freq = freq/10;
    while(1)
    {
        WATCHDOG_CLR();
        if (freq > FRE_MAX)
        {
            freq = FRE_MIN;
        }
        else if (freq < FRE_MIN)
        {
            freq = FRE_MAX;
        }
        if(dis_freq != freq)
        {
            dis_freq = freq;
            line_clear();
            line_update_clear(0);
            task_set_disp_freq(freq, 50, 0);
            /*disp_num(freq, 50, 0, DN_RIGHT);
            lcd_buf[201] = 0x18;
            lcd_buf[202] = 0x18;*/
            line_update_clear(2);
            u16 d_freq = (dis_freq-861) >> 1;
            disp_draw(RES_OCX_FMG_BMP,d_freq,0);
            disp_draw(RES_OCX_FMQT_BMP,0,1);
            line_update_clear(4);
            line_update_clear(6);
        }
        u8 msg = get_msg();
        switch(msg)
        {
        case KU_PREV:
            freq --;
            break;
        case KU_NEXT:
            freq ++;
            break;
        case KU_PLAY:
            param_write16(PARAM_FM_TIME_FREQ,freq*10);
            return;
        case KU_MENU:
            return;
        case KL_MENU:
            task_ctl.work_sta = TASK_EXIT;
            return;
        default:
            deal_msg(msg);
            break;
        }
    }
}
#pragma location="TASK_SETTING_SEG_1"
void task_set_fm_time(void)
{
   u8 time_set_flag = 0;
   t_time.hour = sys_ctl.fm_hour;
   t_time.minute = sys_ctl.fm_min;
   while(1)
   {
       u8 msg = get_msg();
       switch(msg)
       {
       case KU_PLAY:
           time_set_flag = ~time_set_flag;
           break;
       case KU_PREV:
           if(time_set_flag)
           {
               if(!t_time.hour)
                   t_time.hour = 24;
               t_time.hour--;
               break;
           }
           if(!t_time.minute)
               t_time.minute = 60;
           t_time.minute--;
           break;
       case KU_NEXT:
           if(time_set_flag)
           {
               t_time.hour++;
               if(!t_time.hour == 24)
                   t_time.hour = 0;
               break;
           }
           t_time.minute++;
           if(t_time.minute == 60)
               t_time.minute = 0;
           break;
       case KL_PLAY:
           sys_ctl.fm_hour = t_time.hour;
           sys_ctl.fm_min = t_time.minute;
           param_write8(PARAM_FM_HOUR,sys_ctl.fm_hour);
           param_write8(PARAM_FM_MIN,sys_ctl.fm_min);

           return;
       case KU_MENU:
           return;
       case KL_MENU:
           task_ctl.work_sta = TASK_EXIT;
           return;
       default:
           deal_msg(msg);
           break;
       }
       line_clear();
       disp_menu(STR_FM_TIME,0,0, 0,ME_CENTER);
       line_update_clear(0);
       line_update_clear(2);
       disp_num(t_time.hour, 35, 0, DN_RIGHT | DN_ZERO| 2);
       disp_draw(RES_MEDIUMNUM_COLON_BMP, 35+29, 0);
       disp_num(t_time.minute, 35+29+8, 0, DN_RIGHT | DN_ZERO | 2);
       if(time_set_flag)
       {
           disp_invert(35, 0, 21, 2);                          //时
       }
       else
       {
           disp_invert(35+29+8, 0, 21, 2);                //分
       }
       line_update_clear(4);
       line_update_clear(6);
   }
}

#if SYSTEM_TIME_SET

#pragma location="TASK_SETTING_SEG_1"
void task_setting_enter_set_time()
{
    /*
    u8 rtc_buf[5];
    irtcc_read_rtc(rtc_buf);
    make_time(get_time(rtc_buf));
    */
    make_time(irtcc_read_rtc(READ_RTC));
    t_set.disp_flag = 1;
    t_set.sub_sta = STA_SETTING_SET_CLOCK;
    t_set.set_sta = STA_SET_TIME_HOUR;
    set_lcd_buf(LCD_FULL_MODE);
}

//获取本月天数
#pragma location="TASK_SETTING_SEG"
u8 task_setting_get_month_day(void)
{
    u8 ajust = 0;
    if (t_time.month == 1 && t_time.year % 4 == 0) {
        //闰年二月。由于年的有效时间为2000 ~ 2099，所以省略400的判断
        ajust = 1;
    }
    return (day_per_month_setting[t_time.month] + ajust);
}

//设置项++
#pragma location="TASK_SETTING_SEG_1"
void task_setting_set_inc(void)
{
    u8 month_day;
    switch (t_set.set_sta) {
    case STA_SET_TIME_YEAR:
        t_time.year++;
        if (t_time.year > 99) {             //year最大只能到99
            t_time.year = 0;
        }
        month_day = task_setting_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = month_day - 1;
        }
        break;
    case STA_SET_TIME_MONTH:
        t_time.month++;
        if (t_time.month > 11) {
            t_time.month = 0;
        }
        month_day = task_setting_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = month_day - 1;
        }
        break;
    case STA_SET_TIME_DAY:
        t_time.day++;
        month_day = task_setting_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = 0;
        }
        break;
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
#pragma location="TASK_SETTING_SEG_1"
void task_setting_set_dec(void)
{
    u8 month_day;
    switch (t_set.set_sta) {
    case STA_SET_TIME_YEAR:
        if (t_time.year == 0) {             //year最大只能到99
            t_time.year = 100;
        }
        t_time.year--;
        month_day = task_setting_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = month_day - 1;
        }
        break;
    case STA_SET_TIME_MONTH:
        if (t_time.month == 0) {
            t_time.month = 12;
        }
        t_time.month--;
        month_day = task_setting_get_month_day();
        if (t_time.day >= month_day) {
            t_time.day = month_day - 1;
        }
        break;
    case STA_SET_TIME_DAY:
        month_day = task_setting_get_month_day();
        if (t_time.day == 0) {
            t_time.day = month_day;
        }
        t_time.day--;
        break;
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

//时钟设置界面消息处理
#pragma location="TASK_SETTING_SEG"
void task_setting_set_time_deal_msg(u8 msg)
{
    switch(msg) {
        //确认设置
    case KL_PLAY:
        t_time.second = 0;
        set_rtc_time();       //把设定的时间写进RTC
#if ALARM_EN
        task_alarm_set();    //重设闹钟
#endif
        t_set.sub_sta = STA_SETTING_FREE;       //显示时钟
        break;

        //选择设置项
    case KU_PLAY:
        t_set.set_sta++;
        if (t_set.set_sta > STA_SET_TIME_LAST) {
            t_set.set_sta = 0;
        }
        t_set.disp_flag = 1;
        break;

        //调整
    case K_TIME_UP:
    case KL_TIME_UP:
    case KH_TIME_UP:
        task_setting_set_dec();
        t_set.disp_flag = 1;
        break;

        //调整
    case K_TIME_DOWN:
    case KL_TIME_DOWN:
    case KH_TIME_DOWN:
        task_setting_set_inc();
        t_set.disp_flag = 1;
        break;

        //取消调整
#if THEME_SELECT==THEME_LCD_DEFAULT
    case KU_PLAY_POWER:
#endif
    case KU_TIME_SET:
        t_set.sub_sta = STA_SETTING_FREE;       //显示时钟
        break;

    default:
        deal_msg(msg);
        break;
    }
}

//显示时与分
#pragma location="TASK_SETTING_SEG"
void task_setting_display_hour_min(void)
{
    disp_num(t_time.hour, 24, 2, DN_BIG | DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_BIGNUM_COLON_BMP, 24+28+1, 2);
    disp_num(t_time.minute, 24+28+1+12+1, 2, DN_BIG| DN_RIGHT | DN_ZERO | 2);
}

//显示年月日
#pragma location="TASK_SETTING_SEG"
void task_setting_display_year_month_day(void)
{
    disp_num(t_time.year + 2000, 0, 6, DN_RIGHT | DN_ZERO | 4);
    disp_text((void *)"-",0+31, 6, 1);
    disp_num(t_time.month + 1,0+31+4+1, 6, DN_RIGHT | DN_ZERO | 2);
    disp_text((void *)"-",0+31+4+1+16, 6, 1);
    disp_num(t_time.day + 1, 0+32+4+1+16+4+1, 6, DN_RIGHT | DN_ZERO | 2);
}

//显示时钟的反选
#pragma location="TASK_SETTING_SEG"
void task_setting_display_clock_invert(void)
{
    switch (t_set.set_sta) {
    case STA_SET_TIME_YEAR:
        disp_invert(0+14, 6, 15, 2);                        //年
        break;
    case STA_SET_TIME_MONTH:
        disp_invert(0+31+4+1, 6, 15, 2);                    //月
        break;
    case STA_SET_TIME_DAY:
        disp_invert(0+31+4+1+16+4+1, 6, 15, 2);             //日
        break;
    case STA_SET_TIME_HOUR:
        disp_invert(24, 2, 30, 3);                          //时
        break;
    case STA_SET_TIME_MIN:
        disp_invert(24+28+1+12+1, 2, 30, 3);                //分
        break;
    }
}

//显示时钟的设置状态
#pragma location="TASK_SETTING_SEG"
void task_setting_set_time_display(void)
{
    if (!t_set.disp_flag) {
        return;
    }
    t_set.disp_flag = 0;

    disp_clear();

    disp_menu(STR_SYS_TIME,0,0, 0,ME_CENTER);
    task_setting_display_hour_min();
    task_setting_display_year_month_day();
    task_setting_display_clock_invert();
    disp_update();
}


#endif

#if SYSTEM_TIME_SET
#pragma location="TASK_SETTING_SEG_1"
void task_setting_alarm_time_deal_msg(msg)
{
    switch(msg) {
        //确认设置
    case KL_PLAY:
        user_set_alarm_time(t_time.hour, t_time.minute);
#if ALARM_EN
        task_alarm_set();
#endif
        t_set.tool_sub_sta = STA_SETTING_TOOL_FREE;   //退出当前的设置状态
        break;

        //选择设置项
  /*  case KU_NEXT:
    case KU_PREV:
        t_clock.set_sta = (t_clock.set_sta == STA_SET_TIME_HOUR) ? STA_SET_TIME_MIN : STA_SET_TIME_HOUR;
        t_clock.disp_flag = 1;
        break;*/
    case KU_PLAY:
        t_set.set_sta = (t_set.set_sta == STA_SET_TIME_HOUR) ? STA_SET_TIME_MIN : STA_SET_TIME_HOUR;
        t_set.disp_flag = 1;
        break;
        //调整
    case K_TIME_UP:
    case KL_TIME_UP:
    case KH_TIME_UP:
        task_setting_set_dec();
        t_set.disp_flag = 1;
        break;

    case K_TIME_DOWN:
    case KL_TIME_DOWN:
    case KH_TIME_DOWN:
        task_setting_set_inc();
        t_set.disp_flag = 1;
        break;

        //取消调整
#if THEME_SELECT==THEME_LCD_DEFAULT
    case KU_PLAY_POWER:
#endif
    case KU_TIME_SET:
        t_time.hour = sys_ctl.alarm_hour;
        t_time.minute = sys_ctl.alarm_min;
        t_set.tool_sub_sta = STA_SETTING_TOOL_FREE;   //退出当前的设置状态
        break;

    default:
        deal_msg(msg);
        break;
    }
}

#pragma location="TASK_SETTING_SEG_1"
void task_setting_alarm_time_display()
{
    if (!t_set.disp_flag) {
        return;
    }
    t_set.disp_flag = 0;
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
    disp_num(t_time.hour, 24, 2, DN_BIG | DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_BIGNUM_COLON_BMP, 24+28+1, 2);
    disp_num(t_time.minute, 24+28+1+12+1, 2, DN_BIG| DN_RIGHT | DN_ZERO | 2);
    task_setting_display_clock_invert();
    disp_update();
}

#pragma location="TASK_SETTING_SEG_1"
void task_setting_alarm_time_enter()
{
    t_time.hour = sys_ctl.alarm_hour;
    t_time.minute = sys_ctl.alarm_min;
    t_set.tool_sub_sta = STA_SETTING_ALARM_TIME;
    t_set.set_sta = STA_SET_TIME_HOUR;
    t_set.disp_flag = 1;
    set_lcd_buf(LCD_FULL_MODE);
}

//闹钟时间设置任务
#pragma location="TASK_SETTING_SEG_1"
void task_setting_alarm_time()
{
    task_setting_alarm_time_enter();
    while(t_set.tool_sub_sta == STA_SETTING_ALARM_TIME && task_ctl.work_sta == TASK_SETTING)
    {
        u8 msg = get_msg();
        comm_event();
        task_setting_alarm_time_deal_msg(msg);
        task_setting_alarm_time_display();
    }
}

#define CLOCK_WEEK_NUM          7
extern string IAR_CONST tbl_clock_week_checkbox[CLOCK_WEEK_NUM];
extern void task_clock_alm_music_set(void);
//闹钟时间设置任务
#pragma location="TASK_SETTING_SEG_1"
void task_system_tool_alarm_set()
{
    u8 index = 0;
    while(1)
    {
        index = listbox(index, ALARM_ITEMS_NUM_SETTING,  LB_NO_TITLE, sys_ctl.alarm_switch?(u16)tbl_alarmon_items_setting_on:(u16)tbl_alarmon_items_setting_off, LIST_TIME);
        if(index == 0)
        {
            if(sys_ctl.alarm_switch)
                user_set_alarm_switch(0);
            else
                user_set_alarm_switch(1);
        }
        else if(index == 1)
            task_setting_alarm_time();
        else if(index == 2)
        {
            u8 tmp;
#if ALM_TYPE
            tmp = listbox(sys_ctl.alarm_mode, CLOCK_CYC_ITEMS_NUM,  NULL, (u16)tbl_clock_cyc_items, LIST_TIME);
#else
            tmp = checkbox(0, CLOCK_WEEK_NUM,  LB_NO_TITLE, (u16)tbl_clock_week_checkbox, 0, sys_ctl.alarm_mode);
#endif
            if (task_ctl.work_sta != TASK_SETTING) {
                return;
            }
            user_set_alram_mode(tmp);
#if ALARM_EN
            task_alarm_set();
#endif
        }
        else if(index == 3)
        {
            task_clock_alm_music_set();
        }
        else
            return;
    }
}

#endif

#if FM_TIME_EN
#pragma location="TASK_SETTING_SEG"
void task_fm_time_set()
{
    u8 msg;
    u8 index = 0;
    while(1)
    {
        if(sys_ctl.time_fm_on)
        {
            index = listbox(index, TOOL_ITEMS_NUM, LB_NO_TITLE, (u16)tbl_fmtime_items2, 0);
        }
        else
        {
            index = listbox(index, TOOL_ITEMS_NUM, LB_NO_TITLE, (u16)tbl_fmtime_items1, 0);
        }
        switch (index)
        {
        case ITEM_FM_SWITCH:
            if(sys_ctl.time_fm_on)
            {
                sys_ctl.time_fm_on = 0;
            }
            else
                sys_ctl.time_fm_on = 1;
            param_write8(PARAM_FM_TIME,sys_ctl.time_fm_on);
            break;
        case ITEM_FM_TIME:
            task_set_fm_time();
            break;
        case ITEM_FM_FRE:
            task_set_fm_fre();
            break;
        default:
            return;
        }
        msg =get_msg();
        if(msg == KU_MENU)
        {
            return;
        }
        else if(msg == KL_MENU)
        {
            task_ctl.work_sta = TASK_EXIT;
        }
        else
        {
            deal_msg(msg);
        }
        if(task_ctl.work_sta != TASK_SETTING)
            return;
    }
}
#endif
//系统设置中工具任务
#pragma location="TASK_SETTING_SEG"
void task_system_tool_set()
{
    u8 index = 0;
    u8 temp = 0;
    t_set.sub_sta = STA_SETTING_TOOL;
    while(t_set.sub_sta == STA_SETTING_TOOL && task_ctl.work_sta == TASK_SETTING)
    {
        index = listbox(index, TOOL_ITEMS_NUM, LB_NO_TITLE, (u16)tbl_tool_items, 0);
        switch(index)
        {
        case ITEM_AUTOPOWER:
            temp = listbox(sys_ctl.poweroff, AUTOPOWER_ITEMS_NUM, NULL, (u16)tbl_autopower_items, 0);
            user_set_poweroff(temp);
            break;

#if ALARM_EN
        case ITEM_ALARM_SET:
            task_system_tool_alarm_set();
            break;
#endif

#if FM_TIME_EN
        case ITEM_FM_TIME_SET:
            task_fm_time_set();
			break;
#endif

        default:
            t_set.sub_sta = STA_SETTING_FREE;
            break;
        }
    }
}
//任务事务处理
#pragma location="TASK_SETTING_SEG"
void task_setting_event(void)
{
    u8 index;
    index = listbox(t_set.index, SETTING_ITEMS_NUM, LB_NO_TITLE, (u16)tbl_setting_items, 0);
    //printf("index = %u\n", index);
    if (task_ctl.work_sta != TASK_SETTING) {
        return;
    }
    if (index >= SETTING_ITEMS_NUM) {
        task_ctl.work_sta = TASK_EXIT;      //按取消键返回主菜单
        return;
    }
    t_set.index = index;
    switch (index) {
#if SYSTEM_TIME_SET
    case ITEM_SYS_TIME:
        task_setting_enter_set_time();
        while (t_set.sub_sta == STA_SETTING_SET_CLOCK && task_ctl.work_sta == TASK_SETTING) {
            u8 msg = get_msg();
            comm_event();
            task_setting_set_time_deal_msg(msg);
            task_setting_set_time_display();
        }
        break;
#endif
    case ITEM_TOOL:
        task_system_tool_set();
        break;
    case ITEM_LANGUAGE:
        //index = listbox(sys_ctl.lang_id, LANGUAGE_ITEMS_NUM, NULL, (u16)tbl_language_items);
        task_setting_language();
       // printf("buffer = %d\n", index);
        index = listbox(sys_ctl.lang_id, sys_ctl.lang_num+1, NULL, (u16)lang_buffer, 0);
        user_set_lang(index);
        break;

    case ITEM_CONTRAST:
        index = progressbox(sys_ctl.contrast, 0, CONTRAST_MAX, (u32)&tbl_pg_contrast, NULL, 0);
        user_set_contrast(index);
        break;

    case ITEM_BACKLIGHT:
        index = listbox(sys_ctl.backlight, BACKLIGHT_ITEMS_NUM, NULL, (u16)tbl_backlight_items, 0);
        user_set_backlight(index);
        break;


#if RECORD_SET_EN
    case ITEM_RECORDSET:
        index = listbox(sys_ctl.mode_record, REC_ITEMS_NUM, NULL, (u16)tbl_record_set, 0);
        user_set_mode_record(index);
        break;
#endif

    case ITEM_VERSION:
        set_sys_clk(SYS_24M);
        if(task_setting_device_init())
        {
            if(f_open_binfile())
            {
                if(msgbox(STR_UPDATE_FW_CONFIRM, MB_YESNO, 0))
                {
                    task_ctl.work_sta = TASK_UPDATE_FW;
                    return;
                }
                f_close();
            }
        }
        msgbox(STR_FIRMWARE_VERSION, MB_OK, 0);
        set_sys_clk(SYS_12M);
        break;

    case ITEM_FACTORY:
        if (msgbox(STR_FACTORY_CONFIRM, MB_YESNO, 0)) {
            spi_param_factory(PARAM_SYS);
            lcd_contrast(sys_ctl.contrast);
            msgbox(STR_FACTORY_SUCCESS, MB_OK, 0);
        }
        break;
    }
}

//任务主流程
#pragma location="TASK_SETTING_SEG"
void task_setting(void)
{
    //printf("task_setting\n");
    task_setting_enter();
    while (task_ctl.work_sta == TASK_SETTING) {
        task_setting_event();
    }
    task_setting_exit();
}
#endif
