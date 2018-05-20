/*****************************************************************************
* Module    : Display
* File      : ocx.c
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : �ؼ�����ض���
*****************************************************************************/
#ifndef OCX_H
#define OCX_H

typedef struct {
    void (*event_func)(void);            //�ؼ����¼�
    bool (*msg_func)(u8 msg);            //�ؼ���Ҫ��Ӧ����Ϣ
} type_ocx_ctl;

//���������������Ϊֵ�ص�ģʽ
typedef struct {
    string title;
    bool (*on_change)(u8 value);
    bool (*on_click)(u8 value);
} type_progress_param;

extern type_ocx_ctl ocx_ctl;

#define LB_NO_TITLE     0x01    //�Ƿ���Ҫ�˵�������
#define LB_RES          0x02    //����ģʽ������Դģʽ
#define LB_NO_ICON      0x04    //�Ƿ���ʾͼ��
#define LB_PAGE         0x08    //ʹ�÷�ҳ�˵�
#define LB_EQ           0x10    //EQ�б�

#define MB_TITLE        0x01    //�Ƿ���Ҫ�˵�������
#define MB_RES          0x02    //����ģʽ������Դģʽ
#define MB_YESNO        0x80    //����������ģʽ
#define MB_OK           0x40    //��OK��������ʽ

#define PG_VOLKEY       0x10    //Volume Keyģʽ������ӦVolume Key

#define FL_CDUP         0x08    //�Ƿ���Ҫ������һ���Ĳ˵�

//�ļ������1byteΪ��־λ

#define FB_UNICODE      0x00
#define FB_GBK          0x80

#define FB_ICON_MASK    0x0f
#define FB_DEFAULT      0x00
#define FB_FOLDERUP     0x01
#define FB_FOLDER       0x02
#define FB_FILE         0x03
#define FB_MUSIC        0x04

#define TX_CENTER       0x7FFF      //���־�����ʾ
#define TX_HALF         0x01        //�Ƿ���ʾ������

void ocx_init(void (*event_func)(void), bool (*msg_func)(u8 msg));
void ocx_event(void);
void ocx_msg(u8 msg);
bool msgbox(u16 param, u8 mode, u8 show_time);
u16 numbox(u8 value, u8 mode);

#endif
