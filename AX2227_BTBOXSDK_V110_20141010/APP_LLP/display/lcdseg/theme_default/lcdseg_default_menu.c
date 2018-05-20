/*****************************************************************************
 * Module    : Theme
 * File      : lcdseg_default_menu.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD断码屏的默认主题 - 主菜单
 *****************************************************************************/
#include "include.h"

#if (THEME_SELECT_MENU == LCDSEG_THEME_DEFAULT)

//主菜单无显示
#pragma location="TASK_MENU_SEG"
void task_menu_display(void)
{
}

#endif
