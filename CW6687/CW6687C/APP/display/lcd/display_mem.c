/*****************************************************************************
* Module    : Display
* File      : display_mem.c
* Author    : Hanny
* Function  : 用于将SPI的资源拷入Memory中进行管理。部分不允许读Flash的任务会用到
*****************************************************************************/
#include "include.h"
#if IS_LCD_DISPLAY
#include "display_mem.h"

type_dispmem_ctl dispmem_ctl;

extern u32 calc_res_addr(u32 res_addr);         //计算资源的绝对地址

IAR_XDATA_A char disp_mem_buf[256] @ 0x22B80;    //录音时用于存放少量显示资源的Buffer

//初始化管理
#pragma location="DISPLAY_MEM"
void dispmem_init(void *buf, u16 len)
{
    dispmem_ctl.buf = buf;
    dispmem_ctl.len = len;
    dispmem_ctl.num_buf = NULL;
    dispmem_ctl.num_len = 0;
}

//将一个资源加入，返回memory地址
#pragma location="DISPLAY_MEM"
char *dispmem_add(u16 imageindex)
{
    type_mem_res *ptr = (type_mem_res *)dispmem_ctl.buf;
    u32 res_addr;
    u8 icon_width, icon_line;                   //宽高应该都在256以内
    u32 icon_addr;
    u16 icon_size;
//    u16 icon_length;                            //大小最大为128x64 = 8192

    res_addr = calc_res_addr(imageindex * ICON_FILE_HEAD + 4);

    icon_width = spi_read16(res_addr);          //宽
    icon_line = spi_read16(res_addr+2) >> 3;    //行
    icon_size = icon_width * icon_line;
    icon_addr = spi_read32(res_addr+8);         //资源起始
    icon_addr = calc_res_addr(icon_addr);

    if (icon_line > 8) {
        return NULL;
    }
    if (icon_size + 2 > dispmem_ctl.len) {
        return NULL;
    }
    dispmem_ctl.len -= icon_size + 2;
    ptr->width = icon_width;
    ptr->line = icon_line;
    spi_read(ptr->buf, icon_addr, icon_size);
    dispmem_ctl.buf += icon_size + 2;
    return (char *)ptr;
}

//将数字加入。大号 480Byte, 中号 160Byte，小号80Byte
#pragma location="DISPLAY_MEM"
char *dispmem_addnum(u16 imageindex, u8 num)
{
    u8 i;
    dispmem_ctl.num_buf = dispmem_ctl.buf;
    dispmem_add(imageindex);
    dispmem_ctl.num_len = dispmem_ctl.buf - dispmem_ctl.num_buf;
    for (i=1; i!=num; i++) {
        dispmem_add(imageindex+i);
    }
    return dispmem_ctl.num_buf;
}

//显示一个数字的演示程序
#pragma location="DISPLAY_MEM"
void dispmem_drawnum(u8 num, u8 x, u8 line)
{
    u8 ten, one;
    if (num > 99) {
        num = 99;
    }
    ten = num / 10;
    one = num % 10;
    dispmem_draw(dispmem_ctl.num_buf + ten * dispmem_ctl.num_len, x, line);
    dispmem_draw(dispmem_ctl.num_buf + one * dispmem_ctl.num_len, x + 14, line);
}
#endif
