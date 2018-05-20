/*****************************************************************************
 * Module    : Display
 * File      : led_5c7s.h
 * Author    : Hanny
 * Function  : led_5c7s显示函数定义
*****************************************************************************/
#ifndef LED_5C7S_H
#define LED_5C7S_H

//COM_ON条件: 共阳LED, COM为高
#define COM0_ON()   LED_COM0 |= BIT(LED_COM0_BIT)
#define COM1_ON()   LED_COM1 |= BIT(LED_COM1_BIT)
#define COM2_ON()   LED_COM2 |= BIT(LED_COM2_BIT)
#define COM3_ON()   LED_COM3 |= BIT(LED_COM3_BIT)
#define COM4_ON()   LED_COM4 |= BIT(LED_COM4_BIT)

#define LED_COM_INIT()  LED_COM0_DIR &= ~BIT(LED_COM0_BIT); \
    LED_COM1_DIR &= ~BIT(LED_COM1_BIT); \
    LED_COM2_DIR &= ~BIT(LED_COM2_BIT); \
    LED_COM3_DIR &= ~BIT(LED_COM3_BIT); \
    LED_COM4_DIR &= ~BIT(LED_COM4_BIT)

#define COM_ALL_OFF()  LED_COM0 &= ~BIT(LED_COM0_BIT); \
    LED_COM1 &= ~BIT(LED_COM1_BIT); \
    LED_COM2 &= ~BIT(LED_COM2_BIT); \
    LED_COM3 &= ~BIT(LED_COM3_BIT); \
    LED_COM4 &= ~BIT(LED_COM4_BIT)

//SEGx进入检测复用模式，IO设为输入，关闭500R下拉，打开10K上拉
//#define SEG_ENTER_DETECT()  P2DIR |= 0xfe; P2PD &= ~0xfe; P2PU |= 0xfe

//SEGx退出检测复用模式，关闭10K上拉，打开500R下拉，同时SEG_OFF
//#define SEG_EXIT_DETECT()   P2PU &= ~0xfe; P2PD |= 0xfe

extern volatile u8 scan_buf[5];
void led_5c7s_init(void);
void led_5c7s_value_set(void);
__near_func void led_5c7s_scan(void);

#endif
