/*****************************************************************************
 * Module    : Display
 * File      : lcd.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD����
*****************************************************************************/
#include "include.h"
#if LCD_THEME
#include "task.h"

extern u32 calc_res_addr(u32 res_addr);         //������Դ�ľ��Ե�ַ
extern u32 calc_charset_addr(u32 res_addr);     //��������ľ��Ե�ַ
extern u32 calc_font_addr(u32 res_addr);        //�����ֿ�ľ��Ե�ַ
extern u32 calc_word_addr(u32 res_addr);        //�����ֿ�ͼƬ�ľ��Ե�ַ

u32 CharacterDataAddr;    ///<unicode��Դ����ʼ��ַ
u16 CharacterDataLength;

u8 disp_freq_show_buf[16];      //��¼�ϴε���ʾֵ
extern u16 freq_buf[64];

struct {
    u8 mode;                //LCD��ģʽ������1k����ȫģʽ��256Byte��С����ģʽ
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


//1K��LCD_BUFF����512Byteͬʱ������LRC_READ_BUF,f_writeʱ��win_buf
IAR_XDATA_A char lcd_buf[PAGE_MAX * COLUMN_MAX] @ "LCD_BUFF";    //�����ݵ�buffer

//����LCD Buffer��λ�á��ڲ�������£���Ҫ�Ķ����λ��
#pragma location="INIT_SEG_1"
void set_lcd_buf(u8 mode)
{
    lcd_ctl.mode = mode;
}

/*----------------------------------------------------------------------------*/
/**@brief LCD����������������
   @param �����µ���λ�ã�lcd_width, �����µ���λ��lcd_height,
         �������ݵ�ַ��lcd_buff_add, ��Ҫ���µ����ݳ��ȣ�len
   @return ��
   @note ÿ��ֻ����һ��
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
    while (!(SPI0CON & 0x80)); //�ȴ��������
    SPI0BUF = LCD_COLUMN_LOW | (lcd_width & 0x0f);
    while (!(SPI0CON & 0x80)); //�ȴ��������
    SPI0BUF = LCD_PAGE_ADDR | lcd_height;
    while (!(SPI0CON & 0x80)); //�ȴ��������

    SET_LCD_CS;
    SET_LCD_A0;
    asm("nop");
    CLR_LCD_CS;

    SPIDMAPTRH = (u8)((u16)lcd_buff_add >> 8);  //DMA load start address
    SPIDMAPTRL = (u8)((u16)lcd_buff_add);
    SPIDMACNT = (len/2) - 1;
    while (!(SPI0CON & 0x80)); //�ȴ��������
}

/*----------------------------------------------------------------------------*/
/**@brief LCD��ʼ������
   @param [in] cmd LCD����
   @return ��
   @note ��ʼ��LCD����ʾʱ��������
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_SEG"
static void lcd_send_cmd(u8 cmd)
{
    CLR_LCD_A0;    //LCD_COMMAND
    CLR_LCD_CS;
    asm("nop");
    SPI0BUF = cmd;
    while (!(SPI0CON & 0x80)); //�ȴ��������
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

//LCD��ʼ��
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
    SPI0CON = 0x15;//0x1D;  //spi0����ģʽ,p0��
    SPIBAUD = 0x01;  //spi0������

    lcd_send_cmd(LCD_BIAS_SET | 0x00);    //lcd bias setting
    lcd_send_cmd(LCD_ADC_SEL | LCD_INVERT_DISPLAY);     //ADC	 invert display  //0x00 or 0x01
    lcd_send_cmd(LCD_COMMON_MODE & 0xc8); //common output mode //0xc0 or oxc8
    lcd_send_cmd(LCD_V0_RATIO | 0x06);    //Resistor Ratio
    lcd_contrast(sys_ctl.contrast);           //���öԱȶ�
    lcd_send_cmd(LCD_POWER_MODE);         //lcd power control
    lcd_send_cmd(LCD_ON);
    draw_clear();
    //get CharacterDataAddr offset for font
    offset = spi_read16(calc_font_addr(0x4));
    CharacterDataAddr = calc_font_addr(offset + 0x6);
    //ȡ�ֿⳤ��
    CharacterDataLength = spi_read16(calc_font_addr(0));

}


//LCD��ʼ��
#pragma location="INIT_SEG"
void lcd_quick_init(void)
{
    u16 offset;
    LCD_PORT_INIT();

    SPI0CON = 0x15;//0x1D;  //spi0����ģʽ,p0��
    SPIBAUD = 0x01;  //spi0������

    offset = spi_read16(calc_font_addr(0x4));
    CharacterDataAddr = calc_font_addr(offset + 0x6);
    //ȡ�ֿⳤ��
    CharacterDataLength = spi_read16(calc_font_addr(0));
}



/*----------------------------------------------------------------------------*/
/**@brief ת��GBK�ı��뵽unicode �ı��롣
   @param [in] gb ���ַ���GBK���롣
   @return ��
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
/**@brief   ��ȡ�ֵ��ֿ�
   @param [in] utf16:  �ֵ�unicode����;data : �ַ�����ʾ����;length: data �ĳ��ȡ�
   @return   >0 �����ݵĳ��ȡ�  =0 ��û���ҵ��ֿ�
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
            //�Ҳ������룬ʹ��'��'�������һ���ҵõ���
            index = 0x25a1;
            low = 1;                        //���¿�ʼ����
            high = CharacterDataLength;
        }
    }

    return ((u32)(index - FirstFont.StartVaule) * FRONT_STRUCT + FirstFont.startaddress + CharacterDataAddr);
}

//��ȡ��ǰ�ַ��Ŀ�ȡ������ȫ�ǣ�����Ҫ2���ַ�
u8 get_char_width(char ch)
{
    if (ch & 0x80) {
        return 12;              //ȫ���ַ��Ŀ��
    }
    return spi_read8(get_unicode_addr(ch));
}


#if SLIDER_EN
//��ʾһ������
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
        w = half_mode ? ((COLUMN_MAX-slider_width) - x) : 0;        //ȷ���Ƿ񿽱���ĩ�İ����
    }
    if (w == 0) {
        return 0;                               //�Ѿ�����
    }
    if (x < 0) {
        if (half_mode && x + font_w > 0) {
            w = half_mode ? (x + font_w) : 0;        //ȷ���Ƿ񿽱���ǰ�İ����
            font_addr -= x;                     //xΪ������������ƫ��x�ľ���ֵ
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
//���Unicode�ַ�
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

//���GBK�ַ�
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
            code = str[1] | (str[0] << 8);  //��λ��ǰ
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
    //���д���
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
//��ʾһ������
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
        w = half_mode ? (COLUMN_MAX - x) : 0;        //ȷ���Ƿ񿽱���ĩ�İ����
    }
    if (w == 0) {
        return 0;                               //�Ѿ�����
    }
    if (x < 0) {
        if (half_mode && x + font_w > 0) {
            w = half_mode ? (x + font_w) : 0;        //ȷ���Ƿ񿽱���ǰ�İ����
            font_addr -= x;                     //xΪ������������ƫ��x�ľ���ֵ
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

//���Unicode�ַ�
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

//���GBK�ַ�
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
            code = str[1] | (str[0] << 8);  //��λ��ǰ
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
    //���д���
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
//����ȡ��
#pragma location="DISP_SEG"
void disp_invert(u8 x, u8 line, u8 width, u8 line_cnt)
{
    u8 i, j;
    char *disp_ptr;
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        //����ȫģʽ�£���������
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

//���
#pragma location="DRAW_IMG_SEG"
void disp_clear(void)
{
    u16 disp_buf_len = 1024;
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        disp_buf_len = 256;                   //����ȫģʽ֧��
    }
    memset(lcd_buf, 0, disp_buf_len);
}

//���
#pragma location="DISP_SEG"
void line_clear(void)
{
    memset(lcd_buf, 0, 256);
}


//���ֺ���
//mode=0������ʾ���֣�mode=1����ʾ����
#pragma location="DRAW_TEXT_SEG"
char *disp_text(char *str, s16 x, u8 line, u8 mode)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line != 0) {
            return 0;                             //����ȫģʽ֧��
        }
    }
#if SLIDER_EN
    return disp_put_gbk(&lcd_buf[COLUMN_MAX*line], str, x, mode,0);
#else
    return disp_put_gbk(&lcd_buf[COLUMN_MAX*line], str, x, mode);
#endif
}

//���ֺ���
//mode=0������ʾ���֣�mode=1����ʾ����
#pragma location="DRAW_TEXT_SEG"
char *disp_text_u(char *str, s16 x, u8 line, u8 mode)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line != 0) {
            return 0;                             //����ȫģʽ֧��
        }
    }
#if SLIDER_EN
    return disp_put_unicode(&lcd_buf[COLUMN_MAX*line], str, x, mode,0);
#else
    return disp_put_unicode(&lcd_buf[COLUMN_MAX*line], str, x, mode);
#endif
}

#if SLIDER_EN
//���ֺ���
//mode=0������ʾ���֣�mode=1����ʾ����   //�����ұ���ʾ������
#pragma location="DRAW_TEXT_SEG"
char *disp_addslider_text(char *str, s16 x, u8 line, u8 mode)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line != 0) {
            return 0;                             //����ȫģʽ֧��
        }
    }
    return disp_put_gbk(&lcd_buf[COLUMN_MAX*line], str, x, mode,6);
}

//���ֺ���
//mode=0������ʾ���֣�mode=1����ʾ����
#pragma location="DRAW_TEXT_SEG"
char *disp_addsleder_text_u(char *str, s16 x, u8 line, u8 mode)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line != 0) {
            return 0;                             //����ȫģʽ֧��
        }
    }
    return disp_put_unicode(&lcd_buf[COLUMN_MAX*line], str, x, mode,6);
}
#endif


//��ͼ����
#pragma location="DRAW_IMG_SEG"
void disp_draw(u16 imageindex, u8 x, u8 line)
{
    u32 res_addr;
    u8 icon_width, icon_line;                   //���Ӧ�ö���256����
    u32 icon_addr;
//    u16 icon_length;                            //��С���Ϊ128x64 = 8192
    u8 i;
    char *disp_ptr;

    res_addr = calc_res_addr(imageindex * ICON_FILE_HEAD + 4);

    icon_width = spi_read16(res_addr);          //��
    icon_line = spi_read16(res_addr+2) >> 3;    //��
    icon_addr = spi_read32(res_addr+8);         //��Դ��ʼ

//    icon_length = spi_read32(res_addr+12);      //��Դ����
   // printf("%u: w=%u; h=%u; addr=%04lx\n", imageindex, icon_width, icon_line, icon_addr);

    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //����ȫģʽ֧��
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

//д���ֵĺ�����Ĭ��Ϊ�����
#pragma location="DRAW_IMG_SEG"
void disp_num(u16 num, u8 x, u8 line, u8 mode)
{
    u16 i;
    u8 n;
    u16 bound = 1;
    u8 wid = 7;
    u8 right = 0;       //�Ҷ���
    u8 fill = 0;        //������0
    u8 bound_bit = mode & DN_BOUND_MASK;    //�������λ��
    u16 res_index = RES_MEDIUMNUM_0_BMP;
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //����ȫģʽ֧��
        }
    }
    if (bound_bit == 0 || bound_bit > 4) {
        bound_bit = 4;
    }
    while (bound_bit--) {
        bound *= 10;                //������ֵ����
    }
    if (mode & DN_RIGHT) {
        right = 1;                  //�Ҷ���
        if (mode & DN_ZERO) {
            fill = 1;               //�Ҷ��룬�����0
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

    if (num >= bound) {             //�޶����ֵ
        num = bound - 1;
    }
    for (i=bound/10; i>=1; i/=10) {
        n = num / i;
        if (n || fill || (i==1)) {              //����ֵ����0������롢��λʱ��һ����ʾ
            disp_draw(res_index+n, x, line);
            fill = 1;                           //���ֹ����ݣ���һ����Ҫ������ݣ�����0
            x += wid;
        } else if (!fill && right) {            //�Ҷ��벻���
            x += wid;
        }
        num %= i;
    }
}

//��ʾ�и�ʽʱ�䡣������
#pragma location="DRAW_IMG_SEG"
void disp_medium_time1(u8 min, u8 sec, u8 x, u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //����ȫģʽ֧��
        }
    }
    disp_num(min, x, line, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, x + 18, line);
    disp_num(sec, x + 23, line, DN_RIGHT | DN_ZERO | 2);
}

//��ʾС��ʽʱ�䡣�ܿ��Ϊ 7*2 + 5 + 7*2 = 33
#pragma location="DRAW_IMG_SEG"
void disp_small_time(u8 min, u8 sec, u8 x, u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //����ȫģʽ֧��
        }
    }
    disp_num(min, x, line, DN_RIGHT | DN_ZERO | DN_SMALL | 2);
    disp_draw(RES_SMALLNUM_COLON_BMP, x + 14, line);
    disp_num(sec, x + 19, line, DN_RIGHT | DN_ZERO | DN_SMALL | 2);
}

//��ʾ�и�ʽʱ�䡣�ܿ��Ϊ
#pragma location="DRAW_IMG_SEG"
void disp_medium_time(u8 hour, u8 min, u8 sec, u8 x, u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //����ȫģʽ֧��
        }
    }
    disp_num(hour, x, line, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, x + 18, line);
    disp_num(min, x + 23, line, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, x + 41, line);
    disp_num(sec, x + 46, line, DN_RIGHT | DN_ZERO | 2);
}

//��ʾƵ��ֵ
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

//��ʾƵ����
#pragma location="DRAW_FREQ_SEG"
void disp_freq_line(u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //����ȫģʽ֧��
        }
    }
    char *disp_ptr = &lcd_buf[COLUMN_MAX*line];
    u8 x = 8;
    u8 i, k, v;
    for (i=0; i!=16; i++) {
        //������ʾֵ
        v = disp_freq_line_value(i);
        if (v + 1 < disp_freq_show_buf[i]) {
            v = disp_freq_show_buf[i] - 2;
        } else if (v && v == disp_freq_show_buf[i]) {
            v = disp_freq_show_buf[i] - 1;
        }
        disp_freq_show_buf[i] = v;

        //��ʾ
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

//��ʾ������
void progressbar(u8 percent, u8 line)
{
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        if (line >= 2) {
            return;                             //����ȫģʽ֧��
        }
    }
    u8 x = 29 + (percent * 65 >> 7);
    disp_draw(RES_OCX_BAR_BMP, 29, line);
    disp_draw(RES_OCX_INDEX_BMP, x, line);
}

//SPI������ʾ
#pragma location="DISP_SEG"
void disp_update(void)
{
    u8 i;
    char *line_ptr = lcd_buf;
    if (lcd_ctl.mode != LCD_FULL_MODE) {
        return;                             //����ȫģʽ֧��
    }
    for (i=0; i!=PAGE_MAX; i++) {
        lcd_update(0, i, (u16)line_ptr, COLUMN_MAX);
        line_ptr += COLUMN_MAX;
    }
}

//SPI���и�����ʾ��һ�θ������С�
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

//SPI���и��²����
#pragma location="DISP_SEG"
void line_update_clear(u8 line)
{
    line_update(line);
    line_clear();
}
//����ֱ���������
#pragma location="DRAW_IMG_SEG1"

void draw_clear(void)
{
    u8 i;
    memset(lcd_buf, 0, COLUMN_MAX);
    for (i=0; i!=8; i++) {
        lcd_update(0, i, (u16)lcd_buf, COLUMN_MAX);
    }
}

//����ֱ�ӻ�ͼ������Ҫ��ͼ����С��256Byte
#pragma location="DRAW_IMG_SEG"
void draw_img(u16 imageindex, u8 x, u8 line)
{
    u32 res_addr;
    u8 icon_width, icon_line;                 //���Ӧ�ö���256����
    u32 icon_addr;
    u8 i;

    res_addr = calc_res_addr(imageindex * ICON_FILE_HEAD + 4);

    icon_width = spi_read16(res_addr);          //��
    icon_line = spi_read16(res_addr+2) >> 3;    //��
    icon_addr = calc_res_addr(spi_read32(res_addr+8));         //��Դ��ʼ

//    icon_length = spi_read32(res_addr+12);      //��Դ����
//    printf("%u: w=%u; h=%u; addr=%04lx\n", imageindex, icon_width, icon_height, icon_addr);
    for (i=0; i!=icon_line; i++) {
        spi_read(lcd_buf, icon_addr, icon_width);
        lcd_update(x, i + line, (u16)lcd_buf, icon_width);
        icon_addr += icon_width;
    }
}

//�ȴ�����
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
//��ȡ��������
#pragma location="DRAW_IMG_SEG1"
void disp_language_numget(void)
{
    u32 res_addr;
    res_addr = calc_word_addr(0);
    sys_ctl.lang_num = spi_read16(res_addr+12);
}

//���˵����ֺ���

/*#pragma location="DRAW_IMG_SEG"
void disp_menu(u16 menuindex, s16 x, u8 line, u8 mode)
{
    u32 res_addr;
    u16 icon_width,icon_width1, icon_line;                   //���Ӧ�ö���256����
    u32 icon_addr;
//    u16 icon_length;                            //��С���Ϊ128x64 = 8192
    u8 i;
    char *disp_ptr;
    menuindex = menuindex*sys_ctl.lang_num + sys_ctl.lang_id;
    res_addr = calc_word_addr(menuindex * ICON_FILE_HEAD);
   // printf("icon_add%ld\n",res_addr);
    icon_width = spi_read16(res_addr);          //��
    if(mode & ME_LINES)
    {
       icon_width1 = spi_read16(res_addr+8);
    }
   // printf("width%d\n",icon_width);
    icon_line = spi_read16(res_addr+2)>> 3;    //��
    icon_addr = spi_read32(res_addr+4);         //��Դ��ʼ
   //  printf("icon_add%ld\n",icon_addr);
//    icon_length = spi_read32(res_addr+12);      //��Դ����
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
    //u16 icon_line;                   //���Ӧ�ö���256����
    u32 icon_addr;
    //    u16 icon_length;                            //��С���Ϊ128x64 = 8192
    u8 i;
    char *disp_ptr;
    //u8 lines_flag;
    u8 read_width; //Ӧ�ö�ȡ�Ŀ��
    menuindex = menuindex*sys_ctl.lang_num + sys_ctl.lang_id;
    res_addr = calc_word_addr(menuindex * ICON_FILE_HEAD);
    // printf("icon_add%ld\n",res_addr);
    icon_width = spi_read8(res_addr);          //��
    icon_width1 = spi_read8(res_addr+13);       //�Ƿ�������
    if(icon_width1)
    {
        icon_width1 = spi_read8(res_addr+8);   //�ڶ��п��
    }
    read_width = icon_width;
    //icon_line = spi_read16(res_addr+2)>> 3;    //��
   // printf("line%d\n",icon_line);
    //printf("w:%d,w1:%d\n",icon_width,icon_width1);
    icon_addr = spi_read32(res_addr+4);         //��Դ��ʼ
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
    //u16 icon_line;                   //���Ӧ�ö���256����
    u32 icon_addr;
    //    u16 icon_length;                            //��С���Ϊ128x64 = 8192
    u8 i;
    char *disp_ptr;

    //u8 lines_flag = 1;
    u8 read_width; //Ӧ�ö�ȡ�Ŀ��
    menuindex = menuindex*sys_ctl.lang_num + sys_ctl.lang_id;
    res_addr = calc_word_addr(menuindex * ICON_FILE_HEAD);
    // printf("icon_add%ld\n",res_addr);
    icon_width = spi_read8(res_addr);          //��
    icon_width1 = spi_read8(res_addr+13);       //�Ƿ�������
    if(icon_width1)
    {
        icon_width1 = spi_read8(res_addr+8);   //�ڶ��п��
    }
    move_ctl.name_wide1 = icon_width + icon_width1;
    read_width = icon_width;
    //icon_line = spi_read16(res_addr+2)>> 3;    //��
   // printf("line%d\n",icon_line);
    //printf("w:%d,w1:%d\n",icon_width,icon_width1);
    icon_addr = spi_read32(res_addr+4);         //��Դ��ʼ
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
    //u16 icon_line;                   //���Ӧ�ö���256����
    u32 icon_addr;
    //    u16 icon_length;                            //��С���Ϊ128x64 = 8192
    u8 i;
    char *disp_ptr;

    u8 lines_flag = 1;
    u8 read_width; //Ӧ�ö�ȡ�Ŀ��
    menuindex = menuindex*sys_ctl.lang_num + sys_ctl.lang_id;
    res_addr = calc_word_addr(menuindex * ICON_FILE_HEAD);
    // printf("icon_add%ld\n",res_addr);
    icon_width = spi_read8(res_addr);          //��
    icon_width1 = spi_read8(res_addr+13);       //�Ƿ�������
    if(icon_width1)
    {
        icon_width1 = spi_read8(res_addr+8);   //�ڶ��п��
    }
    move_ctl.name_wide1 = icon_width + icon_width1;
    read_width = icon_width;
    //icon_line = spi_read16(res_addr+2)>> 3;    //��
   // printf("line%d\n",icon_line);
    //printf("w:%d,w1:%d\n",icon_width,icon_width1);
    icon_addr = spi_read32(res_addr+4);         //��Դ��ʼ
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
            /*����֮�������϶*/
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
