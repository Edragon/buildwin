/*****************************************************************************
 * Module    : Display
 * File      : password.c
 * Author    : Hanny
 * Function  : �����ؼ������ڴ���UI���õ����������빦�ܡ�ֻ֧��4λ��������
 *****************************************************************************/
#include "include.h"
#if LCD_THEME
#include "task.h"

//password controller
IAR_XDATA_A struct {
    u8 index;                           //��ǰ��ѡ����
    u8 cnt;                             //�����������λ��
    u16 value;                          //�����������ֵ
    u8 disp_flag;                       //������ʾ
} t_pass @ "OCX_XDATA";

//������������
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
        t_pass.value = 0xffff;          //ֱ�ӷ���һ����Ч������
        t_pass.cnt = 4;
    }
}

//��������
void password_restore_mode(void)
{
    if (t_pass.index == 10 && t_pass.value == 888) {
        t_pass.value = 10000;           //����һ����������
        t_pass.cnt = 4;
    }
}

//������¼�����
void password_event(void)
{
    WATCHDOG_CLR();
}

//�������Ϣ����
#pragma location="PASSWORD_SEG"
void password_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //ȷ��
    case KU_PLAY:
        password_input();
        break;

        //��������
    case KL_PLAY:
        password_restore_mode();
        break;

        //ȡ��
    case KU_MENU:
        if (t_pass.cnt) {
            t_pass.value = 0;
            t_pass.cnt = 0;
            t_pass.disp_flag = 1;
        } else {
            t_pass.value = 0xffff;          //ֱ�ӷ���һ����Ч������
            t_pass.cnt = 4;
        }
        break;

        //��һ���һ��
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

        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}

//�������ʾ
#pragma location="PASSWORD_SEG"
void password_display(void)
{
    if (!t_pass.disp_flag) {
        return;                         //����Ƿ������ʾ
    }
    t_pass.disp_flag = 0;

    u8 i, pos, line;

    //��ʾ�����
    line_clear();
    disp_draw(RES_OCX_PASSBOX1_BMP, 24, 0);
    pos = 24 + 8;
    for (i=0; i!=t_pass.cnt; i++) {
        disp_draw(RES_OCX_PASSBOX_V_BMP, pos, 0);       //��ʾ����������***
        pos += 20;
    }
    //��ʾ���밴��
    for (line=0; line!=3; line++) {
        line_update_clear(line<<1);
        disp_draw(RES_OCX_PASSBOX2_BMP + line, 24, 0);
        if (line == (t_pass.index >> 2)) {
            pos = 24 + 20 * (t_pass.index&0x03);
            disp_invert(pos, 0, 20, 2);                 //��ѡ��ǰ�İ���
        }
    }
    line_update(6);
}

//�����ؼ�����ʱֻ֧��4λ����������
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
