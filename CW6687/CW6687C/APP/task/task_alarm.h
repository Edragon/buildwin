/*****************************************************************************
 * Module    : Task
 * File      : task_alarm.h
 * Author    : Hanny
 * Function  : 闹钟任务的相关定义
 *****************************************************************************/
#ifndef TASK_ALARM_H
#define TASK_ALARM_H

#define SECS_ALARM_REBELL           300         //闹铃重响时间，        调试时使用，建议300
#define SECS_ALARM_BELL             60          //每次闹铃的最大时间，  调试时使用  建议60
#define ALARM_REBELL_TIMES          3           //闹铃重响次数          调试时使用  建议3
#define SECS_DIFFER_OF_ALARM        3           //闹铃判断的有效范围


typedef struct {
    u8 ring_times;                      //再响次数

    u8 disp_index;                      //当前显示的选择项，用于判断是否需要刷新
    u8 index;                           //当前的选择项
    u8 cur_state;                       //闹钟状态，0：正常闹铃 1：结束
    u8 alm_music_type;                 //闹钟音乐类型， 0：内置音乐 1：磁盘音乐
    u16 alm_music_num;                  //闹钟歌曲曲目
} type_alarm_task;

extern type_alarm_task t_alarm;


void task_alarm_display(void);

#endif
