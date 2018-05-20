/*****************************************************************************
 * Module    : Task
 * File      : task_clock.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ʱ���������ض���
 *****************************************************************************/
#ifndef TASK_CLOCK_H
#define TASK_CLOCK_H

#define CLOCK_CYC_ITEMS_NUM     3
#define CLOCK_WEEK_NUM          7
#define LUNAR_MONTH_NUM         12
#define LUNAR_DAY_NUM           30


//��״̬
enum {
    STA_SHOW_CLOCK,             //ʱ�ӽ���
    STA_SET_CLOCK,              //ʱ�����ý���
    STA_SHOW_ALARM,             //���ӽ���
    STA_SET_ALARM,              //�������ý���
};

 //������־
enum {
#if DATE_EN
    STA_SET_TIME_YEAR,
    STA_SET_TIME_MONTH,
    STA_SET_TIME_DAY,
#endif
    STA_SET_TIME_HOUR,
    STA_SET_TIME_MIN,
    STA_SET_TIME_LAST = STA_SET_TIME_MIN,
};


typedef struct {
    u8 sub_sta;
    u8 set_sta;         //������־
    u8 index;           //��ǰ�˵���λ��
    u8 disp_flag;       //��ʾ���±�־
#if DATE_EN
    u8 show_year;       //������ʾ�����
    u8 show_month;      //������ʾ���·ݣ��������������Ե���
    u8 show_day;        //������ʾ���죬�̰����������Ե���
    u8 show_wday;
#endif
    u32 disp_rtc;
} type_clock_task;

extern type_clock_task t_clock;

void task_clock_display(void);
#endif
