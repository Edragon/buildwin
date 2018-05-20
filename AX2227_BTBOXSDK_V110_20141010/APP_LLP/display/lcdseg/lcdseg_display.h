/*****************************************************************************
 * Module    : Display
 * File      : lcdseg_display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ����ʾ��������
*****************************************************************************/
#ifndef LCDSEG_DISPLAY_H
#define LCDSEG_DISPLAY_H

//����Ҫ����LCD_BUF
#define set_lcd_buf(x)


extern u8 disp_freq_show_buf[16];

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

void draw_wait(void);
void draw_clear(void);
#endif
