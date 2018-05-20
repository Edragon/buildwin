/*****************************************************************************
 * Module    : COMMON
 * File      : macro.h
 * Author    : Hanny
 * Function  : 常用的宏定义
 *****************************************************************************/
#ifndef MACRO_H
#define MACRO_H

#define BIT(n) (1<<(n))

#define     BYTE0(n)            ((unsigned char)((unsigned short)(n)))
#define     BYTE1(n)            ((unsigned char)(((unsigned short)(n))>>8))
#define     BYTE2(n)            ((unsigned char)(((unsigned short)(((unsigned long)(n))>>8))>>8))
#define     BYTE3(n)            ((unsigned char)(((unsigned short)(((unsigned long)(n))>>16))>>8))

//进入中断服务程序，备份，并设置DPCON
#define ISR_ENTER() asm("push 0x86"); asm("anl 0x86, #0xc0"); \
    asm("push 0x82"); asm("push 0x83"); \
    asm("push 0x84"); asm("push 0x85");

//出中断服务程序，还原
#define ISR_EXIT() asm("pop 0x85"); asm("pop 0x84"); \
    asm("pop 0x83"); asm("pop 0x82"); \
    asm("pop 0x86");

#define IAR_ROOT __root
#define IAR_CONST const __code
//xdata absolute
#define IAR_XDATA_A __root __no_init __xdata
//pdata absolute
#define IAR_PDATA_A __root __no_init __pdata
//idata absolute
#define IAR_IDATA_A __root __no_init __idata
//data absolute
#define IAR_DATA_A __root __no_init __data
//bit absolute
#define IAR_BIT_A __root __no_init __bit

#define TRUE   1
#define FALSE  0
#define true   1
#define false  0
#define NULL   0
#define __bool_true_false_are_defined 1

//大小端控制访问
__near_func u32 load_u32(void __xdata *ptr);
__near_func u16 load_u16(void __xdata *ptr);
__near_func u32 swap_u32(u32 val);
__near_func u16 swap_u16(u16 val);

#define LOAD_U32B(p) load_u32(p)
#define LOAD_U32(p)  (*(u32 __xdata*)(p))
#define LOAD_U16B(p) load_u16(p)
#define LOAD_U16(p)  (*(u16 __xdata*)(p))

#define U32_ACCESS(p) (*(u32 __xdata*)(p))
#define U16_ACCESS(p) (*(u16 __xdata*)(p))

#define SAVE_U32B(p, num) { \
    ((u8 __xdata*)p)[0] = BYTE3(num); \
    ((u8 __xdata*)p)[1] = BYTE2(num); \
    ((u8 __xdata*)p)[2] = BYTE1(num); \
    ((u8 __xdata*)p)[3] = BYTE0(num); \
}

//重定义系统函数
#define printf my_printf
#define memset my_memset
#define memcpy my_memcpy
#define memcmp mem_cmp
#endif
