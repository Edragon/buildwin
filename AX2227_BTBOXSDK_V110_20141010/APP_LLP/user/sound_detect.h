/*****************************************************************************
 * Module    : User
 * File      : sound_detect.h
 * Author    : jingfa
 * Email     :
 * Function  : 声音能量检测
 *****************************************************************************/
#ifndef SOUND_DETECT_H
#define SOUND_DETECT_H


typedef struct {
    void *ptr;          //检测起始地址
    u16 samples;        //检测样点数
    u8 delay_cnt;       //检测延时
    u8 fade_en;         //淡入淡出控制使能
} type_sound_detect_ctl;

__near_func void sound_ctl_init(u8 is_cnt, u32 is_pow, u8 silence_cnt, u32 silence_pow);
__near_func u8 sound_flag(void);

void sound_detect_init(void *ptr, u16 samples);     //配置能量检测起始地址和样点数
void sound_detect_enable(bool fade_en);             //使能能量检测，0=只检测控制DAC，1=控制DAC淡入淡出
void sound_detect_disable(void);                    //禁止能量检测
__near_func void sound_detect_process(void);

#endif
