/*****************************************************************************
 * Module    : User
 * File      : user_music.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户音乐播放控制定义
 *****************************************************************************/
#ifndef USER_MUSIC_H
#define USER_MUSIC_H
extern void dac_fade_in(bool digital);
extern void dac_fade_out(bool digital);
extern __near_func void dac_anl_fade_wait(void);

enum {
    AGCHOOK_NONE,
    AGCHOOK_LINEIN,
    AGCHOOK_TAPE,
    AGCHOOK_TOMCAT,
};

extern IAR_BIT_A bool freq_show_update;
__near_func u16 iir_hp(s16 *adr, u16 samples, u8 step);
__near_func u16 abs_mean(s16 *adr, u16 samples, u8 step);
__near_func u16 adc_fade(s16 *adr, u16 samples, u8 step, u8 rate);


bool device_mount(void);
bool is_other_device(void);
bool music_scan_file(void);

bool music_next_device(void);
bool music_mount_device(void);
bool music_remount_device(void);

bool record_next_device(void);
bool record_mount_device(void);


void id3_transmit(void);
void make_lfn_to_ansi(void);

#endif
