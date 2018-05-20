/*****************************************************************************
 * Module    : TaskSub
 * File      : tasksub_music_menu.c
 * Author    : Hanny
 * Function  : 音乐任务 - 菜单设置任务
 *****************************************************************************/
#ifdef TASK_SUB


#define PLAYMODE_ITEMS_NUM          (MODE_PLAY_NUM + 1)
#define MUSIC_EQ_ITEM_NUM           (MUSIC_EQ_NUM + 1)
#define DEVICE_ITEM_NUM             3


#pragma constseg="LIST_CONTENT"
string IAR_CONST tbl_music_menu1[] = {
    STR_MAIN_MUSIC,

#if FILE_NAV_EN
    STR_CARD_DIR,
#endif
    STR_MENU_EQ, STR_PLAYMODE_SET, STR_DEVICE_SELECT,
#if AB_REPEAT_EN
    STR_MUSIC_REPEAT,
#endif
    STR_MENU_DELETE, STR_DELETE_ALL,STR_BACK_MENU, STR_MENU_EXIT
};
string IAR_CONST tbl_music_menu2[] = {
    STR_MAIN_MUSIC,
#if FILE_NAV_EN
    STR_UDISK_DIR,
#endif
    STR_MENU_EQ, STR_PLAYMODE_SET, STR_DEVICE_SELECT,
#if AB_REPEAT_EN
    STR_MUSIC_REPEAT,
#endif
    STR_MENU_DELETE, STR_DELETE_ALL,STR_BACK_MENU, STR_MENU_EXIT
};

string IAR_CONST tbl_music_select_device[DEVICE_ITEM_NUM] = {
    STR_DEVICE_SELECT, STR_PLAY_SD,STR_PLAY_UDISK
};

enum {
#if FILE_NAV_EN
    ITEM_MUSIC_NAV,
#endif
    ITEM_MUSIC_EQ,
    ITEM_MUSIC_PLAYMODE,
    ITEM_SELECT_DEVICE,
#if AB_REPEAT_EN
    ITEM_REPEAT,
#endif
    ITEM_DEL_FILE,
    ITEM_DEL_ALL,
    ITEM_MENU_BACK,
    ITEM_MUSIC_EXIT,
};

//播放模式选择菜单
string IAR_CONST tbl_playmode_items[PLAYMODE_ITEMS_NUM] = {
    STR_PLAYMODE_SET,
    STR_PLAYMODE_ALL,
    STR_PLAYMODE_SINGLE,

#if MODE_PLAY_NUM > 2
    STR_PLAYMODE_RE_FOLDER,
#endif

#if MODE_PLAY_NUM > 3
    STR_PLAYMODE_NORMAL,
#endif

#if MODE_PLAY_NUM > 4
    STR_PLAYMODE_RAMDOM,
#endif

#if MODE_PLAY_NUM > 5
    STR_PLAYMODE_BROWSE
#endif
};

//EQ选择菜单
string IAR_CONST tbl_music_eq[MUSIC_EQ_ITEM_NUM] = {
    STR_MENU_EQ, STR_EQ_NORMAL, STR_EQ_POP, STR_EQ_ROCK, STR_EQ_JAZZ, STR_EQ_CLASSIC, STR_EQ_COUNTRY,STR_EQ_BASS
};

#pragma constseg=default

//播完本设备自动切换到另一个设备
#pragma location="TASK_MUSIC_MENU_SEG_1"
bool task_music_device_autochange(void)
{
    if (fs_cur_dev() == DEVICE_SDMMC)
    {
        if (device_is_actived(DEVICE_UDISK))
        {
            fs_sel(DEVICE_UDISK);
            task_music_device_remount();

            return true;
        }
    }
    else if (fs_cur_dev() == DEVICE_UDISK)
    {
        if (device_is_actived(DEVICE_SDMMC))
        {
            fs_sel(DEVICE_SDMMC);
            task_music_device_remount();
            return true;
        }
    }
    return false;
}

#pragma location="TASK_MUSIC_MENU_SEG_1"
bool task_music_get_total(void)
{
    //获取歌曲的总数
    t_msc.music_total = music_total();
    if (t_msc.music_total == 0) {
        if(!t_msc.no_file_flag)
        {
            t_msc.no_file_flag = 1;
            if(task_music_device_autochange())
            {
                return true;
            }
        }
        task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
        if(t_msc.music_voice_flag)
        {
            msgbox(STR_NO_VOICE, NULL, MSGBOX_TIME);
        }
        else
        {
            msgbox(STR_NO_MUSIC, NULL, MSGBOX_TIME);
        }
        return false;
    }
    t_msc.no_file_flag = 0;
    return true;
}

//进入音乐任务对歌曲目的获取
#pragma location="TASK_MUSIC_MENU_SEG_1"
u8 task_music_num_get(void)
{
    //获取歌曲的总数
    if (fs_cur_dev() == DEVICE_UDISK)
    {
        t_msc.music_num = param_read16(PARAM_MUSIC_NUM_USB);        //打开的文件编号
    }
    else
    {
        t_msc.music_num = param_read16(PARAM_MUSIC_NUM_SD);        //打开的文件编号
    }
    if (!task_music_get_total()) {
        return false;
    }
    if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
        t_msc.music_num = 1;
    }
    return true;
}

#pragma location="TASK_MUSIC_MENU_SEG_1"
void task_music_device_remount(void)
{
    music_stop();                           //停止当前音乐
    if (fs_need_mounted()) {
        fs_init();
    }
    if (!fs_is_mounted()) {
        task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
        return;
    }
    if(!task_music_num_get())
        return;
    //task_music_get_total();

    t_msc.file_change = 1;                  //需要打开一个新文件
#if BREAK_POINT_EN
    t_msc.break_flag = 1;
#endif
}

