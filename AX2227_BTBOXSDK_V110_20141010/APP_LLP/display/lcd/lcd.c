/*****************************************************************************
 * Module    : Display
 * File      : lcd.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD驱动
*****************************************************************************/
#include "include.h"
#if LCD_THEME
#include "task.h"

extern u32 calc_res_addr(u32 res_addr);         //计算资源的绝对地址
extern u32 calc_charset_addr(u32 res_addr);     //计算编码表的绝对地址
extern u32 calc_font_addr(u32 res_addr);        //计算字库的绝对地址
extern u32 calc_word_addr(u32 res_addr);        //计算字库图片的绝对地址

u32 CharacterDataAddr;    ///<unicode资源的起始地址
u16 CharacterDataLength;

u8 disp_freq_show_buf[16];      //记录上次的显示值
extern u16 freq_buf[64];

struct {
    u8 mode;                //LCD的模式。包括1k的完全模式和256Byte的小缓存模式
} lcd_ctl;

#pragma constseg="DRAW_FREQ_CONST"
IAR_CONST u16 tbl_freq_val[15] = {
//    52, 82, 130, 207, 328, 519, 823, 1305,
//    2068, 3277, 5193, 8231, 13045, 20675, 32768
    52, 104, 207, 413, 734, 1305, 2320, 3677,
    5827, 8231, 11627, 16423, 20675, 26029, 32768
};

