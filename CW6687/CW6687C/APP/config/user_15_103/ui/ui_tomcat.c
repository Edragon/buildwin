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
#elif IS_LCD_DISPLAY
#pragma location="DISP_SEG"
void tomcat_picture_display(void)
{
    line_clear();
    line_update(2);
    line_update(4);
    line_update(6);
    draw_img(RES_MAIN_TOMCAT3_BMP, 48, 3);
}

#pragma location="TASK_TOMCAT_SEG"
void task_tomcat_display(void)
{
    if (!t_tom.disp_flag) {
        return ;
    }
    t_tom.disp_flag = 0;

    //显示播放状态
    if(t_tom.sta == TOM_NULL) {
        draw_img(RES_ICON_STOP_BMP, 0, 0);
    }else if(t_tom.sta == TOM_REC) {
        if(t_tom.sta != t_tom.last_sta)
            draw_img(RES_ICON_REC_BMP, 0, 0);
    }else {
        draw_img(RES_STATUS0_BMP, 0, 0);
    }
    t_tom.last_sta = t_tom.sta;
    if(!t_tom.picture_disp_flag) {
        return;
    }
    t_tom.picture_disp_flag = 0;
    tomcat_picture_display();
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
    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        if(t_tom.sta == TOM_REC)
            break;
        show_volume();
        line_clear();
        disp_menu(STR_MAIN_TOMCAT,0,0, 0,ME_CENTER);
        line_update(0);
        t_tom.picture_disp_flag = 1;
        t_tom.disp_flag = 1;             //设置更新标志
        break;

    default:
        deal_msg(msg);
        break;
    }
}
/*
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
}*/
#endif
