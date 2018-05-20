/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_menu.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 主菜单任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_MENU == LCD_THEME_DEFAULT)
#include "task.h"
#include "task_menu.h"

//任务显示处理
#pragma location="TASK_MENU_SEG"
void task_menu_display(void)
{
    if (t_menu.disp_index != t_menu.index) {
        t_menu.disp_flag = 1;
        t_menu.disp_index = t_menu.index;
        move_ctl.name_x=0;
    }
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_menu.disp_flag = 1;             //设置更新标志
    }
    if (!t_menu.disp_flag) {
        return;
    }
    t_menu.disp_flag = 0;

    u8 i, pos;
    disp_clear();

    pos = 12;
    for (i=0; i!=4 && t_menu.page_index+i<t_menu.total; i++) {
        disp_draw(menu_item[t_menu.page_index+i].r_index, pos, 0);
        if (t_menu.index == t_menu.page_index + i) {
            disp_draw(RES_ICON_FOCUSL_BMP, pos-4, 0);
            disp_menu(menu_item[t_menu.page_index+i].name, 0, move_ctl.name_x, 6, ME_CENTER);//示当前菜单项的文字
            move_ctl.name_wide = move_ctl.name_wide1;
            if(move_ctl.name_wide >= 105)
            {
                move_ctl.move_flag=1;
            }
            else
            {
                move_ctl.move_flag=0;
            }
            disp_draw(RES_ICON_FOCUSR_BMP, pos+16, 0);
            disp_draw(menu_item[t_menu.page_index+i].r_index+t_menu.icon_index, 48, 2);
        }
            pos += 24;
    }

    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    disp_update();
}

#endif
