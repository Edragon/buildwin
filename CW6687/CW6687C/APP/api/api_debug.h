/*****************************************************************************
 * Module    : API
 * File      : api_debug.h
 * Author    : Hanny
 * Function  : 调试相关的操作接口
 *****************************************************************************/
#ifndef API_DEBUG_H
#define API_DEBUG_H

/*****************************************************************************
 * Module    : 打印函数
 *****************************************************************************/

//打印函数初始化
void my_printf_init(void (__near_func *putchar_func)(char));

//printf的函数原型。与标准的printf函数用法相同。
__near_func void my_printf(const char __code *fmt, ...);

//打印一个数组
__near_func void printf_array(const void *buf, u16 len);

/*****************************************************************************
 * Module    : 串口调试函数
 *****************************************************************************/

//串口初始化
__near_func void uart_init(void); ;

//输出一个字符到串口，前台等待发送完毕。有关闭EA处理
__near_func void uart_putchar1_ea(char c);

//输出一个字符到串口，后台等待发送完结。有关闭EA处理
__near_func void uart_putchar_ea(char c);

//输出一个字符到串口，后台等待发送完结。一般使用该函数
__near_func void uart_putchar(char c);

//输出一个字符到串口，前台等待发送完毕。
__near_func void uart_putchar1(char c);

//uart1输出一个字符到串口，后台等待发送完结。一般使用该函数
__near_func void uart1_putchar(char c);

#endif
