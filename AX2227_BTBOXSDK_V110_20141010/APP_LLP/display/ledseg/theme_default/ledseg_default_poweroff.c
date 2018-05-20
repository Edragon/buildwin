/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_poweroff.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 扩音器任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_POWEROFF == LEDSEG_THEME_DEFAULT)
#include "task.h"

typedef struct {
    u8 off_flag;                        //关机标志
    u8 dis_cnt;
    u8 icon_cnt;
}type_ctl_poweroff;

extern type_ctl_poweroff t_poff;
//任务显示处理
#pragma location="TASK_POWER"
void task_poweroff_display(void)
{
}


#endif
