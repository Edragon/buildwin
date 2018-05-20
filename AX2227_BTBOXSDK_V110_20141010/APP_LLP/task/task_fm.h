/*****************************************************************************
 * Module    : Task
 * File      : task_fm.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 收音机任务用到的结构体等
 *****************************************************************************/
#ifndef TASK_FM_H
#define TASK_FM_H

typedef struct {
    u8 fm_sta;                          //子状态
    u8 disp_flag;                       //状态显示更新控制
    u8 disp_ch_flag;                    //频道显示控制
    u8 disp_ch_cnt;                     //当前显示频道总数
    u8 disp_ch_sel;                     //当前显示频道选择
    u16 disp_freq;                      //频率显示更新控制

    u16 freq;                           //当前频段
    u8 ch_cnt;                          //频道总数
    u8 ch_sel;                          //当前频道
    u8 auto_delay;                      //自动搜台时，中间播放时间

    char seek_sta;
    char *num_buf;                      //用于录音时显示录音时间
    u8 num_len;
    char *colon;
    char *rec_icon;
    char *bignum_buf;
    u8 bignum_len;
    char *bat_buf;
    u8 bat_len;
    char *point_buf;

} type_fm_task;

extern type_fm_task t_fm;

void task_fm_disp_clear(void);

enum {
    FM_PLAY,                            //正常播放
    FM_AUTO_SEEK,                       //自动搜台模式
    FM_AUTO_PLAY,                       //自动搜台模式过程的短时播放
};

#endif
