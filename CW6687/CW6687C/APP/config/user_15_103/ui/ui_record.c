/*****************************************************************************
* Module    : UI
* File      : ui_record.c
* Author    : Hanny
* Function  : 录音任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//录音任务显示处理
#if IS_LEDSEG_7PIN_DISPLAY
#pragma location="TASK_MIC_SEG"
void task_record_display(void)
{
    ledseg_ocx_event();
    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        if (task_ctl.work_sta == TASK_MIC) {
            ledseg_disp(MENU_MIC);
        } else if (task_ctl.work_sta == TASK_AUX) {
            ledseg_disp(MENU_AUX);
        }
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

//录音中任务显示处理
#pragma location="TASK_RECORD_SEG"
void tasksub_record_display(void)
{
    //ledseg_ocx_event();
    if (ledseg_ocx.show_time && ocx_show_cnt >= ledseg_ocx.show_time) {
        ledseg_ocx.status = LEDSEG_OCX_NULL;
    }
    if (ledseg_ocx.status != ledseg_ocx.disp_status) {
        ledseg_ocx.disp_status = ledseg_ocx.status;
    }

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_RECORDING);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#elif IS_LCD_DISPLAY
#pragma location="TASK_MIC_SEG"
void task_record_display(void)
{
    u8 hour, min, sec;
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_rec.updata_flag = 1;             //设置更新标志
    }
    if (!t_rec.updata_flag) {
        return;
    }
    t_rec.updata_flag = 0;
/*
    // 清除"请稍候"字样
    line_clear();
    line_update(2);
    line_update(4);
*/
    // 画麦克风大图标
    if(task_ctl.work_sta == TASK_AUX || task_ctl.work_sta == TASK_EXIT){//第二个条件用于插入LINEIN线马上拔出后的显示
        draw_img(RES_RECORD_LINEIN_BMP, 3, 3);
    }else{
        draw_img(RES_RECORD_REC0_BMP, 45, 3);
    }
    line_clear();
    //显示播放状态
    if (t_rec.rec_sta == REC_STA_STOP) {
        disp_draw(RES_ICON_STOP_BMP, 0, 0);
    }else if (t_rec.rec_sta == REC_STA_PAUSE) {
        disp_draw(RES_STATUS1_BMP, 0, 0);
    }else if (t_rec.rec_sta == REC_STA_RECORD) {
        disp_draw(RES_ICON_REC_BMP, 0, 0);
    }

    //显示时间
    hour = t_rec.rec_time / 3600;
    min = (t_rec.rec_time / 60) % 60;
    sec = t_rec.rec_time % 60;
    disp_medium_time(hour, min, sec, 45, 0);
    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    line_update(0);
}

#pragma location="TASK_RECORD_SEG"
void tasksub_record_display(void)
{
    u8 hour, min, sec;
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_rec.updata_flag = 1;             //设置更新标志
    }
    if (!t_rec.updata_flag) {
        if(ocx_show_cnt < 5) {
            return;
        }
    }
    t_rec.updata_flag = 0;

    line_clear();
    //显示播放状态
    if(ocx_show_cnt >= 5)
    {
        t_rec.disp_sta = ~t_rec.disp_sta;
        ocx_show_cnt = 0;
    }
    if(!t_rec.disp_sta)
    {
        disp_draw(RES_ICON_REC_BMP, 0, 0);
    }

    //显示时间
    hour = t_rec.rec_time / 3600;
    min = (t_rec.rec_time / 60) % 60;
    sec = t_rec.rec_time % 60;
    disp_medium_time(hour, min, sec, 45, 0);
    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    line_update(0);
}
#else
#pragma location="TASK_MIC_SEG"
void task_record_display(void)
{
}
#pragma location="TASK_RECORD_SEG"
void tasksub_record_display(void)
{
}
#endif

//录音任务消息处理
#pragma location="TASK_MIC_SEG"
void task_record_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1) \
       && (task_ctl.work_sta == TASK_AUX)) //恢复以前的声音
    {
        aux_continue();
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        flush_msg();
        return;
    }
