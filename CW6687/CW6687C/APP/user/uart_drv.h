/*****************************************************************************
* Module    : BTSTACK
* File      : uart_drv.c
* Author    : Hanny
* Function  : Uart发送与接收控制
*****************************************************************************/
#ifndef UART_DRV_H
#define UART_DRV_H

void bt_uart_init(void);
__near_func void bt_uart_tx(void *buf, u8 len);

#endif
