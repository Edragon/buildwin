/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_music.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 音乐播放任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_BT == LCD_THEME_DEFAULT)
#include "task.h"
#include "btapi.h"
extern u8 BT_STATE;

#pragma location="TASK_BT_SEG"
void task_bt_disp_status(void)
{
    if(!(bt_status_flag))
        return;
    line_clear();
    bt_status_flag = 0;
    u8 play_status_icon;
    if(BT_STATE == StateConnected)
    {
        play_status_icon = RES_STATUS1_BMP;       //显示暂停状态
    }
    else if(BT_STATE == StatePlay)
    {
        play_status_icon = RES_STATUS0_BMP;        //显示播放状态
    }
    else
    {
        play_status_icon = RES_ICON_STOP_BMP;
    }
    disp_draw(play_status_icon, 0, 0);
    if(BT_STATE == OnThePhone)
    {
#if BT_DISP_NUM_TIME
        //显示时间
        u8 hour = on_the_phone_time / 3600;
        u8 min = (on_the_phone_time / 60) % 60;
        u8 sec = on_the_phone_time % 60;
        disp_medium_time(hour, min, sec, 28, 0);
#endif
    }
    else if(BT_STATE < StateConnected)
    {
        disp_menu(STR_BT_NOT_LINK, 0, 0, 0, ME_CENTER);
    }
    else
        disp_menu(STR_BT_CONNECTED, 0, 0, 0, ME_CENTER);

    /*if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }*/
    line_update_clear(0);
}
#pragma location="TASK_BT_SEG"
void task_bt_disp_bt_state(void)
{
    /*if(ocx_show_cnt>3 && (BT_STATE == StatePlay))
    {
        ocx_show_cnt = 0;
        move_ctl.move_flag++;
        if(move_ctl.move_flag>3)
            move_ctl.move_flag = 1;
        bt_state_flag = 1;
    }
    else if(BT_STATE != StatePlay)*/
        move_ctl.move_flag = 3;
    if(!bt_state_flag)
        return;
    bt_state_flag = 0;
    line_clear();
    if((BT_STATE == OutGoingCall || BT_STATE == InComingCall) && bt_phonenum_flag) //有些手机不支持号码显示时则显示bt
    {
        line_update_clear(2);
        phone_call_number[11] = 0;
        disp_text((char *)phone_call_number, 20, 0, 0);
        line_update_clear(3);
        line_update_clear(5);
    }
    else
    {
        line_update_clear(2);
        line_update_clear(4);
        draw_img(RES_MAIN_BT0_BMP+move_ctl.move_flag,45,2);
    }

}

//显示频谱
#if BT_FREQ_SHOW
#pragma location="TASK_BT_SEG"
void task_bt_display_disp_freq(void)
{
    if (!freq_show_update) {
        return;
    }
    freq_show_update = 0;
    if (music_freq_get()) {
        line_clear();
        disp_freq_line(0);
        music_freq_kick();
        line_update(6);
    }
}
#endif



//蓝牙任务显示处理
#pragma location="TASK_BT_SEG"
void task_bt_display(void)
{
    task_bt_disp_status();
    task_bt_disp_bt_state();
#if BT_FREQ_SHOW
    task_bt_display_disp_freq();
#endif
}


#endif
