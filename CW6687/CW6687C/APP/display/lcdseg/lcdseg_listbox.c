/*****************************************************************************
 * Module    : Display
 * File      : lcdseg_msgbox.c
 * Author    : Hanny
 * Function  : 列表控件，用于处于UI中用到的静态列表功能
 *****************************************************************************/
#include "include.h"
#if IS_LCDSEG_DISPLAY
#include "task.h"

//index: 初始索引;  total: 索引总数;  mode模式选择;  param: 内容参数; show_time: 列表框显示时间
//返回选中的值
#pragma location="LIST_BOX_SEG"
u8 listbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time)
{
    return 0;
}

#endif