#endif
    switch(msg) {
        //开始录音
    case KL_VOL_UP:
    case KL_VOL_DOWN:
        if (!t_rec.aux_record_flag && (task_ctl.work_sta != TASK_MIC)) {
            break;
        }
        if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
            tasksub_recording();
            if(t_rec.play_flag) {
#if (RECORD_FILE_NAME == SHORT_NAME)
                filename[FILE_NAME_FLAG] |= FB_GBK;     //filename没有重新读，短文件名要置位GBK标志
#endif
                tasksub_recplay();//播放刚刚录下的文件
                task_record_init();
#if IS_LCD_DISPLAY
                t_rec.updata_flag = 1;      // 设置更新标志
#endif
                // 清除录音文件名及时间显示
                line_clear();
                line_update(2);
                line_update(4);
            }
        } else {
            show_no_device();
        }
        break;
   case KU_PLAY:
        if(task_ctl.work_sta == TASK_MIC || (task_ctl.work_sta == TASK_AUX && t_rec.rec_sta == REC_STA_PAUSE)){
            if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
                tasksub_recording();
                if(t_rec.play_flag) {
#if (RECORD_FILE_NAME == SHORT_NAME)
                    filename[FILE_NAME_FLAG] |= FB_GBK;     //filename没有重新读，短文件名要置位GBK标志
#endif
                    tasksub_recplay();//播放刚刚录下的文件
                    task_record_init();
#if IS_LCD_DISPLAY
                    t_rec.updata_flag = 1;      // 设置更新标志
#endif
                    // 清除录音文件名及时间显示
                    line_clear();
                    line_update(2);
                    line_update(4);
                }
            } else {
                show_no_device();
            }
        }else{
            aux_mute_control();
        }
        break;

    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;

    case KU_VOL_UP:
    case KU_VOL_DOWN:
        if (task_ctl.work_sta == TASK_AUX) {
            show_volume();
            t_rec.updata_flag = 1;
        }
        break;

    case KU_PLAY_POWER:
        if (t_rec.rec_sta == REC_STA_PAUSE) {
                t_rec.play_flag = 1;            //结束录音，播放刚刚录下的文件
                t_rec.rec_sta = REC_STA_STOP;
            }
        if(t_rec.play_flag) {
            task_record_stop();
#if (RECORD_FILE_NAME == SHORT_NAME)
            filename[FILE_NAME_FLAG] |= FB_GBK;     //filename没有重新读，短文件名要置位GBK标志
#endif
            tasksub_recplay();//播放刚刚录下的文件
            task_record_init();
#if IS_LCD_DISPLAY
            t_rec.updata_flag = 1;      // 设置更新标志
#endif
            // 清除录音文件名及时间显示
            line_clear();
            line_update(2);
            line_update(4);
        }
        break;

        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}

//正在录音中任务消息处理
#pragma location="TASK_RECORD_SEG"
void tasksub_record_deal_msg(u8 msg)
{
    switch(msg) {
        //暂停录音
    case KU_PLAY:
        task_record_pause();
        break;

        //停止录音
    case KU_MENU:
    case KU_PLAY_POWER:
        t_rec.play_flag = 1;
        task_record_stop();
        break;

    case QSYSTEM_1S:
        t_rec.rec_time++;                           //录音计时
        if (t_rec.sync_cnt++ < REC_SYNC_CNT) {
            break;
        }
        t_rec.sync_cnt = 0;                     //同步
        if (!sync_file()) {
            //printf("file sync Fail!\n");
            adc_stop();
        }
        break;
        //调用公共消息函数
    default:
        break;
    }
}

//录音播放任务消息处理
#pragma location="TASK_MUSIC_SEG"
void tasksub_recplay_deal_msg(u8 msg)
{
    switch(msg) {
    case KU_PLAY:
        if (music_start_pause()) {
            t_msc.pause = 0;        // 继续播放
        } else {
            t_msc.pause = 1;        // 暂停播放
        }
        t_msc.disp_status_flag = 1; //更新显示状态

        break;

    case KU_PLAY_POWER:
        music_stop();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
        show_volume();
        t_msc.disp_flag = 1;
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_ctl.volume) {
            sys_unmute();
        }
#endif
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        show_volume();
        t_msc.disp_flag = 1;
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_ctl.volume) {
            sys_unmute();
        }
#endif
        break;
    }
}
#endif
