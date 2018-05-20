/*****************************************************************************
 * Module    : Display
 * File      : no_display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 无显示函数定义
*****************************************************************************/
#ifndef NO_DISPLAY_H
#define NO_DISPLAY_H

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
u8 listbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time);
void task_bt_display(void);
void task_bt_display_device(u8 show_time);
#endif
