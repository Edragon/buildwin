/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_fm.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 收音机任务显示主题
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_fm.h"

#if (THEME_SELECT_FM == LEDSEG_THEME_DEFAULT)


/*****************************************************************************
 * Module    : 预装数据，以便从RAM中载入资源
 *****************************************************************************/
//任务显示处理
#pragma location="TASK_FMREC_SEG"
void task_fm_record_display(void)
{

}

//任务显示处理
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_FREQ);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

//#pragma location="TASK_FMREC_SEG"
void task_fm_disp_clear(void)
{
    ledseg_ocx_clear();
}

#pragma location="TASK_FMREC_SEG"
void task_fm_ledseg_disp_init(void)
{
    ledseg_disp(MENU_FREQ);
}
#endif