IAR_CONST u8 tbl_freq_line_0[15] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
IAR_CONST u8 tbl_freq_line_1[15] = {0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#pragma constseg=default


//1K的LCD_BUFF，后512Byte同时复用于LRC_READ_BUF,f_write时的win_buf
IAR_XDATA_A char lcd_buf[PAGE_MAX * COLUMN_MAX] @ "LCD_BUFF";    //屏数据的buffer

//设置LCD Buffer的位置。在部份情况下，需要改动这个位置
#pragma location="INIT_SEG_1"
void set_lcd_buf(u8 mode)
{
    lcd_ctl.mode = mode;
}

/*----------------------------------------------------------------------------*/
/**@brief LCD更新屏的驱动函数
   @param 屏更新的列位置：lcd_width, 屏更新的行位置lcd_height,
         更新内容地址：lcd_buff_add, 需要更新的数据长度：len
   @return 无
   @note 每次只更新一行
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_SEG"
void lcd_update(u8 lcd_width, u8 lcd_height, u16 lcd_buff_add, u8 len)
{
    SET_LCD_CS;
    if (lcd_height>=PAGE_MAX)
    {
        return;
    }
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

    SPIDMAPTRH = (u8)((u16)lcd_buff_add >> 8);  //DMA load start address
    SPIDMAPTRL = (u8)((u16)lcd_buff_add);
    SPIDMACNT = (len/2) - 1;
    while (!(SPI0CON & 0x80)); //等待发送完成
}

/*----------------------------------------------------------------------------*/
/**@brief LCD初始化命令
   @param [in] cmd LCD命令
   @return 无
   @note 初始化LCD屏显示时发送命令
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_SEG"
static void lcd_send_cmd(u8 cmd)
{
    CLR_LCD_A0;    //LCD_COMMAND
    CLR_LCD_CS;
    asm("nop");
    SPI0BUF = cmd;
    while (!(SPI0CON & 0x80)); //等待发送完成
    SET_LCD_CS;
}
#pragma location="DISP_SEG"
void lcd_contrast(u8 value)
{
    if (value > 15) {
        value = 15;
    }
    lcd_send_cmd(LCD_ELE_VOLUME);        //electronic Volume
    lcd_send_cmd(value);                   //line start
}

//LCD初始化
#pragma location="LCD_INIT_SEG"
void lcd_init(void)
{
    u16 offset;
    LCD_PORT_INIT();
    sys_ctl.contrast = LCD_CONTRAST_DEFAULT;
#if (LCD_SOFT_RESET)
    SET_LCD_RES;
    delay(20);
    CLR_LCD_RES;
    delay(255);
    SET_LCD_RES;
#endif
    delay(20);
    SPI0CON = 0x15;//0x1D;  //spi0两线模式,p0口
    SPIBAUD = 0x01;  //spi0波特率

    lcd_send_cmd(LCD_BIAS_SET | 0x00);    //lcd bias setting
    lcd_send_cmd(LCD_ADC_SEL | LCD_INVERT_DISPLAY);     //ADC	 invert display  //0x00 or 0x01
    lcd_send_cmd(LCD_COMMON_MODE & 0xc8); //common output mode //0xc0 or oxc8
    lcd_send_cmd(LCD_V0_RATIO | 0x06);    //Resistor Ratio
    lcd_contrast(sys_ctl.contrast);           //设置对比度
    lcd_send_cmd(LCD_POWER_MODE);         //lcd power control
    lcd_send_cmd(LCD_ON);
    draw_clear();
    //get CharacterDataAddr offset for font
    offset = spi_read16(calc_font_addr(0x4));
    CharacterDataAddr = calc_font_addr(offset + 0x6);
    //取字库长度
    CharacterDataLength = spi_read16(calc_font_addr(0));

}


//LCD初始化
#pragma location="INIT_SEG"
void lcd_quick_init(void)
{
    u16 offset;
    LCD_PORT_INIT();

    SPI0CON = 0x15;//0x1D;  //spi0两线模式,p0口
    SPIBAUD = 0x01;  //spi0波特率

    offset = spi_read16(calc_font_addr(0x4));
    CharacterDataAddr = calc_font_addr(offset + 0x6);
    //取字库长度
    CharacterDataLength = spi_read16(calc_font_addr(0));
}



/*----------------------------------------------------------------------------*/
/**@brief 转换GBK的编码到unicode 的编码。
   @param [in] gb ：字符的GBK编码。
   @return 无
   @note ConvertGBKtoUTF16
*/
/*----------------------------------------------------------------------------*/
#pragma location="FONT_SEG"
u16 ConvertGBKtoUTF16(u16 gb)
{
    u32 index;
    FontTable FontTabHead;
    u32 CharacterAddr;
    u16 offset = 0;
    if(gb<256)
        return gb;

    do {
        spi_read(&FontTabHead, calc_charset_addr(offset), sizeof(FontTable));
        FontTabHead.sign = swap_u16(FontTabHead.sign);
        FontTabHead.lang_id = swap_u16(FontTabHead.lang_id);
        FontTabHead.address = swap_u32(FontTabHead.address);
        if(FontTabHead.sign != 0xFEFE) {
            break;
        }
        CharacterAddr = calc_charset_addr(FontTabHead.address);
        offset += sizeof(FontTable);
    } while(FontTabHead.lang_id != 0);
#if 1
//GBK
    index=(BYTE1(gb) - 0x81) * 0xbf + (BYTE0(gb) - 0x40);
#else
//GB2312
    if(BYTE1(gb) <= 0xa9) {
	    index = (BYTE1(gb) - 0xa1) * 0x5e + (BYTE0(gb) - 0xa1);
    } else {
	    index = (BYTE1(gb) - 0xb0 + 9) * 0x5e + (BYTE0(gb) - 0xa1);
	}
#endif
    return spi_read16(index*2+CharacterAddr);
}

/*----------------------------------------------------------------------------*/
/**@brief   获取字的字库
   @param [in] utf16:  字的unicode编码;data : 字符的显示数据;length: data 的长度。
   @return   >0 ：数据的长度。  =0 ：没有找到字库
   @note GetCharacterData
*/
/*----------------------------------------------------------------------------*/
#pragma location="FONT_SEG"
u32 get_unicode_addr(u16 index)
{
    FontSegment FirstFont;
    u16 low, mid, high;
    low = 1;
    high = CharacterDataLength;
    while (1) {
        mid = (low + high) / 2;
        spi_read(&FirstFont, CharacterDataAddr + (mid - 1) * sizeof(FontSegment), sizeof(FontSegment));

        if (index < FirstFont.StartVaule) {
            high = mid -1;
        } else if (index > FirstFont.EndVaule) {
            low = mid + 1;
        } else {
            break;
        }

        if(low > high) {
            //找不到编码，使用'□'。这个是一定找得到的
            index = 0x25a1;
            low = 1;                        //重新开始查找
            high = CharacterDataLength;
        }
    }

    return ((u32)(index - FirstFont.StartVaule) * FRONT_STRUCT + FirstFont.startaddress + CharacterDataAddr);
}

//获取当前字符的宽度。如果是全角，则需要2个字符
u8 get_char_width(char ch)
{
    if (ch & 0x80) {
        return 12;              //全角字符的宽度
    }
    return spi_read8(get_unicode_addr(ch));
}


#if SLIDER_EN
//显示一个文字
#pragma location="FONT_SEG"
u8 get_char_data(char *disp_buf, u16 code, s16 x, u8 mode,u8 slider_width)
{
    u8 line, w;
    u8 font_w;
    u32 font_addr;
    u8 half_mode = (mode & TX_HALF);

    font_addr = get_unicode_addr(code);

    font_w = spi_read8(font_addr);
    if (font_w > CHAR_WIDTH_MAX) {
        font_w = CHAR_WIDTH_MAX;
    }
    font_addr = CharacterDataAddr + spi_read32(font_addr + 4);

    w = font_w;

    if ((x + font_w) > (COLUMN_MAX-slider_width)) {
        w = half_mode ? ((COLUMN_MAX-slider_width) - x) : 0;        //确定是否拷贝行末的半个字
    }
    if (w == 0) {
        return 0;                               //已经结束
    }
    if (x < 0) {
        if (half_mode && x + font_w > 0) {
            w = half_mode ? (x + font_w) : 0;        //确定是否拷贝行前的半个字
            font_addr -= x;                     //x为负数，即往后偏移x的绝对值
            x = 0;
        }
    }
    if (x >= 0) {
        disp_buf += x;
        for(line=0; line!=2; line++) {
            spi_read(disp_buf, font_addr, w);
            disp_buf += COLUMN_MAX;
            font_addr += font_w;
        }
    }
    return font_w;
}
//输出Unicode字符
#pragma location="FONT_SEG"
char *disp_put_unicode(char *disp_buf, char *str, s16 x, u8 mode,u8 slider_width)
{
    u8 w;

    while ((*str || *(str+1)) && x < (COLUMN_MAX-slider_width)) {
        w = get_char_data(disp_buf, *((u16 *)str), x, mode,slider_width);
        if(w == 0) {
            break;
        }
        x += w;
        str += 2;
    }
    return str;
}

//输出GBK字符
#pragma location="FONT_SEG"
char *disp_put_gbk(char * disp_buf, char * str, s16 x, u8 mode,u8 slider_width)
{
    u16 code = 0;
    u8 w;
    u8 ch_len;
    u8 center_flag = 0;
    if (x == TX_CENTER) {
        x = 0;
        center_flag = 1;
    }
    while (*str && x < (COLUMN_MAX-slider_width)) {
        if (*str & 0x80) {
            code = str[1] | (str[0] << 8);  //高位在前
            ch_len = 2;
        } else  {
            code = *str;
            ch_len = 1;
        }
        if (code >= 256) {
            //printf("gbk=%04x\n", code);
            //code = 0xb0a1;
            code=ConvertGBKtoUTF16(code);
            //printf("gbk=%04x\n", code);
        }
        w = get_char_data(disp_buf, code, x, mode,slider_width);
        if (w == 0) {
            break;
        }
        x += w;
        str += ch_len;
    }
    //居中处理
    if (center_flag && x < (COLUMN_MAX-slider_width)) {
        char *src_buf = disp_buf + x - 1;
        w = ((COLUMN_MAX-slider_width) - x) / 2;
        char *des_buf = src_buf + w;
        while (x--) {
            *des_buf = *src_buf;
            *(des_buf + (COLUMN_MAX-slider_width)) = *(src_buf + (COLUMN_MAX-slider_width));
            des_buf--;
            src_buf--;
        }
        while (w--) {
            *des_buf = 0;
            *(des_buf +(COLUMN_MAX-slider_width)) = 0;
            des_buf--;
        }
    }
    return str;
}


#else
//显示一个文字
#pragma location="FONT_SEG"
u8 get_char_data(char *disp_buf, u16 code, s16 x, u8 mode)
{
    u8 line, w;
    u8 font_w;
    u32 font_addr;
    u8 half_mode = (mode & TX_HALF);

    font_addr = get_unicode_addr(code);

    font_w = spi_read8(font_addr);
    if (font_w > CHAR_WIDTH_MAX) {
        font_w = CHAR_WIDTH_MAX;
    }
    font_addr = CharacterDataAddr + spi_read32(font_addr + 4);

    w = font_w;

    if ((x + font_w) > COLUMN_MAX) {
        w = half_mode ? (COLUMN_MAX - x) : 0;        //确定是否拷贝行末的半个字
    }
    if (w == 0) {
        return 0;                               //已经结束
    }
    if (x < 0) {
        if (half_mode && x + font_w > 0) {
            w = half_mode ? (x + font_w) : 0;        //确定是否拷贝行前的半个字
            font_addr -= x;                     //x为负数，即往后偏移x的绝对值
            x = 0;
        }
    }
    if (x >= 0) {
        disp_buf += x;
        for(line=0; line!=2; line++) {
            spi_read(disp_buf, font_addr, w);
            disp_buf += COLUMN_MAX;
            font_addr += font_w;
        }
    }
    return font_w;
}

//输出Unicode字符
#pragma location="FONT_SEG"
char *disp_put_unicode(char *disp_buf, char *str, s16 x, u8 mode)
{
    u8 w;

    while ((*str || *(str+1)) && x < COLUMN_MAX) {
        w = get_char_data(disp_buf, *((u16 *)str), x, mode);
        if(w == 0) {
            break;
        }
        x += w;
        str += 2;
    }
    return str;
}

//输出GBK字符
#pragma location="FONT_SEG"
char *disp_put_gbk(char *disp_buf, char *str, s16 x, u8 mode)
{
    u16 code = 0;
    u8 w;
    u8 ch_len;
    u8 center_flag = 0;
    if (x == TX_CENTER) {
        x = 0;
        center_flag = 1;
    }
    while (*str && x < COLUMN_MAX) {
        if (*str & 0x80) {
            code = str[1] | (str[0] << 8);  //高位在前
            ch_len = 2;
        } else  {
            code = *str;
            ch_len = 1;
        }
        if (code >= 256) {
            //printf("gbk=%04x\n", code);
            //code = 0xb0a1;
            code=ConvertGBKtoUTF16(code);
            //printf("gbk=%04x\n", code);
        }
        w = get_char_data(disp_buf, code, x, mode);
        if (w == 0) {
            break;
        }
        x += w;
        str += ch_len;
    }
    //居中处理
    if (center_flag && x < COLUMN_MAX) {
        char *src_buf = disp_buf + x - 1;
        w = (COLUMN_MAX - x) / 2;
        char *des_buf = src_buf + w;
        while (x--) {
            *des_buf = *src_buf;
            *(des_buf + COLUMN_MAX) = *(src_buf + COLUMN_MAX);
            des_buf--;
            src_buf--;
        }
        while (w--) {
            *des_buf = 0;
            *(des_buf + COLUMN_MAX) = 0;
            des_buf--;
        }
    }
    return str;
}
#endif
//区域取反
#pragma location="DISP_SEG"
void disp_invert(u8 x, u8 line, u8 width, u8 line_cnt)
{
    u8 i, j;
    char *disp_ptr;
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        //非完全模式下，进行修整
        if (line >= 2) {
            return;
        }
        if (line + line_cnt >= 2) {
            line_cnt = 2 - line;
        }
    }
    if (x+width > COLUMN_MAX || line+line_cnt > PAGE_MAX) {
        return;
    }

    for (i=0; i!=line_cnt; i++) {
        disp_ptr = &lcd_buf[(COLUMN_MAX * (line + i)) + x];
        for (j=0; j!=width; j++) {
            *disp_ptr = ~*disp_ptr;
            disp_ptr++;
        }
    }
}

