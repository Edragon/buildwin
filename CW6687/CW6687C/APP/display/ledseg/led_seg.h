/*****************************************************************************
 * Module    : Display
 * File      : led_seg.h
 * Author    : Hanny
 * Function  : ledseg显示函数定义
*****************************************************************************/
#ifndef LED_SEG_H
#define LED_SEG_H

#define LED_BIT_A       BIT(6)
#define LED_BIT_B       BIT(5)
#define LED_BIT_C       BIT(4)
#define LED_BIT_D       BIT(3)
#define LED_BIT_E       BIT(2)
#define LED_BIT_F       BIT(1)
#define LED_BIT_G       BIT(0)

//图标显示
#define ICON_NULL       0x00    //不存在
#define ICON_PLAY       LED_BIT_A
#define ICON_PAUSE      LED_BIT_B
#define ICON_USB        LED_BIT_C
#define ICON_SD         LED_BIT_D
#define ICON_DOT2       LED_BIT_E
#define ICON_MP3        LED_BIT_F
#define ICON_AUX        ICON_NULL
#define ICON_FM         LED_BIT_G
#define ICON_DOT        ICON_NULL

//ASCII 配置
#define LEDSEG_0        (LED_BIT_A | LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_E | LED_BIT_F)
#define LEDSEG_1        (LED_BIT_B | LED_BIT_C)
#define LEDSEG_2        (LED_BIT_A | LED_BIT_B | LED_BIT_G | LED_BIT_E | LED_BIT_D)
#define LEDSEG_3        (LED_BIT_A | LED_BIT_B | LED_BIT_G | LED_BIT_C | LED_BIT_D)
#define LEDSEG_4        (LED_BIT_B | LED_BIT_C | LED_BIT_F | LED_BIT_G)
#define LEDSEG_5        (LED_BIT_A | LED_BIT_C | LED_BIT_D | LED_BIT_F | LED_BIT_G)
#define LEDSEG_6        (LED_BIT_A | LED_BIT_C | LED_BIT_D | LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_7        (LED_BIT_A | LED_BIT_B | LED_BIT_C)
#define LEDSEG_8        (LED_BIT_A | LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_9        (LED_BIT_A | LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_F | LED_BIT_G)
#define LEDSEG_A        (LED_BIT_A | LED_BIT_B | LED_BIT_C | LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_B        (LED_BIT_C | LED_BIT_D | LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_C        (LED_BIT_A | LED_BIT_D | LED_BIT_E | LED_BIT_F)
#define LEDSEG_D        (LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_E | LED_BIT_G)
#define LEDSEG_E        (LED_BIT_A | LED_BIT_D | LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_F        (LED_BIT_A | LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_G        (LED_BIT_A | LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_F | LED_BIT_G)
#define LEDSEG_H        (LED_BIT_B | LED_BIT_C | LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_I        (LED_BIT_B | LED_BIT_C)
#define LEDSEG_J        (LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_E)
#define LEDSEG_K        (LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_L        (LED_BIT_D | LED_BIT_E | LED_BIT_F)
#define LEDSEG_M        (LED_BIT_C | LED_BIT_E | LED_BIT_G)
#define LEDSEG_N        (LED_BIT_A | LED_BIT_B | LED_BIT_C | LED_BIT_E | LED_BIT_F)
#define LEDSEG_O        (LED_BIT_A | LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_E | LED_BIT_F)
#define LEDSEG_P        (LED_BIT_A | LED_BIT_B | LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_Q        (LED_BIT_A | LED_BIT_B | LED_BIT_C | LED_BIT_F | LED_BIT_G)
#define LEDSEG_R        (LED_BIT_E | LED_BIT_G)
#define LEDSEG_S        (LED_BIT_A | LED_BIT_C | LED_BIT_D | LED_BIT_F | LED_BIT_G)
#define LEDSEG_T        (LED_BIT_D | LED_BIT_E | LED_BIT_F | LED_BIT_G)
#define LEDSEG_U        (LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_E | LED_BIT_F)
#define LEDSEG_V        (LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_F | LED_BIT_E)
#define LEDSEG_W        (LED_BIT_C | LED_BIT_D | LED_BIT_E)
#define LEDSEG_X        (LED_BIT_B | LED_BIT_C | LED_BIT_E | LED_BIT_F)
#define LEDSEG_Y        (LED_BIT_B | LED_BIT_C | LED_BIT_D | LED_BIT_F | LED_BIT_G)
#define LEDSEG_Z        (LED_BIT_A | LED_BIT_B | LED_BIT_D | LED_BIT_E | LED_BIT_G)

#define LEDSEG__        (LED_BIT_G)
#define LEDSEG_ON       (LEDSEG_8)
#define LEDSEG_OFF      (0x00)
#define LEDSEG_DOT      (ICON_DOT)

enum {
    MENU_POWERON = 0,
    MENU_VOL,
    MENU_FREQ,
    MENU_FQ_CHAN,
    MENU_NUM_SEL,
    MENU_MUSIC_PLAY,
    MENU_MUSIC_PAUSE,
    MENU_NUM,
    MENU_AUX,
    MENU_AUX_EQ,
    MENU_SPEAKER,
    MENU_PC,
    MENU_BT,
    MENU_RTC_TIME,
    MENU_ALARM,
    MENU_ALARM_STA,
    MENU_EQ,
    MENU_MODE_PLAY,
    MENU_MIC,
    MENU_RECORDING,
    MENU_TOMCAT,
    MENU_POWEROFF,
    MENU_NO_DEVICE,
    MENU_DEL,
    MENU_FULL,
    MENU_ERR,
};

#if IS_LEDSEG_5C7S_DISPLAY
    #include "led_5c7s.h"
    #define ledseg_init()           led_5c7s_init()
    #define ledseg_value_set()      led_5c7s_value_set()
    #define ledseg_scan()           led_5c7s_scan()
#elif IS_LEDSEG_7PIN_DISPLAY
    #include "led_7p7s.h"
    #define ledseg_init()           led_7p7s_init()
    #define ledseg_value_set()      led_7p7s_value_set()
    #define ledseg_scan()           led_7p7s_scan()
#else
    #define ledseg_init()
    #define ledseg_value_set()
    #define ledseg_scan()
#endif

extern __no_init u8 disp_buf[5];


extern u16 disp_sel_num;
void ledseg_disp(u8 menu_number);

#endif
