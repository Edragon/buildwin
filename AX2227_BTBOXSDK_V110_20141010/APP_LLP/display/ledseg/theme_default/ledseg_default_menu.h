/*****************************************************************************
 * Module    : Display
 * File      : no_display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 无显示函数定义
*****************************************************************************/
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

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

#endif
