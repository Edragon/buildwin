/*****************************************************************************
 * Module    : Display
 * File      : no_display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ����ʾ��������
*****************************************************************************/
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#if MENU_MUSIC
//��Ҫ�����˵�����
#define SET_MENU_ITEM(task_id, res_id, str_id, mp3_id) { \
    .sta = task_id, \
    .mp3res = mp3_id, \
},

#else
//����Ҫ�����˵�����
#define SET_MENU_ITEM(task_id, res_id, str_id, mp3_id) { \
    .sta = task_id, \
},
#endif

#endif
