/*****************************************************************************
* Module    : BTSTACK
* File      : uart_drv.c
* Author    : Hanny
* Function  : Uart��������տ���
*****************************************************************************/
#ifndef UART_DRV_H
#define UART_DRV_H

void bt_uart_init(void);
__near_func void bt_uart_tx(void *buf, u8 len);

#endif
