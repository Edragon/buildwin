/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_poweroff.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 扩音器任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_POWEROFF == LCD_THEME_DEFAULT)
#include "task.h"
void poweroff_display(void)
{
    disp_clear();
    disp_draw(RES_POWEROFF_BYEBYE0_BMP,0,2);//
    disp_update();
}


#endif
