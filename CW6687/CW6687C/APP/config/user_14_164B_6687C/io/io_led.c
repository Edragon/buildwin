/*****************************************************************************
 * Module    : display
 * File      : led.c
 * Author    : luoyunfeng
 * Function  : LED灯相关的操作函数
 ******************************************************************************/
#ifndef CONFIG_C

__near_func void led_scan(void){}
void led_blue_on(void){}
void led_blue_off(void){}
void led_blue_100ms(void){}
void led_blue_200ms(void){}
void led_blue_500ms(void){}
void led_blue_1s(void){}
void led_blue_2s(void){}
void led_blue_on500ms_off2s(void){}
void led_blue_on50ms_off100ms(void){}
void led_green_on(void){}
void led_green_off(void){}
void led_green_100ms(void){}
void led_green_200ms(void){}
void led_green_500ms(void){}
void led_green_1s(void){}
void led_green_2s(void){}
void led_blue_green_500ms(void){}
//--------------------------------------------------------------------------------
/*
#define led_mode_on()
#define led_mode_off()
#define led_bt_none()           led_blue_100ms()    //蓝牙开机时蓝灯以100ms间隔闪烁
#define led_bt_voice()          led_blue_100ms()    //播放提示音，蓝灯以100ms间隔闪烁
#define led_bt_idle()           led_blue_100ms()    //未连接时蓝灯以100ms间隔快闪
#define led_bt_connect()        led_blue_1s()       //连接成功未开始播放或者播放后暂停时蓝灯以1S间隔慢闪
#define led_bt_play()           led_blue_1s()       //正在播放及拨接电话时蓝灯以1S间隔慢闪
#define led_idle()              led_blue_on()
#define led_busy()              led_blue_100ms()
#define led_music_play()        led_blue_1s()
#define led_aux_play()          led_blue_off()
改写成函数
*/
void led_mode_on(void){}
void led_mode_off(void){}
void led_bt_none(void){}
void led_bt_voice(void){}
void led_bt_idle(void){}
void led_bt_connect(void){}
void led_bt_play(void){}
void led_idle(void){}
void led_busy(void){}
void led_music_play(void){}
void led_aux_play(void){}
void led_aux_pause(void){}
void led_fm_play(void){}
void led_fm_scan(void){}

#endif


