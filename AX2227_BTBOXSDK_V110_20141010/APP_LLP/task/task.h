/*****************************************************************************
* Module    : Task
* File      : task.h
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : �������������ض���
*****************************************************************************/
#ifndef TASK_H
#define TASK_H

//����ı��
enum {
    TASK_NULL,                  //������
    TASK_MENU,
    TASK_MUSIC,
    TASK_REPEATER,
    TASK_MIC,
    TASK_VOICE,
    TASK_FM,
    TASK_AUX,
    TASK_TAPE,
    TASK_LOUD_SPEAKER,
    TASK_USBDEV,
    TASK_BT,
    TASK_AUX_EQ,
    TASK_CLOCK,
    TASK_SETTING,
    TASK_TEMPLATE,
    TASK_EBOOK,
    TASK_NAV,
    TASK_UPDATE_FW,
    TASK_ALARM,
    TASK_SHOW_TIME,
    TASK_USBBT,
    TASK_HOLDMODE,
    TASK_POWEROFF,
    TASK_EXIT,
    TASK_IDLE,
};


typedef struct {
    u8 work_sta;                //����״̬
    u8 last_work;               //��һ��ִ�е�����
    u8 dev_change;              //�豸�����䶯
    u8 back_up_work_sta;        //����ϵͳʱ����ʾ֮ǰ�Ĺ���״̬
} type_task_ctl;

extern type_task_ctl task_ctl;

__near_func bool device_change(void);

extern void run_task(void);
extern void task_menu(void);
extern void task_music(void);
extern void task_bt(void);
extern void task_repeater(void);
extern void task_record(u8 record_source);
extern void task_voice(void);
extern void task_usbdev(void);
extern void task_setting(void);
extern void task_ebook(void);
extern void task_clock(void);
extern void task_fm(void);
extern void task_tape(void);
extern void task_nav(void);
extern void task_update_fw(void);
extern void task_alarm(void);
extern void task_poweroff(void);
extern void task_loud_speaker(void);
extern void task_exit(void);
extern void task_show_time(void);
extern void task_aux_eq(void);
extern void task_usbbt(void);
extern void task_idle(void);
extern void hold_mode_enter(void);
extern void task_alarm_clear(void);
extern void task_aux(void);
extern void task_mic(void);
extern void task_recplay(void);

bool task_alarm_check(void);
__near_func void task_fm_check(void);
__near_func void task_sleep_check(void);
__near_func void task_poweroff_check(void);
//���ػ���������
__near_func void deal_msg(u8 msg);
__near_func bool comm_event(void);

//clock���
#define SECS_PER_DAY            86400UL

enum ALARM_MODE{
    ALARM_CYC_7D, ALARM_CYC_5D,
};
enum TIME_WEEK {
    SATURDAY, SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY,
};

typedef struct {
    u8 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
    u8 wday;            //week_day
} type_t_time;

typedef struct {
    u8 move_flag;//�Ƿ���Ҫ������ʾ�ļ���
    u16 name_wide;//�ļ������
    u16 name_wide1;//�ļ�����Ȼ���
    u8 ok_flag;
    s16 name_x;//�ļ�������ʾλ��
    u8 ms_flag;
    u8 ms3_flag;
    u8 wait;
} type_move_ctl;

extern type_t_time t_time;
extern type_move_ctl move_ctl;
void alarm_init(void);
extern void task_alarm_set(void);

typedef struct {
    u8 updata_flag;                     //��ʾ���±�־
    u8 step;                          //���ڻ�����Ӽ��
} type_ctl_spk;
extern type_ctl_spk t_spk;

extern u8 work_sta_interrupt;          //�����жϱ�־

#endif
