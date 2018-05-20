/*****************************************************************************
 * Module    : Display
 * File      : display.h
 * Author    : Hanny
 * Function  : LCD显示函数定义
*****************************************************************************/
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "res.h"

#if MENU_MUSIC
//需要语音菜单功能
#define SET_MENU_ITEM(task_id, res_id, str_id, mp3_id) { \
    .sta = task_id, \
    .r_index = res_id, \
    .name = str_id, \
    .mp3res = mp3_id, \
},

#else
//不需要语音菜单功能
#define SET_MENU_ITEM(task_id, res_id, str_id, mp3_id) { \
    .sta = task_id, \
    .r_index = res_id, \
    .name = str_id, \
},
#endif

u8 char_width(u8 ch);
u8 char_unicode_width(u8 *str);
u16 name_width(char *str, u8 name_encode_flag_pos);

#endif
