/*****************************************************************************
 * Module    : Display
 * File      : password.c
 * Author    : Hanny
 * Function  : 密码框控件，用于处于UI中用到的密码输入功能。只支持4位数字密码
 *****************************************************************************/
#include "include.h"
#if LCD_THEME
#include "task.h"

//password controller
IAR_XDATA_A struct {
    u8 index;                           //当前的选择项
    u8 cnt;                             //已输入的密码位数
    u16 value;                          //已输入的密码值
    u8 disp_flag;                       //更新显示
} t_pass @ "OCX_XDATA";

//密码的输入过程
#pragma location="PASSWORD_SEG"
void password_input(void)
{
    if (t_pass.index <= 9) {
        t_pass.value = (t_pass.value * 10) + t_pass.index;
        if (t_pass.cnt < 4) {
            t_pass.cnt++;
        }
        t_pass.disp_flag = 1;
    } else if (t_pass.index == 10) {
        t_pass.value = 0;
        t_pass.cnt = 0;
        t_pass.disp_flag = 1;
    } else if (t_pass.index == 11) {
        t_pass.value = 0xffff;          //直接返回一个无效的密码
        t_pass.cnt = 4;
    }
}

//超级密码
void password_restore_mode(void)
{
    if (t_pass.index == 10 && t_pass.value == 888) {
        t_pass.value = 10000;           //返回一个超级密码
        t_pass.cnt = 4;
    }
}

//密码框事件处理
void password_event(void)
{
    WATCHDOG_CLR();
}

//密码框消息处理
#pragma location="PASSWORD_SEG"
void password_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //确定
    case KU_PLAY:
        password_input();
        break;

        //超级密码
    case KL_PLAY:
        password_restore_mode();
        break;

        //取消
    case KU_MENU:
        if (t_pass.cnt) {
            t_pass.value = 0;
            t_pass.cnt = 0;
            t_pass.disp_flag = 1;
        } else {
            t_pass.value = 0xffff;          //直接返回一个无效的密码
            t_pass.cnt = 4;
        }
        break;

        //上一项，下一项
    case K_PREV:
    case KL_PREV:
    case KH_PREV:
        if (t_pass.index == 0) {
            t_pass.index = 12;
        }
        t_pass.index--;
        t_pass.disp_flag = 1;
        break;
    case K_NEXT:
    case KL_NEXT:
    case KH_NEXT:
        t_pass.index++;
        if (t_pass.index >= 12) {
            t_pass.index = 0;
        }
        t_pass.disp_flag = 1;
        break;

        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}

//密码框显示
#pragma location="PASSWORD_SEG"
void password_display(void)
{
    if (!t_pass.disp_flag) {
        return;                         //检查是否更新显示
    }
    t_pass.disp_flag = 0;

    u8 i, pos, line;

    //显示密码框
    line_clear();
    disp_draw(RES_OCX_PASSBOX1_BMP, 24, 0);
    pos = 24 + 8;
    for (i=0; i!=t_pass.cnt; i++) {
        disp_draw(RES_OCX_PASSBOX_V_BMP, pos, 0);       //显示已输入密码***
        pos += 20;
    }
    //显示密码按键
    for (line=0; line!=3; line++) {
        line_update_clear(line<<1);
        disp_draw(RES_OCX_PASSBOX2_BMP + line, 24, 0);
        if (line == (t_pass.index >> 2)) {
            pos = 24 + 20 * (t_pass.index&0x03);
            disp_invert(pos, 0, 20, 2);                 //反选当前的按键
        }
    }
    line_update(6);
}

//密码框控件，暂时只支持4位纯数字密码
#pragma location="PASSWORD_SEG"
u16 password(void)
{
    memset(&t_pass, 0, sizeof(t_pass));
    t_pass.disp_flag = 1;

    while (t_pass.cnt < 4) {
        password_event();
        password_deal_msg();
        password_display();
    }
    return t_pass.value;
}
#endif
