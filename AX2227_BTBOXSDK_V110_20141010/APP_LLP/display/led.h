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


enum
{
    LED_OFF         = 0x00, //LED熄灭
    LED_ON          = 0x01, //LED常亮
    LED_FLASH       = 0x82, //LED闪烁,亮500ms/灭500ms
};

void led_init(void);
__near_func void led_scan(void);
void led_blue_on(void);
void led_blue_off(void);
void led_blue_100ms(void);
void led_blue_200ms(void);
void led_blue_500ms(void);
void led_blue_1s(void);
void led_blue_2s(void);
void led_blue_on500ms_off2s(void);
void led_blue_on50ms_off100ms(void);

void led_green_on(void);
void led_green_off(void);
void led_green_100ms(void);
void led_green_200ms(void);
void led_green_500ms(void);
void led_green_1s(void);
void led_green_2s(void);
void led_blue_green_500ms(void);


//以下是通用的LED闪烁方式
#if LED_BLUE_BLINK && !LED_GREEN_BLINK
    #define led_mode_on()
    #define led_mode_off()
    #define led_bt_none()           led_blue_on()       //蓝牙开机时蓝灯长亮
    #define led_bt_voice()          led_blue_200ms()    //播放提示音，蓝灯以200ms间隔闪烁
    #define led_bt_idle()           led_blue_100ms()    //未连接时蓝灯以100ms间隔快闪
    #define led_bt_connect()        led_blue_on()       //连接成功未开始播放或者播放后暂停时蓝灯长亮
    #define led_bt_play()           led_blue_1s()       //正在播放及拨接电话时蓝灯以1S间隔慢闪
    #define led_idle()              led_blue_on()
    #define led_busy()              led_blue_100ms()
    #define led_music_play()        led_blue_1s()
    #define led_aux_pause()
    #define led_aux_play()          led_blue_2s()
#elif !LED_BLUE_BLINK && LED_GREEN_BLINK
    #define led_mode_on()
    #define led_mode_off()
    #define led_bt_none()           led_green_on()       //蓝牙开机时蓝灯长亮
    #define led_bt_voice()          led_green_200ms()    //播放提示音，蓝灯以200ms间隔闪烁
    #define led_bt_idle()           led_green_100ms()    //未连接时蓝灯以100ms间隔快闪
    #define led_bt_connect()        led_green_on()       //连接成功未开始播放或者播放后暂停时蓝灯长亮
    #define led_bt_play()           led_green_1s()       //正在播放及拨接电话时蓝灯以1S间隔慢闪
    #define led_idle()              led_green_on()
    #define led_busy()              led_green_100ms()
    #define led_music_play()        led_green_1s()
    #define led_aux_pause()
    #define led_aux_play()          led_green_2s()
#elif LED_BLUE_BLINK && LED_GREEN_BLINK
    #define led_mode_on()           led_blue_on()
    #define led_mode_off()          led_blue_off()
    #define led_bt_none()           {led_blue_on();led_green_off();}//蓝牙开机时蓝灯长亮
    #define led_bt_voice()          led_blue_200ms()                //播放提示音，蓝灯以200ms间隔闪烁
    #define led_bt_idle()           led_blue_green_500ms()          //未连接时蓝绿灯以500ms间隔交替闪
    #define led_bt_connect()        {led_blue_on();led_green_off();}//连接成功未开始播放或者播放后暂停时蓝灯长亮
    #define led_bt_play()           led_blue_1s()                   //正在播放及拨接电话时蓝灯亮300ms灭700ms
    #define led_off()               led_green_off()
    #define led_idle()              led_green_on()
    #define led_busy()              led_green_100ms()
    #define led_music_play()        led_green_1s()
    #define led_aux_pause()
    #define led_aux_play()          led_green_2s()
#else
    #define led_mode_on()
    #define led_mode_off()
    #define led_bt_none()
    #define led_bt_voice()
    #define led_bt_idle()
    #define led_bt_connect()
    #define led_bt_play()
    #define led_idle()
    #define led_busy()
    #define led_music_play()
    #define led_aux_play()
    #define led_aux_pause()
#endif

#endif //_LED_H_
