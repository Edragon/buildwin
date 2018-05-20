/*****************************************************************************
 * Module    : Display
 * File      : ledseg_display.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD��������ʾ����
*****************************************************************************/
#include "include.h"

#if LEDSEG_THEME

type_ledseg_ocx_ctl ledseg_ocx;

//�ؼ��¼�
#pragma location="LEDSEG_OCX"
bool ledseg_ocx_event(void)
{
    if (ledseg_ocx.show_time && ocx_show_cnt >= ledseg_ocx.show_time) {
        ledseg_ocx.status = LEDSEG_OCX_NULL;
    }
    if (ledseg_ocx.status != ledseg_ocx.disp_status || ledseg_ocx.disp_flag) {
        ledseg_ocx.disp_status = ledseg_ocx.status;
        ledseg_ocx.disp_flag = 0;
        return true;
    }
    return false;
}

//�ؼ����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_clear(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    ledseg_ocx.show_time = 0;
    ledseg_ocx.status = LEDSEG_OCX_NULL;
    ledseg_ocx.disp_status = 0xff;
    IE_EA = ie_ea;
}

//��ʾ����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_num(void)
{
    ledseg_ocx.status = LEDSEG_OCX_NUMBOX;
    ledseg_ocx.disp_flag = 1;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾ����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_vol(void)
{
    ledseg_ocx.status = LEDSEG_OCX_VOLBOX;
    ledseg_ocx.disp_flag = 1;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾƵ��
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_chan(void)
{
    ledseg_ocx.status = LEDSEG_OCX_CHANBOX;
    ledseg_ocx.disp_flag = 1;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾEQ
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_eq(void)
{
    ledseg_ocx.status = LEDSEG_OCX_EQ;
    ledseg_ocx.disp_flag = 1;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾPLAY MODE
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_mode_play(void)
{
    ledseg_ocx.status = LEDSEG_OCX_MODE_PLAY;
    ledseg_ocx.disp_flag = 1;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾALARM����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_alarm_status(void)
{
    ledseg_ocx.status = LEDSEG_OCX_ALARM_STATUS;
    ledseg_ocx.disp_flag = 1;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾALARM����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_mix_step(void)
{
    ledseg_ocx.status = LEDSEG_OCX_MIX_STEP;
    ledseg_ocx.disp_flag = 1;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾ���豸
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_no_device(void)
{
    ledseg_disp(MENU_NO_DEVICE);
    ledseg_ocx.status = LEDSEG_OCX_NO_DEVICE;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾɾ��
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_del(void)
{
    ledseg_disp(MENU_DEL);
    ledseg_ocx.status = LEDSEG_OCX_DEL;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾ������
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_full(void)
{
    ledseg_disp(MENU_FULL);
    ledseg_ocx.status = LEDSEG_OCX_FULL;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾ����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_err(void)
{
    ledseg_disp(MENU_ERR);
    ledseg_ocx.status = LEDSEG_OCX_ERR;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//��ʾ�ؼ�
#pragma location="LEDSEG_OCX"
void ledseg_ocx_display(void)
{
    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NUMBOX:
        ledseg_disp(MENU_MUSIC_NUM);
        break;
    case LEDSEG_OCX_VOLBOX:
        ledseg_disp(MENU_VOL);
        break;
    case LEDSEG_OCX_CHANBOX:
        ledseg_disp(MENU_FQ_CHAN);
        break;
    case LEDSEG_OCX_EQ:
        ledseg_disp(MENU_EQ);
        break;
    case LEDSEG_OCX_MODE_PLAY:
        ledseg_disp(MENU_MODE_PLAY);
        break;
    case LEDSEG_OCX_ALARM_STATUS:
        ledseg_disp(MENU_ALARM_STA);
        break;
    /*
    //�Ѿ����������ledseg_disp��
    case LEDSEG_OCX_NO_DEVICE:
        ledseg_disp(MENU_NO_DEVICE);
        break;
    case LEDSEG_OCX_DEL:
        ledseg_disp(MENU_DEL);
        break;
    case LEDSEG_OCX_FULL:
        ledseg_disp(MENU_FULL);
        break;
    case LEDSEG_OCX_ERR:
        ledseg_disp(MENU_ERR);
        break;
        */
    }
}

u8 disp_freq_show_buf[16];      //��¼�ϴε���ʾֵ

//�ȴ�����
#pragma location="DRAW_IMG_SEG1"
void draw_wait(void)
{
}

//����
void draw_clear(void)
{
}

//����ѡ�е�ֵ
#pragma location="LIST_BOX_SEG"
u8 listbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time)
{
    return 0;
}

void poweroff_display(void)
{
    ledseg_disp(MENU_POWEROFF);
}
#endif
