/*****************************************************************************
 * Module    : Display
 * File      : numbox.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 数选框控件，用于数选功能。最大支持输入4位数字
 *****************************************************************************/
#include "include.h"
#if LCD_THEME
#include "task.h"

//list box controller
IAR_XDATA_A struct {
    u8 cnt;                             //已输入的数字位数
    u16 value;                          //已输入的数字值
    u16 disp_value;                     //当前显示的数值
    u8 ok_flag;                         //是否按下确定键
} t_num @ "OCX_XDATA";

#pragma location="NUM_BOX_SEG"
void numbox_event(void)
{
    ocx_event();
    if (!comm_event()) {
        t_num.ok_flag = 0xff;
    }
    if (ocx_show_cnt >= 30) {
        t_num.ok_flag = 1;
    }
}

//数选框处理消息
#pragma location="NUM_BOX_SEG"
void numbox_deal_msg(void)
{
    u8 msg = get_msg();
    if(msg != NO_KEY)
    {

        switch(msg) {
            //确定
        case T_KEY_OK:
            t_num.ok_flag = 1;
            break;

            //取消
        case T_KEY_BACK:
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
        case QSYSTEM_1S:
            deal_msg(msg);
            break;
        default:
            put_msg(msg);
            t_num.ok_flag = 0xff;
            break;
        }
    }
}

//数选框显示接口
#pragma location="NUM_BOX_SEG"
void numbox_display(void)
{
   // char *msg_ptr;
    if (t_num.disp_value == t_num.value) {
        return;
    }
    t_num.disp_value = t_num.value;

    //由于使用小显存，不足内存使用disp_来更新内容，改使用draw_img
    u8 v;
    v = t_num.disp_value / 1000;
    draw_img(RES_BIGNUM_0_BMP+v, 24, 3);
    v = (t_num.disp_value / 100) % 10;
    draw_img(RES_BIGNUM_0_BMP+v, 24 + 20, 3);
    v = (t_num.disp_value / 10) % 10;
    draw_img(RES_BIGNUM_0_BMP+v, 24 + 20 * 2, 3);
    v = t_num.disp_value % 10;
    draw_img(RES_BIGNUM_0_BMP+v, 24 + 20 * 3, 3);
}
//value初始值; mode为模式
//返回选中值
#pragma location="NUM_BOX_SEG"
u16 numbox(u8 value, u8 mode)
{
    t_num.disp_value = 0xffff;
    t_num.ok_flag = 0;
    t_num.value = value;
    t_num.cnt = 1;
    ocx_show_cnt = 0;

    draw_clear();               //清屏
    while (!t_num.ok_flag) {
        numbox_event();
        numbox_deal_msg();
        numbox_display();
    }

    if (t_num.ok_flag == 1) {
        return t_num.value;                     //按下确定键，返回选择的值
    }
    return 0xffff;                              //返回无效值
}
#endif
