/*****************************************************************************
* Module    : Task
* File      : task_clock.c
* Author    : Caiyue_Chen, Yinglang_Li, Hanny
* Email     :
* Function  : 日历
*****************************************************************************/
#include "include.h"
#if TASK_SHOWTIME_EN
#include "task.h"


//task clock controller
struct {
    u8 disp_flag;                       //刷新显示
    u32 disp_rtc;       //上次显示的RTC时间
} t_show_time;



#pragma location="TASK_SHOW_TIME_SEG"
void task_show_time_enter()
{
    task_ctl.work_sta = TASK_SHOW_TIME;
    t_show_time.disp_rtc = 0;
    set_lcd_buf(LCD_FULL_MODE);
}

#pragma location="TASK_SHOW_TIME_SEG"
task_show_time_exit()
{
}

#pragma location="TASK_SHOW_TIME_SEG"
void task_show_time_display_hour_min_sec()
{
/*
    u8 rtc_buf[5];
    irtcc_read_rtc(rtc_buf);
    make_time(get_time(rtc_buf));
*/
    make_time(irtcc_read_rtc(READ_RTC));

    disp_num(t_time.hour, 6, 2, DN_BIG | DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_BIGNUM_COLON_BMP, 6+28+1, 2);
    disp_num(t_time.minute, 6+28+1+12+1, 2, DN_BIG| DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_BIGNUM_COLON_BMP, 6+28+1+12+1+28+1, 2);
    disp_num(t_time.second, 6+28+1+12+1+28+1+12+1, 2, DN_BIG| DN_RIGHT | DN_ZERO | 2);
}

#pragma location="TASK_SHOW_TIME_SEG"
void task_show_time_display(void)
{
/*
    u8 rtc_buf[5];
    irtcc_read_rtc(rtc_buf);
    u32 rtc_time = get_time(rtc_buf);
*/
    u32 rtc_time = irtcc_read_rtc(READ_RTC);
    if (t_show_time.disp_rtc != rtc_time) {
        t_show_time.disp_rtc = rtc_time;
        t_show_time.disp_flag = 1;
    }
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_show_time.disp_flag = 1;             //设置更新标志
    }
    if (!t_show_time.disp_flag) {
        return;
    }
    t_show_time.disp_flag = 0;

    disp_clear();
    task_show_time_display_hour_min_sec();
    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    if(sys_ctl.alarm_switch)
    {
        disp_draw(RES_MAIN_RTC0_BMP, 60, 0);
    }
    if(sys_ctl.time_fm_on)
    {
        disp_draw(RES_MAIN_RADIO0_BMP, 80, 0);
    }
    disp_update();
}

#pragma location="TASK_SHOW_TIME_SEG"
void task_show_time_deal_msg(u8 msg)
{
    deal_msg(msg);
    if (task_ctl.work_sta == TASK_POWEROFF)     // 检测到poweroff状态后直接跳出，执行关机任务
        return;
    if((msg & T_KEY_SHORT_UP) && msg!= QSYSTEM_1S)    //短按抬起键退出
      task_ctl.work_sta = task_ctl.back_up_work_sta;
}

//任务主流程
#pragma location="TASK_SHOW_TIME_SEG"
void task_show_time(void)
{
    task_show_time_enter();
    while (task_ctl.work_sta == TASK_SHOW_TIME) {
        u8 msg = get_msg();
        comm_event();
        task_show_time_deal_msg(msg);
        task_show_time_display();
    }
    task_show_time_exit();
}

#endif
