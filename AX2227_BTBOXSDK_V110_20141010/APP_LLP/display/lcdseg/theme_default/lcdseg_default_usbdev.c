/*****************************************************************************
 * Module    : Theme
 * File      : lcdseg_default_usbdev.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : USB DEV������ʾ����
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_USBDEV == LCDSEG_THEME_DEFAULT)
#include "task_usbdev.h"

//������ʾ����
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_display(void)
{

}


#endif
