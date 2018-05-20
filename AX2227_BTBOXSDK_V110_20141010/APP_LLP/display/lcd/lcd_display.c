/*****************************************************************************
 * Module    : Display
 * File      : lcd_display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD显示函数
*****************************************************************************/
#include "include.h"
#if LCD_THEME
#include "task.h"
//获取字符宽度
#pragma location="TASK_EBOOK_SEG1"
u8 char_width(u8 ch)
{
    if (ch & 0x80)     //若为双字节字符
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
    //return(ch_width[ch]);//由于改为了宋体显示，字符宽度都是固定的，故屏蔽此项
}

//获取unicode字符宽度
#pragma location="TASK_EBOOK_SEG1"
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


#pragma location="TASK_EBOOK_SEG"
u16 name_width(char *str, u8 fb_gbk)
{
    u16 width=0;
    if (fb_gbk) {
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

void lcd_volume_set(void)
{
    u8 show_time = 35;
    u8 task_temp = task_ctl.work_sta;
    u8 disp_volume = 0xff;
    ocx_show_cnt = 0;
    draw_clear();
    draw_img(RES_ICON_VOL_BMP, 10, 2);

    while (1) {
        if (!comm_event()) {
            return;
        }
        ocx_event();
        if (show_time && ocx_show_cnt >= show_time) {
            return;
        }
        u8 msg = get_msg();
        if(msg != NO_KEY)
        {
            switch(msg) {
                //确定
                // case KU_PLAY:
                //     return;
            case K_VOL_DOWN:
            case KL_VOL_DOWN:
            case KH_VOL_DOWN:
            case BT_PHONE_VOL_DOWN:

                ocx_show_cnt = 0;
                user_set_volume(user_volume_dec(sys_ctl.volume));
                break;
            case K_VOL_UP:
            case KL_VOL_UP:
            case KH_VOL_UP:
            case BT_PHONE_VOL_UP:
                ocx_show_cnt = 0;
                user_set_volume(user_volume_inc(sys_ctl.volume));
                break;
            case QSYSTEM_1S:
                deal_msg(msg);
                break;
            default:
                if(((msg & 0x1f)!= T_KEY_VOL_DOWN) && ((msg & 0x1f)!=  T_KEY_VOL_UP))
                {
                    put_msg(msg);
                    return;
                }
                else
                {
                    deal_msg(msg);
                    break;
                }
            }
        }
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_flag.mute_flag) {
            if(sys_ctl.volume) {
                music_unmute();
                dac_fade_in(1);
            }
        }
#endif
        if(disp_volume != sys_ctl.volume)
        {
            u8 num_h = sys_ctl.volume/10 + RES_BIGNUM_0_BMP;
            draw_img(num_h, 60, 3);
            num_h = sys_ctl.volume%10 + RES_BIGNUM_0_BMP;
            draw_img(num_h, 60+18, 3);
            disp_volume = sys_ctl.volume;
        }
        if (task_ctl.work_sta != task_temp)
        {
            return;
        }
    }
}

#endif
