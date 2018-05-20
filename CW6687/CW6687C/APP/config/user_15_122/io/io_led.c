/*****************************************************************************
 * Module    : display
 * File      : led.c
 * Author    : luoyunfeng
 * Email     :
 * Function  : LED灯相关的操作函数
 ******************************************************************************/
#ifndef CONFIG_C

enum
{
    LED_OFF         = 0x00, //LED熄灭
    LED_ON          = 0x01, //LED常亮
    LED_FLASH       = 0x82, //LED闪烁,亮500ms/灭500ms
};
//--------------------------------------------------------------------------------
typedef struct
{
    u8 speed;    //闪烁时间控制, sp_cnt的值与speed相等时,亮灭会转换
    u8 sp_cnt;   //++计数处理. 这里50ms加一次
    u8 sp_on;    //亮时间.
    u8 sp_off;   //灭的时间.
    u8 flag;     //亮/灭/闪烁标志.
} type_led_ctl;
type_led_ctl led_blue;

__near_func void led_scan(void)
{
#if LED_BLUE_BLINK
    if(led_blue.flag & 0x80) {   //bit7为1,表示灯闪烁.
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


