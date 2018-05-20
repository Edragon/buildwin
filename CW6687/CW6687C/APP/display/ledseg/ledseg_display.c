/*****************************************************************************
 * Module    : Display
 * File      : ledseg_display.c
 * Author    : Hanny
 * Function  : LED���������ʾ����
*****************************************************************************/
#include "include.h"
#include "task_clock.h"

#if IS_LEDSEG_DISPLAY

type_ledseg_ocx_ctl ledseg_ocx;

//�ؼ��¼�
#pragma location="LEDSEG_OCX"
void ledseg_ocx_event(void)
{
    if (ledseg_ocx.show_time && ocx_show_cnt >= ledseg_ocx.show_time) {
        ledseg_ocx.status = LEDSEG_OCX_NULL;
    }
    if (ledseg_ocx.status != ledseg_ocx.disp_status) {
        ledseg_ocx.disp_status = ledseg_ocx.status;
    }
}

//�ؼ����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_clear(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    ledseg_ocx.show_time = 0;
    ledseg_ocx.status = LEDSEG_OCX_NULL;
    IE_EA = ie_ea;
}

//��ʾ����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_num(u16 num)
{
    ledseg_ocx.number = num;
    ledseg_ocx.status = LEDSEG_OCX_NUMBOX;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ledseg_ocx.disp_flag = 1;
    ocx_show_cnt = 0;
}

//��ʾ����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_vol(void)
{
    ledseg_ocx.status = LEDSEG_OCX_VOLBOX;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
    ledseg_disp(MENU_VOL);  //���ϸ�����ʾ
}

//��ʾƵ��
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_chan(void)
{
    ledseg_ocx.status = LEDSEG_OCX_CHANBOX;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}

//��ʾEQ
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_eq(void)
{
    ledseg_ocx.status = LEDSEG_OCX_EQ;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}

//��ʾPLAY MODE
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_mode_play(void)
{
    ledseg_ocx.status = LEDSEG_OCX_MODE_PLAY;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}

//��ʾALARM����
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_alarm_status(void)
{
    ledseg_ocx.status = LEDSEG_OCX_ALARM_STATUS;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}

//��ʾ�ؼ�
#pragma location="LEDSEG_OCX"
void ledseg_ocx_display(void)
{
    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NUMBOX:
        ledseg_disp(MENU_NUM);
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
    }
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

//��������ʱ����ʾ
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_setting_clock(void)
{
    if (t_clock.sub_sta == STA_SET_CLOCK) {
        ledseg_ocx.show_time = 1;
        ocx_show_cnt = 1;
        if (t_clock.setting_flag) {
            ledseg_ocx.show_time = DISP_TIME_COUNT;
            ocx_show_cnt = 0;
        }
    }
}
//��������������ʾ
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_setting_alarm(void)
{
    if (t_clock.sub_sta == STA_SET_ALARM) {
        ledseg_ocx.show_time = 1;
        ocx_show_cnt = 1;
        if (t_clock.setting_flag) {
            ledseg_ocx.show_time = DISP_TIME_COUNT;
            ocx_show_cnt = 0;
        }
    }
}

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

#endif
