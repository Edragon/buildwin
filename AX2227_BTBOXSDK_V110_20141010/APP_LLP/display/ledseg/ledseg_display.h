/*****************************************************************************
 * Module    : Display
 * File      : ledseg_display.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ����ʾ��������
*****************************************************************************/
#ifndef LEDSEG_DISPLAY_H
#define LEDSEG_DISPLAY_H

#define DISP_TIME_COUNT  20         //2s
typedef struct {
    u8 show_time;                   //��ʾ��ʱ�����
    u8 status;                      //״̬
    u8 disp_status;                 //��ǰ��ʾ��״̬
    u8 disp_flag;
} type_ledseg_ocx_ctl;

enum {
    LEDSEG_OCX_NULL,
    LEDSEG_OCX_PHONE_NUMBER,       //�绰����
    LEDSEG_OCX_PHONE_TIME,         //ͨ��ʱ��
    LEDSEG_OCX_NUMBOX,             //���Կ�
    LEDSEG_OCX_VOLBOX,             //������
    LEDSEG_OCX_CHANBOX,            //��̨��
    LEDSEG_OCX_EQ,                 //EQ��
    LEDSEG_OCX_MODE_PLAY,          //PLAY MODE��ʾ��
    LEDSEG_OCX_ALARM_STATUS,       //���ӿ���״̬��ʾ
    LEDSEG_OCX_MIX_STEP,
    LEDSEG_OCX_NO_DEVICE,
    LEDSEG_OCX_DEL,
    LEDSEG_OCX_FULL,
    LEDSEG_OCX_ERR,
};

extern type_ledseg_ocx_ctl ledseg_ocx;
bool ledseg_ocx_event(void);
void ledseg_ocx_clear(void);
void ledseg_ocx_display(void);
void ledseg_ocx_show_num(void);
void ledseg_ocx_show_vol(void);
void ledseg_ocx_show_chan(void);
void ledseg_ocx_show_eq(void);
void ledseg_ocx_show_mode_play(void);
void ledseg_ocx_show_alarm_status(void);
void ledseg_ocx_show_mix_step(void);
void ledseg_ocx_show_no_device(void);
void ledseg_ocx_show_del(void);
void ledseg_ocx_show_full(void);
void ledseg_ocx_show_err(void);

extern u8 disp_freq_show_buf[16];

//����Ҫ�����˵�����
#define SET_MENU_ITEM(task_id, res_id, str_id, mp3_id) { \
    .sta = task_id, \
},

void draw_wait(void);
void draw_clear(void);

#endif
