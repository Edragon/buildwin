/*****************************************************************************
 * Module    : Display
 * File      : progressbox.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �������ؼ������ڸ�����ֵ���õ�
 *****************************************************************************/
#include "include.h"
#if LCD_THEME
#include "task.h"
//list box controller
IAR_XDATA_A struct {
    u8 disp_value;                      //��ǰ��ʾ��ѡ��������ж��Ƿ���Ҫˢ��
    u8 value;                           //��ǰ��ѡ����
    u8 in_value;                        //�����ԭ��ֵ
    u8 min;                             //��Сֵ
    u8 max;                             //���ֵ
    u8 on_change_flag;                  //�Ƿ���Ӧ�����¼�
    u8 on_click_flag;                   //�Ƿ���Ӧ̧���¼�
    u8 key_mode;                        //��Ӧ���¼�����������
    u8 ok_flag;                         //�Ƿ���ȷ����
    string title;                       //��������
    u8 show_time;                       //��ʾʱ��
    bool (*on_change)(u8 value);        //���ýӿڣ���Ӧ�����¼�������false��ΪԤ�裬����true���ʾ���趨
    bool (*on_click)(u8 value);         //���ýӿڣ���Ӧ̧���¼�
} t_prog @ "OCX_XDATA";

//��������
#pragma location="PROGRESS_SEG"
void progressbar_dec(void)
{
    if (t_prog.value > t_prog.min) {
        t_prog.value--;
        if (NULL != t_prog.on_change && (*t_prog.on_change)(t_prog.value)) {
            t_prog.in_value = t_prog.value;
        }
    }
    ocx_show_cnt = 0;
}

//��������
#pragma location="PROGRESS_SEG"
void progressbar_inc(void)
{
    if (t_prog.value < t_prog.max) {
        t_prog.value++;
        if (NULL != t_prog.on_change && (*t_prog.on_change)(t_prog.value)) {
            t_prog.in_value = t_prog.value;
        }
    }
    ocx_show_cnt = 0;
}

#pragma location="PROGRESS_SEG"
void progressbar_event(void)
{
    if (!comm_event()) {
        t_prog.ok_flag = 0xff;
        return;
    }
    ocx_event();
    if (t_prog.show_time && ocx_show_cnt >= t_prog.show_time) {
        t_prog.ok_flag = 0xff;
    }
    /*
    if(t_prog.auto_next)
    {
      if (music_get_status() <= STATUS_PLAY_STOPPING)
      {
          t_prog.ok_flag = 0xff;
      }
    }
    */
}

//�������ؼ�������Ϣ
#pragma location="PROGRESS_SEG"
void progressbar_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //ȷ��
    case KU_OK:
        t_prog.ok_flag = 1;
        break;

        //ȡ��
    case KU_BACK:
        t_prog.ok_flag = 0xff;
        break;

        //�����������˵�
    case KL_MENU:
        task_ctl.work_sta = TASK_EXIT;
        break;

        //��һ���һ��
    case K_PREV:
    case KL_PREV:
    case KH_PREV:
        if (!t_prog.key_mode) {
            progressbar_dec();
        }
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
        if (t_prog.key_mode) {
            progressbar_dec();
        }
        break;

    case K_NEXT:
    case KL_NEXT:
    case KH_NEXT:
        if (!t_prog.key_mode) {
            progressbar_inc();
        }
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
        if (t_prog.key_mode) {
            progressbar_inc();
        }
        break;

    case KU_PREV:
    case KU_NEXT:
    case KU_VOL_DOWN:
    case KU_VOL_UP:
    case KLU_PREV:
    case KLU_NEXT:
    case KLU_VOL_DOWN:
    case KLU_VOL_UP:
        if (t_prog.on_click_flag) {
            if ((*t_prog.on_click)(t_prog.value)) {
                t_prog.in_value = t_prog.value;
            }
            ocx_show_cnt = 0;
        }
        break;

        //���ù�����Ϣ����
    default:
        ocx_msg(msg);
        break;
    }
}

//�������ؼ���ʾ�ӿ�
#pragma location="PROGRESS_SEG"
void progressbar_display(void)
{
    if (t_prog.value == t_prog.disp_value) {
        return;
    }
    t_prog.disp_value = t_prog.value;
    line_clear();
    //disp_llp_text(t_prog.title, TX_CENTER, 0, 0);
    disp_menu(t_prog.title,1,0, 0,ME_CENTER);
    line_update_clear(0);
    disp_num(t_prog.value, 56, 0, DN_RIGHT | 2);
    line_update_clear(3);

    progressbar(((t_prog.value - t_prog.min) << 7) / (t_prog.max - t_prog.min), 0);
    line_update(5);
}

//value: ��ǰֵ;  max: ���ֵ;  modeģʽѡ��;  param: ���ݲ���; mode: ģʽ; show_time: ��ʾʱ��
//���ص�����ֵ
#pragma location="PROGRESS_SEG"
u8 progressbox(u8 value, u8 min, u8 max, u32 param, u8 mode, u8 show_time)
{
    t_prog.disp_value = 0xFF;
    if (value > max) {
        value = max;
    }
    t_prog.in_value = value;
    t_prog.value = value;
    t_prog.min = min;
    t_prog.max = max;
    if (t_prog.min > max) {
        t_prog.min = 0;
    }
    t_prog.ok_flag = 0;
    t_prog.key_mode = (mode & PG_VOLKEY) ? 1 : 0;
#pragma diag_remark=Pe1053
    //��ʱ����Pe1053������棬ָ�����ʹ�תС
    t_prog.title = ((type_progress_param *)param)->title;
    t_prog.on_change = ((type_progress_param *)param)->on_change;
    t_prog.on_click = ((type_progress_param *)param)->on_click;
    t_prog.show_time = show_time;
    t_prog.on_change_flag = (t_prog.on_change == NULL) ? 0 : 1;
    t_prog.on_click_flag = (t_prog.on_click == NULL) ? 0 : 1;
#pragma diag_warning=Pe1053
    draw_clear();
    ocx_show_cnt = 0;
    while (!t_prog.ok_flag) {
        progressbar_event();
        progressbar_deal_msg();
        progressbar_display();
#if ALARM_EN
        if (task_ctl.work_sta == TASK_EXIT || task_ctl.work_sta == TASK_ALARM) {
#else
        if (task_ctl.work_sta == TASK_EXIT) {
#endif
            t_prog.ok_flag = 0xfe;
        }
    }
    if (t_prog.ok_flag == 1) {
        return t_prog.value;            //����ȷ����������ѡ����ȷ����ť
    }
    if (t_prog.on_change_flag) {
        (*t_prog.on_change)(t_prog.in_value);
    }
    return 0xff;
}

#endif
