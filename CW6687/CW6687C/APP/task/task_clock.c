/*****************************************************************************
* Module    : Task
* File      : task_clock.c
* Author    : Caiyue_Chen, Hanny
* Email     :
* Function  : 日历
*****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_clock.h"

void task_clock_set_inc(void);
void task_clock_set_dec(void);
#define ALARM_ITEMS_NUM         3

#if ALARM_EN

#define CLOCK_ITEMS_NUM         4

#else


#define CLOCK_ITEMS_NUM         3

#endif

//#define SECS_PER_DAY            86400
#define DAYS_FOUR_YEAR          1461UL
#define DAYS_ONE_YEAR           365UL
#define SECS_PER_HOUR           3600UL

//task clock controller
IAR_XDATA_A type_clock_task t_clock @ "CLOCK_XDATA";

#include SET_USER_PATH(CFG_UI_CLOCK_DIR, /ui/ui_clock.c)

void task_clock_event(void);

#if !SYSTEM_TIME_SET

#pragma constseg="LIST_CONTENT"
u16 IAR_CONST tbl_clock_items[CLOCK_ITEMS_NUM] ={
   // STR_MAIN_CLOCK,             //标题：电子钟
    STR_TIME_SET,              //时钟设置
#if ALARM_EN
    STR_ALARM_SET,              //闹钟设置
    STR_ALARM_CYCLE,            //闹钟周期
    STR_ALARM_MUSIC,            //闹钟音乐
#endif
};
#endif

#pragma constseg="LIST_CONTENT"     // 这里必须指定位置，不然会显示乱码
string IAR_CONST tbl_alarm_music_items[3] ={
    STR_ALARM_MUSIC,            //title 闹钟音乐
    STR_ALMMUSIC_IN,           //内置音乐
    STR_ALMMUSIC_DISK,           //磁盘音乐
};

#pragma constseg="LIST_CONTENT"
string IAR_CONST tbl_alarmon_items[ALARM_ITEMS_NUM] ={
    STR_ALARM_SET,              //title 闹钟设置
    STR_ALARM_OFF,              //闹铃关闭
    STR_ALARM_ON,               //闹铃开启
};

#pragma constseg="CHECK_CONTENT"
string IAR_CONST tbl_clock_week_checkbox[CLOCK_WEEK_NUM] ={
    STR_SATURDAY,
    STR_SUNDAY,
    STR_MONDAY,
    STR_TUESDAY,
    STR_WEDNESDAY,
    STR_THURSDAY,
    STR_FRIDAY,
};

enum {
    ITEM_SETTING, ITEM_ALARM, ITEM_ALARM_CYC, ITEM_ALM_MUSIC,
};

#pragma constseg="TASK_CLOCK_CONST"

IAR_CONST u8 day_per_month[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
#pragma constseg=default

extern u16 crc_calculate(void *buf, u16 len);


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
void task_show_time_init(void)
{
    make_time(irtcc_read_rtc(READ_RTC));
#if IS_LCD_DISPLAY
    t_clock.show_year = t_time.year;
    t_clock.show_month = t_time.month;
    t_clock.show_day = t_time.day;
    t_clock.show_wday = task_show_time_get_weekday(t_clock.show_year, t_clock.show_month, t_clock.show_day);
#endif
}

//任务初始化
#pragma location="TASK_CLOCK_SEG_1"
void task_clock_enter(void)
{
#if (WARNING_TASK_CLOCK && TASK_RTC_EN)
    set_sys_clk(SYS_24M);
    if (work_sta_interrupt == TASK_NULL) {
        dac_enable();
        mp3_res_play_wait(RES_MP3_CLOCK_MODE);
        dac_disable();
    }
#endif
    set_sys_clk(SYS_12M);
    task_ctl.work_sta = TASK_CLOCK;
    t_clock.sub_sta = STA_SHOW_CLOCK;
    t_clock.set_sta = 0;
    t_clock.index = 0;
#if TASK_SHOWTIME_EN
    t_clock.back_sec = 0;
#endif
    work_sta_interrupt = TASK_NULL;
    task_show_time_init();

    //时钟下，显存比较充裕，使用大显存
    set_lcd_buf(LCD_FULL_MODE);
}

//任务退出
#pragma location="TASK_CLOCK_SEG_1"
void task_clock_exit(void)
{
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

//设置项++
#pragma location="TASK_CLOCK_SEG_1"
void task_clock_set_inc(void)
{
#if IS_LCD_DISPLAY
    u8 month_day;
#endif
    switch (t_clock.set_sta) {
#if IS_LCD_DISPLAY
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
#if IS_LCD_DISPLAY
    u8 month_day;
#endif
    switch (t_clock.set_sta) {
#if IS_LCD_DISPLAY
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

#if FILE_NAV_EN
#pragma location="TASK_CLOCK_SEG_1"
void task_clock_alm_music_set(void)
{
    u8 index;
    u16 f_index;
    u16 name_crc;
    index = listbox(NULL, 3,  NULL, (u16)tbl_alarm_music_items, LIST_TIME);
    if(index<2)
    {
        if(index == 1)
        {
            device_change();
            if (fs_need_mounted()) {
                draw_wait();
                set_sys_clk(SYS_48M);                   //提高主频，加速
                fs_init();
                set_sys_clk(SYS_24M);
            }
            if (!device_is_actived(fs_cur_dev())) {
                index = 0;
                return;
            }
            if (!fs_is_mounted()) {
                index = 0;
                return;
            }
            f_scan_set(SCAN_SUB_FOLDER, &file_filter_music, &dir_filter_music);
            if(!music_total())
            {
                index = 0;
                return;
            }
            f_index = filelist(1, FL_CDUP);
            if (f_index == 0) {
                index = 0;
                return;
            }
            else
            {

                if (!f_open_opt(f_index)) {
                    index = 0;
                    return;
                }
                f_get_opt_filename(filename);
                name_crc = crc_calculate(filename,8);
                irtcc_write_ram16(ALM_FILE_NUM, f_index);
                irtcc_write_ram16(ALM_NAME_CRC, name_crc);
            }
        }
        irtcc_write_ram8(ALM_MUSIC_TYPE, index);
    }
}
#endif

//任务事件处理
#pragma location="TASK_CLOCK_SEG"
void task_clock_event(void)
{
    comm_event();
#if WARNING_LOWPOWER
    set_sys_clk(SYS_24M);
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        dac_enable();
        mp3_res_play_wait(RES_MP3_PLEASE_CHARGE);
        dac_disable();
    }
#endif
}

#pragma constseg="TASK_CLOCK_SEG_CONST"
IAR_CONST char str_show_taskclock[] = "task_clock\n";
#pragma constseg=default
//任务主流程
#pragma location="TASK_CLOCK_SEG"
void task_clock(void)
{
    printf(str_show_taskclock);
    task_clock_enter();
    while (task_ctl.work_sta == TASK_CLOCK) {
        u8 msg = get_msg();
        task_clock_event();
        switch (t_clock.sub_sta) {
        case STA_SHOW_CLOCK:
            task_clock_deal_msg(msg);
            break;
#if !SYSTEM_TIME_SET
        case STA_SET_CLOCK:
            task_clock_set_deal_msg(msg);
            break;
#endif
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

