/*****************************************************************************
 * Module    : Task
 * File      : task_fm.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �����������õ��Ľṹ���
 *****************************************************************************/
#ifndef TASK_RECORD_H
#define TASK_RECORD_H

enum{
    REC_STA_STOP,
    REC_STA_STOPPING,
    REC_STA_RECORD,
    REC_STA_PAUSE,
};

//task record controller
typedef struct {
    u8 rec_src;                         //¼����Դ
    u8 rec_sta;                         //¼��״̬
    u8 file_open;                       //�ļ��򿪱�־
    u8 dir_open;                        //�ļ����Ѵ�����־
    u8 sync_cnt;                        //ͬ������
    u16 rec_time;                       //������ʾ��¼��ʱ��
    u8 play_flag;                       //ֹͣ¼��ʱ��Ҫ�Զ����ŵ�ǰ¼���ļ���־
    u8 full_flag;                       //��ǰ�豸��¼����־������¼����ʱ�Զ��л�¼���豸
#if (RECORD_FILE_NAME == LONG_NAME)
    u8 short_name_ptr;                  //���ļ�������ʼλ��
    u8 long_name_ptr;
#endif
    u8 update_flag;
} type_task_rec_ctl;
extern type_task_rec_ctl t_rec;
#endif
