/*****************************************************************************
* Module    : UI
* File      : ui_record.c
* Author    : Hanny
* Function  : ¼�������û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C

//¼��������ʾ����
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

//¼����������ʾ����
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
        t_rec.updata_flag = 1;             //���ø��±�־
    }
    if (!t_rec.updata_flag) {
        return;
    }
    t_rec.updata_flag = 0;
/*
    // ���"���Ժ�"����
    line_clear();
    line_update(2);
    line_update(4);
*/
    // ����˷��ͼ��
    if(task_ctl.work_sta == TASK_AUX || task_ctl.work_sta == TASK_EXIT){//�ڶ����������ڲ���LINEIN�����ϰγ������ʾ
        draw_img(RES_RECORD_LINEIN_BMP, 3, 3);
    }else{
        draw_img(RES_RECORD_REC0_BMP, 45, 3);
    }
    line_clear();
    //��ʾ����״̬
    if (t_rec.rec_sta == REC_STA_STOP) {
        disp_draw(RES_ICON_STOP_BMP, 0, 0);
    }else if (t_rec.rec_sta == REC_STA_PAUSE) {
        disp_draw(RES_STATUS1_BMP, 0, 0);
    }else if (t_rec.rec_sta == REC_STA_RECORD) {
        disp_draw(RES_ICON_REC_BMP, 0, 0);
    }

    //��ʾʱ��
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
        t_rec.updata_flag = 1;             //���ø��±�־
    }
    if (!t_rec.updata_flag) {
        if(ocx_show_cnt < 5) {
            return;
        }
    }
    t_rec.updata_flag = 0;

    line_clear();
    //��ʾ����״̬
    if(ocx_show_cnt >= 5)
    {
        t_rec.disp_sta = ~t_rec.disp_sta;
        ocx_show_cnt = 0;
    }
    if(!t_rec.disp_sta)
    {
        disp_draw(RES_ICON_REC_BMP, 0, 0);
    }

    //��ʾʱ��
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

//¼��������Ϣ����
#pragma location="TASK_MIC_SEG"
void task_record_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1) \
       && (task_ctl.work_sta == TASK_AUX)) //�ָ���ǰ������
    {
        aux_continue();
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        flush_msg();
        return;
    }
#endif
    switch(msg) {
        //��ʼ¼��
    case KL_VOL_UP:
    case KL_VOL_DOWN:
        if (!t_rec.aux_record_flag && (task_ctl.work_sta != TASK_MIC)) {
            break;
        }
        if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
            tasksub_recording();
            if(t_rec.play_flag) {
#if (RECORD_FILE_NAME == SHORT_NAME)
                filename[FILE_NAME_FLAG] |= FB_GBK;     //filenameû�����¶������ļ���Ҫ��λGBK��־
#endif
                tasksub_recplay();//���Ÿո�¼�µ��ļ�
                task_record_init();
#if IS_LCD_DISPLAY
                t_rec.updata_flag = 1;      // ���ø��±�־
#endif
                // ���¼���ļ�����ʱ����ʾ
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
                    filename[FILE_NAME_FLAG] |= FB_GBK;     //filenameû�����¶������ļ���Ҫ��λGBK��־
#endif
                    tasksub_recplay();//���Ÿո�¼�µ��ļ�
                    task_record_init();
#if IS_LCD_DISPLAY
                    t_rec.updata_flag = 1;      // ���ø��±�־
#endif
                    // ���¼���ļ�����ʱ����ʾ
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
                t_rec.play_flag = 1;            //����¼�������Ÿո�¼�µ��ļ�
                t_rec.rec_sta = REC_STA_STOP;
            }
        if(t_rec.play_flag) {
            task_record_stop();
#if (RECORD_FILE_NAME == SHORT_NAME)
            filename[FILE_NAME_FLAG] |= FB_GBK;     //filenameû�����¶������ļ���Ҫ��λGBK��־
#endif
            tasksub_recplay();//���Ÿո�¼�µ��ļ�
            task_record_init();
#if IS_LCD_DISPLAY
            t_rec.updata_flag = 1;      // ���ø��±�־
#endif
            // ���¼���ļ�����ʱ����ʾ
            line_clear();
            line_update(2);
            line_update(4);
        }
        break;

        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}

//����¼����������Ϣ����
#pragma location="TASK_RECORD_SEG"
void tasksub_record_deal_msg(u8 msg)
{
    switch(msg) {
        //��ͣ¼��
    case KU_PLAY:
        task_record_pause();
        break;

        //ֹͣ¼��
    case KU_MENU:
    case KU_PLAY_POWER:
        t_rec.play_flag = 1;
        task_record_stop();
        break;

    case QSYSTEM_1S:
        t_rec.rec_time++;                           //¼����ʱ
        if (t_rec.sync_cnt++ < REC_SYNC_CNT) {
            break;
        }
        t_rec.sync_cnt = 0;                     //ͬ��
        if (!sync_file()) {
            //printf("file sync Fail!\n");
            adc_stop();
        }
        break;
        //���ù�����Ϣ����
    default:
        break;
    }
}

//¼������������Ϣ����
#pragma location="TASK_MUSIC_SEG"
void tasksub_recplay_deal_msg(u8 msg)
{
    switch(msg) {
    case KU_PLAY:
        if (music_start_pause()) {
            t_msc.pause = 0;        // ��������
        } else {
            t_msc.pause = 1;        // ��ͣ����
        }
        t_msc.disp_status_flag = 1; //������ʾ״̬

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
