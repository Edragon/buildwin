/*****************************************************************************
 * Module    : User
 * File      : user_music.h
 * Author    : Hanny
 * Function  : 用户音乐播放控制定义
 *****************************************************************************/
#ifndef USER_MUSIC_H
#define USER_MUSIC_H

enum {
    AGCHOOK_NONE,
    AGCHOOK_LINEIN,
    AGCHOOK_TAPE,
    AGCHOOK_TOMCAT,
};

typedef struct _TYPE_DAC_E {
    u8 mute;                //系统是否已MUTE住
    u8 dynamic;             //动态降噪模式选择
    u8 digit;               //数字是否已淡入。
    u8 out;                 //DAC是否需要出声
    u8 dmode;               //当前为数字模式，需要数字部份的淡入淡出
    u8 sound_detect;        //是否要检测dac buf能量
    u16 digit_vol;          //数字音量
    u8 fade_out_en;        //是否需要淡出dac
} TYPE_DAC_E;

typedef struct _TYPE_DAC_CTL {
    u8 sample;              //DAC当前采样率
    u8 anl_volcur;          //DAC当前值
    u8 anl_volset;          //DAC应设值
    u8 anl_vol;             //DAC实际音量，主要考虑淡出音量后的值
    u16 dig_vol;            //数字DAC的实际音量
    u8 fade;                //模拟是否淡入。
} TYPE_DAC_CTL;

extern TYPE_DAC_E dac_e;
extern TYPE_DAC_CTL dac_ctl;
extern IAR_CONST u8 tbl_anl_vol[];

void dac_fade_out(void);
void dac_fade_in(void);
__near_func void dac_fade_auto_step(void);
__near_func u32 voice_power(void *buf, u16 len);

void umsc_sound_detect(u8 sound_detect);        //设置是否需要进行能量检测自动复读
void agc_set_hook(u8 mode);

__near_func u16 iir_hp(s16 *adr, u16 samples, u8 step);
__near_func u16 abs_mean(s16 *adr, u16 samples, u8 step);
__near_func u16 adc_fade(s16 *adr, u16 samples, u8 step, u8 rate);
bool device_mount(void);
bool is_other_device(void);
bool music_scan_file(void);

bool music_next_device(void);
bool music_mount_device(void);
bool music_remount_device(void);
void task_music_get_num(void);
void task_music_cal_time(void);
bool record_mount_device(void);
bool music_decode_ID3(void);                             //获取歌曲的ID3信息

#endif
