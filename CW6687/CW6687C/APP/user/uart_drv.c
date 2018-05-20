/*****************************************************************************
* Module    : BTSTACK
* File      : uart_drv.c
* Author    : Hanny
* Function  : Uart发送与接收控制
*****************************************************************************/
#include "include.h"
#include "uart_drv.h"

__near_func void uart1_puts(void *buf, u8 len);
__near_func bool bt_uart_route(void);

__near_func void btrx_isr(unsigned char rdata);
__near_func void bt_uart_rxskip(u8 len);
__near_func void bt_uart_getbytes(void *buf, u8 len);

extern IAR_BIT_A bool flag_btmode;            //为1时启用完整的蓝牙功能

__near_func void bt_uart_route_hook(void)
{
    if (bt_uart_route()) {
        bt_sleep_cnt = BT_SLEEP_CNT;
    }
}
