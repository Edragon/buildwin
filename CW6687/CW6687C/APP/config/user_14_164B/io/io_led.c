/*****************************************************************************
 * Module    : display
 * File      : led.c
 * Author    : luoyunfeng
 * Function  : LED����صĲ�������
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
#define led_bt_none()           led_blue_100ms()    //��������ʱ������100ms�����˸
#define led_bt_voice()          led_blue_100ms()    //������ʾ����������100ms�����˸
#define led_bt_idle()           led_blue_100ms()    //δ����ʱ������100ms�������
#define led_bt_connect()        led_blue_1s()       //���ӳɹ�δ��ʼ���Ż��߲��ź���ͣʱ������1S�������
#define led_bt_play()           led_blue_1s()       //���ڲ��ż����ӵ绰ʱ������1S�������
#define led_idle()              led_blue_on()
#define led_busy()              led_blue_100ms()
#define led_music_play()        led_blue_1s()
#define led_aux_play()          led_blue_off()
��д�ɺ���
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


