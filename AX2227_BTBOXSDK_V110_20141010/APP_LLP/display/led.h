/*****************************************************************************
 * Module    : display
 * File      : led.h
 * Author    : luoyunfeng
 * Email     :
 * Function  : LED����صĲ�������
 *****************************************************************************/


#ifndef  _LED_H_
#define  _LED_H_

#include "include.h"


enum
{
    LED_OFF         = 0x00, //LEDϨ��
    LED_ON          = 0x01, //LED����
    LED_FLASH       = 0x82, //LED��˸,��500ms/��500ms
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


//������ͨ�õ�LED��˸��ʽ
#if LED_BLUE_BLINK && !LED_GREEN_BLINK
    #define led_mode_on()
    #define led_mode_off()
    #define led_bt_none()           led_blue_on()       //��������ʱ���Ƴ���
    #define led_bt_voice()          led_blue_200ms()    //������ʾ����������200ms�����˸
    #define led_bt_idle()           led_blue_100ms()    //δ����ʱ������100ms�������
    #define led_bt_connect()        led_blue_on()       //���ӳɹ�δ��ʼ���Ż��߲��ź���ͣʱ���Ƴ���
    #define led_bt_play()           led_blue_1s()       //���ڲ��ż����ӵ绰ʱ������1S�������
    #define led_idle()              led_blue_on()
    #define led_busy()              led_blue_100ms()
    #define led_music_play()        led_blue_1s()
    #define led_aux_pause()
    #define led_aux_play()          led_blue_2s()
#elif !LED_BLUE_BLINK && LED_GREEN_BLINK
    #define led_mode_on()
    #define led_mode_off()
    #define led_bt_none()           led_green_on()       //��������ʱ���Ƴ���
    #define led_bt_voice()          led_green_200ms()    //������ʾ����������200ms�����˸
    #define led_bt_idle()           led_green_100ms()    //δ����ʱ������100ms�������
    #define led_bt_connect()        led_green_on()       //���ӳɹ�δ��ʼ���Ż��߲��ź���ͣʱ���Ƴ���
    #define led_bt_play()           led_green_1s()       //���ڲ��ż����ӵ绰ʱ������1S�������
    #define led_idle()              led_green_on()
    #define led_busy()              led_green_100ms()
    #define led_music_play()        led_green_1s()
    #define led_aux_pause()
    #define led_aux_play()          led_green_2s()
#elif LED_BLUE_BLINK && LED_GREEN_BLINK
    #define led_mode_on()           led_blue_on()
    #define led_mode_off()          led_blue_off()
    #define led_bt_none()           {led_blue_on();led_green_off();}//��������ʱ���Ƴ���
    #define led_bt_voice()          led_blue_200ms()                //������ʾ����������200ms�����˸
    #define led_bt_idle()           led_blue_green_500ms()          //δ����ʱ���̵���500ms���������
    #define led_bt_connect()        {led_blue_on();led_green_off();}//���ӳɹ�δ��ʼ���Ż��߲��ź���ͣʱ���Ƴ���
    #define led_bt_play()           led_blue_1s()                   //���ڲ��ż����ӵ绰ʱ������300ms��700ms
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