//切换设备菜单
#pragma location="TASK_MUSIC_MENU_SEG_1"
void task_music_device_select(void)
{
    u8 index = listbox(fs_cur_dev(), DEVICE_ITEM_NUM, NULL, (u16)tbl_music_select_device, LIST_TIME);
    draw_wait();
    if (index == DEVICE_UDISK)
    {
        if (fs_cur_dev() == DEVICE_SDMMC)
        {
            if (device_is_actived(DEVICE_UDISK))
            {
                fs_sel(DEVICE_UDISK);
                task_music_device_remount();
            }
        }
    } else if(index == DEVICE_SDMMC) {
        if (fs_cur_dev() == DEVICE_UDISK)
        {
            if (device_is_actived(DEVICE_SDMMC))
            {
                fs_sel(DEVICE_SDMMC);
                task_music_device_remount();
            }
        }
    }
}

//音乐菜单
#pragma location="TASK_MUSIC_MENU_SEG_1"
void task_music_menu(void)
{
    u8 index;
#if FILE_NAV_EN
    u16 f_index;
    u8 last_work_sta = task_ctl.work_sta;   // 保存工作模式
#endif
    if(fs_cur_dev() == DEVICE_SDMMC) {
        index = listbox(0, sizeof(tbl_music_menu1)>>1, NULL, (u16)tbl_music_menu1, LIST_TIME);
    }else {
        index = listbox(0, sizeof(tbl_music_menu2)>>1, NULL, (u16)tbl_music_menu2, LIST_TIME);
    }
    switch (index) {
#if FILE_NAV_EN
    case ITEM_MUSIC_NAV:
        music_stop();   // 音乐停止后再弄，这样不会造成出错
        f_index = filelist(t_msc.music_num, FL_CDUP);
        if (f_index == 0) {
            t_msc.file_change = 1;              // 重新打开
            t_msc.file_prev_next = 0;           // 清除方向
#if BREAK_POINT_EN
            t_msc.break_flag = 1;               // 跳到断点继续播放
#endif
            task_ctl.work_sta = last_work_sta;     // 直接break会退出到menu模式
            music_play();
            break;
        }
        t_msc.music_num = f_index;                //打开播放音乐
        t_msc.file_change = 1;
        t_msc.file_prev_next = 0;               //清除方向
        music_play();
        break;
#endif
    case ITEM_MUSIC_EQ:
        index = listbox(sys_ctl.eq_num, MUSIC_EQ_ITEM_NUM, LB_EQ, (u16)tbl_music_eq, LIST_TIME);
        user_set_eq(index);
        music_eq(sys_ctl.eq_num);
        break;
    case ITEM_MUSIC_PLAYMODE:
        index = listbox(sys_ctl.mode_play, PLAYMODE_ITEMS_NUM, NULL, (u16)tbl_playmode_items, LIST_TIME);
        user_set_mode_play(index);
        break;
    case ITEM_DEL_FILE:
        if (msgbox(STR_DELETE_CONFIRM, MB_YESNO, 0)) {
            //确认删除对话框
            music_stop();
            if (f_delete(t_msc.music_num)) {
                t_msc.music_total -=1;
                if (t_msc.music_num ==0) {
                    t_msc.music_num = t_msc.music_total;
                } else {
                    t_msc.music_num--;            //最后一项
                }
                t_msc.file_change = 1;
                if (t_msc.music_total == 0) {
                    msgbox(STR_NO_MUSIC, NULL, MSGBOX_TIME);
                    task_ctl.work_sta = TASK_EXIT;
                }
            }else {
                msgbox(STR_DELETE_FAIL, NULL, MSGBOX_TIME);
            }
        }
        break;
    case ITEM_DEL_ALL:
        if (msgbox(STR_DELETEALL_CONFIRM, MB_YESNO, 0)) {
            music_stop();
            while(t_msc.music_total) {
                if (f_delete(1)) {
                    t_msc.music_total--;
                }else {
                    msgbox(STR_DELETE_FAIL, NULL, MSGBOX_TIME);
                    break;
                }
            }
            if (t_msc.music_total == 0) {
                msgbox(STR_NO_MUSIC, NULL, MSGBOX_TIME);
                task_ctl.work_sta = TASK_EXIT;
            }
            WATCHDOG_CLR();
        }
        break;
    case ITEM_MENU_BACK:
        task_ctl.work_sta = TASK_EXIT;      //退出
        break;
    case ITEM_SELECT_DEVICE:
        task_music_device_select();
        break;
#if AB_REPEAT_EN
    case ITEM_REPEAT:
        t_msc.repeat_flag = 1;
        break;
#endif
    }
    t_msc.disp_flag = 1;
}

#pragma location="TASK_MUSIC_MENU_SEG"
void tasksub_music_menu_enter(void)
{

}

#pragma location="TASK_MUSIC_MENU_SEG"
void tasksub_music_menu_exit(void)
{

}

#pragma location="TASK_MUSIC_MENU_SEG"
void tasksub_music_menu_event(void)
{

}

#pragma location="TASK_MUSIC_MENU_SEG"
void task_music_menu_deal_msg(u8 msg)
{

}

#pragma location="TASK_MUSIC_MENU_SEG"
void tasksub_music_menu_loop(void)
{
    task_music_menu();
}

#pragma location="TASK_MUSIC_MENU_SEG"
void tasksub_music_menu(void)
{
   // printf("tasksub_music_menu\n");
    tasksub_music_menu_enter();
    tasksub_music_menu_loop();
    tasksub_music_menu_exit();
    //printf("tasksub_music_menu exit\n");
}



#endif
