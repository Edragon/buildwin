/*****************************************************************************
 * Module    : display
 * File      : led.c
 * Author    : luoyunfeng
 * Email     :
 * Function  : LED����صĲ�������
 ******************************************************************************/
#ifndef CONFIG_C

enum
{
    LED_OFF         = 0x00, //LEDϨ��
    LED_ON          = 0x01, //LED����
    LED_FLASH       = 0x82, //LED��˸,��500ms/��500ms
};
//--------------------------------------------------------------------------------
typedef struct
{
    u8 speed;    //��˸ʱ�����, sp_cnt��ֵ��speed���ʱ,�����ת��
    u8 sp_cnt;   //++��������. ����50ms��һ��
    u8 sp_on;    //��ʱ��.
    u8 sp_off;   //���ʱ��.
    u8 flag;     //��/��/��˸��־.
} type_led_ctl;
type_led_ctl led_blue;

__near_func void led_scan(void)
{
#if LED_BLUE_BLINK
    if(led_blue.flag & 0x80) {   //bit7Ϊ1,��ʾ����˸.
        led_blue.sp_cnt++;
        if(led_blue.sp_cnt >= led_blue.speed) {
            if(LED_BLUE_IS_ON()) {
                led_blue.speed = led_blue.sp_off;
            } else {
                led_blue.speed = led_blue.sp_on;
            }
            led_blue.sp_cnt = 0;
            LED_BLUE_TOG();
        }
    }
#endif
}
#if LED_BLUE_BLINK
//BLUE
#pragma location="DISP_LED_SEG"
void led_blue_on(void)
{
    led_blue.flag = LED_ON;
    LED_BLUE_ON();
}

#pragma location="DISP_LED_SEG"
void led_blue_off(void)
{
    led_blue.flag = LED_OFF;
    LED_BLUE_OFF();
}

#pragma location="DISP_LED_SEG"
void led_blue_100ms(void)
{
    bool ie_ea = IE_EA;     //2014-3-5, ����С��2S�������ֻ�����ͣ�١���������
    IE_EA = 0;				//2014-3-5, ��ʱ�䴦��ֱ�ӹ�IE_EA
    led_blue.speed = 2;
    led_blue.sp_off = 2;
    led_blue.sp_on = 2;
    led_blue.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_blue_200ms(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_blue.speed = 4;
    led_blue.sp_off = 4;
    led_blue.sp_on = 4;
    led_blue.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_blue_500ms(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_blue.speed = 10;
    led_blue.sp_off = 10;
    led_blue.sp_on = 10;
    led_blue.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_blue_1s(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_blue.speed = 20;
    led_blue.sp_off = 20;
    led_blue.sp_on = 20;
    led_blue.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_blue_2s(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_blue.speed = 40;
    led_blue.sp_off = 40;
    led_blue.sp_on = 40;
    led_blue.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_blue_on500ms_off2s(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_blue.speed = 0;
    led_blue.sp_off = 40;
    led_blue.sp_on = 10;
    led_blue.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_blue_on200ms_off2s(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_blue.speed = 0;
    led_blue.sp_off = 40;
    led_blue.sp_on = 4;
    led_blue.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_blue_on50ms_off100ms(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_blue.speed = 0;
    led_blue.sp_off = 2;
    led_blue.sp_on = 1;
    led_blue.flag = LED_FLASH;
    IE_EA = ie_ea;
}
#endif

void led_green_on(void)
{
}

void led_green_off(void)
{
}

void led_green_100ms(void)
{
}

void led_green_200ms(void)
{
}

void led_green_500ms(void)
{
}

void led_green_1s(void)
{
}


void led_green_2s(void)
{
}


void led_blue_green_500ms(void)
{
}

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
void led_mode_on(void)
{
    led_blue_on();
}

void led_mode_off(void)
{
    led_blue_off();
}
void led_bt_none(void)
{
    led_blue_200ms();
}
void led_bt_voice(void)
{
    led_blue_200ms();
}
void led_bt_idle(void)
{
     led_blue_200ms();
}
void led_bt_connect(void)
{
     led_blue_on();
}
void led_bt_play(void)
{
    led_blue_on200ms_off2s();
}
void led_idle(void)
{
    led_blue_on();
}

void led_busy(void)
{

}

void led_music_play(void)
{
    led_blue_1s();
}

void led_aux_pause(void)
{
     led_blue_on();
}

void led_aux_play(void)
{
     led_blue_1s();
}

void led_fm_play(void)
{
    led_blue_on();
}

void led_fm_scan(void)
{
    led_blue_200ms();
}

#endif


