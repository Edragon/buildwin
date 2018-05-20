/*****************************************************************************
* Module    : UI
* File      : ui_tomcat.c
* Author    : Hanny
* Function  : 汤姆猫任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//汤姆猫任务显示处理
#if IS_LEDSEG_7PIN_DISPLAY
#pragma location="TASK_TOMCAT_SEG"
void task_tomcat_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_TOMCAT);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_TOMCAT_SEG"
void task_tomcat_display(void)
{
}
#endif

//任务消息处理
#pragma location="TASK_TOMCAT_SEG"
void task_tomcat_deal_msg(u8 msg)
{
    switch(msg) {

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
        break;

    default:
        deal_msg(msg);
        break;
    }
}
#endif
