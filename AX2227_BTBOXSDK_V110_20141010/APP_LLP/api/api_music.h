/*****************************************************************************
 * Module    : API
 * File      : api_music.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
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

/*****************************************************************************
 * Module    : 音乐播放相关
 *****************************************************************************/

#define REPEAT_LOOP 0xff        //无限循环复读
#define REPEAT_END  0xfe        //复读一次后停止。用于跟读

__near_func u32 voice_power(void *buf, u16 len);        //获取一包声音的平均能量值
__near_func u16 voice_maxpow(void *buf, u16 len);       //获取一包声音的最大音量

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
u8 music_get_repeat(void);                               //获取剩余复读次数
bool music_decode_ID3(void);                             //获取歌曲的ID3信息

//频率线初始化选择
enum {
    FREQ_DAC,                   //计算DAC的频率线
    FREQ_ADC,                   //计算ADC的频率线
};
void music_freq_init(u8 freq_type);                     //初始化频率线, freq_type参考频率线初始化选择枚举
bool music_freq_get(void);                              //获取频率线
void music_freq_kick(void);                             //Kick下一次
extern u16 freq_buf[64];
__near_func u16 voice_maxpow_step(void *ptr, u16 len, u8 step);

//EQ选择
enum{
    EQ_NORMAL,                  //正常，无EQ
    EQ_POP,                     //流行
    EQ_ROCK,                    //摇滚
    EQ_JAZZ,                    //爵士
    EQ_CLASSIC,                 //古典
    EQ_COUNTRY,                 //乡村
    EQ_BASS,                    //重低音
};
void music_eq(u8 eqnum);                                //配置EQ
void aux_fm_eq(u8 eqnum);                               //配置Linein和FM EQ

