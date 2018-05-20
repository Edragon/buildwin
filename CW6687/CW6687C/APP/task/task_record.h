/*****************************************************************************
 * Module    : Task
 * File      : task_record.h
 * Author    : Hanny
 * Function  : 录音任务用到的结构体等
 *****************************************************************************/
#ifndef TASK_RECORD_H
#define TASK_RECORD_H

#define KICK_SOFT_INT()         SPMODE |= BIT(7);

#define DIR_Attr                11
#define REC_SYNC_CNT        	10       //每10秒同步一次

#define WAV_OUT_BUF_SIZE        0x1800
#define BLOCK_SIZE              128L    //BLK的大小
#define BLOCK_LIMIT             WAV_OUT_BUF_SIZE/BLOCK_SIZE //BLK的个数

#if (REC_TYPE == REC_MP3)
    #define MICREC_SAMPLES      384
    #define AUXREC_SAMPLES      MICREC_SAMPLES
#elif (REC_TYPE == REC_WAV_ADPCM)
    #define MICREC_SAMPLES      249
    #if ((AUX_CHANNEL_SEL&0x80) == 0x80)
        #define AUXREC_SAMPLES  121
    #else
        #define AUXREC_SAMPLES  249
    #endif
#elif (REC_TYPE == REC_WAV_PCM)
    #define MICREC_SAMPLES      128
    #define AUXREC_SAMPLES      MICREC_SAMPLES
#endif

#if (REC_TYPE == REC_MP3)
    #define MREC_BITR           128
    #define LREC_BITR           160
#else
    #define MREC_BITR           0
    #define LREC_BITR           0
#endif

#define MP3EN_BUF_BEG           0xC000
#define MP3EN_OUT_END           0xE000

enum{
    REC_STA_PAUSE,
    REC_STA_STOP,
    REC_STA_STOPPING,
    REC_STA_RECORD,
};

typedef struct {
    u8  rec_sta;                    //录音状态
    u8  dir_open;                   //文件夹已创建标志
    u8  file_open;                  //文件打开标志
    u8  sync_cnt;                   //同步控制
    u32 rec_time;                   //现在显示的录音时间
    u8  sname_ptr;
#if (RECORD_FILE_NAME == LONG_NAME)
    u8  lname_ptr;
#endif
    u8  volatile pos;               //录音buf标志
    u8  volatile block_cnt;         //录音的BLK数
    u32 file_size;                  //录音文件大小
    u32 file_ssect;                 //录音文件第一个扇区
    u8 *block_adr;                  //录音buf的当前地址
    u8  slice;                      //每个BLK的SLC数
    u16 spl;                        //每个BLK的样点数
    u16 spr;                        //录音的采样率
    u8 	play_flag;                  //是否播放刚录下的文件
    u16 adc_sample_cnt;
    u8  aux_record_flag;            //是否需要AUX录音
    u8  aux_mute;                   //AUX MUTE控制
#if IS_LCD_DISPLAY
    u8  updata_flag;                //更新标志
    u8  disp_sta;                   //播放闪烁状态
#endif
} type_task_rec;

extern type_task_rec t_rec;

extern u8 pcm_valid;
extern IAR_XDATA_A u16 adpcm_input;
extern IAR_XDATA_A u16 adpcm_output;
extern IAR_DATA_A  u8  adpcm_channel;
extern IAR_IDATA_A u8  adpcm_index[2];

extern u8 *mp3en_rptr;
extern u16 mp3en_out_len;

__near_func void adpcm_encode(u16 blocks, u8 part);
__near_func u32 clust2sect(u32 clust);
__near_func u32 search_free_clust(void);
__near_func bool move_window(u32 sector);
__near_func void mkitem_exfat(u8 *dir, char *fn, u8 fattrib, u32 dclust, u32 fsize);
__near_func void mkitem_fat(u8 *dir, char *fn, u8 fattrib, u32 dclust, u32 fsize);
__near_func u8 f_open_exist(FIL* hFile);
__near_func void adpcm_encode(u16 blocks, u8 part);

void tasksub_recording(void);
void tasksub_recplay(void);
void tasksub_recplay_event(void);
void tasksub_recplay_deal_msg(u8 msg);

bool play_record_file(void);
bool sync_file(void);
void wav_record_abort(void);
void task_record_pause(void);
void mp3en_init(void);

bool wav_load_code(u16 addr);
bool mp3en_load_code(u16 addr);
void task_record_display(void);
void task_recording_display(void);
void task_record_stop(void);
void task_record_init(void);

void aux_continue(void);
void aux_play_vol_music(void);
void aux_mute_control(void);
void aux_vol_up(void);
void aux_vol_down(void);

#endif
