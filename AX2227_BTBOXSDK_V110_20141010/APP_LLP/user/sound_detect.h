/*****************************************************************************
 * Module    : User
 * File      : sound_detect.h
 * Author    : jingfa
 * Email     :
 * Function  : �����������
 *****************************************************************************/
#ifndef SOUND_DETECT_H
#define SOUND_DETECT_H


typedef struct {
    void *ptr;          //�����ʼ��ַ
    u16 samples;        //���������
    u8 delay_cnt;       //�����ʱ
    u8 fade_en;         //���뵭������ʹ��
} type_sound_detect_ctl;

__near_func void sound_ctl_init(u8 is_cnt, u32 is_pow, u8 silence_cnt, u32 silence_pow);
__near_func u8 sound_flag(void);

void sound_detect_init(void *ptr, u16 samples);     //�������������ʼ��ַ��������
void sound_detect_enable(bool fade_en);             //ʹ��������⣬0=ֻ������DAC��1=����DAC���뵭��
void sound_detect_disable(void);                    //��ֹ�������
__near_func void sound_detect_process(void);

#endif
