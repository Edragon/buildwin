/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_poweroff.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ������������ʾ����
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_POWEROFF == LEDSEG_THEME_DEFAULT)
#include "task.h"

typedef struct {
    u8 off_flag;                        //�ػ���־
    u8 dis_cnt;
    u8 icon_cnt;
}type_ctl_poweroff;

extern type_ctl_poweroff t_poff;
//������ʾ����
#pragma location="TASK_POWER"
void task_poweroff_display(void)
{
}


#endif
