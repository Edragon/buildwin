/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_fm.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 收音机任务显示主题
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_fm.h"

#if (THEME_SELECT_FM == LCDSEG_THEME_DEFAULT)

IAR_XDATA_A char fm_dispmem_buf[2048] @ 0x39000;

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
}

#pragma location="TASK_FMREC_SEG"
void task_fm_disp_clear(void)
{
}
#endif
