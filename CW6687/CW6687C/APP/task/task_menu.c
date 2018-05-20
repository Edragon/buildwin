/*****************************************************************************
 * Module    : Task
 * File      : task_menu.c
 * Author    : Hanny
 * Function  : 主菜单任务流程
 *****************************************************************************/
#include "include.h"
#if TASK_MENU_EN
#include "task.h"
#include "task_menu.h"
#include "listbox.h"

//task menu controller
type_task_menu_ctl t_menu;

//选择图片资源
#include SET_USER_PATH(CFG_UI_MENU_DIR, /ui/ui_menu.c)
#include SET_USER_PATH(CFG_UI_MENU_DIR, /ui/ui_menu.h)

#pragma constseg="TASK_MENU_CONST"
//所有相关的任务菜单。数据顺序即为菜单顺序
extern IAR_CONST type_menu_item menu_item[] = {
    //SET_MENU_ITEM(任务状态名, 任务图标名, 任务字符串名, 任务响应语音菜单资源)
#if TASK_MUSIC_EN
    //音乐菜单
    SET_MENU_ITEM(TASK_MUSIC, RES_MAIN_MUSIC, STR_MAIN_MUSIC, RES_MP3_MUSIC)
#endif

#if TASK_TAPE_EN
    //磁带菜单
    SET_MENU_ITEM(TASK_TAPE, RES_MAIN_TAPE, STR_MAIN_TAPE, RES_MP3_TAPE)
#endif

#if TASK_AUX_EN
    //AUX LINEIN菜单
    SET_MENU_ITEM(TASK_AUX, RES_MAIN_AUX, STR_MAIN_AUX, RES_MP3_LINE_IN)
#endif

#if TASK_AUX_EQ_EN
    //AUX_EQ LINEIN菜单
    SET_MENU_ITEM(TASK_AUX_EQ, RES_MAIN_AUX, STR_MAIN_AUX, RES_MP3_LINE_IN)
#endif

#if TASK_MIC_EN
    //MIC菜单
    SET_MENU_ITEM(TASK_MIC, RES_MAIN_MIC, STR_MAIN_RECORDER, RES_MP3_RECORD)
#endif

#if TASK_VOICE_EN
    //录音播放菜单
    SET_MENU_ITEM(TASK_VOICE, RES_MAIN_VOICE, STR_MAIN_VOICE, RES_MP3_VOICE)
#endif


#if TASK_FM_EN
    //收音机菜单
    SET_MENU_ITEM(TASK_FM, RES_MAIN_RADIO, STR_MAIN_FM, RES_MP3_FM)
#endif

#if TASK_BT_EN
    //收音机菜单
    SET_MENU_ITEM(TASK_BT, RES_MAIN_BT, STR_MAIN_BT, RES_MP3_BT)
#endif

#if TASK_BTHID_EN
    //独立自拍器菜单
    SET_MENU_ITEM(TASK_BT_HID, RES_MAIN_BT, STR_MAIN_BT, RES_MP3_BT)
#endif

#if TASK_RTC_EN
    //电子钟菜单
    SET_MENU_ITEM(TASK_CLOCK, RES_MAIN_RTC, STR_MAIN_CLOCK, RES_MP3_CLOCK)
#endif

#if TASK_USBDEV_EN
    //USB设备菜单
    SET_MENU_ITEM(TASK_USBDEV, RES_MAIN_USB, STR_MAIN_USB_DEVICE, RES_MP3_USB_DEVICE)
#endif

#if TASK_EBOOK_EN
    //电子书菜单
    SET_MENU_ITEM(TASK_EBOOK, RES_MAIN_EBOOK, STR_MAIN_EBOOK, RES_MP3_EBOOK)
#endif

    //SET_MENU_ITEM(TASK_NAV, RES_MAIN_EXPLORER0_BMP,)

#if TASK_SETTING_EN
    //系统设置菜单
    SET_MENU_ITEM(TASK_SETTING, RES_MAIN_SYSTEM, STR_MAIN_SETTING, RES_MP3_SETTING)
#endif

#if TASK_SPEAKER_EN
    //扩音器菜单
    SET_MENU_ITEM(TASK_SPEAKER, RES_MAIN_SPEARKER, STR_MAIN_LOUD_SPEAKER, RES_MP3_speaker)
#endif

#if TASK_TOMCAT_EN
    //汤姆猫菜单
    SET_MENU_ITEM(TASK_TOMCAT, RES_MAIN_TOMCAT, STR_MAIN_TOMCAT, RES_MP3_TOMCAT)
#endif
};
#pragma constseg=default