//播放状态
enum {
    STATUS_PLAY_STOP,                       //完全停止
    STATUS_PLAY_STOPPING,                   //正在停止
    STATUS_PLAY_PLAYING,                    //正在播放
    STATUS_PLAY_PAUSE,                      //暂停中
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
u32 music_get_mtime(void);
u32 music_point_a_mtime(void);

void spi_music_wait(void);                             //语音播放等待函数
void mp3_res_play(u8 music_name);                             //选择语言菜单播放
void mp3_res_play_wait(u8 music_name);                       //选择语言菜单播放,并等待播放完毕
void wav_res_play(u8 music_name);                             //选择语言菜单播放

void usb_audio_sound_play(u8 music_name);
__near_func bool spi_mp3res_read(void *buf);

/*****************************************************************************
 * Module    : 录音相关
 *****************************************************************************/
//AMUX
#define CHANNEL_0           0x10            //单声道，使用P00 L
#define CHANNEL_1           0x01            //单声道，使用P01 R
#define CHANNEL_2           0x02            //单声道，使用P02 R
#define CHANNEL_3           0x20            //单声道，使用P03 L
#define CHANNEL_6           0x03            //单声道，使用P36 R
#define CHANNEL_7           0x30            //单声道，使用P37 L

//MIC 只支持 P02 或 P03
#define MCHANNEL            0x08
#define MCHANNEL_2          (MCHANNEL | CHANNEL_2)       //单声道，使用P02 R
#define MCHANNEL_3          (MCHANNEL | CHANNEL_3)       //单声道，使用P03 L

//AMUX单声道双输出
#define CHANNEL_DL          0x70
#define CHANNEL_DR          0x07
#define CHANNEL_00          (CHANNEL_0 | CHANNEL_DR)        //单声道双输出，使用P00 L
#define CHANNEL_11          (CHANNEL_DL | CHANNEL_1)        //单声道双输出，使用P01 R
#define CHANNEL_22          (CHANNEL_DL | CHANNEL_2)        //单声道双输出，使用P02 R
#define CHANNEL_33          (CHANNEL_3 | CHANNEL_DR)        //单声道双输出，使用P03 L
#define CHANNEL_66          (CHANNEL_DL | CHANNEL_6)        //单声道双输出，使用P36 R
#define CHANNEL_77          (CHANNEL_7 | CHANNEL_DR)        //单声道双输出，使用P37 L

//双声道，AMUX可以一左一右自由组合
#define TWO_CHANNEL         0x80
#define CHANNEL_01          (TWO_CHANNEL | CHANNEL_0 | CHANNEL_1)     //双声道，使用P00 P01
#define CHANNEL_02          (TWO_CHANNEL | CHANNEL_0 | CHANNEL_2)     //双声道，使用P00 P02
#define CHANNEL_06          (TWO_CHANNEL | CHANNEL_0 | CHANNEL_6)     //双声道，使用P00 P36
#define CHANNEL_31          (TWO_CHANNEL | CHANNEL_3 | CHANNEL_1)     //双声道，使用P03 P01
#define CHANNEL_32          (TWO_CHANNEL | CHANNEL_3 | CHANNEL_2)     //双声道，使用P03 P02
#define CHANNEL_36          (TWO_CHANNEL | CHANNEL_3 | CHANNEL_6)     //双声道，使用P03 P36
#define CHANNEL_71          (TWO_CHANNEL | CHANNEL_7 | CHANNEL_1)     //双声道，使用P37 P01
#define CHANNEL_72          (TWO_CHANNEL | CHANNEL_7 | CHANNEL_2)     //双声道，使用P37 P02
#define CHANNEL_76          (TWO_CHANNEL | CHANNEL_7 | CHANNEL_6)     //双声道，使用P37 P36
#define CHANNEL_10          CHANNEL_01
#define CHANNEL_20          CHANNEL_02
#define CHANNEL_60          CHANNEL_06
#define CHANNEL_13          CHANNEL_31
#define CHANNEL_23          CHANNEL_32
#define CHANNEL_63          CHANNEL_36
#define CHANNEL_17          CHANNEL_71
#define CHANNEL_27          CHANNEL_72
#define CHANNEL_67          CHANNEL_76

enum {
    SAMPLE_48000,
    SAMPLE_44100,
    SAMPLE_32000,
    SAMPLE_24000,
    SAMPLE_22050,
    SAMPLE_16000,
    SAMPLE_12000,
    SAMPLE_11025,
    SAMPLE_8000,
    SAMPLE_BT_MIC,
};
#define SPEAKER_EN          0x01
#define AUX_EQ_EN           0x02
#define TWO_CHANAL          0x04

typedef struct {
    u8 dac_volume;
} type_volume_ctl;

typedef struct {
    //AMUX的属性
    u8 mp3_en;                          //是否需要MP3压缩模块
    u8 vol_out;                         //DAC是否播出声音
    u8 channel;                         //AMUX的通道
    //ADC、录音的属性
    u8 agc_en;                          //是否开启AGC
    u8 rec_volume;                      //录音增益AGCCON2,-12~72,2时为0
    u8 rec_channel;                     //选用的录音通道
    u8 sample;                          //0为低采样率，1为高采样率
    u16 bitrate;                        //码率
    u8 dac_out;                        //直接把采到的数据推出DAC播放
    u8 dac_sample;                     //dac 采样率，要和adc采样率配一样
} type_mp3en_ctrl;

extern type_volume_ctl volume_ctl;      //音量控制器
extern type_mp3en_ctrl mp3en_ctrl;      //录音控制器

void mp3en_init(type_mp3en_ctrl *mp3en_ctrl);   //复位初始化编码模块
void mp3en_start(bool mode);                    //开始录音。mode: true自动模式，false手动扩展模式
void mp3en_stop(void);                          //停止录音
void mp3en_exit(void);                          //退出录音功能，关闭所有录音模块
u8 mp3en_write(void);                         //将Buffer的数据写入，放在主循环中调用
bool mp3en_write_spi(void);
__near_func void imdct_neg(void);            //消除人声
__near_func bool wma_init_sdk(u8 __xdata *const __code *buffer_tbl, u8 total_num);
__near_func bool m_wma_init(u8 __xdata *const __code *buffer_tbl, u8 total_num);
bool mp3_get_ID3(void);
bool wav_get_info(void);


__near_func void at_write(u8 adr, u8 dat);
__near_func u8 at_read(u8 adr);
void adc_enable(void);


extern u8 mp3_adc_buf[0xd80];
extern __root __no_init char filename[100];

u8 lrc_init(void);
u8 music_get_lrc(u32 time_display);
u32 music_lrc_first_time(void);                         //获取第一条LRC的时间
u32 music_lrc_cur_time(void);                           //获取本条LRC的时间
u32 music_lrc_next_time(void);                          //获取下一个LRC的时间
#endif