//清空
#pragma location="DRAW_IMG_SEG"
void disp_clear(void)
{
    u16 disp_buf_len = 1024;
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        disp_buf_len = 256;                   //仅完全模式支持
    }
    memset(lcd_buf, 0, disp_buf_len);
}

//清空
#pragma location="DISP_SEG"
void line_clear(void)
{
    memset(lcd_buf, 0, 256);
}


//画字函数
//mode=0，不显示半字；mode=1，显示半字
#pragma location="DRAW_TEXT_SEG"
char *disp_text(char *str, s16 x, u8 line, u8 mode)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line != 0) {
            return 0;                             //仅完全模式支持
        }
    }
#if SLIDER_EN
    return disp_put_gbk(&lcd_buf[COLUMN_MAX*line], str, x, mode,0);
#else
    return disp_put_gbk(&lcd_buf[COLUMN_MAX*line], str, x, mode);
#endif
}

//画字函数
//mode=0，不显示半字；mode=1，显示半字
#pragma location="DRAW_TEXT_SEG"
char *disp_text_u(char *str, s16 x, u8 line, u8 mode)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line != 0) {
            return 0;                             //仅完全模式支持
        }
    }
#if SLIDER_EN
    return disp_put_unicode(&lcd_buf[COLUMN_MAX*line], str, x, mode,0);
