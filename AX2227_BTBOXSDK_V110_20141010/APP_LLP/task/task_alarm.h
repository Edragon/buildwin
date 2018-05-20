/*****************************************************************************
 * Module    : Task
 * File      : task_alarm.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �����������ض���
 *****************************************************************************/
#ifndef TASK_ALARM_H
#define TASK_ALARM_H

#define ALARM_REBELL_TIMES          3           //�����������������3
#define SECS_ALARM_NEXT             300         //���������죬����300
#define SECS_ALARM_BELL             60          //ÿ������ʱ�䣬����60
#define SECS_DIFFER_OF_ALARM        3           //�����жϵ���Ч��Χ
typedef struct {
    u8 disp_flag;                      //�������
} type_alarm_task;

extern type_alarm_task t_alarm;

void task_alarm_display(void);

#endif
