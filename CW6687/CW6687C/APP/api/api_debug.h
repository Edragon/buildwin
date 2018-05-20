/*****************************************************************************
 * Module    : API
 * File      : api_debug.h
 * Author    : Hanny
 * Function  : ������صĲ����ӿ�
 *****************************************************************************/
#ifndef API_DEBUG_H
#define API_DEBUG_H

/*****************************************************************************
 * Module    : ��ӡ����
 *****************************************************************************/

//��ӡ������ʼ��
void my_printf_init(void (__near_func *putchar_func)(char));

//printf�ĺ���ԭ�͡����׼��printf�����÷���ͬ��
__near_func void my_printf(const char __code *fmt, ...);

//��ӡһ������
__near_func void printf_array(const void *buf, u16 len);

/*****************************************************************************
 * Module    : ���ڵ��Ժ���
 *****************************************************************************/

//���ڳ�ʼ��
__near_func void uart_init(void); ;

//���һ���ַ������ڣ�ǰ̨�ȴ�������ϡ��йر�EA����
__near_func void uart_putchar1_ea(char c);

//���һ���ַ������ڣ���̨�ȴ�������ᡣ�йر�EA����
__near_func void uart_putchar_ea(char c);

//���һ���ַ������ڣ���̨�ȴ�������ᡣһ��ʹ�øú���
__near_func void uart_putchar(char c);

//���һ���ַ������ڣ�ǰ̨�ȴ�������ϡ�
__near_func void uart_putchar1(char c);

//uart1���һ���ַ������ڣ���̨�ȴ�������ᡣһ��ʹ�øú���
__near_func void uart1_putchar(char c);

#endif
