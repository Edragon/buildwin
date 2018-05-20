/*****************************************************************************
 * Module    : API
 * File      : api_music.h
 * Author    : Hanny
 * Function  : 音乐播放API接口
 *****************************************************************************/
#ifndef API_MUSIC_H
#define API_MUSIC_H

typedef struct {
    u32 frame;
    u32 pos;
} type_music_point;

typedef struct {
    type_music_point pa;
    type_music_point pb;
} type_music_point_ab;

typedef struct {
    u8 min;
    u8 sec;
} type_play_time;

enum {
    MUSIC_FAST_FORWORD,
    MUSIC_FAST_BACKWORD,
};

enum {
    TYPE_NONE,             //不支持的格式
    TYPE_MP3,
    TYPE_WMA,
    TYPE_WAV,
};

typedef struct {
    u8 type;                                //音乐格式
    u8 status;
    u32 cur_mtime;                          //当前播放时间，单位为0.1s
    u8 repeat;                              //重复次数
    u8 data_start_set;                      //起始位置设置
    bool (__near_func *read)(void*);        //真正的读接口
    type_music_point_ab point_ab;           //AB点
    u16 all_time;

    u8 err_cnt;                             //错误次数管理
} type_msc_ctl;

extern type_msc_ctl msc_ctl;

/*****************************************************************************
 * Module    : 音乐播放相关
 *****************************************************************************/

#define REPEAT_LOOP 0xff        //无限循环复读
#define REPEAT_END  0xfe        //复读一次后停止。用于跟读

__near_func bool decoder_read(void *buf);               //解码器的读接口
__near_func bool decoder_seek(u8 mode, u32 offset);     //解码器的SEEK接口

#define f_read_opt decoder_read
#define f_seek_opt decoder_seek

u8 music_init(void);                                    //初始化音乐解码

bool mp3_init(void);                                    //初始化MP3解码
bool mp3_init_spi(void);                                //初始化MP3解码的SPIFlash精简定制版
//bool mp3_init_alarm(void);                              //初始化闹钟

void music_dac_exit(void);

u32 music_get_bitrate(void);                          //获取歌曲码率

void music_clear(void);                                 //清除解码的RAM
u16 music_total(void);                                  //获取总共的歌曲数目
bool music_init_tmpfile(void);                          //初始化复读的tempfile
void music_stop(void);                                  //停止音乐播放
void music_event(void);                                 //解码需要的事件
u16 music_get_alltime(void);                            //获取总时间
void music_play(void);                                  //开始播放
void music_pause(void);                                 //暂停
bool music_start_pause(void);                           //播放/暂停
void music_fast(u8 direct, u8 sec);                     //快进/快退
void music_fast_end(void);                              //快进/快退结束，恢复播放
void music_get_point(type_music_point *point);          //获取当前播放位置
void music_set_a_point(type_music_point *point);        //设置A点，以便复读时间计算
//AB复读。repeat为复读次数。当repeat = REPEAT_LOOP，无限复读。为REPEAT_END时，复读一次停止
void music_repeat(u8 repeat, type_music_point_ab *pab);
void music_repeat_clear(void);                          //清除当前的复读状态，继续正常播放
void music_jump(type_music_point *point);               //跳至某点进行播放
bool music_set_speed(u8 speed);                         //设置播放速度
__near_func void dac_set_volume(u8 volume);             //设置播放音量
u8 music_get_repeat(void);                              //获取剩余复读次数
u32 music_lrc_first_time(void);                         //获取第一条LRC的时间
u32 music_lrc_cur_time(void);                           //获取本条LRC的时间
u32 music_lrc_next_time(void);                          //获取下一个LRC的时间
__near_func bool music_repeat_hook2(void);              //复读接口，加在解码过程中

__near_func u16 voice_maxpow(void *buf, u16 len);       //获取一包声音的最大音量（能量值）
__near_func u16 voice_maxpow_step(void *ptr, u16 len, u8 step);

//EQ选择
enum{
    EQ_NORMAL,                  //正常，无EQ
    EQ_POP,                     //流行
    EQ_ROCK,                    //摇滚
    EQ_JAZZ,                    //爵士
    EQ_CLASSIC,                 //古典
    EQ_COUNTRY,                 //乡村
};
void music_eq(u8 eqnum);        //MP3设置EQ的接口

void eq_disable(void);          //关闭EQ

//播放状态
enum {
    STATUS_PLAY_STOP,                       //完全停止
    STATUS_PLAY_STOPPING,                   //正在停止
    STATUS_PLAY_PAUSE,                      //暂停中
    STATUS_PLAY_PLAYING,                    //正在播放
    STATUS_PLAY_FAST,                       //快进快退中
    STATUS_PLAY_CONTINUE,                   //等待再次KICK继续播放
};

extern struct
{
    u32 addr;//资源首地址
    u16 len;//资源大小
    u16 pos;
    u8 spr;//采样率
    u8 bits;//采样精度
}mp3res;

__near_func music_get_status(void);     //获取音乐的播放状态
__near_func u32 music_calc_mtime(u32 frame_count);
__near_func u32 music_get_mtime(void);
__near_func u32 music_point_a_mtime(void);
void mp3_res_play(u8 music_name);                             //选择语言菜单播放
void mp3_res_play_wait(u8 music_name);                       //选择语言菜单播放,并等待播放完毕
void mp3_res_stop(void);                      //语音播放停止函数
void mp3_res_loop_set(bool loop_flag);//应在调用完mp3_res_play后再调用；语音循环播放，1:语音循环播放，直至调用mp3_res_stop(); 0:语音播放完一次停止
void wav_res_play(u8 music_name);                             //选择语言菜单播放
void usb_audio_sound_play(u8 music_name);
__near_func bool spi_mp3res_read_256(void *buf);
__near_func bool spi_mp3res_read(void *buf);

__near_func u8 sound_detect(u16 v_pow);
__near_func void sound_ctl_init(u8 is_cnt, u16 is_pow, u8 silence_cnt, u16 silence_pow);
__near_func u8 sound_flag(void);
__near_func void sound_set_flag(u8 flag, u8 cnt);

__near_func bool wma_init_sdk(u8 __xdata *const __code *buffer_tbl, u8 total_num);
__near_func bool m_wma_init(u8 __xdata *const __code *buffer_tbl, u8 total_num);
bool wav_get_info(void);
bool mp3_get_ID3(void);

/*****************************************************************************
 * Module    : 歌词显示相关
 *****************************************************************************/
extern __root __no_init char filename[100];
u8 lrc_init(void);
u8 music_get_lrc(u32 time_display);
#endif
