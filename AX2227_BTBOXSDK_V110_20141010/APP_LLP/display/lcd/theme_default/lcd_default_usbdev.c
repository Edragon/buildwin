/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_usbdev.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : USB DEV任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_USBDEV == LCD_THEME_DEFAULT)
#include "task_usbdev.h"

//音量控件显示接口
#pragma location="TASK_USBDEV_SEG"
void volume_display(void)
{
    if(t_usbdev.vol_flag == 1)
    {
        draw_clear();
        draw_img(RES_ICON_VOL_BMP, 10, 2);
    }
    u8 num_h = sys_ctl.volume/10 + RES_BIGNUM_0_BMP;
    draw_img(num_h, 60, 3);
    num_h = sys_ctl.volume%10 + RES_BIGNUM_0_BMP;
    draw_img(num_h, 60+18, 3);
}
//任务显示处理
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_display_status(void)
{
    if(!t_usbdev.vol_flag)
    {
        if (t_usbdev.disp_opt != t_usbdev.opt_status) {     //判断需不需要更新状态
            t_usbdev.disp_opt = t_usbdev.opt_status;
            t_usbdev.disp_update = 1;
        }
        if (sys_ctl.disp_bat_val != sys_ctl.bat_val) {     //判断需不需要更新状态
            sys_ctl.disp_bat_val = sys_ctl.bat_val;
            t_usbdev.disp_update = 1;
        }
    }
    else
    {
        if (t_usbdev.disp_volume != sys_ctl.volume) {
            t_usbdev.disp_volume = sys_ctl.volume;
            t_usbdev.disp_update = 1;
        }
    }
    if (!t_usbdev.disp_update) {
        return;
    }
    t_usbdev.disp_update = 0;
    line_clear();
    //disp_draw(RES_ICON_VOL1_BMP, 2, 0);
    //disp_num(sys_ctl.volume, 20, 0, DN_RIGHT | DN_ZERO | 2);
    if(!t_usbdev.vol_flag)
    {
        if (sys_ctl.bat_val >= 1) {
            disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
            line_update(0);
        }
    }
    if(t_usbdev.vol_flag)
    {
        volume_display();
    }
    else
    {
        draw_img(RES_MASS_IDLE_BMP+t_usbdev.disp_opt, 0, 2);
    }
}

#if USBAUDIO_FREQ_SHOW
//显示频谱
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_disp_freq(void)
{
    if (!freq_show_update) {
        return;
    }
    freq_show_update = 0;

    if (music_freq_get()) {
        line_clear();
        disp_freq_line(0);
        music_freq_kick();
        line_update(6);
    }
}
#endif
//任务显示处理
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_display(void)
{
    task_usbdev_display_status();
#if USBAUDIO_FREQ_SHOW
    task_usbdev_disp_freq();
#endif
}


#endif
