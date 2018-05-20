/*****************************************************************************
 * Module    : Display
 * File      : display.h
 * Author    :
 * Email     :
 * Function  : LCD��ʾ��������
*****************************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

#include "ocx.h"
#include "led.h"

#if LCD_THEME
//ʹ��LCD��
#include "lcd.h"
#include "lcd_display.h"
#define ocx_clear()
#define show_music_num()
#define show_volume()
#define show_freq_chan()
#define show_eq()
#define show_mode_play()
#define show_alarm_status()
#define show_mix_step()
#define show_no_device()  msgbox(STR_NO_DEVICE, NULL, MSGBOX_TIME)
#define show_del()
#define show_full()
#define show_err()
#define show_update()

#elif LCDSEG_THEME
//ʹ�ö�����
#include "lcd_seg.h"
#include "lcdseg_display.h"
#define ocx_clear()            lcdseg_ocx_clear()
#define show_music_num()       lcdseg_ocx_show_num()
#define show_volume()          lcdseg_ocx_show_vol()
#define show_freq_chan()       lcdseg_ocx_show_chan()
#define show_eq()              lcdseg_ocx_show_eq()
#define show_mode_play()       lcdseg_ocx_show_mode_play()
#define show_alarm_status()    lcdseg_ocx_show_alarm_status()
#define show_mix_step()
#define show_no_device()
#define show_del()
#define show_full()
#define show_err()
#define show_update()

#elif LEDSEG_THEME
//ʹ��LED��
#include "led_seg.h"
#include "ledseg_display.h"
#define ocx_clear()            ledseg_ocx_clear()
#define show_music_num()       ledseg_ocx_show_num()
#define show_volume()          ledseg_ocx_show_vol()
#define show_freq_chan()       ledseg_ocx_show_chan()
#define show_eq()              ledseg_ocx_show_eq()
#define show_mode_play()       ledseg_ocx_show_mode_play()
#define show_alarm_status()    ledseg_ocx_show_alarm_status()
#define show_mix_step()        ledseg_ocx_show_mix_step()
#define show_no_device()       ledseg_ocx_show_no_device()
#define show_del()             ledseg_ocx_show_del()
#define show_full()            ledseg_ocx_show_full()
#define show_err()             ledseg_ocx_show_err()
#define show_update()          ledseg_ocx.disp_flag = 1        //������ʾ

#else
//������
#include "no_display.h"
#define ocx_clear()
#define show_music_num()
#define show_volume()
#define show_freq_chan()
#define show_eq()
#define show_mode_play()
#define show_alarm_status()
#define show_mix_step()
#define show_no_device()
#define show_del()
#define show_full()
#define show_err()
#define show_update()
#endif


#define DIR_DEPTH       8
#define FILE_NAME_LEN   100
#define FILE_NAME_FLAG  (FILE_NAME_LEN-1)       //�ļ������λ. BIT 7 ���룻BIT 3 - 0 �б�ʱ����ͼ��

#if BT_DISP_NUM_TIME
extern u16 on_the_phone_time;
extern u8 disp_phone_num_cnt;
#endif


#endif
