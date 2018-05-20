/*****************************************************************************
 * Module    : No
 * File      : no_theme.c
 * Author    : Hanny
 * Function  : 无主题
 *****************************************************************************/
#include "include.h"
#include "task.h"

#if (THEME_SELECT == THEME_NO)

//等待界面
#pragma location="DRAW_IMG_SEG1"
void draw_wait(void)
{
}

//清屏
void draw_clear(void)
{
}

//返回确定或取消
#pragma location="MSG_BOX_SEG"
bool msgbox(u16 param, u8 mode, u8 show_time)
{
    return true;
}

//返回选中的值
#pragma location="LIST_BOX_SEG"
u8 listbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time)
{
    return 0;
}

#if (HARD_IR_EN || SOFT_IR_EN)

IAR_XDATA_A struct {
    u8 cnt;                             //已输入的数字位数
    u16 value;                          //已输入的数字值
    u16 disp_value;                     //当前显示的数值
    u8 ok_flag;                         //是否按下确定键
} t_num @ "OCX_XDATA";

#pragma location="NUM_BOX_SEG"
void numbox_event(void)
{
    if (!comm_event()) {
        t_num.ok_flag = 0xff;
    }
    if (ocx_show_cnt >= 20) {
        t_num.ok_flag = 1;
    }
}

//数选框处理消息
#pragma location="NUM_BOX_SEG"
void numbox_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //确定
    case KU_OK:
        t_num.ok_flag = 1;
        break;

        //取消
    case KU_BACK:
        t_num.ok_flag = 0xff;
        break;

    case T_KEY_NUM_0:
    case T_KEY_NUM_1:
    case T_KEY_NUM_2:
    case T_KEY_NUM_3:
    case T_KEY_NUM_4:
    case T_KEY_NUM_5:
    case T_KEY_NUM_6:
    case T_KEY_NUM_7:
    case T_KEY_NUM_8:
    case T_KEY_NUM_9:
        if (t_num.cnt >= 4) {
            t_num.value = 0;
        }
        if (t_num.value == 0) {
            t_num.cnt = 0;
        }
        t_num.value = t_num.value * 10 + (msg - T_KEY_NUM_0);
        t_num.cnt++;
        ocx_show_cnt = 0;
        break;

        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}
#endif

//value初始值; mode为模式
//返回选中值
#pragma location="NUM_BOX_SEG"
u16 numbox(u8 value, u8 mode)
{
#if (HARD_IR_EN || SOFT_IR_EN)
    t_num.disp_value = 0xffff;
    t_num.ok_flag = 0;
    t_num.value = value;
    t_num.cnt = 1;
    ocx_show_cnt = 0;

    while (!t_num.ok_flag) {
        numbox_event();
        numbox_deal_msg();
    }

    if (t_num.ok_flag == 1) {
        return t_num.value;                     //按下确定键，返回选择的值
    }
    return 0xffff;                              //返回无效值
#else
    return 0;
#endif
}

#pragma location="PROGRESS_SEG"
u8 progressbox(u8 value, u8 min, u8 max, u32 param, u8 mode, u8 show_time)
{
    return 0;
}

#endif