#else
    return disp_put_unicode(&lcd_buf[COLUMN_MAX*line], str, x, mode);
#endif
}

#if SLIDER_EN
//画字函数
//mode=0，不显示半字；mode=1，显示半字   //界面右边显示滚动条
#pragma location="DRAW_TEXT_SEG"
char *disp_addslider_text(char *str, s16 x, u8 line, u8 mode)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line != 0) {
            return 0;                             //仅完全模式支持
        }
    }
    return disp_put_gbk(&lcd_buf[COLUMN_MAX*line], str, x, mode,6);
}

//画字函数
//mode=0，不显示半字；mode=1，显示半字
#pragma location="DRAW_TEXT_SEG"
char *disp_addsleder_text_u(char *str, s16 x, u8 line, u8 mode)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line != 0) {
            return 0;                             //仅完全模式支持
        }
    }
    return disp_put_unicode(&lcd_buf[COLUMN_MAX*line], str, x, mode,6);
}
#endif


//画图函数
#pragma location="DRAW_IMG_SEG"
void disp_draw(u16 imageindex, u8 x, u8 line)
{
    u32 res_addr;
    u8 icon_width, icon_line;                   //宽高应该都在256以内
    u32 icon_addr;
//    u16 icon_length;                            //大小最大为128x64 = 8192
    u8 i;
    char *disp_ptr;

    res_addr = calc_res_addr(imageindex * ICON_FILE_HEAD + 4);

    icon_width = spi_read16(res_addr);          //宽
    icon_line = spi_read16(res_addr+2) >> 3;    //行
    icon_addr = spi_read32(res_addr+8);         //资源起始

//    icon_length = spi_read32(res_addr+12);      //资源长度
   // printf("%u: w=%u; h=%u; addr=%04lx\n", imageindex, icon_width, icon_line, icon_addr);

    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //仅完全模式支持
        }
        if (line + icon_line >= 2) {
            icon_line = 2 - line;
        }
    }
    if (icon_line > 8) {
        return;
    }

    icon_addr = calc_res_addr(icon_addr);
    disp_ptr = &lcd_buf[x + COLUMN_MAX*line];
    for (i=0; i!=icon_line; i++) {
        spi_read(disp_ptr, icon_addr, icon_width);
        icon_addr += icon_width;
        disp_ptr += COLUMN_MAX;
    }
}

