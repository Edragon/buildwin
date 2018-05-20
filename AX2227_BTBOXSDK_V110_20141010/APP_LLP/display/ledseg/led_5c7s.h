/*****************************************************************************
 * Module    : Display
 * File      : led_5c7s.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : led_5c7s显示函数定义
*****************************************************************************/
#ifndef LED_5C7S_H
#define LED_5C7S_H

/**** 5c7s引脚映射 ****/
// ----------------------------------------------------------------
// | COM0  COM1  COM2  COM3  COM4  S0   S1   S2   S3   S4   S5   S6  |
// |  |     |     |     |     |     |    |    |    |    |    |    |  |
// | P13   P12   P11   P10   P32   P22  P23  P24  P25  P26  P30  P31 |
// |  |     |     |     |     |     |    |    |    |    |    |    |  |
// | B0    B1    B2    B3    B4    C0   C1   C2   C3   C4   C5   C6  |
// ----------------------------------------------------------------

#define LED_COM0_DIR   P1DIR
#define LED_COM1_DIR   P1DIR
#define LED_COM2_DIR   P1DIR
#define LED_COM3_DIR   P1DIR
#define LED_COM4_DIR   P3DIR

#define LED_COM0       P1
#define LED_COM1       P1
#define LED_COM2       P1
#define LED_COM3       P1
#define LED_COM4       P3

#define LED_COM0_bit   3
#define LED_COM1_bit   2
#define LED_COM2_bit   1
#define LED_COM3_bit   0
#define LED_COM4_bit   2

//COM_ON条件: 共阳LED, COM为高
#define COM0_ON()   LED_COM0 |= BIT(LED_COM0_bit)        //P13
#define COM1_ON()   LED_COM1 |= BIT(LED_COM1_bit)        //P12
#define COM2_ON()   LED_COM2 |= BIT(LED_COM2_bit)        //P11
#define COM3_ON()   LED_COM3 |= BIT(LED_COM3_bit)        //P10
#define COM4_ON()   LED_COM4 |= BIT(LED_COM4_bit)        //P32

//COM_OFF条件:共阳LED, COM为低
#define COM_ALL_OFF()  LED_COM0 &= ~(BIT(LED_COM0_bit) | BIT(LED_COM1_bit) | BIT(LED_COM2_bit) | BIT(LED_COM3_bit));\
                       LED_COM4 &= ~BIT(LED_COM4_bit);

//SEGx_ON条件: IO口方向设为输入,且下拉使能
#define SEG0_ON()   P2DIR |= BIT(2);     //C0
#define SEG1_ON()   P2DIR |= BIT(3);     //C1
#define SEG2_ON()   P2DIR |= BIT(4);     //C2
#define SEG3_ON()   P2DIR |= BIT(5);     //C3
#define SEG4_ON()   P2DIR |= BIT(6);     //C4
#define SEG5_ON()   P3DIR |= BIT(0);     //C5
#define SEG6_ON()   P3DIR |= BIT(1);     //C6

//SEGx_OFF条件: 关掉下拉, IO口方向设为输出,且输出为1
#define SEG_ALL_OFF()  P2 |= 0x7C; P3 |= (BIT(0) | BIT(1)); P2DIR &= ~0x7C; P3DIR &= ~(BIT(0) | BIT(1));

//SEGx进入检测复用模式，IO设为输入，关闭500R下拉，打开10K上拉
#define SEG_ENTER_DETECT()  P2DIR |= 0x7c;  P3DIR |= 0x03; P2PD0 &= ~0xf0; P2PD1 &= ~0x3f; \
                            P3PD0 &= ~0x0f; P2PU0 |= 0x50; P2PU1 |= 0x15;  P3PU0 |= 0x05;

//SEGx退检测复用模式，关闭10K上拉，打开500R下拉，同时SEG_OFF
#define SEG_EXIT_DETECT()   P2PU0 &= ~0xf0; P2PU1 &= ~0x3f; P3PU0 &= ~0x0f; \
                            P2PD0 |= 0xa0;  P2PD1 |= 0x2a;  P3PD0 |= 0x0a;

void led_5c7s_init(void);
void led_5c7s_value_set(void);
__near_func void led_5c7s_scan(void);

#endif
