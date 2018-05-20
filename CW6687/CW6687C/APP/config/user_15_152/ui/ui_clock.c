/*****************************************************************************
* Module    : UI
* File      : ui_clock.c
* Author    : Hanny
* Function  : ʱ���û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C

//ʱ�ӽ�����ʾ
#if IS_LEDSEG_5C7S_DISPLAY
#pragma location="TASK_CLOCK_SEG"
void task_clock_show_display(void)
{
    if((t_clock.sub_sta == STA_SHOW_CLOCK) || (task_ctl.work_sta == TASK_ALARM))
    {
        if(IRTCON & BIT(3))
        {
            IRTCON &= ~BIT(3);
            //u32 rtc_time = irtcc_read_rtc(READ_RTC);
            //printf("clock:%08lx\n",rtc_time);
            make_time(irtcc_read_rtc(READ_RTC));
        }
    }
    ledseg_disp(MENU_RTC_TIME);
}

//ʱ�ӽ�����ʾ
#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        task_clock_show_display();
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_CLOCK_SEG"
void task_clock_show_display(void)
{
}

#pragma location="TASK_CLOCK_SEG"
void task_clock_display(void)
{
}
#endif

//ʱ�ӽ�����Ϣ����
#pragma location="TASK_CLOCK_SEG"
void task_clock_deal_msg(u8 msg)
{
   //u32 cur_time ;
    switch(msg) {
#if ALARM_EN
    case KU_PLAY:
        t_time.hour = sys_ctl.alarm_hour;
        t_time.minute = sys_ctl.alarm_min;
        t_clock.sub_sta = STA_SHOW_ALARM;       //��ʾ����
        t_clock.disp_flag = 1;
        //cur_time =  irtcc_read_rtc(READ_RTC);
        //next_alarm = irtcc_read_rtc(READ_ALM);
        //printf("cur=0x%lX\n",cur_time);
        //printf("nxaxx=0x%lX\n",next_alarm);
        break;

    case KU_PLAY_POWER:
        if(sys_ctl.alarm_switch){
            user_set_alarm_switch(0);
        }else{
            user_set_alarm_switch(1);
        }
        break;
#endif

    case KL_PLAY:
        t_clock.set_sta = 0;
        t_clock.sub_sta = STA_SET_CLOCK;        //��ʾ����ʱ��˵�
        t_clock.setting_flag = 0;
        show_setting_clock();
        break;

    default:
        deal_msg(msg);
        break;
    }
}

//ʱ�����ý�����Ϣ����
#pragma location="TASK_CLOCK_SEG"
void task_clock_set_deal_msg(u8 msg)
{
    switch(msg) {
        //ȷ������
    case KL_PLAY:
        t_time.second = 0;
        set_rtc_time();                         //���趨��ʱ��д��RTC
#if ALARM_EN
        task_alarm_set();                       //��������
#endif
        t_clock.sub_sta = STA_SHOW_CLOCK;       //��ʾʱ��
        break;

        //ѡ��������
    case KU_PLAY:
        t_clock.set_sta++;
        if (t_clock.set_sta > STA_SET_TIME_LAST) {
            t_clock.set_sta = 0;
        }
        break;

#if ALARM_EN
    case KU_PLAY_POWER:
        if(sys_ctl.alarm_switch){
            user_set_alarm_switch(0);
        }else{
            user_set_alarm_switch(1);
        }
        break;
#endif

        //����
    case K_TIME_UP:
    case KL_TIME_UP:
    case KH_TIME_UP:
    case K_PREV_VOL_DOWN:
    case KL_PREV_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        task_clock_set_dec();
        t_clock.setting_flag = 1;
        show_setting_clock();
        break;

        //����
    case K_TIME_DOWN:
    case KL_TIME_DOWN:
    case KH_TIME_DOWN:
    case K_NEXT_VOL_UP:
    case KL_NEXT_VOL_UP:
    case KH_NEXT_VOL_UP:
        task_clock_set_inc();
        t_clock.setting_flag = 1;
        show_setting_clock();
        break;

        //ȡ������
    case KU_HSF:
        t_clock.sub_sta = STA_SHOW_CLOCK;       //��ʾʱ��
        break;

    default:
        deal_msg(msg);
        break;
    }
}

#if ALARM_EN
//���ӽ�����Ϣ����
#pragma location="TASK_CLOCK_SEG"
void task_clock_alarm_deal_msg(u8 msg)
{
    switch(msg) {
    case KU_PLAY:
        t_clock.sub_sta = STA_SHOW_CLOCK;       //��ʾʱ��
        t_clock.disp_flag = 1;
        break;

    case KU_PLAY_POWER:
        if(sys_ctl.alarm_switch){
            user_set_alarm_switch(0);
        }else{
            user_set_alarm_switch(1);
        }
        break;

    case KL_PLAY:
        //printf("set al\n");
        t_clock.set_sta = 0;
        t_clock.sub_sta = STA_SET_ALARM;
        t_clock.setting_flag = 0;
        show_setting_alarm();
        break;
    case KU_MODE:
        task_ctl.work_sta = TASK_EXIT;
        break;
    default:
        deal_msg(msg);
    }
}

//�������ý�����Ϣ����
#pragma location="TASK_CLOCK_SEG"
void task_clock_alarmset_deal_msg(u8 msg)
{
    switch(msg) {
        //ȷ������
    case KL_PLAY:
        user_set_alarm_switch(1);
        user_set_alarm_time(t_time.hour, t_time.minute);
        task_alarm_set();
        t_time.hour = sys_ctl.alarm_hour;
        t_time.minute = sys_ctl.alarm_min;
        t_clock.sub_sta = STA_SHOW_ALARM;       //��ʾ����
        t_clock.disp_flag = 1;
        break;

    case KU_PLAY_POWER:
        if(sys_ctl.alarm_switch){
            user_set_alarm_switch(0);
        }else{
            user_set_alarm_switch(1);
        }
        break;

       //ѡ��������
  /*  case KU_NEXT:
    case KU_PREV:
        t_clock.set_sta = (t_clock.set_sta == STA_SET_TIME_HOUR) ? STA_SET_TIME_MIN : STA_SET_TIME_HOUR;
        t_clock.disp_flag = 1;
        break;*/
    case KU_PLAY:
        t_clock.set_sta = (t_clock.set_sta == STA_SET_TIME_HOUR) ? STA_SET_TIME_MIN : STA_SET_TIME_HOUR;
        break;
        //����
    case K_TIME_UP:
    case KL_TIME_UP:
    case KH_TIME_UP:
        task_clock_set_dec();
        t_clock.setting_flag = 1;
        show_setting_alarm();
        break;

    case K_TIME_DOWN:
    case KL_TIME_DOWN:
    case KH_TIME_DOWN:
        task_clock_set_inc();
        t_clock.setting_flag = 1;
        show_setting_alarm();
        break;

        //ȡ������
    case KU_HSF:
        t_time.hour = sys_ctl.alarm_hour;
        t_time.minute = sys_ctl.alarm_min;
        t_clock.sub_sta = STA_SHOW_ALARM;       //��ʾʱ��
        t_clock.setting_flag = 0;
        break;

    case KU_MODE:
        task_ctl.work_sta = TASK_EXIT;
        break;
    }
}
#endif

#endif
