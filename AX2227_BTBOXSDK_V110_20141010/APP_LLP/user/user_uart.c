/*****************************************************************************
 * Module    : User
 * File      : user_ir.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户的IO设置
 *****************************************************************************/
#include "include.h"

IAR_DATA_A u8 bt_tx_buf @ 0x7e;
IAR_DATA_A u8 bt_rx_buf @ 0x7f;

//串口初始化
#pragma location="INIT_SEG"
void uart0_init(void)
{
    UARTBAUD = 0x33;    //UARTBAUD = (系统时钟/波特率/8) - 1，改变系统时钟要重新配置UARTBAUD
    UARTCON = 0x91;     //STOP BIT AND ENABLE UART
    UARTSTA = 0x00;     //选择P16 P34
    P1DIR &= ~BIT(6);   //TX
    P3DIR |= BIT(4);    //RX
    P3PU0 |= BIT(7);    //P34上拉10k
    IE1 |= BIT(6);      //接收中断
}
extern __near_func void uart_bt6635_deal(void);
extern __near_func uart_bt6638_deal(void);
__near_func void uart_deal_hook(void)
{
#if(BT_TYPE == BT6638)
    uart_bt6638_deal();//6638串口中断处理函数
#else
    uart_bt6635_deal();//6635串口中断处理函数
#endif
}

#pragma location="BT_INIT_SEG"
void bt_uart_init(void)
{
#if BT_UART_IO
    //P36 P37
    PWRCON1 &= ~BIT(4);                         //VPG dis
    PMUXCON0 &= ~BIT(6);
    P3DIR |= BIT(7);
    P3PUD1 |= BIT(5);
    P3 |= BIT(7);
    P3DIR &= ~BIT(6);
    P3 |= BIT(6);
#else
    //P16 P17
    PMUXCON0 |= BIT(6);
    P1DIR |= BIT(7);
    P1PU0 |= BIT(7);
    P1 |= BIT(7);
    P1DIR &= ~BIT(6);
    P1 |= BIT(6);

    //驱动力加强些,调8202的FPGA时加强可以免拔UART线
    //P1DRV1 &= ~BIT(2); // P1.6

#endif
    bt_tx_buf = 0x9f;
    bt_rx_buf = 0x9e;

    UARTDIV = 0x0c;                             //采样点个数，范围为4~16，取值为0x03~0x0f，越大越好
    UART1BAUD = 0x03;                           //采样速率
    //UARTDIV = 0x07;
    //UART1BAUD = 0x33;

    UART1CON |= BIT(4);                    //UART1 enable

    IE1 |= BIT(6);                         //RX interrupt enable
    IPH1 |= BIT(6);
    IP1  |= BIT(6);

    IE_EA = 1;
    UART1CON |= BIT(2);                    //TX / RX interrupt enable
    //timer2_init();
}

