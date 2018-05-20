/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_menu.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 主菜单任务显示主题
 *****************************************************************************/
#if IS_LCD_DISPLAY

#ifndef CONFIG_C

//菜单上一项
#pragma location="TASK_MENU_SEG"
void task_menu_prev(void)
{

    if (t_menu.index <= t_menu.page_index) {
#if MENU_ROLL
        t_menu.page_index--;
#else
        t_menu.page_index -= NUM_PAGE;
#endif
    }
    if (t_menu.index == 0) {
        t_menu.index = t_menu.total;
#if MENU_ROLL
        if (t_menu.total > NUM_PAGE) {
            t_menu.page_index = t_menu.total - NUM_PAGE;
        } else {
            t_menu.page_index = 0;
        }
#else
        t_menu.page_index = ((t_menu.total - 1) / NUM_PAGE) * NUM_PAGE;
#endif
    }
    t_menu.index--;
    t_menu.icon_index = 1;
    t_menu.icon_cnt = 0;
}


//菜单下一项
#pragma location="TASK_MENU_SEG"
void task_menu_next(void)
{
    t_menu.index++;
    if (t_menu.index - t_menu.page_index >= NUM_PAGE) {
#if MENU_ROLL
        t_menu.page_index++;
#else
        t_menu.page_index += NUM_PAGE;
#endif
    }
    if (t_menu.index == t_menu.total) {
        t_menu.index = 0;
        t_menu.page_index = 0;
    }
    t_menu.icon_index = 1;
    t_menu.icon_cnt = 0;
}

//任务显示处理
#pragma location="TASK_MENU_SEG"
void task_menu_display(void)
{
    if (t_menu.disp_index != t_menu.index) {
        t_menu.disp_flag = 1;
        t_menu.disp_index = t_menu.index;
        move_ctl.name_x=0;
    }
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_menu.disp_flag = 1;             //设置更新标志
    }
    if (!t_menu.disp_flag) {
        return;
    }
    t_menu.disp_flag = 0;

    u8 i, pos;
    disp_clear();

    pos = 12;
    for (i=0; i!=4 && t_menu.page_index+i<t_menu.total; i++) {
        disp_draw(menu_item[t_menu.page_index+i].r_index, pos, 0);
        if (t_menu.index == t_menu.page_index + i) {
            disp_draw(RES_ICON_FOCUSL_BMP, pos-4, 0);
            disp_menu(menu_item[t_menu.page_index+i].name, 0, move_ctl.name_x, 6, ME_CENTER);//示当前菜单项的文字
            move_ctl.name_wide = move_ctl.name_wide1;
            if(move_ctl.name_wide >= 105)
            {
                move_ctl.move_flag=1;
            }
            else
            {
                move_ctl.move_flag=0;
            }
            disp_draw(RES_ICON_FOCUSR_BMP, pos+16, 0);
            disp_draw(menu_item[t_menu.page_index+i].r_index+t_menu.icon_index, 48, 2);
        }
            pos += 24;
    }

    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    disp_update();
}

//任务消息处理
#pragma location="TASK_MENU_SEG"
void task_menu_deal_msg(u8 msg)
{
    switch(msg) {

        //选择菜单
    case KU_OK:
        music_stop();
        task_ctl.work_sta = menu_item[t_menu.index].sta;
        break;

        //上一项，下一项
    case KU_PREV:
        task_menu_prev();
#if MENU_MUSIC
        music_stop();
        mp3_res_play(menu_item[t_menu.index].mp3res);
#endif
        break;

    case KU_NEXT:
        task_menu_next();
#if MENU_MUSIC
        music_stop();
        mp3_res_play(menu_item[t_menu.index].mp3res);
#endif
        break;

    case QSYSTEM_1S:
#if TASK_SHOWTIME_EN
        t_menu.back_sec++;
#endif
        deal_msg(msg);  // 调用公共事件
        break;

#if(SOFT_POWER_ON_OFF) //使用软开关机功能
    case KL_PLAY_POWER:
    #if BT_AUTO_PLAY
        if(sys_ctl.bt_sta == BT_STA_PLAYING){
            sys_ctl.bt_auto_play_flag = 1;
            sys_param_write8(PARAM_BT_AUTO_PLAY, sys_ctl.bt_auto_play_flag);
            param_write(Link_BdAddr, PARAM_REMOTE_DEV_ADDR, 6);
        }else {
            sys_ctl.bt_auto_play_flag = 0;
            sys_param_write8(PARAM_BT_AUTO_PLAY, sys_ctl.bt_auto_play_flag);
        }
    #endif
        sys_ctl.soft_poweroff_flag = 1;
        task_ctl.work_sta = TASK_POWEROFF;
        break;
#endif

    default:
        deal_msg(msg);
        break;
    }
#if TASK_SHOWTIME_EN
    if(msg != NO_KEY && msg!= QSYSTEM_1S)
        t_menu.back_sec = 0;
#endif
}
#endif
#endif
