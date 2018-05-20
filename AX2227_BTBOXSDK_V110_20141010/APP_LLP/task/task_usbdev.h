/*****************************************************************************
 * Module    : Task
 * File      : task_usbdev.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : USB�豸����ض���
 *****************************************************************************/
#ifndef TASK_ALARM_H
#define TASK_ALARM_H

#define USB_AUDIO_VOLMAX      15  //��������

typedef struct {
    u8 disp_opt;                            //��ʾ�Ķ�д״̬
    u8 opt_status;                          //��ǰ�Ķ�д״̬
    u8 disp_update;                         //�������Ը��¡���Ҫ���Ƹ���Ƶ��
    u8 disp_volume;                         //��ʾ������ֵ
    u8 first_key;                           //���͵�һ������
    u8 vol_flag;                            //����������־
} type_usbdev_task;

extern type_usbdev_task t_usbdev;


void task_usbdev_display(void);

#endif
