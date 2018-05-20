/*****************************************************************************
 * Module    : Display
 * File      : led_5c7s.h
 * Author    : Hanny
 * Function  : led_5c7s��ʾ��������
*****************************************************************************/
#ifndef LED_5C7S_H
#define LED_5C7S_H

//COM_ON����: ����LED, COMΪ��
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

//SEGx�����⸴��ģʽ��IO��Ϊ���룬�ر�500R��������10K����
//#define SEG_ENTER_DETECT()  P2DIR |= 0xfe; P2PD &= ~0xfe; P2PU |= 0xfe

//SEGx�˳���⸴��ģʽ���ر�10K��������500R������ͬʱSEG_OFF
//#define SEG_EXIT_DETECT()   P2PU &= ~0xfe; P2PD |= 0xfe

extern volatile u8 scan_buf[5];
void led_5c7s_init(void);
void led_5c7s_value_set(void);
__near_func void led_5c7s_scan(void);

#endif
