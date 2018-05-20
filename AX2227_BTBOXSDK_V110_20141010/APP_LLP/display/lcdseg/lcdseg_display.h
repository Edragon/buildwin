/*****************************************************************************
 * Module    : Display
 * File      : lcdseg_display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 无显示函数定义
*****************************************************************************/
#ifndef LCDSEG_DISPLAY_H
#define LCDSEG_DISPLAY_H

//不需要设置LCD_BUF
#define set_lcd_buf(x)


extern u8 disp_freq_show_buf[16];

#if MENU_MUSIC
//需要语音菜单功能
#define SET_MENU_ITEM(task_id, res_id, str_id, mp3_id) { \
    .sta = task_id, \
    .mp3res = mp3_id, \
},

#else
//不需要语音菜单功能
#define SET_MENU_ITEM(task_id, res_id, str_id, mp3_id) { \
    .sta = task_id, \
},
#endif

void draw_wait(void);
void draw_clear(void);
#endif
