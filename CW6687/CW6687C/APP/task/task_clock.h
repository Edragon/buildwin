/*****************************************************************************
 * Module    : Task
 * File      : task_clock.h
 * Author    : Hanny
 * Function  : ʱ���������ض���
 *****************************************************************************/
#ifndef TASK_CLOCK_H
#define TASK_CLOCK_H

#define CLOCK_CYC_ITEMS_NUM     3
#define CLOCK_WEEK_NUM          7
#define LUNAR_MONTH_NUM         12
#define LUNAR_DAY_NUM           30

#define DAYS_FOUR_YEAR          1461UL
#define DAYS_ONE_YEAR           365UL
#define SECS_PER_HOUR           3600UL

#define SECS_PER_DAY            86400UL     	//ÿ���ж�����
#define SECS_PER_HOUR           3600UL        	//ÿСʱ�ж�����
#define SECS_PER_MINUTE         60UL          	//ÿ�����ж�����
#define YEAR_DAYS               365UL
#define FOURYEAR_DAYS           ((YEAR_DAYS * 4) + 1)

//��״̬
enum {
    STA_SHOW_CLOCK,             //ʱ�ӽ���
    STA_SET_CLOCK,              //ʱ�����ý���
    STA_SHOW_ALARM,             //���ӽ���
    STA_SET_ALARM,              //�������ý���
    STA_SHOW_SETTING,           //���ò˵�
};

 //������־
enum {
#if IS_LCD_DISPLAY
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
    u32 disp_rtc;       //�ϴ���ʾ��RTCʱ��
    u8 setting_flag;    //��������ʱ���־
#if TASK_SHOWTIME_EN
    u8 back_sec;        //һ��ʱ���޲����󷵻�ʱ�����
#endif
    u8 show_year;       //������ʾ�����
    u8 show_month;      //������ʾ���·ݣ��������������Ե���
    u8 show_day;        //������ʾ���죬�̰����������Ե���
    u8 show_wday;
} type_clock_task;

extern type_clock_task t_clock;

void task_clock_display(void);
u8 task_show_time_get_month_day(u8 month, u8 year);
u8 task_show_time_get_weekday(u8 show_year, u8 show_month, u8 show_day);
void task_clock_alm_music_set(void);

#endif
