/*****************************************************************************
 * Module    : Display
 * File      : checkbox.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ��ѡ��ؼ�
 *****************************************************************************/
#include "include.h"
#if LCD_THEME
#include "task.h"
#include "checkbox.h"

//list box controller
IAR_XDATA_A type_checkbox_ctl t_check @ "OCX_XDATA";

//�б��¼�
#pragma location="CHECK_BOX_SEG"
void checkbox_event(void)
{
    if (!comm_event()) {
        t_check.ok_flag = 0xfe;
    }
    ocx_event();
    if (t_check.show_time && ocx_show_cnt >= t_check.show_time) {
        t_check.ok_flag = 0xff;
    }
}

//�б���һ��
#pragma location="CHECK_BOX_SEG"
void checkbox_prev(void)
{
    if (t_check.index <= t_check.page_index) {
        if (t_check.m_roll) {
            t_check.page_index--;
        } else {
            t_check.page_index -= t_check.n_page;
        }
    }
    if (t_check.index == 0) {
        t_check.index = t_check.total;
        if (t_check.m_roll) {
            t_check.page_index = (t_check.total > t_check.n_page) ? t_check.total - t_check.n_page : 0;
        } else {
            t_check.page_index = ((t_check.total - 1) / t_check.n_page) * t_check.n_page;
        }
    }
    t_check.index--;
    ocx_show_cnt = 0;
    if(t_check.m_eq)
    {
        music_eq(t_check.index);
    }
}

//�б���һ��
#pragma location="CHECK_BOX_SEG"
void checkbox_next(void)
{
    t_check.index++;
    if (t_check.index - t_check.page_index >= t_check.n_page) {
        if (t_check.m_roll) {
            t_check.page_index++;
        } else {
            t_check.page_index += t_check.n_page;
        }
    }
    if (t_check.index == t_check.total) {
        t_check.index = 0;
        t_check.page_index = 0;
    }
    ocx_show_cnt = 0;
    if(t_check.m_eq)
    {
        music_eq(t_check.index);
    }
}

//�б�ؼ�������Ϣ
#pragma location="CHECK_BOX_SEG"
void checkbox_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //ȷ��
    case KU_OK:
        if(t_check.menu_value & (1<<t_check.index))
            t_check.menu_value &= ~(1<<t_check.index);
        else
            t_check.menu_value |= (1<<t_check.index);
        t_check.disp_flag = 1;
        //t_check.ok_flag = 1;
        break;

        //ȡ��
    case KU_BACK:
        t_check.ok_flag = 1;
        break;

        //�����������˵�
    case KL_MENU:
        task_ctl.work_sta = TASK_EXIT;
        break;

        //��һ���һ��
    case KU_PREV:
        checkbox_prev();
        break;
    case KU_NEXT:
        checkbox_next();
        break;

        //���ù�����Ϣ����
    default:
        ocx_msg(msg);
        break;
    }
}

#pragma location="CHECK_BOX_SEG"
void checkbox_display_line(string menu_str, u8 num)
{
    if (t_check.m_icon) {
        if(t_check.menu_value & (1<<num))
            disp_draw(RES_SELECTED_BMP, 0, 0);
        else
            disp_draw(RES_BOX_BMP, 0, 0);
        disp_menu(menu_str, 18, 0, 0, 0);
    } else {
        disp_menu(menu_str, 5, 0, 0, 0);
    }
}

//�б���ʾ�ӿ�
#pragma location="CHECK_BOX_SEG"
void checkbox_display(void)
{
    u8 pos, line;
    u8 num, menu_num;
    string *item = t_check.llp_item;
    if (t_check.index == t_check.disp_index && !t_check.disp_flag) {
        return;
    }
    t_check.disp_index = t_check.index;

    num = t_check.total - t_check.page_index;         //��ʾ������
    if (t_check.total - t_check.page_index > t_check.n_page) {
        num = t_check.n_page;
    }
    pos = t_check.index - t_check.page_index;         //��ѡ��λ��
    item += t_check.page_index;
    line = 0;
    menu_num = t_check.page_index;

    line_clear();
    if (t_check.m_title) {
        //������ģʽ
       // disp_llp_text(*t_check.llp_item, TX_CENTER, 0, 0);
        disp_menu(*t_check.llp_item,0,0, 0,ME_CENTER);
        line_update_clear(line);
        item++;
        line += 2;
    }
    //�ޱ�����ģʽ
    while (num--) {
        checkbox_display_line(*item, menu_num);
        if (pos-- == 0) {
            disp_invert(0, 0, 128, 2);           //��ѡ�˵�
        }
        line_update_clear(line);
        item++;
        line += 2;
        menu_num++;
    }
    while (line < 8) {
        line_update(line);
        line += 2;
    }
}

//index: ��ʼ����;  total: ��������;  modeģʽѡ��;  param: ���ݲ���; show_time: �б����ʾʱ��
//����ѡ�е�ֵ
#pragma location="CHECK_BOX_SEG"
u32 checkbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time, u32 lparam)
{
    t_check.disp_index = 0xFFFF;
    t_check.index = index;
    t_check.total = total;
    t_check.m_title = (mode & LB_NO_TITLE) ? 0 : 1;
    t_check.m_res = (mode & LB_RES) ? 1 : 0;
    t_check.m_icon = (mode & LB_NO_ICON) ? 0 : 1;
    t_check.m_roll = (mode & LB_PAGE) ? 0 : 1;
    t_check.m_eq = (mode & LB_EQ) ? 1 : 0;
    t_check.n_page = 4;
    t_check.ok_flag = 0;
    t_check.show_time = show_time;
    t_check.menu_value = lparam;

    if (t_check.m_title) {
        t_check.n_page--;        //������ģʽ�£�ÿҳ��ʾ����һ��
        t_check.total--;         //��һ��Ϊ���⣬������Ч��ѡ��
    }
    if (t_check.m_roll) {
        if (t_check.index == 0) {
            t_check.page_index = 0;
        } else if (t_check.index - 1 + t_check.n_page > t_check.total) {
            t_check.page_index = (t_check.total > t_check.n_page) ? t_check.total - t_check.n_page : 0;
        } else {
            t_check.page_index = t_check.index - 1;
        }
    } else {
        t_check.page_index = (t_check.index / t_check.n_page) * t_check.n_page;
    }
    t_check.llp_item = (string *)param;

    ocx_show_cnt = 0;
    while (!t_check.ok_flag) {
        checkbox_event();
        checkbox_deal_msg();
        checkbox_display();
#if ALARM_EN
        if (task_ctl.work_sta == TASK_EXIT || task_ctl.work_sta == TASK_ALARM) {
#else
        if (task_ctl.work_sta == TASK_EXIT) {
#endif
            t_check.ok_flag = 0xfe;
        }
    }

    if (t_check.ok_flag == 1) {
        return t_check.menu_value;
    }
    return 0xffffffff;
}
#endif
