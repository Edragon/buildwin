/*****************************************************************************
 * Module    : display
 * File      : led.h
 * Author    : luoyunfeng
 * Email     :
 * Function  : LED灯相关的操作函数
 *****************************************************************************/


#ifndef  _LED_H_
#define  _LED_H_

#include "include.h"

void led_init(void);
__near_func void led_scan(void);
void led_mode_on(void);
void led_mode_off(void);
void led_bt_none(void);
void led_bt_voice(void);
void led_bt_idle(void);
void led_bt_connect(void);
void led_bt_play(void);
void led_idle(void);
void led_busy(void);
void led_music_play(void);
void led_aux_play(void);
void led_aux_pause(void);
void led_fm_play(void);
void led_fm_scan(void);

#endif //_LED_H_
