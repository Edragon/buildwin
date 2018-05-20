/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_fm.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 收音机任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_FM == LCD_THEME_DEFAULT)
#include "task.h"
#include "task_fm.h"
#include "display_mem.h"
#include "task_record.h"

IAR_XDATA_A char fm_dispmem_buf[2048] @ 0x39000;

/*****************************************************************************
 * Module    : 录音状态下的屏幕显示
 *****************************************************************************/
//显示一个Memory里面的图片
#pragma location="TASK_FM_RECORD_DISP"
u8 task_fm_record_dispmem_draw(void *ptr, u8 x, u8 line)
{
    return dispmem_draw(ptr, x, line);
}

//显示一个数字的演示程序
#pragma location="TASK_FM_RECORD_DISP"
void task_fm_dispmem_medium_num(u8 num, u8 x, u8 line)
{
    u8 num0, num1;
    if (num > 99) {
        num = 99;
    }
    num0 = num / 10;
    num1 = num % 10;
    task_fm_record_dispmem_draw(t_fm.num_buf + num0 * t_fm.num_len, x, line);
    task_fm_record_dispmem_draw(t_fm.num_buf + num1 * t_fm.num_len, x + 7, line);
}


//显示中格式时间。总宽度为
#pragma location="TASK_FM_RECORD_DISP"
void task_fm_disp_record_time(u8 x, u8 line)
{
    u8 hour, min, sec;
    hour = t_rec.rec_time / 3600;
    min = (t_rec.rec_time / 60) % 60;
    sec = t_rec.rec_time % 60;
    task_fm_dispmem_medium_num(hour, x, line);
    task_fm_record_dispmem_draw(t_fm.colon, x + 18, line);
    task_fm_dispmem_medium_num(min, x + 23, line);
    task_fm_record_dispmem_draw(t_fm.colon, x + 41, line);
    task_fm_dispmem_medium_num(sec, x + 46, line);
}

//更新
#pragma location="TASK_FM_RECORD_DISP"
void task_fmrec_update(u8 lcd_width, u8 lcd_height, char *line_buf, u8 len)
{
    dispmem_update(lcd_width, lcd_height, line_buf, len);
}

//任务显示处理
#pragma location="TASK_FMREC_SEG"
void task_fm_record_display(void)
{
    if (!t_rec.update_flag) {
        if(ocx_show_cnt < 5)
        {
            return;
        }
    }

    if(t_rec.update_flag)
    {
        memset(lcd_buf, 0, 256);
        //显示播放状态
        task_fm_disp_record_time(1, 0);
        task_fmrec_update(44, 6, lcd_buf, 84);
        task_fmrec_update(44, 7, lcd_buf+128, 84);
    }
    t_rec.update_flag = 0;
    memset(lcd_buf, 0, 256);
    if(ocx_show_cnt >= 5)
    {
        move_ctl.move_flag = !move_ctl.move_flag;
        ocx_show_cnt = 0;
    }
    if(move_ctl.move_flag)
    {
        task_fm_record_dispmem_draw(t_fm.rec_icon, 0, 0);
    }
    task_fmrec_update(0, 0, lcd_buf, 16);
    task_fmrec_update(0, 1, lcd_buf+128, 16);

}

/*****************************************************************************
 * Module    : 非录音状态下的屏幕显示
 *****************************************************************************/
//更新
#pragma location="TASK_FM_DISP"
void task_fm_update(u8 lcd_width, u8 lcd_height, char *line_buf, u8 len)
{
    dispmem_update(lcd_width, lcd_height, line_buf, len);
}

//显示一个Memory里面的图片
#pragma location="TASK_FM_SEG"
u8 task_fm_dispmem_draw(void *ptr, u8 x, u8 line)
{
    return dispmem_draw(ptr, x, line);
}

//显示一个数字的演示程序
#pragma location="TASK_FM_DISP"
void task_fm_dispmem_big_num(u8 num, u8 x, u8 line)
{
    task_fm_dispmem_draw(t_fm.bignum_buf + num * t_fm.bignum_len, x, line);
}

//显示频道
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_dispmem_ch_num(u8 num, u8 x, u8 line)
{

    u8 num0, num1;
    if (num > 99) {
        num = 99;
    }
    num0 = num / 10;
    num1 = num % 10;
    task_fm_dispmem_draw(t_fm.num_buf + num0 * t_fm.num_len, 0, 0);
    task_fm_dispmem_draw(t_fm.num_buf + num1 * t_fm.num_len, 0 + 7, 0);

    task_fm_update(x, line, lcd_buf, 14);
    task_fm_update(x, line+1, lcd_buf+128, 14);

}

//显示频率
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_disp_freq(u16 d_freq, u8 x, u8 line)
{

    u8 num0, num1, num2, num3;
    if (d_freq > 9999) {
        d_freq = 9999;
    }
    num0 = d_freq / 1000;
    num1 = (d_freq / 100) % 10;
    num2 = (d_freq / 10) % 10;
    num3 = d_freq % 10;
    if (num0) {
       task_fm_dispmem_draw(t_fm.num_buf + num0 * t_fm.num_len, x, line);
        //task_fm_dispmem_big_num(num0, x, line);
    }
    if (num0 || num1) {
       // task_fm_dispmem_big_num(num1, x + 15, line);
        task_fm_dispmem_draw(t_fm.num_buf + num1 * t_fm.num_len, x+7, line);
    }
    //task_fm_dispmem_big_num(num2, x + 30, line);
    //task_fm_dispmem_big_num(num3, x + 51, line);
     task_fm_dispmem_draw(t_fm.num_buf + num2 * t_fm.num_len, x+14, line);

    lcd_buf[x+128+23] = 0x18;
    lcd_buf[x+128+24] = 0x18;

    task_fm_dispmem_draw(t_fm.num_buf + num3 * t_fm.num_len, x+26, line);

}


