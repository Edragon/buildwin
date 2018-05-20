/*****************************************************************************
 * Module    : Display
 * File      : ledseg_msgbox.c
 * Author    : Hanny
 * Function  : ��ѡ��ؼ���������ѡ���ܡ����֧������4λ����
 *****************************************************************************/
#include "include.h"

#if IS_LEDSEG_DISPLAY
#include "task.h"

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
    if (ocx_show_cnt >= 20) {
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

        //ȡ��
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
        //if(task_ctl.work_sta != TASK_BT){
            if (t_num.cnt >= 4) {
                t_num.value = 0;
            }
            if (t_num.value == 0) {
                t_num.cnt = 0;
            }
            t_num.value = t_num.value * 10 + (msg - T_KEY_NUM_0);
            t_num.cnt++;
            ocx_show_cnt = 0;
        /*}
        else{
            if (t_num.cnt >= 32) {
                t_num.value = 0;
            }
            t_num.value = (t_num.value % 1000 * 10 + (msg - T_KEY_NUM_0)) % 10000;
            *((u8 *)&bt_name[t_num.cnt]) = msg - T_KEY_NUM_0 + '0';
            t_num.cnt++;
            ocx_show_cnt = 0;
            if(t_num.cnt >= 32){
                t_num.ok_flag = 1;
            }
        }*/
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
    disp_sel_num = t_num.disp_value;
    ledseg_disp(MENU_NUM_SEL);
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
