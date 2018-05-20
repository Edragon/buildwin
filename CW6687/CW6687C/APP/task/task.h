/*****************************************************************************
* Module    : Task
* File      : task.h
* Author    : Hanny
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
    TASK_SPEAKER,
    TASK_USBDEV,
    TASK_BT,
    TASK_BT_HID,
    TASK_AUX_EQ,
    TASK_CLOCK,
    TASK_SETTING,
    TASK_TEMPLATE,
    TASK_EBOOK,
    TASK_NAV,
    TASK_UPDATE_FW,
    TASK_ALARM,
    TASK_TOMCAT,
    TASK_SHOW_TIME,
    TASK_HOLD,
    TASK_POWEROFF,
    TASK_EXIT,
    TASK_IDLE,
};

typedef struct {
    u8 work_sta;                //����״̬
    u8 last_work;               //��һ��ִ�е�����
    u8 work_sub;                 //������״̬
    u8 dev_change;              //�豸�����䶯
    u8 back_up_work_sta;        //����ϵͳʱ����ʾ֮ǰ�Ĺ���״̬
} type_task_ctl;

extern type_task_ctl task_ctl;

extern void run_task(void);
extern void task_menu(void);
extern void task_music(void);
extern void task_bt(void);
extern void task_bt_hid(void);
extern void task_repeater(void);
extern void task_record(void);
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
extern void task_hold(void);
extern void task_poweroff(void);
extern void task_tomcat(void);
extern void task_speaker(void);
extern void task_exit(void);
extern void task_show_time(void);
extern void task_aux_eq(void);
extern void task_idle(void);
extern void hold_mode_enter(void);
extern void task_alarm_clear(void);

__near_func bool device_change(void);
__near_func bool task_alarm_check(void);
__near_func void task_fm_check(void);
__near_func void task_sleep_check(void);
__near_func void task_poweroff_check(void);
__near_func void task_backlight_check(void);
__near_func void deal_msg(u8 msg);
bool comm_event(void);

enum ALARM_MODE{
    ALARM_CYC_7D, ALARM_CYC_5D,
};
enum TIME_WEEK {
    SATURDAY, SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY,
};

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

extern type_move_ctl move_ctl;
extern u8 work_sta_interrupt;          //�����жϱ�־

void alarm_init(void);
void task_alarm_set(void);

#endif
