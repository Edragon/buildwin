/*****************************************************************************
* Module    : UI
* File      : ui_clock.c
* Author    : Hanny
* Function  : ʱ���û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C

#if IS_LEDSEG_5C7S_DISPLAY
#pragma location="TASK_CLOCK_SEG"
void task_alarm_show_display(void)
{
    if(task_ctl.work_sta == TASK_ALARM)
    {
        if(IRTCON & BIT(3))
        {
            IRTCON &= ~BIT(3);
            //u32 rtc_time = irtcc_read_rtc(READ_RTC);
            //printf("alarm:%08lx\n",rtc_time);
            make_time(irtcc_read_rtc(READ_RTC));
        }
    }
    ledseg_disp(MENU_RTC_TIME);
}

//ʱ�ӽ�����ʾ
#pragma location="TASK_CLOCK_SEG"
void task_alarm_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        task_alarm_show_display();
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_CLOCK_SEG"
void task_alarm_show_display(void)
{
}
#pragma location="TASK_CLOCK_SEG"
void task_alarm_display(void)
{
}
#endif

//������Ϣ����
#pragma location="TASK_ALARM_SEG"
void task_alarm_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
    case KU_PLAY:      //�رյ�ǰ����
    case KL_PLAY:
    case KU_PLAY_POWER:
        t_alarm.index = 0;
        t_alarm.cur_state = 1;
        break;
    case KU_HSF:        //����
        t_alarm.cur_state = 1;
        t_alarm.index = 1;
        break;
        //��һ���һ��
    case KU_PREV:
        t_alarm.index = 1;
        break;
    case KU_NEXT:
        t_alarm.index = 0;
        break;

        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}
#endif
