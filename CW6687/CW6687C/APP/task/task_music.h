/*****************************************************************************
 * Module    : Task
 * File      : task_music.h
 * Author    : Hanny
 * Function  : 音乐播放任务流程
 *****************************************************************************/
#ifndef TASK_MUSIC_H
#define TASK_MUSIC_H

#define PLAYMODE_ITEMS_NUM          (MODE_PLAY_NUM + 1)
#define MUSIC_EQ_ITEM_NUM           (MUSIC_EQ_NUM + 1)
#define DEVICE_ITEM_NUM             3

//显示更新标志
void task_music_display(void);              //这个函数在主题中定义

enum {
    ITEM_LOOP_ALL,
    ITEM_LOOP_SINGLE,
    ITEM_LOOP_FOLDER,
    ITEM_LOOP_NORMAL,
    ITEM_LOOP_RAMDOM,
    ITEM_LOOP_BROWSE,
};

#define LRC_FILENAME_TIME                   (3 + 1)           //有歌词时，至少先保留3秒歌名

typedef struct {
    u8 play_sta;                            //复读机状态控制
    u16 music_num;                          //播放曲目编号
    u16 music_total;                        //曲目总数
    u16 dir_last;                           //目录最后一个文件编号
    u8 music_type;                          //音频格式
    u8 eq_num;                              //EQ选择
    u8 speed;                               //速度选择
    union {
        struct {
            u8 pause:                       1;     //暂停，用于状态显示
            u8 file_change:                 1;     //文件改变，需要打开新文件
            u8 file_prev_next:              1;     //文件方向，0为next，1为prev
            u8 no_file_flag:                1;     //设备内无音乐文件 用于无文件时切设备
            u8 mode_flag:                   1;     //设备内无音乐文件 用于无文件时切设备
            u8 music_voice_flag:            1;     //模式键切换设备
            u8 break_flag:                  1;     //断点记忆
        };
        u8 music_ctl;
    };
    type_play_time cur_time;                //当前播放时间
    type_play_time disp_time;               //现在显示的时间
    type_play_time disp_music_time;         //现在显示的歌曲时间
    u32 bit_rate;                           //码率
    u8 disp_flag:                  1;  //显示全部更新标志
    u8 lrc_change:                 1;  //是否需要更新歌词
    u8 filename_change:            1;  //更新显示文件名
    u8 disp_status_flag:           1;  //是否需要更新状态显示
#if AB_REPEAT_EN
    type_music_point_ab point;              //记录AB点
    u16 disp_rtime;                         //显示复读时间
    u16 rtime;                              //当前复读时间
     u8 repeat_flag;                        //复读标志
     u8 repeat_show;                        //控制复读标志闪烁
    u8 auto_repeat;                         //自动复读标志
    u8 auto_repeat_start;                   //开始自动复读
    u32 next_sentence;                      //本句结束，下一句开始的时间
#endif

#if MUSIC_LRC_EN
    u8 lrc_flag;                            //是否有歌词
    u8 lrc_type;                           //lrc文件编码类型
#endif
#if BREAK_POINT_EN
     u8 break_dev;
    u16 filename_crc;                       //歌曲名crc
    type_music_point break_point;           //记录断点信息
#endif
    u16 dir_total;  //文件夹总数
    u16 dir_num;//当前文件夹编号
    //bool pause_before_menu_music;           //播放提示音前音乐是否处于暂停状态
} type_task_music_ctl;

extern type_task_music_ctl t_msc;


#endif
