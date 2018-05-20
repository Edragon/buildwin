/*****************************************************************************
 * Module    : Display
 * File      : display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD��ʾ��������
*****************************************************************************/
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "res.h"
#include "Menu.h"
#if MENU_MUSIC
//��Ҫ�����˵�����
#define SET_MENU_ITEM(task_id, res_id, str_id, mp3_id) { \
    .sta = task_id, \
    .r_index = res_id, \
    .name = str_id, \
    .mp3res = mp3_id, \
},

#else
//����Ҫ�����˵�����
#define SET_MENU_ITEM(task_id, res_id, str_id, mp3_id) { \
    .sta = task_id, \
    .r_index = res_id, \
    .name = str_id, \
},
#endif

u8 char_width(u8 ch);
u8 char_unicode_width(u8 *str);
u16 name_width(char *str, u8 fb_gbk);
void lcd_volume_set(void);
#endif
