/*****************************************************************************
* Module    : Task
* File      : task_alarm.c
* Author    : Yinglang_Li
* Email     :
* Function  : 闹钟模块
*****************************************************************************/
#include "include.h"
#if ALARM_EN
#include "task.h"
#include "task_alarm.h"


#define RES_MP3_ALARM       RES_MP3_ALARM_BELL
IAR_XDATA_A type_alarm_task t_alarm @ "CLOCK_XDATA";

//任务初始化
#pragma location="TASK_ALARM_SEG"
void task_alarm_enter(void)
{
#if LCD_THEME
    set_lcd_buf(LCD_FULL_MODE);
#endif
    set_sys_clk(SYS_48M);
    user_change_volume(sys_ctl.volume);         //闹钟音量
    dac_enable();
    music_unmute();
    t_alarm.disp_flag = 1;
    mp3_res_play(RES_MP3_ALARM);
}

//任务退出
#pragma location="TASK_ALARM_SEG"
void task_alarm_exit(void)
{
    music_stop();                               //结束Music播放
    dac_disable();
}

#pragma location="TASK_ALARM_SEG"
void task_alarm_set_rebell(void)
{
    sys_ctl.next_alarm += SECS_ALARM_NEXT;          //设置再响时间
    if (sys_ctl.next_alarm > 3155759999UL) {
        sys_ctl.alarm_switch = 0;
    }
    sys_ctl.ring_times++;

    if(sys_ctl.ring_times > ALARM_REBELL_TIMES) {
        task_alarm_set();                   //响铃多次，明天再响
    }
}

//任务事务处理
#pragma location="TASK_ALARM_SEG"
void task_alarm_event(void)
{
    u32 cur_time;
    u8 rtc_buf[5];
    WATCHDOG_CLR();
    if (music_get_status() <= STATUS_PLAY_STOPPING) {
        mp3_res_play(RES_MP3_ALARM);
    }
    music_event();
    irtcc_read_rtc(rtc_buf);
    cur_time = get_time(rtc_buf);
    make_time(cur_time);
    if (task_ctl.work_sta == TASK_ALARM) {
        if (cur_time - sys_ctl.next_alarm > SECS_ALARM_BELL) {
            task_alarm_set_rebell();
            task_ctl.work_sta = task_ctl.last_work;
        }
    }
}

//任务消息处理
#pragma location="TASK_ALARM_SEG"
void task_alarm_deal_msg(void)
{
    u8 msg = get_msg();

    if( msg == QSYSTEM_1S) {
        deal_msg(msg);
    } else if( (msg & 0xe0) == T_KEY_SHORT_UP ) {
        task_alarm_set_rebell();    //几分钟后再响
        task_ctl.work_sta = task_ctl.last_work;
    } else if( (msg & 0xe0) == T_KEY_LONG ) {
        task_alarm_set();           //明天再响
        task_ctl.work_sta = task_ctl.last_work;
    }
}

//设置闹钟
#pragma location="TASK_ALARM_SEG"
void task_alarm_set(void)
{
    u16 day;
    u32 rtc_cnt, alm_cnt;
    u8 rtc_buf[5];
    irtcc_read_rtc(rtc_buf);
    rtc_cnt = get_time(rtc_buf);    //获取当前时间，并转换为秒

    rtc_buf[2] = sys_ctl.alarm_hour;
    rtc_buf[3] = sys_ctl.alarm_min;
    rtc_buf[4] = 0;
    alm_cnt = get_time(rtc_buf);    //获取闹钟时间，并转换为秒

    day = ((u16)rtc_buf[0]<<8) + rtc_buf[1];
    if (rtc_cnt > alm_cnt) {        //每日闹钟，当天的闹钟时间已过期
        day++;
    }
    rtc_buf[0] = (u8)(day>>8);
    rtc_buf[1] = (u8)day;
    sys_ctl.next_alarm = get_time(rtc_buf);
    irtcc_write_ram32(NEXT_ALARM, sys_ctl.next_alarm);
    param_write8(PARAM_ALARM_SWITCH, sys_ctl.alarm_switch);
    if (sys_ctl.next_alarm > 3155759999UL) {
        sys_ctl.alarm_switch = 0;
    }
    sys_ctl.ring_times = 0;
}

//清除次数
#pragma location="TASK_ALARM_SEG"
void task_alarm_clear(void)
{
    sys_ctl.ring_times = 0;
}

bool task_alarm_check(void)
{
    //闹钟
    u8 rtc_buf[5];
    u32 cur_time;
    if(sys_ctl.alarm_switch) {
        irtcc_read_rtc(rtc_buf);
        cur_time = get_time(rtc_buf);
        if(cur_time - sys_ctl.next_alarm <= SECS_DIFFER_OF_ALARM) {
            task_ctl.work_sta = TASK_ALARM;
            return true;
        }
    }
    return false;
}

#pragma constseg="TASK_ALARM_CONST"
IAR_CONST char str_task_alarm[] = "task_alarm\n";
#pragma constseg=default

//任务主流程
#pragma location="TASK_ALARM_SEG"
void task_alarm(void)
{
   // printf(str_task_alarm);
    task_alarm_enter();
    while (task_ctl.work_sta == TASK_ALARM) {
        task_alarm_deal_msg();
        task_alarm_event();
        task_alarm_display();
    }
    task_alarm_exit();
}

#endif

