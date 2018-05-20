/*****************************************************************************
 * Module    : Display
 * File      : checkbox.h
 * Author    : Hanny
 * Function  : ��ѡ��ؼ�����ض���
 *****************************************************************************/
#ifndef CHECKBOX_H
#define CHECKBOX_H

typedef struct {
    u32 menu_value;                     //ÿ���˵����Ӧ�ñ�����1bit���ӵ�λ��ʼ����
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
    u8 disp_flag;                       //��ȷ������Ҫˢ����ʾ
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
} type_checkbox_ctl;

extern type_checkbox_ctl t_check;

#endif