//频道显示处理
#pragma location="TASK_FMPLAY_DISP_SEG"
__root __near_func void task_fm_display_ch(void)
{
    if (t_fm.disp_ch_sel != t_fm.ch_sel) {
        t_fm.disp_ch_sel = t_fm.ch_sel;
        t_fm.disp_ch_flag = 1;
    }
    if (t_fm.disp_ch_cnt != t_fm.ch_cnt) {
        t_fm.disp_ch_cnt = t_fm.ch_cnt;
        t_fm.disp_ch_flag = 1;
    }
    if (!t_fm.disp_ch_flag) {
        return;
    }
    t_fm.disp_ch_flag = 0;

    memset(lcd_buf, 0, 256);
    //显示频率

    task_fm_dispmem_ch_num(t_fm.disp_ch_sel, 50, 0);
    task_fm_dispmem_ch_num(t_fm.disp_ch_cnt, 71, 0);
}

//频率显示处理
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display_freq(void)
{
    u16 d_freq = t_fm.freq / 10;
    if (t_fm.disp_freq != d_freq) {
        t_fm.disp_freq = d_freq;
        t_fm.disp_flag = 1;
    }
    if (!t_fm.disp_flag) {
        return;
    }
    memset(lcd_buf, 0, 256);
    t_fm.disp_flag = 0;

    //显示频率
    task_fm_disp_freq(d_freq, 0, 0);
    task_fm_update(50, 2, lcd_buf, 68);
    task_fm_update(50, 3, lcd_buf+128, 68);
    memset(lcd_buf, 0, 256);
    task_fm_update(0, 4, lcd_buf, 128);
    task_fm_dispmem_draw(t_fm.point_buf, 0, 0);
    d_freq = t_fm.disp_freq-861;
    task_fm_update(d_freq/2, 4, lcd_buf, 50);
}

//电池显示处理
#pragma location="TASK_FM_SEG"
void task_fm_display_val(void)
{
     if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        task_fm_dispmem_draw(t_fm.bat_buf + (sys_ctl.bat_val - 1) * t_fm.bat_len, 0, 0);
        task_fm_update(108, 0, lcd_buf, 19);
        task_fm_update(108, 1, lcd_buf+128, 19);
    }
}

#if FM_FREQ_SHOW
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_disp_freqline(void)
{
    if (!freq_show_update) {
        return;
    }
    freq_show_update = 0;

    if (music_freq_get()) {
        memset(lcd_buf, 0, 256);
        disp_freq_line(0);
        music_freq_kick();
        task_fm_update(0, 6, lcd_buf, 128);
        task_fm_update(0, 7, lcd_buf+128, 128);
    }
}
#endif

//任务显示处理
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display(void)
{
    task_fm_display_ch();
    task_fm_display_freq();
    task_fm_display_val();
#if FM_FREQ_SHOW
    task_fm_disp_freqline();
#endif
}

/*****************************************************************************
 * Module    : 预装数据，以便从RAM中载入资源
 *****************************************************************************/
#pragma location="TASK_FM_INIT"
void task_fm_dispmem_init(void)
{
    dispmem_init(disp_mem_buf, 256);
    t_fm.colon = dispmem_add(RES_MEDIUMNUM_COLON_BMP);
    t_fm.num_buf = dispmem_addnum(RES_MEDIUMNUM_0_BMP,10);
    t_fm.num_len = dispmem_ctl.num_len;
    t_fm.rec_icon = dispmem_add(RES_ICON_REC_BMP);
}

#pragma location="TASK_FM_INIT"
void task_fm_play_dispmem_init(void)
{
    dispmem_init(fm_dispmem_buf, sizeof(fm_dispmem_buf));                   //不录音时，将此段作为大字显存
    t_fm.bat_buf = dispmem_addnum(RES_ICON_BAT0_BMP,5);
    t_fm.bat_len = dispmem_ctl.num_len;
    t_fm.point_buf = dispmem_add(RES_OCX_FMG_BMP);//RES_OCX_FMG_BMP);
}

//显示初始内容
#pragma location="TASK_FM_INIT"
void task_fm_display_init(void)
{
    /*disp_clear();
    //显示播放状态
    disp_draw(RES_ICON_REC_BMP, 0, 0);
    disp_menu(STR_FM_CHANNEL, 20, 0, 0);
     disp_text((char *)"/", 65, 0, 0);
     disp_draw(RES_OCX_FMQT_BMP, 0,  5);
    //disp_text((char *)"FM", 0, 2, 0);

    disp_update();*/
    line_clear();
    disp_draw(RES_ICON_STOP_BMP, 0, 0);
    disp_menu(STR_FM_CHANNEL, 20, 0, 0, 0);
    disp_text((char *)"/", 65, 0, 0);
    line_update_clear(0);
    line_update_clear(2);
    disp_draw(RES_OCX_FMQT_BMP, 0, 0);
    line_update_clear(5);
    line_update(6);
    t_fm.disp_flag = 1;
    t_fm.disp_ch_flag = 1;
}

#pragma location="TASK_FM_SEG_1"
void task_fm_list_dispmem_init(void)
{
    task_fm_display_init();
    task_fm_dispmem_draw(t_fm.bat_buf + (sys_ctl.bat_val - 1) * t_fm.bat_len, 0, 0);
    task_fm_update(108, 0, lcd_buf, 19);
    task_fm_update(108, 1, lcd_buf+128, 19);
}

#pragma location="TASK_FMREC_SEG"
void task_fm_disp_clear(void)
{
    line_clear();
    line_update(6);
}

#endif
