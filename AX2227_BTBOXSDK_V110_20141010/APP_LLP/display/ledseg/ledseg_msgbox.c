/*****************************************************************************
 * Module    : Display
 * File      : ledseg_msgbox.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 对话框控件，用于处于UI中用到的对话框功能
 *****************************************************************************/
#include "include.h"
#if LEDSEG_THEME
#include "task.h"
//mode模式选择;  param: 内容参数
//返回确定或取消
#pragma location="MSG_BOX_SEG"
bool msgbox(u16 param, u8 mode, u8 show_time)
{
    return true;
}
#endif
