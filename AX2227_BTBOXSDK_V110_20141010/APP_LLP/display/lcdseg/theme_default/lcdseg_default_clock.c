/*****************************************************************************
 * Module    : Theme
 * File      : lcdseg_default_clock.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ʱ��������ʾ����
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_CLOCK == LCDSEG_THEME_DEFAULT)
#include "task.h"
#include "task_clock.h"

//ʱ�ӽ�����ʾ
#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
}
#endif