//设置上一次的任务
#pragma location="TASK_MENU_SEG"
void task_menu_set_lastwork(void)
{
    u8 index;
    t_menu.index = 0;
    for (index=0; index!=(sizeof(menu_item)/sizeof(type_menu_item)); index++) {
        if (task_ctl.last_work == menu_item[index].sta) {
            t_menu.index = index;
            break;
        }
    }

    if (t_menu.index >= MENU_NUM) {
        t_menu.index = MENU_NUM - 1;
    }
    t_menu.page_index = (t_menu.index / NUM_PAGE) * NUM_PAGE;
    if (t_menu.index == 0) {
        t_menu.page_index = 0;
    } else if (t_menu.index - 1 + NUM_PAGE > MENU_NUM) {
        t_menu.page_index = (MENU_NUM > NUM_PAGE) ? MENU_NUM - NUM_PAGE : 0;
    } else {
        t_menu.page_index = t_menu.index - 1;
    }
    task_ctl.back_up_work_sta = TASK_MENU;

}

//任务初始化
#pragma location="TASK_MENU_SEG"
void task_menu_enter(void)
{
#if MENU_MUSIC
        set_sys_clk(SYS_24M);
#else
        set_sys_clk(SYS_12M);
#endif
    task_ctl.work_sta = TASK_MENU;

    task_menu_set_lastwork();
    //初始化菜单项
    t_menu.total = MENU_NUM;

    if (t_menu.index >= MENU_NUM) {
        t_menu.index = MENU_NUM - 1;
    }

    //主菜单下，显存比较充裕，使用大显存
    set_lcd_buf(LCD_FULL_MODE);

    t_menu.disp_index = 0xff;                //故意与index不同
    t_menu.disp_flag = 1;                   //刷新显示
    t_menu.icon_index = 1;
    t_menu.icon_cnt = 0;
#if TASK_SHOWTIME_EN
    t_menu.back_sec = 0;
#endif
    //sys_ctl.bat_val = get_bat_val();
#if MENU_MUSIC
    sound_ctl_init(3, 0x800, 25, 0x800);     //连续3次超过0x800认为开始有声；连续25次0x800认为静音
    user_change_volume(sys_ctl.volume);//数字音量
    dac_enable();
#endif
}

//任务退出
#pragma location="TASK_MENU_SEG"
void task_menu_exit(void)
{
    //printf("task_menu_exit\n");
#if MENU_MUSIC
    dac_disable();
#endif
}

//任务事务处理
#pragma location="TASK_MENU_SEG"
void task_menu_event(void)
{
    if (move_ctl.ms_flag) {
        move_ctl.ms_flag = 0;
        if(move_ctl.move_flag)
        {
            menu_move_control(101);
        }
        t_menu.icon_cnt++;
        if (t_menu.icon_cnt >= 3) {
            t_menu.icon_cnt = 0;
            if (t_menu.icon_index >= 3) {
                t_menu.icon_index = 0;
            }
            t_menu.icon_index++;
            t_menu.disp_flag = 1;
        }
    }
    music_event();
    if (!comm_event()) {
        return;
    }
    if (device_need_activate(DEVICE_SDMMC) || device_need_activate(DEVICE_UDISK)) {
#if MENU_MUSIC
        music_stop();
#endif
        draw_wait();
        t_menu.disp_flag = 1;
    }
    if (device_need_activate(DEVICE_PC)) {
        if (device_activate(DEVICE_PC)) {
            task_ctl.work_sta = TASK_USBDEV;
            return;
        }
    }
#if TASK_SHOWTIME_EN
    if(t_menu.back_sec >= RETURN_SHOW_TIME_SEC)
    {
        task_ctl.back_up_work_sta = TASK_MENU;
        task_ctl.work_sta = TASK_SHOW_TIME;
        t_menu.back_sec = 0;
    }
#endif
    device_change();
    if (fs_need_mounted()) {
        draw_wait();
        t_menu.disp_flag = 1;                   //刷新显示
        set_sys_clk(SYS_48M);                   //提高主频，加速
        fs_init();
#if MENU_MUSIC
        set_sys_clk(SYS_24M);
#else
        set_sys_clk(SYS_12M);
#endif
    }
    if (fs_is_mounted()) {
        //task_ctl.work_sta = TASK_MUSIC;
    }
}

#if !IS_LCD_DISPLAY
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
  /*  case KU_MENU:
            hold_mode_enter();
            break;*/
/*
    case KL_POWER:
        printf("power off\n");
        //task_ctl.work_sta = TASK_POWEROFF;
        break;
*/
        //长按MENU关机
#if SOFTKEY
    case KL_MENU:
        task_ctl.work_sta = TASK_POWEROFF;
        break;
#endif
    case QSYSTEM_1S:
#if TASK_SHOWTIME_EN
        t_menu.back_sec++;
#endif
        break;
    default:
        break;
    }
#if TASK_SHOWTIME_EN
    if(msg != NO_KEY && msg!= QSYSTEM_1S)
        t_menu.back_sec = 0;
#endif
}
#endif

//任务主流程
#pragma location="TASK_MENU_SEG"
void task_menu(void)
{
    u8 msg;
    task_menu_enter();
    //printf("menu_enter\n");
    while (task_ctl.work_sta == TASK_MENU) {
        msg = get_msg();
        task_menu_event();
        task_menu_deal_msg(msg);
        task_menu_display();

    }
    task_menu_exit();
}

#endif