//写数字的函数。默认为左对齐
#pragma location="DRAW_IMG_SEG"
void disp_num(u16 num, u8 x, u8 line, u8 mode)
{
    u16 i;
    u8 n;
    u16 bound = 1;
    u8 wid = 7;
    u8 right = 0;       //右对齐
    u8 fill = 0;        //左边填充0
    u8 bound_bit = mode & DN_BOUND_MASK;    //数字最大位数
    u16 res_index = RES_MEDIUMNUM_0_BMP;
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //仅完全模式支持
        }
    }
    if (bound_bit == 0 || bound_bit > 4) {
        bound_bit = 4;
    }
    while (bound_bit--) {
        bound *= 10;                //计算数值区域
    }
    if (mode & DN_RIGHT) {
        right = 1;                  //右对齐
        if (mode & DN_ZERO) {
            fill = 1;               //右对齐，并填充0
        }
    }
    if (mode & DN_SMALL) {
        res_index = RES_SMALLNUM_0_BMP;
    } else if (mode & DN_BIG) {
        res_index = RES_BIGNUM_0_BMP;
        wid = 15;
    } else if (mode & DN_SSMALL) {
        wid = 6;
        res_index = RES_SMALLNUM_SS0_BMP;
    }

    if (num >= bound) {             //限定最大值
        num = bound - 1;
    }
    for (i=bound/10; i>=1; i/=10) {
        n = num / i;
        if (n || fill || (i==1)) {              //当有值、填0、左对齐、个位时，一定显示
            disp_draw(res_index+n, x, line);
            fill = 1;                           //出现过数据，则一定需要填充数据，包括0
            x += wid;
        } else if (!fill && right) {            //右对齐不填充
            x += wid;
        }
        num %= i;
    }
}

//显示中格式时间。。分秒
#pragma location="DRAW_IMG_SEG"
void disp_medium_time1(u8 min, u8 sec, u8 x, u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //仅完全模式支持
        }
    }
    disp_num(min, x, line, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, x + 18, line);
    disp_num(sec, x + 23, line, DN_RIGHT | DN_ZERO | 2);
}

//显示小格式时间。总宽度为 7*2 + 5 + 7*2 = 33
#pragma location="DRAW_IMG_SEG"
void disp_small_time(u8 min, u8 sec, u8 x, u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //仅完全模式支持
        }
    }
    disp_num(min, x, line, DN_RIGHT | DN_ZERO | DN_SMALL | 2);
    disp_draw(RES_SMALLNUM_COLON_BMP, x + 14, line);
    disp_num(sec, x + 19, line, DN_RIGHT | DN_ZERO | DN_SMALL | 2);
}

//显示中格式时间。总宽度为
#pragma location="DRAW_IMG_SEG"
void disp_medium_time(u8 hour, u8 min, u8 sec, u8 x, u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //仅完全模式支持
        }
    }
    disp_num(hour, x, line, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, x + 18, line);
    disp_num(min, x + 23, line, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, x + 41, line);
    disp_num(sec, x + 46, line, DN_RIGHT | DN_ZERO | 2);
}

//显示频率值
#pragma location="DRAW_FREQ_SEG"
u8 disp_freq_line_value(u8 v_num)
{
    u8 i = 0;
    u16 value = freq_buf[v_num];
    while (value > tbl_freq_val[i] && i < 14) {
        i++;
    }
    return i;
}

//显示频率线
#pragma location="DRAW_FREQ_SEG"
void disp_freq_line(u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //仅完全模式支持
        }
    }
    char *disp_ptr = &lcd_buf[COLUMN_MAX*line];
    u8 x = 8;
    u8 i, k, v;
    for (i=0; i!=16; i++) {
        //计算显示值
        v = disp_freq_line_value(i);
        if (v + 1 < disp_freq_show_buf[i]) {
            v = disp_freq_show_buf[i] - 2;
        } else if (v && v == disp_freq_show_buf[i]) {
            v = disp_freq_show_buf[i] - 1;
        }
        disp_freq_show_buf[i] = v;

        //显示
        for (k=0; k!=6; k++) {
            disp_ptr[x] = tbl_freq_line_0[v];
            disp_ptr[COLUMN_MAX+x] = tbl_freq_line_1[v];
            x++;
        }
        disp_ptr[x] = 0;
        disp_ptr[COLUMN_MAX+x] = 0;
        x++;
    }
}

//显示进度条
void progressbar(u8 percent, u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //仅完全模式支持
        }
    }
    u8 x = 29 + (percent * 65 >> 7);
    disp_draw(RES_OCX_BAR_BMP, 29, line);
    disp_draw(RES_OCX_INDEX_BMP, x, line);
}

