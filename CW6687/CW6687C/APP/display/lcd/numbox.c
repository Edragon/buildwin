/*****************************************************************************
 * Module    : Display
 * File      : numbox.c
 * Author    : Hanny
 * Function  : ��ѡ��ؼ���������ѡ���ܡ����֧������4λ����
 *****************************************************************************/
#include "include.h"
#if IS_LCD_DISPLAY
#include "task.h"

//list box controller
IAR_XDATA_A struct {
    u8 cnt;                             //�����������λ��
    u16 value;                          //�����������ֵ
    u16 disp_value;                     //��ǰ��ʾ����ֵ
    u8 ok_flag;                         //�Ƿ���ȷ����
} t_num @ "OCX_XDATA";

#pragma location="NUM_BOX_SEG"
void numbox_event(void)
{
    if (!comm_event()) {
        t_num.ok_flag = 0xff;
    }
    if (ocx_show_cnt >= 30) {
        t_num.ok_flag = 1;
    }
}

//��ѡ������Ϣ
#pragma location="NUM_BOX_SEG"
void numbox_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //ȷ��
    case KU_OK:
        t_num.ok_flag = 1;
        break;

#if THEME_SELECT==THEME_LCD_DEFAULT
    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;

        //ȡ��
    case KU_PLAY_POWER:
#else
    case KU_BACK:
#endif
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
        break;

        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}

//��ѡ����ʾ�ӿ�
#pragma location="NUM_BOX_SEG"
void numbox_display(void)
{
   // char *msg_ptr;
    if (t_num.disp_value == t_num.value) {
        return;
    }
    t_num.disp_value = t_num.value;

    //����ʹ��С�Դ棬�����ڴ�ʹ��disp_���������ݣ���ʹ��draw_img
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

//value��ʼֵ; modeΪģʽ
//����ѡ��ֵ
#pragma location="NUM_BOX_SEG"
u16 numbox(u8 value, u8 mode)
{
    t_num.disp_value = 0xffff;
    t_num.ok_flag = 0;
    t_num.value = value;
    t_num.cnt = 1;
    ocx_show_cnt = 0;

    draw_clear();               //����
    while (!t_num.ok_flag) {
        numbox_event();
        numbox_deal_msg();
        numbox_display();
    }

    if (t_num.ok_flag == 1) {
        return t_num.value;                     //����ȷ����������ѡ���ֵ
    }
    return 0xffff;                              //������Чֵ
}
#endif
