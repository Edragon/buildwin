/*****************************************************************************
 * Module    : Theme
 * File      : lcd_theme_act_menu.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ������
 *****************************************************************************/
#include "include.h"
#include "task.h"

#if (THEME_SELECT == NO_THEME)

#if BT_NAME_DYMANIC_CHANGE
extern IAR_CONST u8 bt_name[32];
#endif

#if NUMBER_KEY_TO_CALL
extern IAR_XDATA_A u8 call_number_key[24];
#endif

//���˵�����ʾ
#pragma location="TASK_MENU_SEG"
void task_menu_display(void)
{
}

//��������������ʾ
#pragma location="TASK_MUSIC_SEG"
void task_music_display(void)
{
}
//������ʾ����
#pragma location="TASK_ALARM_SEG"
void task_alarm_display(void)
{

}
//¼��������ʾ����
#pragma location="TASK_AUX_EQ_SEG"
void task_aux_eq_display(void)
{
}
#pragma location="TASK_BT_SEG"
void task_bt_display(void)
{
}

#pragma location="BT_API2"
void task_bt_display_device(u8 show_time)
{
}
//ʱ�ӽ�����ʾ
#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
}
#pragma location="TASK_FMREC_SEG"
void task_fm_record_display(void)
{

}

//������ʾ����
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display(void)
{
}

void task_fm_disp_clear(void)
{
}

#pragma location="TASK_POWER"
void task_poweroff_display(void)
{
}
#pragma location="TASK_RECORD_SEG"
void task_recording_display(void)
{
}
#pragma location="TASK_MIC_SEG"
void task_mic_display(void)
{
}
#pragma location="TASK_AUX_SEG"
void task_aux_display(void)
{
}

#pragma location="TASK_SPEAKER_SEG"
void task_speaker_display(void)
{
}
#pragma location="TASK_UPDATE_SEG"
void task_updatefw_dispmem_init(void)
{

}

//������ʾ
#pragma location="TASK_UPDATE_SEG"
void task_update_fw_progress_display(u8 percent)
{

}
//������ʾ����
#pragma location="TASK_USBDEV_SEG"
void task_usbdev_display(void)
{

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
//����ȷ����ȡ��
#pragma location="MSG_BOX_SEG"
bool msgbox(u16 param, u8 mode, u8 show_time)
{
    return true;
}
//����ѡ�е�ֵ
#pragma location="LIST_BOX_SEG"
u8 listbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time)
{
    return 0;
}

#if (HARD_IR_EN || SOFT_IR_EN)

IAR_XDATA_A struct {
    u8 cnt;                             //�����������λ��
    u16 value;                          //�����������ֵ
    u16 disp_value;                     //��ǰ��ʾ����ֵ
    u8 ok_flag;                         //�Ƿ���ȷ����
} t_num @ "OCX_XDATA";

#pragma location="NUM_BOX_SEG"
void numbox_event(void)
{
    if (!comm_event()) {
        t_num.ok_flag = 0xff;
    }
    if (ocx_show_cnt >= 20) {
        t_num.ok_flag = 1;
    }
}

//��ѡ������Ϣ
#pragma location="NUM_BOX_SEG"
void numbox_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //ȷ��
    case T_KEY_OK:
        t_num.ok_flag = 1;
        break;

        //ȡ��
    case T_KEY_BACK:
        t_num.ok_flag = 0xff;
        break;

    case T_KEY_NUM_0:
    case T_KEY_NUM_1:
    case T_KEY_NUM_2:
    case T_KEY_NUM_3:
    case T_KEY_NUM_4:
    case T_KEY_NUM_5:
    case T_KEY_NUM_6:
    case T_KEY_NUM_7:
    case T_KEY_NUM_8:
    case T_KEY_NUM_9:
        if(task_ctl.work_sta != TASK_BT){
            if (t_num.cnt >= 4) {
                t_num.value = 0;
            }
            if (t_num.value == 0) {
                t_num.cnt = 0;
            }
            t_num.value = t_num.value * 10 + (msg - T_KEY_NUM_0);
            t_num.cnt++;
            ocx_show_cnt = 0;
        }
        else{
            if (t_num.cnt >= 32) {
                t_num.value = 0;
            }
            t_num.value = (t_num.value % 1000 * 10 + (msg - T_KEY_NUM_0)) % 10000;
#if BT_NAME_DYMANIC_CHANGE
            //*((u8 *)&bt_name[t_num.cnt]) = msg - T_KEY_NUM_0 + '0';
#endif
#if NUMBER_KEY_TO_CALL
            *((u8 *)&call_number_key[t_num.cnt + 3]) = msg - T_KEY_NUM_0 + '0';
#endif
            t_num.cnt++;
            ocx_show_cnt = 0;
            if(t_num.cnt >= 32){
                t_num.ok_flag = 1;
            }
        }
        break;

        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}
#endif

//value��ʼֵ; modeΪģʽ
//����ѡ��ֵ
#pragma location="NUM_BOX_SEG"
u16 numbox(u8 value, u8 mode)
{
#if (HARD_IR_EN || SOFT_IR_EN)
    t_num.disp_value = 0xffff;
    t_num.ok_flag = 0;
    t_num.value = value;
    t_num.cnt = 1;
    ocx_show_cnt = 0;

    while (!t_num.ok_flag) {
        numbox_event();
        numbox_deal_msg();
    }

    if (t_num.ok_flag == 1) {
        return t_num.value;                     //����ȷ����������ѡ���ֵ
    }
    return 0xffff;                              //������Чֵ
#else
    return 0;
#endif
}

#pragma location="PROGRESS_SEG"
u8 progressbox(u8 value, u8 min, u8 max, u32 param, u8 mode, u8 show_time)
{
    return 0;
}


#endif
