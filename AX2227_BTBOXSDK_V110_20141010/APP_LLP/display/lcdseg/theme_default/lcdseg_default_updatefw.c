/*****************************************************************************
 * Module    : Theme
 * File      : lcdseg_default_updatefw.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ����������ʾ����
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_UPDATEFW == LCDSEG_THEME_DEFAULT)
#include "task_update_fw.h"

/*****************************************************************************
 * Module    : ��Ļ��ʾ����
 *****************************************************************************/
#pragma location="TASK_UPDATE_SEG"
void task_updatefw_dispmem_init(void)
{

}

//������ʾ
#pragma location="TASK_UPDATE_SEG"
void task_update_fw_progress_display(u8 percent)
{

}

#endif
