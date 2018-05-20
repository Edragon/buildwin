/*****************************************************************************
 * Module    : User
 * File      : user_music.h
 * Author    : Hanny
 * Function  : �û����ֲ��ſ��ƶ���
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
    u8 mute;                //ϵͳ�Ƿ���MUTEס
    u8 dynamic;             //��̬����ģʽѡ��
    u8 digit;               //�����Ƿ��ѵ��롣
    u8 out;                 //DAC�Ƿ���Ҫ����
    u8 dmode;               //��ǰΪ����ģʽ����Ҫ���ֲ��ݵĵ��뵭��
    u8 sound_detect;        //�Ƿ�Ҫ���dac buf����
    u16 digit_vol;          //��������
    u8 fade_out_en;        //�Ƿ���Ҫ����dac
} TYPE_DAC_E;

typedef struct _TYPE_DAC_CTL {
    u8 sample;              //DAC��ǰ������
    u8 anl_volcur;          //DAC��ǰֵ
    u8 anl_volset;          //DACӦ��ֵ
    u8 anl_vol;             //DACʵ����������Ҫ���ǵ����������ֵ
    u16 dig_vol;            //����DAC��ʵ������
    u8 fade;                //ģ���Ƿ��롣
} TYPE_DAC_CTL;

extern TYPE_DAC_E dac_e;
extern TYPE_DAC_CTL dac_ctl;
extern IAR_CONST u8 tbl_anl_vol[];

void dac_fade_out(void);
void dac_fade_in(void);
__near_func void dac_fade_auto_step(void);
__near_func u32 voice_power(void *buf, u16 len);

void umsc_sound_detect(u8 sound_detect);        //�����Ƿ���Ҫ������������Զ�����
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
bool music_decode_ID3(void);                             //��ȡ������ID3��Ϣ

#endif
