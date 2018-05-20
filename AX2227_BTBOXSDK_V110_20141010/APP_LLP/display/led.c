/*****************************************************************************
 * Module    : display
 * File      : led.c
 * Author    : luoyunfeng
 * Email     :
 * Function  : LED灯相关的操作函数
 *****************************************************************************/
#include "include.h"
#include "led.h"
#include "display.h"

typedef struct
{
    u8 speed;
    u8 sp_cnt;
    u8 sp_on;
    u8 sp_off;
    u8 flag;
} type_led_ctl;

type_led_ctl led_blue;
type_led_ctl led_green;


#pragma location="DISP_LED_SEG"
void led_init(void)
{
#if LED_BLUE_BLINK
    led_blue_off();
#endif

#if LED_GREEN_BLINK
    led_green_off();
#endif
}

__near_func void led_scan(void)
{
#if LED_BLUE_BLINK
    if(led_blue.flag & 0x80) {
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

#if LED_GREEN_BLINK
    if(led_green.flag & 0x80) {
        led_green.sp_cnt++;
        if(led_green.sp_cnt >= led_green.speed) {
            if(LED_GREEN_IS_ON()) {
                led_green.speed = led_green.sp_off;
            } else {
                led_green.speed = led_green.sp_on;
            }
            led_green.sp_cnt = 0;
            LED_GREEN_TOG();
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
    bool ie_ea = IE_EA;     //2014-3-5, 修正小米2S及部分手机播歌停顿、无声问题
    IE_EA = 0;				//2014-3-5, 短时间处理，直接关IE_EA
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

#if LED_GREEN_BLINK
//GREEN
#pragma location="DISP_LED_SEG"
void led_green_on(void)
{
    led_green.flag = LED_ON;
    LED_GREEN_ON();
}

#pragma location="DISP_LED_SEG"
void led_green_off(void)
{
    led_green.flag = LED_OFF;
    LED_GREEN_OFF();
}

void led_green_100ms(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_green.speed = 2;
    led_green.sp_off = 2;
    led_green.sp_on = 2;
    led_green.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_green_200ms(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_green.speed = 4;
    led_green.sp_off = 4;
    led_green.sp_on = 4;
    led_green.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_green_500ms(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_green.speed = 10;
    led_green.sp_off = 10;
    led_green.sp_on = 10;
    led_green.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_green_1s(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_green.speed = 20;
    led_green.sp_off = 20;
    led_green.sp_on = 20;
    led_green.flag = LED_FLASH;
    IE_EA = ie_ea;
}

#pragma location="DISP_LED_SEG"
void led_green_2s(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    led_green.speed = 40;
    led_green.sp_off = 40;
    led_green.sp_on = 40;
    led_green.flag = LED_FLASH;
    IE_EA = ie_ea;
}
#endif

#if LED_BLUE_BLINK && LED_GREEN_BLINK
#pragma location="DISP_LED_SEG"
void led_blue_green_500ms(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    LED_BLUE_ON();
    led_blue.sp_cnt = 0;
    led_blue.speed = 10;
    led_blue.sp_off = 10;
    led_blue.sp_on = 10;
    led_blue.flag = LED_FLASH;

    LED_GREEN_OFF();
    led_green.sp_cnt = 0;
    led_green.speed = 10;
    led_green.sp_off = 10;
    led_green.sp_on = 10;
    led_green.flag = LED_FLASH;
    IE_EA = ie_ea;
}
#endif
