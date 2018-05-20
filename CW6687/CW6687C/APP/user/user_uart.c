/*****************************************************************************
 * Module    : User
 * File      : user_uart.c
 * Author    : Hanny
 * Function  : ������������س���
 *****************************************************************************/
#include "include.h"

IAR_XDATA_A u8 bt_uart_rxbuf[256] @ 0x3E00;

extern IAR_DATA_A volatile u8 bt_uart_rxptr;     //����ָ��
extern IAR_DATA_A volatile u8 bt_uart_rxlen;
extern IAR_DATA_A u8 bt_rx_buf;
volatile u8 bt_sleep_cnt;        //BTģ�������������ģʽ

#pragma location="BT_UART_TX_SEG"
__near_func void bt_check_wake_up(void)
{
    u8 timeout = 5;                         // 5 * 25ms == 125ms
    while (bt_sleep_cnt == 0 && timeout--) {
        UART1DATA = 0xff;
        delay_5ms(5);                       // delay 20ms
    }
}

#pragma location="BT_UART_TX_SEG"
__near_func void uart1_putchar(char ch)
{
    while((UART1STA & BIT(2)) == 0);
    UART1DATA = ch;
}

#pragma location="BT_UART_TX_SEG"
__near_func void uart1_puts(void *buf, u8 len)
{
#if 0
    u8 i;
    for (i=0; i<len; i++) {
        uart1_putchar(((u8 *)buf)[i]);
    }
#else
    u8 *pbuf = (u8 *)buf;
    if (len == 0) {
        return;
    }
    if (BYTE0(pbuf) & 1) {
        uart1_putchar(pbuf[0]);
        pbuf++;
        len--;
        if (len == 0) {
            return;
        }
    }
    while((UART1STA & BIT(2)) == 0);
    UARTDMATXPTR = BYTE1(pbuf);
    UARTDMATXPTR = BYTE0(pbuf);
    UARTDMATXCNT = len - 1;
#endif
}

#pragma location="BT_UART_TX_SEG"
__near_func void uart1_wait(void)
{
    while((UART1STA & BIT(2)) == 0);
}

#pragma location="BT_INIT_SEG"
void bt_uart_init(void)
{
    UART1CON = 0;
    PMUXCON0 |= BIT(6);     //Chip Bluetooth
    PMUXCON1 |= BIT(1);     //BT Wake UP

    bt_uart_rxptr = 0;
    bt_uart_rxlen = 0;
    bt_rx_buf = BYTE1(bt_uart_rxbuf);

    UARTDMARXPTR = bt_rx_buf;
    UARTDMARXPTR = 0x00;
    UART1LOOPCNT = 0x1c;

    UART1DIV  = 0x0c;       //921600

    UART1CON = BIT(4) | BIT(0);
    UART1STA |= BIT(0);     //kick rx

#if 0
    extern volatile u8 __data bt_uart_rxlen;
    __near_func u8 bt_uart_rxget(void);

    WDTCON = 0;
    while(1)
    {
        if (bt_uart_rxlen) {
            printf("RE: %02X\n", bt_uart_rxget());
        }
    }
#endif
}

//---------------------------------------------------------------------------------
//UART ���ڲ�ѯ���ͣ� �жϽ���ʾ��

#if 0
#pragma vector = VECTOR_OFFSET(RTCC_INT)
#pragma register_bank=2  //0���ȼ�(������ȼ�)������bank1 //2���ȼ�������bank2 //3���ȼ�������bank3
__interrupt __near_func void uart_rtc_iis_isr(void)
{
    ISR_ENTER();
    if(UARTSTA & BIT(5)){
        UARTSTA &= ~BIT(5);
        uart_putchar(UARTDATA);
    }
    ISR_EXIT();
}


//��48MHz����ʼ������
void uart_init_test(void)
{
    UARTBAUDL = 0x33;//0x33;
    UARTBAUDH = 0;
    UARTDIV = 0x07;//0x07;  //48Mϵͳʱ��ʱ��  48000/((0x33+1)*(0x07+1)) = 115.38K
    UARTCON = 0x11;         //1bit stop bit   //uart en //uart rx int en
#if 1
    UARTSTA = 0x01;         //select TX:P01 RX:P00
    PIE0 |= BIT(0)|BIT(1);  //ȷ��P00/P01������IO�ڡ�
    P0DIR &= ~BIT(1);
    P0DIR |= BIT(0);
    P0PU |= BIT(0);         //P10 10K����
#else
    UARTSTA = 0x00;         //select TX:P16 RX:P34
    P1DIR &= ~BIT(6);
    P3DIR |= BIT(6);
    P1PU |= BIT(6);
    P3PU |= BIT(4);         //P34 10K����
    P3PUS0 &= ~BIT(4);
    P3PUS1 &= ~BIT(4);
#endif
    IPH1 |= BIT(6);   //��2�����ȼ�
    IP1 &= ~BIT(6);
    IE1 |= BIT(6);    //int en

}

//���һ���ַ������ڣ���̨�ȴ��������
void uart_putchar_test(char c)
{
	while((UARTSTA & 0x10) == 0);
	UARTDATA = c;
}

void uart_int_recv_test(void)
{
    uart_init_test();
    WDTCON = 0;
    while(1){
        uart_putchar_test(0x55); //uart_putchar_test(CLKCON1);
        delay_5ms(100);
    }
}
#endif
