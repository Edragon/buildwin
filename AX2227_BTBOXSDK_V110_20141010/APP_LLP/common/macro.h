/*****************************************************************************
 * Module    : COMMON
 * File      : macro.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 常用的宏定义
 *****************************************************************************/
#ifndef MACRO_H
#define MACRO_H


#define BANK_NUM                    0x3dUL                          //必须与hescript.hs和APP_LLP.xlc的-D_BANK_NUM对应

//以下是自动计算的宏
#define BT_TASK_FLASH_ADDR          (0x2000UL+0x800UL*BANK_NUM)         //蓝牙任务公共区
#define BT_SCO_FLASH_ADDR           (BT_TASK_FLASH_ADDR+0x800UL)        //DCODE4 蓝牙SCO
#define BT_SBC_FLASH_ADDR           (BT_SCO_FLASH_ADDR+0x800UL)         //DCODE3 蓝牙
#define FM_CODE_FLASH_ADDR          (BT_SBC_FLASH_ADDR+0x1800UL)        //DCODE2 FM
#define USBDEV_CODE_FLASH_ADDR      (FM_CODE_FLASH_ADDR+0x1800UL)       //DCODE1 USB设备
#define LOUDSPK_CODE_FLASH_ADDR     (USBDEV_CODE_FLASH_ADDR+0x1800UL)   //DCODE0 LOUD SPEAKER
#define FAT_CODE_FLASH_ADDR         (LOUDSPK_CODE_FLASH_ADDR+0x800UL)   //DCODE5 FAT

#define COMM_NORMAL                 (u16)(0x1000>>8)
#define COMM_BT                     (u16)(BT_TASK_FLASH_ADDR>>8)
#define DCODE_BT_SCO                (u16)(BT_SCO_FLASH_ADDR>>8)
#define DCODE_BT_SBC                (u16)(BT_SBC_FLASH_ADDR>>8)
#define DCODE_FM                    (u16)(FM_CODE_FLASH_ADDR>>8)
#define DCODE_USBDEV                (u16)(USBDEV_CODE_FLASH_ADDR>>8)
#define DCODE_LOUDSPK               (u16)(LOUDSPK_CODE_FLASH_ADDR>>8)
#define DCODE_FAT                   (u16)(FAT_CODE_FLASH_ADDR>>8)



#define BIT(n) (1<<(n))

#define     BYTE0(n)            ((unsigned char)(n))
#define     BYTE1(n)            ((unsigned char)((n)>>8))
#define     BYTE2(n)            ((unsigned char)(((unsigned short)((n)>>8))>>8))
#define     BYTE3(n)            ((unsigned char)(((unsigned short)((n)>>16))>>8))

//进入中断服务程序，备份，并设置DPCON
#define ISR_ENTER() asm("push 0x86"); asm("anl 0x86, #0xc0"); \
    asm("push 0x82"); asm("push 0x83"); \
    asm("push 0x84"); asm("push 0x85");

//出中断服务程序，还原
#define ISR_EXIT() asm("pop 0x85"); asm("pop 0x84"); \
    asm("pop 0x83"); asm("pop 0x82"); \
    asm("pop 0x86");

#define ENTER_DCODE()       asm("push 0x8b"); asm("orl 0x8b, #0x10")
#define EXIT_DCODE()        asm("pop 0x8b")

#define IAR_ROOT __root
#define IAR_CONST const __code
//xdata absolute
#define IAR_XDATA_A __root __no_init __xdata
//data absolute
#define IAR_DATA_A __root __no_init __data
//bit absolute
#define IAR_BIT_A __root __no_init __bit
//xdata absolute
#define IAR_XDATA __no_init __xdata
//data absolute
#define IAR_DATA __no_init __data
//idata absolute
#define IAR_IDATA __no_init __idata
//bdata absolute
#define IAR_BDATA __no_init __bdata

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
void memset_i(void __idata *ptr, u8 val, u8 len);
//重定义系统函数
#define printf my_printf
#define SENDATCMD printf
#define memset my_memset
#define memcpy my_memcpy
#define memcmp mem_cmp
#endif