//SPI更新显示
#pragma location="DISP_SEG"
void disp_update(void)
{
    u8 i;
    char *line_ptr = lcd_buf;
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        return;                             //仅完全模式支持
    }
    for (i=0; i!=PAGE_MAX; i++) {
        lcd_update(0, i, (u16)line_ptr, COLUMN_MAX);
        line_ptr += COLUMN_MAX;
    }
}

//SPI按行更新显示，一次更新两行。
#pragma location="DISP_SEG"
void line_update(u8 line)
{
    u8 i;
    char *line_ptr = lcd_buf;
    if (line >= PAGE_MAX-1) {
        return;
    }
    for (i=0; i!=2; i++) {
        lcd_update(0, i + line, (u16)line_ptr, COLUMN_MAX);
        line_ptr += COLUMN_MAX;
    }
}

//SPI按行更新并清空
#pragma location="DISP_SEG"
void line_update_clear(u8 line)
{
    line_update(line);
    line_clear();
}
//按行直接清空屏显
#pragma location="DRAW_IMG_SEG1"

void draw_clear(void)
{
    u8 i;
    memset(lcd_buf, 0, COLUMN_MAX);
    for (i=0; i!=8; i++) {
        lcd_update(0, i, (u16)lcd_buf, COLUMN_MAX);
    }
}

//快速直接画图函数。要求图像宽度小于256Byte
#pragma location="DRAW_IMG_SEG"
void draw_img(u16 imageindex, u8 x, u8 line)
{
    u32 res_addr;
    u8 icon_width, icon_line;                 //宽高应该都在256以内
    u32 icon_addr;
    u8 i;

    res_addr = calc_res_addr(imageindex * ICON_FILE_HEAD + 4);

    icon_width = spi_read16(res_addr);          //宽
    icon_line = spi_read16(res_addr+2) >> 3;    //行
    icon_addr = calc_res_addr(spi_read32(res_addr+8));         //资源起始

//    icon_length = spi_read32(res_addr+12);      //资源长度
//    printf("%u: w=%u; h=%u; addr=%04lx\n", imageindex, icon_width, icon_height, icon_addr);
    for (i=0; i!=icon_line; i++) {
        spi_read(lcd_buf, icon_addr, icon_width);
        lcd_update(x, i + line, (u16)lcd_buf, icon_width);
        icon_addr += icon_width;
    }
}

//等待界面
#pragma location="DRAW_IMG_SEG1"
void draw_wait(void)
{
    u8 i;
    line_clear();
    for (i=0; i!=3; i++) {
        lcd_update(0, i, (u16)lcd_buf, COLUMN_MAX);
    }
    for (i=5; i!=8; i++) {
        lcd_update(0, i, (u16)lcd_buf, COLUMN_MAX);
    }
   // disp_llp_text(llp_waiting, TX_CENTER, 0, 0);
    disp_menu(STR_WAITING, 0, 0, 0, ME_CENTER);
    line_update(3);
}
//获取语言种数
#pragma location="DRAW_IMG_SEG1"
void disp_language_numget(void)
{
    u32 res_addr;
    res_addr = calc_word_addr(0);
    sys_ctl.lang_num = spi_read16(res_addr+12);
}

//画菜单文字函数

