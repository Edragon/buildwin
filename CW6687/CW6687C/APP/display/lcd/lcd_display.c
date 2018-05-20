/*****************************************************************************
 * Module    : Display
 * File      : lcd_display.h
 * Author    : Hanny
 * Function  : LCD��ʾ����
*****************************************************************************/
#include "include.h"
#if IS_LCD_DISPLAY

//��ȡ�ַ����
#pragma location="DISP_SEG_1"
u8 char_width(u8 ch)
{
    if (ch & 0x80)     //��Ϊ˫�ֽ��ַ�
    {
#if(FONT_SEL == SMALL_FONT)
        return(13);
#else
        return(15);
#endif
    }
#if(FONT_SEL == SMALL_FONT)
        return(7);
#else
        return(8);
#endif
    //return(ch_width[ch]);//���ڸ�Ϊ��������ʾ���ַ���ȶ��ǹ̶��ģ������δ���
}

//��ȡunicode�ַ����
#pragma location="DISP_SEG_1"
u8 char_unicode_width(u8 *str)
{
    u8 width = 0;
    if (*str & BIT(7) || *(str+1) != 0)
    {
#if(FONT_SEL == SMALL_FONT)
        width += 13;
#else
        width += 15;
#endif
    }
    else
    {
#if(FONT_SEL == SMALL_FONT)
        width += 7;
#else
        width += 8;
#endif
        //width += char_width(*str);
    }
    return width;
}


#pragma location="DISP_SEG_1"
u16 name_width(char *str, u8 name_encode_flag_pos)
{
    u16 width=0;
    if (str[name_encode_flag_pos] & FB_GBK) {   //FILE_NAME_FLAG
        while (1)
        {
            width += char_width(*str);
            // printf("width%d\n",width);
            if (*str == 0)
            {
                break;
            }
            if (*str & 0x80)
            {
                str++;
            }
            str++;
        }
    } else {
        while (1)
        {
            if (*str == 0 && *(str+1) == 0)
            {
                break;
            }
            else if (*str & BIT(7) || *(str+1) != 0)
            {
#if(FONT_SEL == SMALL_FONT)
                width += 13;
#else
                width += 15;
#endif
            }
            else
            {
#if(FONT_SEL == SMALL_FONT)
                width += 7;
#else
                width += 8;
#endif
                //width += char_width(*str);
            }
            str += 2;

        }
    }
    //    printf("width%d\n", width);
    return(width);
}

#endif
