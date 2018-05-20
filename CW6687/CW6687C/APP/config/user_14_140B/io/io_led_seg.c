/*****************************************************************************
 * Module    : IO
 * File      : io_led_seg.c
 * Author    : Hanny
 * Function  : LED数码管显示屏的相关操作函数
 ******************************************************************************/
#ifndef CONFIG_C

#if IS_LEDSEG_5C7S_DISPLAY

// ----------------------------------------------------------------
// | COM0  COM1  COM2  COM3  COM4  S0   S1   S2   S3   S4   S5   S6  |
// |  |     |     |     |     |     |    |    |    |    |    |    |  |
// | P07   P06   P33   P17   P20   P27  P26  P25  P24  P23  P22  P21 |
// ----------------------------------------------------------------

#define LED_COM0_DIR    P0DIR
#define LED_COM0        P0
#define LED_COM0_BIT    7

#define LED_COM1_DIR    P0DIR
#define LED_COM1        P0
#define LED_COM1_BIT    6

#define LED_COM2_DIR    P3DIR
#define LED_COM2        P3
#define LED_COM2_BIT    3

#define LED_COM3_DIR    P1DIR
#define LED_COM3        P1
#define LED_COM3_BIT    7

#define LED_COM4_DIR    P2DIR
#define LED_COM4        P2
#define LED_COM4_BIT    0

//SEGx_ON条件: IO口方向设为输入,且下拉使能
#define SEG0_ON()       P2DIR |= BIT(7)     //S0
#define SEG1_ON()       P2DIR |= BIT(6)     //S1
#define SEG2_ON()       P2DIR |= BIT(5)     //S2
#define SEG3_ON()       P2DIR |= BIT(4)     //S3
#define SEG4_ON()       P2DIR |= BIT(3)     //S4
#define SEG5_ON()       P2DIR |= BIT(2)     //S5
#define SEG6_ON()       P2DIR |= BIT(1)     //S6

//SEG_OFF条件: 关掉下拉, IO口方向设为输出,且输出为1
#define SEG_ALL_OFF()   P2 |= 0xfe; P2DIR &= ~0xfe

//SEG开内部500R下拉
#define SEG_PULL_DOWN() P2PDS1 &= 0x01; P2PDS0 = (P2PDS0 & 0x01) | 0xfE; P2PD = (P2PD & 0x01) | 0xFE;

#endif

#endif