/*#pragma location="DRAW_IMG_SEG"
void disp_menu(u16 menuindex, s16 x, u8 line, u8 mode)
{
    u32 res_addr;
    u16 icon_width,icon_width1, icon_line;                   //宽高应该都在256以内
    u32 icon_addr;
//    u16 icon_length;                            //大小最大为128x64 = 8192
    u8 i;
    char *disp_ptr;
    menuindex = menuindex*sys_ctl.lang_num + sys_ctl.lang_id;
    res_addr = calc_word_addr(menuindex * ICON_FILE_HEAD);
   // printf("icon_add%ld\n",res_addr);
    icon_width = spi_read16(res_addr);          //宽
    if(mode & ME_LINES)
    {
       icon_width1 = spi_read16(res_addr+8);
    }
   // printf("width%d\n",icon_width);
    icon_line = spi_read16(res_addr+2)>> 3;    //行
    icon_addr = spi_read32(res_addr+4);         //资源起始
   //  printf("icon_add%ld\n",icon_addr);
//    icon_length = spi_read32(res_addr+12);      //资源长度
  //  printf("%u: w=%u; h=%u; addr=%04lx\n", imageindex, icon_width, icon_line, icon_addr);
    icon_addr = calc_word_addr(icon_addr);
    if (lcd_ctl.mode == LCD_FULL_MODE)
    {
	    disp_ptr = &lcd_buf[x + COLUMN_MAX*line];
    }
    else
    {
        disp_ptr =  &lcd_buf[x];
    }
    if(mode & ME_CENTER)
    {
        disp_ptr -= x;
        x = COLUMN_MAX - icon_width;
        x >>=1;
        disp_ptr += x;
    }
    for (i=0; i!=icon_line; i++) {
        spi_read(disp_ptr, icon_addr, icon_width);
        icon_addr += icon_width;
        disp_ptr += COLUMN_MAX;
    }
   if(mode & ME_MSG)
       line_update_clear(2);
    if(mode & ME_LINES)
    {
        if(!icon_width1)
            return;
        disp_ptr =  &lcd_buf[x];
        for (i=0; i!=icon_line; i++) {
            spi_read(disp_ptr, icon_addr, icon_width1);
            icon_addr += icon_width1;
            disp_ptr += COLUMN_MAX;
        }
    }
}
*/
/*#pragma location="DRAW_IMG_SEG"
void disp_menu(u16 menuindex, s16 x, u8 line, u8 mode)
{
    u32 res_addr;
    u8 icon_width,icon_width1;
    //u16 icon_line;                   //宽高应该都在256以内
    u32 icon_addr;
    //    u16 icon_length;                            //大小最大为128x64 = 8192
    u8 i;
    char *disp_ptr;
    //u8 lines_flag;
    u8 read_width; //应该读取的宽度
    menuindex = menuindex*sys_ctl.lang_num + sys_ctl.lang_id;
    res_addr = calc_word_addr(menuindex * ICON_FILE_HEAD);
    // printf("icon_add%ld\n",res_addr);
    icon_width = spi_read8(res_addr);          //宽
    icon_width1 = spi_read8(res_addr+13);       //是否有两行
    if(icon_width1)
    {
        icon_width1 = spi_read8(res_addr+8);   //第二行宽度
    }
    read_width = icon_width;
    //icon_line = spi_read16(res_addr+2)>> 3;    //行
   // printf("line%d\n",icon_line);
    //printf("w:%d,w1:%d\n",icon_width,icon_width1);
    icon_addr = spi_read32(res_addr+4);         //资源起始
    icon_addr = calc_word_addr(icon_addr);
    s16 s = x;
    if(x<0)
        x = 0;
    if (lcd_ctl.mode == LCD_FULL_MODE)
    {
        disp_ptr = &lcd_buf[x + COLUMN_MAX*line];
    }
    else
    {
        disp_ptr =  &lcd_buf[x];
    }
    if(mode & ME_CENTER)
    {
        disp_ptr -= x;
        x = COLUMN_MAX - icon_width;
        x >>=1;
        disp_ptr += x;
    }
    if(s<0)
    {
        icon_addr = icon_addr - s;
        read_width = icon_width + s;
    }
    //printf("w:%d,w1:%d\n",read_width,icon_width1);
    for (i=0; i!=2; i++) {
        spi_read(disp_ptr, icon_addr, read_width);
        icon_addr += icon_width;
        disp_ptr += COLUMN_MAX;
    }
    if(icon_width1)
    {
        if(mode & ME_LINES)
        {
            line_update_clear(2);
            disp_ptr =  &lcd_buf[x];
            read_width = icon_width1;
        }
        else
        {
            if(s<0)
            {
                icon_addr += s;
            }
            disp_ptr = &lcd_buf[x] + read_width;
            read_width = COLUMN_MAX - read_width;
            if(icon_width1 < read_width)
                read_width = icon_width1;
        }
        for (i=0; i!=2; i++) {
            spi_read(disp_ptr, icon_addr, read_width);
            icon_addr += icon_width1;
            disp_ptr += COLUMN_MAX;
        }
    }
}
*/
/*
#pragma location="DRAW_IMG_SEG"
void disp_menu(u16 menuindex, u8 x, s16 mov_x, u8 line, u8 mode)
{
    u32 res_addr;
    u8 icon_width,icon_width1;
    //u16 icon_line;                   //宽高应该都在256以内
    u32 icon_addr;
    //    u16 icon_length;                            //大小最大为128x64 = 8192
    u8 i;
    char *disp_ptr;

    //u8 lines_flag = 1;
    u8 read_width; //应该读取的宽度
    menuindex = menuindex*sys_ctl.lang_num + sys_ctl.lang_id;
    res_addr = calc_word_addr(menuindex * ICON_FILE_HEAD);
    // printf("icon_add%ld\n",res_addr);
    icon_width = spi_read8(res_addr);          //宽
    icon_width1 = spi_read8(res_addr+13);       //是否有两行
    if(icon_width1)
    {
        icon_width1 = spi_read8(res_addr+8);   //第二行宽度
    }
    move_ctl.name_wide1 = icon_width + icon_width1;
    read_width = icon_width;
    //icon_line = spi_read16(res_addr+2)>> 3;    //行
   // printf("line%d\n",icon_line);
    //printf("w:%d,w1:%d\n",icon_width,icon_width1);
    icon_addr = spi_read32(res_addr+4);         //资源起始
    icon_addr = calc_word_addr(icon_addr);
    //s16 s = mov_x;
    if (lcd_ctl.mode == LCD_FULL_MODE)
    {
        disp_ptr = &lcd_buf[x + COLUMN_MAX*line];
    }
    else
    {
        disp_ptr =  &lcd_buf[x];
    }
    if(mode & ME_CENTER)
    {
        disp_ptr -= x;
        x = COLUMN_MAX - icon_width;
        x >>=1;
        disp_ptr += x;
    }
    if(mov_x<0)
    {
        icon_addr = icon_addr - mov_x;
        read_width = icon_width + mov_x;
    }
    if (read_width + x > COLUMN_MAX)
    {
        read_width = COLUMN_MAX - x;
    }
    //printf("w:%d,w1:%d\n",read_width,icon_width1);
    for (i=0; i!=2; i++) {
        spi_read(disp_ptr, icon_addr, read_width);
        icon_addr += icon_width;
        disp_ptr += COLUMN_MAX;
    }
    if(mode & ME_MSG)
        line_update_clear(2);
    if(icon_width1)
    {
        if(mode & ME_LINES)
        {
            //line_update_clear(2);
            disp_ptr =  &lcd_buf[x];
            read_width = icon_width1;
        }
        else
        {
            if(mov_x<0)
            {
                icon_addr += mov_x;
            }
            disp_ptr = &lcd_buf[x] + read_width ;
            read_width = COLUMN_MAX - read_width - x;
            if(icon_width1 < read_width)
                read_width = icon_width1;
        }
        for (i=0; i!=2; i++) {
            spi_read(disp_ptr, icon_addr, read_width);
            icon_addr += icon_width1;
            disp_ptr += COLUMN_MAX;
        }
    }
}
*/

