/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_auxeq.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : EQ AUX任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_AUXEQ == LEDSEG_THEME_DEFAULT)
#include "task_aux_eq.h"

//录音任务显示处理
#pragma location="TASK_AUX_EQ_SEG"
void task_aux_eq_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_LINEIN);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}


#endif
