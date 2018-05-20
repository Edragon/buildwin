/*****************************************************************************
 * Module    : Task
 * File      : task_fm.h
 * Author    : Hanny
 * Function  : 收音机任务用到的结构体等
 *****************************************************************************/
#ifndef TASK_FM_H
#define TASK_FM_H

#if (REC_TYPE == REC_MP3)
    #define FMREC_SAMPLES       384
#elif (REC_TYPE == REC_WAV_ADPCM)
    #if ((FM_CHANNEL_SEL&0x80) == 0x80)
        #define FMREC_SAMPLES   121
    #else
        #define FMREC_SAMPLES   249
    #endif
#elif (REC_TYPE == REC_WAV_PCM)
    #define FMREC_SAMPLES       128
#endif

#if (REC_TYPE == REC_MP3)
    #define FREC_BITR           160
#else
    #define FREC_BITR           0
#endif

typedef struct _TYPE_TASK_FM {
    u8 mute;
    u8 fm_sta;                          //子状态
    u8 disp_flag;                       //状态显示更新控制
    u8 disp_ch_flag;                    //频道显示控制
    u8 disp_ch_cnt;                     //当前显示频道总数
    u8 disp_ch_sel;                     //当前显示频道选择
    u16 disp_freq;                      //频率显示更新控制

    u8 eq_num;                          //EQ
    u16 freq;                           //当前频段
    u8 ch_cnt;                          //频道总数
    u8 ch_sel;                          //当前频道
    u8 auto_delay;                      //自动搜台时，中间播放时间

    char *num_buf;                      //用于录音时显示录音时间
    u8 num_len;
    char *colon;
    char *rec_icon;
    char *bignum_buf;
    u8 bignum_len;
    char *bat_buf;
    u8 bat_len;
    char *point_buf;
    char seek_sta;
    u8 clk_set;
    u8 sta;
} TYPE_TASK_FM;

typedef struct {
    u8  dir_open;                       //文件夹已创建标志
    u8  file_open;                      //文件打开标志
    u16 spl;                            //每个BLK的样点数
    u8  volatile pos;                   //录音buf标志
    u16 adc_sample_cnt;
    u8 	play_flag;                      //是否播放刚录下的文件
    u32 rec_time;                       //录音时间
    u8  sync_cnt;                      	//同步控制

    u8  volatile block_cnt;             //录音的BLK数
    u8  slice;                          //每个BLK的SLC数
    u32 file_size;                      //录音文件大小
    u8 *block_adr;                      //录音buf的当前地址
    u32 file_ssect;                     //录音文件第一个扇区
    u16 spr;                            //录音的采样率
#if IS_LCD_DISPLAY
    u8  updata_flag;                    //更新标志
    u8  disp_sta;                       //播放闪烁状态
#endif
} type_task_fm_rec;

enum {
    FM_PLAY,                            //正常播放
    FM_AUTO_SEEK,                       //自动搜台模式
    FM_AUTO_PLAY,                       //自动搜台模式过程的短时播放
    FM_RECORD,                          //录音模式
    FM_REC_PAUSE,                       //录音暂停
    FM_REC_PLAY,                        //录音播放
};

extern TYPE_TASK_FM t_fm;
extern type_task_fm_rec t_fm_rec;

void task_fm_disp_init(void);
void task_fm_list(void);
void task_fm_volume_set(void);
void task_fm_load(void);

void tasksub_fm_record(void);
void task_fm_record_stop(void);
bool fm_sync_file(void);
void tasksub_fm_recplay(void);
void task_fm_reuse_exit(void);
void task_fm_record_pause(void);
void task_fm_record_sync(void);

#endif
