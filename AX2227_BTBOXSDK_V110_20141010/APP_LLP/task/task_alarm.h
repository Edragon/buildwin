/*****************************************************************************
 * Module    : Task
 * File      : task_alarm.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 闹钟任务的相关定义
 *****************************************************************************/
#ifndef TASK_ALARM_H
#define TASK_ALARM_H

#define ALARM_REBELL_TIMES          3           //闹铃重响次数，建议3
#define SECS_ALARM_NEXT             300         //间隔多久再响，建议300
#define SECS_ALARM_BELL             60          //每次响铃时间，建议60
#define SECS_DIFFER_OF_ALARM        3           //闹铃判断的有效范围
typedef struct {
    u8 disp_flag;                      //再响次数
} type_alarm_task;

extern type_alarm_task t_alarm;

void task_alarm_display(void);

#endif
