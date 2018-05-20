/*****************************************************************************
 * Module    : Display
 * File      : listbox.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �б�ؼ�����ض���
 *****************************************************************************/
#ifndef LISTBOX_H
#define LISTBOX_H

typedef struct {
    u16 disp_index;                     //��ǰ��ʾ�Ĳ˵�������ж��Ƿ���Ҫˢ��
    u16 index;                          //��ǰ�˵�
    u16 total;                          //�˵�����
    u16 page_index;                     //��ǰ�˵�����ҳ�ĵ�һ��
    u8 n_page;                          //ÿҳ��ʾ����
    u8 m_icon;                          //�Ƿ���ʾ�˵���ͼ��
    u8 m_title;                         //�Ƿ���Ҫ��ʾ�б�����
    u8 m_roll;                          //�����˵����Ƿ�ҳ�˵�
    u8 m_eq;                            //�Ƿ�����ʾeq
    u8 ok_flag;                         //�Ƿ���ȷ����
    u8 show_time;                       //��ʾʱ��
    union {
        //For ListBox
        struct {
            u8 m_res;                           //����ģʽ����Դ
            string *llp_item;               //�������ַ����б�
        };
        //For ListBoxA
        struct {
            u8 itemlen;                         //�˵�����ַ�������
            char *item;                         //�б�����
            char *(*get_list)(u16 index, u8 cnt);  //�б����ݻ�ȡ����
        };
    };
} type_list_ctl;

#if SLIDER_EN
typedef struct {
   u8 line;  //�������ʼ�У����У�1������16С��
   u8 angle_line_nums; // �����С��������С��
   u8 line_remainder; // �����С�� ��������ʣ��Ŀհ�С��
   u8 line_start;  // ������д�����ĵڼ�С�У����ֵΪ16��
   u8 pre_num;
} type_slider_ctl;

extern type_slider_ctl t_slider;
extern type_slider_ctl t_slider;
extern void listbox_display_slider_init(u8 num);
extern void listbox_display_slider(u8 line,u8 num);
#endif
extern type_list_ctl t_list;
extern void menu_move_control(u8 wide);
#endif
