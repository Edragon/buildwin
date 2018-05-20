/*****************************************************************************
* Module    : UI
* File      : ui_clock.c
* Author    : Hanny
* Function  : 时钟用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

#if IS_LEDSEG_7PIN_DISPLAY
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

//时钟界面显示
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
#elif IS_LCD_DISPLAY
#pragma location="TASK_CLOCK_SEG"
void task_alarm_display(void)
{
    //char *msg_ptr;
    if (t_alarm.index == t_alarm.disp_index) {
        return;
    }
    t_alarm.disp_index = t_alarm.index;

    disp_clear();

    //    if ((ALARM_REBELL_TIMES-1) != t_alarm.ring_times) {
    disp_menu(STR_ALARM_DELAY_CONFIRM, 2, 0, 0, 0);
    //disp_text(msg_ptr, 2, 0, 0);
    //disp_text(msg_ptr, 2, 0, 0);
    disp_menu(STR_ALARM_MSG_OK, 25, 0, 6, 0);
    disp_menu(STR_ALARM_MSG_CANCEL, 79, 0, 6, 0);
#if(FONT_SEL == SMALL_FONT)
    disp_invert((t_alarm.index == 1) ? 22 : 76,        //反色选取
                6,
                (sys_ctl.lang_id == 0 || t_alarm.index == 1) ? 30 : 42,
                2
                    );
#else
    disp_invert((t_alarm.index == 1) ? 22 : 76,        //反色选取
                6,
                (sys_ctl.lang_id == 0 || t_alarm.index == 1) ? 37 : 49,
                2
                    );
#endif

    //    } else {
    //        disp_menu(STR_MSG_OK, 52, 0, 6, 0);
    //#if(FONT_SEL == SMALL_FONT)
    //        disp_invert(49, 6, 30, 2);
    //#else
    //        disp_invert(49, 6, 35, 2);
    //#endif
    //    }

    u16 temp;
    temp = (u16)sys_ctl.alarm_hour * 60 + sys_ctl.alarm_min + t_alarm.ring_times*(SECS_ALARM_REBELL/60);
    disp_num(temp/60, 45 , 3, DN_RIGHT | DN_ZERO | 2);
    disp_draw(RES_MEDIUMNUM_COLON_BMP, 62, 3);
    disp_num(temp%60, 65, 3, DN_RIGHT | DN_ZERO | 2);
    disp_update();
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

//任务消息处理
#pragma location="TASK_ALARM_SEG"
void task_alarm_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
    //关闭当前闹钟
    case KU_PLAY_POWER:
        t_alarm.index = 0;
        t_alarm.cur_state = 1;
        break;
    // 选择 重响
    case KU_PLAY:
    case KL_PLAY:
        t_alarm.cur_state = 1;
        break;
    //上一项，下一项
    case KU_PREV:
        t_alarm.index = 1;
        break;
    case KU_NEXT:
        t_alarm.index = 0;
        break;

        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}
#endif
