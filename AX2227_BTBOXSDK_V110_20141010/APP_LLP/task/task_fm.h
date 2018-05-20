/*****************************************************************************
 * Module    : Task
 * File      : task_fm.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �����������õ��Ľṹ���
 *****************************************************************************/
#ifndef TASK_FM_H
#define TASK_FM_H

typedef struct {
    u8 fm_sta;                          //��״̬
    u8 disp_flag;                       //״̬��ʾ���¿���
    u8 disp_ch_flag;                    //Ƶ����ʾ����
    u8 disp_ch_cnt;                     //��ǰ��ʾƵ������
    u8 disp_ch_sel;                     //��ǰ��ʾƵ��ѡ��
    u16 disp_freq;                      //Ƶ����ʾ���¿���

    u16 freq;                           //��ǰƵ��
    u8 ch_cnt;                          //Ƶ������
    u8 ch_sel;                          //��ǰƵ��
    u8 auto_delay;                      //�Զ���̨ʱ���м䲥��ʱ��

    char seek_sta;
    char *num_buf;                      //����¼��ʱ��ʾ¼��ʱ��
    u8 num_len;
    char *colon;
    char *rec_icon;
    char *bignum_buf;
    u8 bignum_len;
    char *bat_buf;
    u8 bat_len;
    char *point_buf;

} type_fm_task;

extern type_fm_task t_fm;

void task_fm_disp_clear(void);

enum {
    FM_PLAY,                            //��������
    FM_AUTO_SEEK,                       //�Զ���̨ģʽ
    FM_AUTO_PLAY,                       //�Զ���̨ģʽ���̵Ķ�ʱ����
};

#endif
