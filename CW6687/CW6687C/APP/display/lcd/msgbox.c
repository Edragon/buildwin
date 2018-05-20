/*****************************************************************************
 * Module    : Display
 * File      : msgbox.c
 * Author    : Hanny
 * Function  : �Ի���ؼ������ڴ���UI���õ��ĶԻ�����
 *****************************************************************************/
#include "include.h"
#if IS_LCD_DISPLAY
#include "task.h"
//list box controller
IAR_XDATA_A struct {
    u8 disp_index;                      //��ǰ��ʾ��ѡ��������ж��Ƿ���Ҫˢ��
    u8 index;                           //��ǰ��ѡ����
    u8 m_icon;                          //�Ƿ���ʾ�Ի����ͼ��
    u8 m_res;                           //����ģʽ����Դ
    u8 m_title;                         //�Ƿ���Ҫ��ʾ�б�����
    u8 m_yesno;                         //�Ƿ���Ҫ��ʾ��������
    u8 m_ok;                            //�Ƿ���ҪOK��
    u8 ok_flag;                         //�Ƿ���ȷ����
    u8 show_time;                       //��ʾʱ��
    string msg;                         //��Ϣ����
} t_msg @ "OCX_XDATA";

#pragma location="MSG_BOX_SEG"
void msgbox_event(void)
{
    if (!comm_event()) {
        t_msg.ok_flag = 0xff;
    }
    if (t_msg.show_time && ocx_show_cnt >= t_msg.show_time) {
        t_msg.ok_flag = 0xff;
    }
}

//�б�ؼ�������Ϣ
#pragma location="MSG_BOX_SEG"
void msgbox_deal_msg(void)
{
    u8 msg = get_msg();
    if(msg >= 0x6d && msg < 0x7e) { //��Ҫ��ϵͳ��Ϣ���������ȴ��˳�����
        put_msg(msg);
        return;
    }
    switch(msg) {
        //ȷ��
    case KU_OK:
        t_msg.ok_flag = 1;
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
        t_msg.ok_flag = 0xff;
        break;

        //��һ���һ��
    case KU_PREV:
        if (t_msg.m_yesno) {
            ocx_show_cnt = 0;
            t_msg.index = 1;
        }
        break;
    case KU_NEXT:
        if (t_msg.m_yesno) {
            ocx_show_cnt = 0;
            t_msg.index = 0;
        }
        break;

        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}

//�б���ʾ�ӿ�
#pragma location="MSG_BOX_SEG"
void msgbox_display(void)
{
   // char *msg_ptr;
    if (t_msg.index == t_msg.disp_index) {
        return;
    }
#if MSGBOX_SIMPLE
    if (!t_msg.m_yesno && !t_msg.m_ok) {
        //������Ϣ��
        line_update_clear(0);
        disp_menu(t_msg.msg, 0, 0, 0, ME_LINES|ME_MSG);//
        line_update_clear(4);
        line_update(6);
        return;
    }
#endif
    t_msg.disp_index = t_msg.index;
    line_clear();
    if (t_msg.m_title) {
        //������ģʽ
    } else {
        //�ޱ�����ģʽ
        disp_menu(STR_MSG_TITLE, 0, 0, 0, ME_CENTER);
    }
    line_update_clear(0);
   /* disp_menu(t_msg.msg, 0, 0, 0);
     line_update_clear(2);
    if(t_msg.msg==STR_FACTORY_CONFIRM)
    {
      disp_menu(t_msg.msg+1, 2, 0, 0);
    }
    line_update_clear(4);*/
    disp_menu(t_msg.msg, 0, 0, 0, ME_LINES|ME_MSG);//
    line_update_clear(4);
    if (t_msg.m_yesno) {
        disp_menu(STR_MSG_OK, 25, 0, 0, 0);
        disp_menu(STR_MSG_CANCEL, 79, 0, 0, 0);
#if(FONT_SEL == SMALL_FONT)
        disp_invert((t_msg.index == 1) ? 22 : 76,        //��ɫѡȡ
            0,
            (sys_ctl.lang_id == 0 || t_msg.index == 1) ? 30 : 42,
            2
        );
#else
        disp_invert((t_msg.index == 1) ? 22 : 76,        //��ɫѡȡ
            0,
            (sys_ctl.lang_id == 0 || t_msg.index == 1) ? 37 : 49,
            2
        );
#endif
    } else {
        disp_menu(STR_MSG_OK, 52, 0, 0, 0);
#if(FONT_SEL == SMALL_FONT)
        disp_invert(49, 0, 30, 2);
#else
        disp_invert(49, 0, 35, 2);
#endif
    }
    line_update(6);
}
//modeģʽѡ��;  param: ���ݲ���
//����ȷ����ȡ��
#pragma location="MSG_BOX_SEG"
bool msgbox(u16 param, u8 mode, u8 show_time)
{
    t_msg.disp_index = 0xFF;
    t_msg.index = 0;                                //Ĭ��ѡ���
    t_msg.m_title = (mode & MB_TITLE) ? 1 : 0;
    t_msg.m_res = (mode & MB_RES) ? 1 : 0;
    t_msg.m_yesno = (mode & MB_YESNO) ? 1 : 0;
    t_msg.m_ok = (mode & MB_OK) ? 1 : 0;
    t_msg.ok_flag = 0;
    t_msg.show_time = show_time;

#pragma diag_remark=Pe1053
    //��ʱ����Pe1053������棬ָ�����ʹ�תС
    t_msg.msg = (string)param;
#pragma diag_warning=Pe1053
    if (!t_msg.m_yesno) {
        t_msg.index = 1;                //ֻ��ȷ�Ͽ�ѡ
    }

    ocx_show_cnt = 0;
    while (!t_msg.ok_flag) {
        msgbox_event();
        msgbox_deal_msg();
        msgbox_display();
#if ALARM_EN
        if (task_ctl.work_sta == TASK_USBDEV || task_ctl.work_sta == TASK_ALARM) {
#else
        if (task_ctl.work_sta == TASK_USBDEV) {
#endif
            t_msg.ok_flag = 0xfe;
        }
    }

    if (t_msg.ok_flag == 1 && t_msg.index == 1) {
        return true;                    //����ȷ����������ѡ����ȷ����ť
    }
    return false;
}
#endif