#pragma location="DRAW_IMG_SEG"
void disp_menu(u16 menuindex, u8 x, s16 mov_x, u8 line, u8 mode)
{
    u32 res_addr;
    u8 icon_width,icon_width1;
    //u16 icon_line;                   //宽高应该都在256以内
    u32 icon_addr;
    //    u16 icon_length;                            //大小最大为128x64 = 8192
    u8 i;
    char *disp_ptr;

    u8 lines_flag = 1;
    u8 read_width; //应该读取的宽度
    menuindex = menuindex*sys_ctl.lang_num + sys_ctl.lang_id;
    res_addr = calc_word_addr(menuindex * ICON_FILE_HEAD);
    // printf("icon_add%ld\n",res_addr);
    icon_width = spi_read8(res_addr);          //宽
    icon_width1 = spi_read8(res_addr+13);       //是否有两行
    if(icon_width1)
    {
        icon_width1 = spi_read8(res_addr+8);   //第二行宽度
    }
    move_ctl.name_wide1 = icon_width + icon_width1;
    read_width = icon_width;
    //icon_line = spi_read16(res_addr+2)>> 3;    //行
   // printf("line%d\n",icon_line);
    //printf("w:%d,w1:%d\n",icon_width,icon_width1);
    icon_addr = spi_read32(res_addr+4);         //资源起始
    icon_addr = calc_word_addr(icon_addr);
    if (lcd_ctl.mode == LCD_FULL_MODE)
    {
        disp_ptr = &lcd_buf[x + COLUMN_MAX*line];
    }
    else
    {
        disp_ptr =  &lcd_buf[x];
    }
    if(mode & ME_CENTER)
    {
        disp_ptr -= x;
        x = COLUMN_MAX - icon_width;
        x >>=1;
        disp_ptr += x;
    }
    if(mov_x<0)
    {
        icon_addr = icon_addr - mov_x;
        read_width = icon_width + mov_x;
    }
    if (read_width + x > COLUMN_MAX)
    {
        read_width = COLUMN_MAX - x;
        lines_flag = 0;
    }
    //printf("w:%d,w1:%d\n",read_width,icon_width1);
    for (i=0; i!=2; i++) {
        spi_read(disp_ptr, icon_addr, read_width);
        icon_addr += icon_width;
        disp_ptr += COLUMN_MAX;
    }
    if(mode & ME_MSG)
        line_update_clear(2);
    if(!lines_flag)
        return;
    if(icon_width1)
    {
        if(mode & ME_LINES)
        {
            //line_update_clear(2);
            disp_ptr =  &lcd_buf[x];
            read_width = icon_width1;
        }
        else
        {
            if(mov_x<0)
            {
                icon_addr += mov_x;
            }
            disp_ptr = &lcd_buf[x] + read_width ;
            read_width = COLUMN_MAX - read_width - x;
            /*两行之间留点空隙*/
            if(read_width >= 6)
            {
                read_width -= 6;
                disp_ptr += 6;
            }
            else
            {
                read_width = 0;
                return;
            }
            if(icon_width1 < read_width)
                read_width = icon_width1;
        }
        for (i=0; i!=2; i++) {
            spi_read(disp_ptr, icon_addr, read_width);
            icon_addr += icon_width1;
            disp_ptr += COLUMN_MAX;
        }
    }
}
#endif
