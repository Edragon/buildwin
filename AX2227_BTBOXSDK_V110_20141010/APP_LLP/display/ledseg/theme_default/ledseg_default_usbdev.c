/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_usbdev.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : USB DEV任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_USBDEV == LEDSEG_THEME_DEFAULT)
#include "task_usbdev.h"

//任务显示处理
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_PC);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}


#endif
