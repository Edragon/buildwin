/*****************************************************************************
* Module    : Task
* File      : task.h
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : 任务控制器的相关定义
*****************************************************************************/
#ifndef TASK_H
#define TASK_H

//任务的编号
enum {
    TASK_NULL,                  //无任务
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
    u8 work_sta;                //任务状态
    u8 last_work;               //上一个执行的任务
    u8 dev_change;              //设备发生变动
    u8 back_up_work_sta;        //进入系统时间显示之前的工作状态
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
//开关机函数定义
__near_func void deal_msg(u8 msg);
__near_func bool comm_event(void);

//clock相关
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
    u8 move_flag;//是否需要滑动显示文件名
    u16 name_wide;//文件名宽度
    u16 name_wide1;//文件名宽度缓存
    u8 ok_flag;
    s16 name_x;//文件名的显示位置
    u8 ms_flag;
    u8 ms3_flag;
    u8 wait;
} type_move_ctl;

extern type_t_time t_time;
extern type_move_ctl move_ctl;
void alarm_init(void);
extern void task_alarm_set(void);

typedef struct {
    u8 updata_flag;                     //显示更新标志
    u8 step;                          //调节混响叠加间距
} type_ctl_spk;
extern type_ctl_spk t_spk;

extern u8 work_sta_interrupt;          //任务中断标志

#endif
