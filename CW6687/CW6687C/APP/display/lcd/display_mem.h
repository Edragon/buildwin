/*****************************************************************************
* Module    : Display
* File      : display_mem.c
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : 用于将SPI的资源拷入Memory中进行管理的头定义
*****************************************************************************/
#ifndef DISPLAY_MEM_H
#define DISPLAY_MEM_H

typedef struct {
    u8 width;           //宽
    u8 line;            //高
    char buf[0];        //内容
} type_mem_res;

typedef struct {
    char *buf;
    u16 len;
    char *num_buf;      //管理数字的
    u8 num_len;
} type_dispmem_ctl;

extern type_dispmem_ctl dispmem_ctl;
extern char disp_mem_buf[256];

void dispmem_init(void *buf, u16 len);
char *dispmem_add(u16 imageindex);
char *dispmem_addnum(u16 imageindex, u8 num);

void dispmem_drawnum(u8 num, u8 x, u8 line);

//显示一个Memory里面的图片
#pragma inline=forced
static u8 dispmem_draw(void *ptr, u8 x, u8 line)
{
    u8 i;
    type_mem_res *mr_ptr = ptr;
    char *buf_ptr = mr_ptr->buf;
    char *disp_ptr = &lcd_buf[x + COLUMN_MAX*line];
    u8 icon_width = mr_ptr->width;
    for (i=0; i!=mr_ptr->line; i++) {
        memcpy(disp_ptr, buf_ptr, icon_width);
        buf_ptr += icon_width;
        disp_ptr += COLUMN_MAX;
    }
    return icon_width;
}

//更新
#pragma inline=forced
static void dispmem_update(u8 lcd_width, u8 lcd_height, char *line_buf, u8 len)
{
    SET_LCD_CS;
 //   if (lcd_height>=PAGE_MAX)
 //   {
 //       return;
 //   }
    lcd_width += LCD_X_FIX;
    CLR_LCD_A0;    //LCD_COMMAND
    CLR_LCD_CS;
    asm("nop");
    SPI0BUF = LCD_COLUMN_HIGH | (lcd_width >> 4);
    while (!(SPI0CON & 0x80)); //等待发送完成
    SPI0BUF = LCD_COLUMN_LOW | (lcd_width & 0x0f);
    while (!(SPI0CON & 0x80)); //等待发送完成
    SPI0BUF = LCD_PAGE_ADDR | lcd_height;
    while (!(SPI0CON & 0x80)); //等待发送完成

    SET_LCD_CS;
    SET_LCD_A0;
    asm("nop");
    CLR_LCD_CS;

    SPIDMAPTRH = (u8)((u16)line_buf >> 8);  //DMA load start address
    SPIDMAPTRL = (u8)((u16)line_buf);
    SPIDMACNT = (len/2) - 1;
    while (!(SPI0CON & 0x80)); //等待发送完成
}


#endif
