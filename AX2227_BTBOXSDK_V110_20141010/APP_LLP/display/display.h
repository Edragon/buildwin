/*****************************************************************************
 * Module    : Display
 * File      : display.h
 * Author    :
 * Email     :
 * Function  : LCD显示函数定义
*****************************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

#include "ocx.h"
#include "led.h"

#if LCD_THEME
//使用LCD屏
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
//使用断码屏
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
//使用LED屏
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
#define show_update()          ledseg_ocx.disp_flag = 1        //更新显示

#else
//无主题
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
#define FILE_NAME_FLAG  (FILE_NAME_LEN-1)       //文件名标记位. BIT 7 编码；BIT 3 - 0 列表时用于图标

#if BT_DISP_NUM_TIME
extern u16 on_the_phone_time;
extern u8 disp_phone_num_cnt;
#endif


#endif
