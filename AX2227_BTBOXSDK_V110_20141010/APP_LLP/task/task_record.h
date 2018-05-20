/*****************************************************************************
 * Module    : Task
 * File      : task_fm.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 收音机任务用到的结构体等
 *****************************************************************************/
#ifndef TASK_RECORD_H
#define TASK_RECORD_H

enum{
    REC_STA_STOP,
    REC_STA_STOPPING,
    REC_STA_RECORD,
    REC_STA_PAUSE,
};

//task record controller
typedef struct {
    u8 rec_src;                         //录音音源
    u8 rec_sta;                         //录音状态
    u8 file_open;                       //文件打开标志
    u8 dir_open;                        //文件夹已创建标志
    u8 sync_cnt;                        //同步控制
    u16 rec_time;                       //现在显示的录音时间
    u8 play_flag;                       //停止录音时需要自动播放当前录音文件标志
    u8 full_flag;                       //当前设备已录满标志，用于录音满时自动切换录音设备
#if (RECORD_FILE_NAME == LONG_NAME)
    u8 short_name_ptr;                  //短文件名的起始位置
    u8 long_name_ptr;
#endif
    u8 update_flag;
} type_task_rec_ctl;
extern type_task_rec_ctl t_rec;
#endif
