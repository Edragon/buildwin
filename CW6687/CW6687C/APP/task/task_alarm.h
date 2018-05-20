/*****************************************************************************
 * Module    : Task
 * File      : task_alarm.h
 * Author    : Hanny
 * Function  : �����������ض���
 *****************************************************************************/
#ifndef TASK_ALARM_H
#define TASK_ALARM_H

#define SECS_ALARM_REBELL           300         //��������ʱ�䣬        ����ʱʹ�ã�����300
#define SECS_ALARM_BELL             60          //ÿ����������ʱ�䣬  ����ʱʹ��  ����60
#define ALARM_REBELL_TIMES          3           //�����������          ����ʱʹ��  ����3
#define SECS_DIFFER_OF_ALARM        3           //�����жϵ���Ч��Χ


typedef struct {
    u8 ring_times;                      //�������

    u8 disp_index;                      //��ǰ��ʾ��ѡ��������ж��Ƿ���Ҫˢ��
    u8 index;                           //��ǰ��ѡ����
    u8 cur_state;                       //����״̬��0���������� 1������
    u8 alm_music_type;                 //�����������ͣ� 0���������� 1����������
    u16 alm_music_num;                  //���Ӹ�����Ŀ
} type_alarm_task;

extern type_alarm_task t_alarm;


void task_alarm_display(void);

#endif
