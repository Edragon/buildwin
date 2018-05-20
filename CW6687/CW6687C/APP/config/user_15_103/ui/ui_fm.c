/*****************************************************************************
* Module    : UI
* File      : ui_fm.c
* Author    : Hanny
* Function  : FM�����û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C
#include "display_mem.h"

#define MEDIUMNUM_ICON_LINE         2   // ����ͼƬ������
#define MEDIUMNUM_ICON_WIDTH        8   // ����ͼƬ������
#define BAT_ICON_LINE               2   // ����ͼƬ������
#define BAT_ICON_WIDTH              19  // ����ͼƬ������
#define POINT_ICON_LINE             1   // ��ͷͼƬ������
#define POINT_ICON_WIDTH            8   // ��ͷͼƬ������

//������ʾ����
#if IS_LEDSEG_7PIN_DISPLAY
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_FREQ);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

//¼����������ʾ����
#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_display(void)
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

//����
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_update(u8 lcd_width, u8 lcd_height, char *line_buf, u8 len)
{
    dispmem_update(lcd_width, lcd_height, line_buf, len);
}

#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_draw(char *buf_ptr, u8 icon_width, u8 icon_line, u8 x, u8 line)
{
    u8 i;

    char *disp_ptr =&lcd_buf[x + COLUMN_MAX*line];

    if(buf_ptr == NULL)
        return;

    for (i=0; i<icon_line; i++) {
        memcpy(disp_ptr, buf_ptr, icon_width);
        buf_ptr += icon_width;
        disp_ptr += COLUMN_MAX;
    }
}

//��ʾƵ��
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_disp_freq(u16 d_freq, u8 x, u8 line)
{

    u8 num0, num1, num2, num3;
    if (d_freq > 9999) {
        d_freq = 9999;
    }
    num0 = d_freq / 1000;
    num1 = (d_freq / 100) % 10;
    num2 = (d_freq / 10) % 10;
    num3 = d_freq % 10;
    if (num0) {
        task_fm_draw(&lcd_buf[128*5 + num0*MEDIUMNUM_ICON_WIDTH*2], MEDIUMNUM_ICON_WIDTH, MEDIUMNUM_ICON_LINE, x, line);
    }
    if (num0 || num1) {
        task_fm_draw(&lcd_buf[128*5 + num1*MEDIUMNUM_ICON_WIDTH*2], MEDIUMNUM_ICON_WIDTH, MEDIUMNUM_ICON_LINE, x+7, line);
    }
    task_fm_draw(&lcd_buf[128*5 + num2*MEDIUMNUM_ICON_WIDTH*2], MEDIUMNUM_ICON_WIDTH, MEDIUMNUM_ICON_LINE, x+14, line);

    lcd_buf[x+128+23] = 0x18;
    lcd_buf[x+128+24] = 0x18;

    task_fm_draw(&lcd_buf[128*5 + num3*MEDIUMNUM_ICON_WIDTH*2], MEDIUMNUM_ICON_WIDTH, MEDIUMNUM_ICON_LINE, x+26, line);

}

//��ʾƵ��
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_dispmem_ch_num(u8 num, u8 x, u8 line)
{
    u8 num0, num1;
    if (num > 99) {
        num = 99;
    }
    num0 = num / 10;
    num1 = num % 10;
    task_fm_draw(&lcd_buf[128*5 + num0*MEDIUMNUM_ICON_WIDTH*2], MEDIUMNUM_ICON_WIDTH, MEDIUMNUM_ICON_LINE, x-1, line);
    task_fm_draw(&lcd_buf[128*5 + num1*MEDIUMNUM_ICON_WIDTH*2], MEDIUMNUM_ICON_WIDTH, MEDIUMNUM_ICON_LINE, x-1+7, line);

    task_fm_update(x, line, lcd_buf+x, 14);
    task_fm_update(x, line+1, lcd_buf+x+128, 14);
}

//Ƶ����ʾ����
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display_freq(void)
{
    u16 d_freq = t_fm.freq / 10;
    if (t_fm.disp_freq != d_freq) {
        t_fm.disp_freq = d_freq;
        t_fm.disp_flag = 1;
    }
    if (!t_fm.disp_flag) {
        return;
    }
    memset(lcd_buf, 0, 256);
    t_fm.disp_flag = 0;

    //��ʾƵ��
    task_fm_disp_freq(d_freq, 0, 0);
    task_fm_update(50, 2, lcd_buf, 68);
    task_fm_update(50, 3, lcd_buf+128, 68);
    memset(lcd_buf, 0, 256);
    task_fm_update(0, 4, lcd_buf, 128);
    task_fm_draw(&lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*10+BAT_ICON_WIDTH*2*5], POINT_ICON_WIDTH, POINT_ICON_LINE, 0, 0);

    d_freq = t_fm.disp_freq-861;
    task_fm_update(d_freq/2, 4, lcd_buf, 50);
}


//Ƶ����ʾ����
#pragma location="TASK_FMPLAY_DISP_SEG"
__root __near_func void task_fm_display_ch(void)
{
    if (t_fm.disp_ch_sel != t_fm.ch_sel) {
        t_fm.disp_ch_sel = t_fm.ch_sel;
        t_fm.disp_ch_flag = 1;
    }
    if (t_fm.disp_ch_cnt != t_fm.ch_cnt) {
        t_fm.disp_ch_cnt = t_fm.ch_cnt;
        t_fm.disp_ch_flag = 1;
    }
    if (!t_fm.disp_ch_flag) {
        return;
    }
    t_fm.disp_ch_flag = 0;

    memset(lcd_buf, 0, 256);
    //��ʾƵ��
    task_fm_dispmem_ch_num(t_fm.disp_ch_sel, 50, 0);
    task_fm_dispmem_ch_num(t_fm.disp_ch_cnt, 71, 0);
}

//�����ʾ����
#pragma location="TASK_FM_DISP"
void task_fm_display_val(void)
{
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        //task_fm_dispmem_draw(t_fm.bat_buf + (sys_ctl.bat_val - 1) * t_fm.bat_len, 0, 0);
        if (sys_ctl.bat_val >= 1) {
            //disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 0, 0);
            task_fm_draw(&lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*10+BAT_ICON_WIDTH*2*(sys_ctl.bat_val-1)], BAT_ICON_WIDTH, BAT_ICON_LINE, 0, 0);
        }
        task_fm_update(108, 0, lcd_buf, 19);
        task_fm_update(108, 1, lcd_buf+128, 19);
    }
}
void tasksub_fm_record_display();
#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display(void)
{
    if (t_fm.fm_sta != FM_REC_PAUSE) {
    task_fm_display_ch();
    task_fm_display_freq();
    task_fm_display_val();
    }
    else{
        tasksub_fm_record_display();
    }
}
#if TASK_FM_EN
#pragma location="TASK_FM_DISP"
void task_fm_disp_init(void)
{
    line_clear();
    disp_draw(RES_ICON_STOP_BMP, 0, 0);
    disp_menu(STR_FM_CHANNEL, 20, 0, 0, 0);
    disp_text((char *)"/", 65, 0, 0);
    line_update_clear(0);
    line_update_clear(2);
    disp_draw(RES_OCX_FMQT_BMP, 0, 0);
    line_update_clear(5);
    line_update(6);
    t_fm.disp_flag = 1;
    t_fm.disp_ch_flag = 1;

    // �����ֵ�ͼƬ����ram��
    picture_read(RES_MEDIUMNUM_0_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*0]);
    picture_read(RES_MEDIUMNUM_1_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*1]);
    picture_read(RES_MEDIUMNUM_2_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*2]);
    picture_read(RES_MEDIUMNUM_3_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*3]);
    picture_read(RES_MEDIUMNUM_4_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*4]);
    picture_read(RES_MEDIUMNUM_5_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*5]);
    picture_read(RES_MEDIUMNUM_6_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*6]);
    picture_read(RES_MEDIUMNUM_7_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*7]);
    picture_read(RES_MEDIUMNUM_8_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*8]);
    picture_read(RES_MEDIUMNUM_9_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*9]);
    // �ѵ���ͼƬ����ram��
    picture_read(RES_ICON_BAT0_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*10+BAT_ICON_WIDTH*2*0]);
    picture_read(RES_ICON_BAT1_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*10+BAT_ICON_WIDTH*2*1]);
    picture_read(RES_ICON_BAT2_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*10+BAT_ICON_WIDTH*2*2]);
    picture_read(RES_ICON_BAT3_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*10+BAT_ICON_WIDTH*2*3]);
    picture_read(RES_ICON_BAT4_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*10+BAT_ICON_WIDTH*2*4]);
    // ��Ƶ�ʼ�ͷ����ram��
    picture_read(RES_OCX_FMG_BMP, &lcd_buf[128*5+MEDIUMNUM_ICON_WIDTH*2*10+BAT_ICON_WIDTH*2*5]);
}
#endif
/*****************************************************************************
 * Module    : Ԥװ���ݣ��Ա��RAM��������Դ
 *****************************************************************************/
/*
#pragma location="TASK_FM_DISP"
void task_fm_dispmem_init(void)
{
    dispmem_init(disp_mem_buf, 256);
    t_fm.colon = dispmem_add(RES_MEDIUMNUM_COLON_BMP);
    t_fm.num_buf = dispmem_addnum(RES_MEDIUMNUM_0_BMP,10);
    t_fm.num_len = dispmem_ctl.num_len;
    t_fm.rec_icon = dispmem_add(RES_ICON_REC_BMP);
}

#pragma location="TASK_FM_DISP"
void task_fm_play_dispmem_init(void)
{
    dispmem_init(fm_dispmem_buf, sizeof(fm_dispmem_buf));                   //��¼��ʱ�����˶���Ϊ�����Դ�
    t_fm.bat_buf = dispmem_addnum(RES_ICON_BAT0_BMP,5);
    t_fm.bat_len = dispmem_ctl.num_len;
    t_fm.point_buf = dispmem_add(RES_OCX_FMG_BMP);//RES_OCX_FMG_BMP);
}
*/

/*
#pragma location="TASK_FM_DISP"
void task_fm_list_dispmem_init(void)
{
    task_fm_display_init();
    task_fm_dispmem_draw(t_fm.bat_buf + (sys_ctl.bat_val - 1) * t_fm.bat_len, 0, 0);
    task_fm_update(108, 0, lcd_buf, 19);
    task_fm_update(108, 1, lcd_buf+128, 19);
}
*/

#pragma location="TASK_FM_DISP"
void task_fm_disp_clear(void)
{
}
#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_display(void)
{
    u8 hour, min, sec;
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_fm_rec.updata_flag = 1;             //���ø��±�־
    }
    if (!t_fm_rec.updata_flag) {
        if(ocx_show_cnt < 5) {
            return;
        }
    }
    t_fm_rec.updata_flag = 0;

    line_clear();
    //��ʾ����״̬
    if(ocx_show_cnt >= 5)
    {
        t_fm_rec.disp_sta = ~t_fm_rec.disp_sta;
        ocx_show_cnt = 0;
    }
    if(!t_fm_rec.disp_sta)
    {
        disp_draw(RES_ICON_REC_BMP, 0, 0);
    }
        //��ʾ����״̬
    if (t_fm.fm_sta == FM_REC_PAUSE) {
        disp_draw(RES_STATUS1_BMP, 0, 0);
    }
    //��ʾʱ��
    hour = t_fm_rec.rec_time / 3600;
    min = (t_fm_rec.rec_time / 60) % 60;
    sec = t_fm_rec.rec_time % 60;
    disp_medium_time(hour, min, sec, 45, 0);
    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    line_update(0);
}
#else

#pragma location="TASK_FMPLAY_DISP_SEG"
__near_func void task_fm_display(void)
{
}

#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_display(void)
{
}
#endif

//������Ϣ����
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1)){
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        fm_play_continue();
        flush_msg();
        return;
    }
#endif

    switch(msg) {
    case KL_PLAY_POWER:
        //task_record_setting();
        task_ctl.work_sta = TASK_MENU;              //�������˵�
        break;

        //�̰��˵���������Ŀ¼����
    case KU_MENU:
    case KU_PLAY_POWER:
        if(t_fm.seek_sta == SEEK_AUTO)
        {
            break;
        }else if(t_fm.fm_sta == FM_REC_PAUSE){
            t_fm_rec.play_flag = 1;
            task_fm_record_stop();
            if(t_fm_rec.play_flag) {
#if (RECORD_FILE_NAME == SHORT_NAME)
                filename[FILE_NAME_FLAG] |= FB_GBK;     //filenameû�����¶������ļ���Ҫ��λGBK��־
#endif
                tasksub_fm_recplay();//���Ÿո�¼�µ��ļ�
#if IS_LCD_DISPLAY
                t_fm_rec.updata_flag = 1;      // ���ø��±�־
                // ���¼���ļ�����ʱ����ʾ
                line_clear();
                line_update(2);
                line_update(4);
                task_fm_disp_init();
#endif
                }
        }
        else{
            task_fm_list();
        }
        break;

        //��������
    case KU_VOL_DOWN:
    case KU_VOL_UP:
        if(t_fm.fm_sta == FM_PLAY)
        {
            task_fm_volume_set();
            sys_ctl.disp_bat_val = 0;
        }
        break;

    case KL_PLAY:
        if(t_fm.seek_sta == SEEK_AUTO){
            task_fm_auto_stop();        //ֹͣ�Զ���̨
            task_fm_auto_play();
            led_fm_play();
        } else {
            task_fm_auto_start();       //��ʼ�Զ���̨
            t_fm.seek_sta = SEEK_AUTO;
            sys_ctl.sleep_cnt = 0xffff;
            led_fm_scan();
        }
        break;

    case KU_PLAY:   //����̨ʱ,ֹͣ��̨. �ڲ���ʱΪPP
        if(t_fm.seek_sta == SEEK_AUTO){
            task_fm_auto_stop();        //ֹͣ�Զ���̨
            task_fm_auto_play();
        }else if(t_fm.fm_sta == FM_REC_PAUSE){
            if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
                tasksub_fm_record();
                if(t_fm_rec.play_flag) {
#if (RECORD_FILE_NAME == SHORT_NAME)
                filename[FILE_NAME_FLAG] |= FB_GBK;     //filenameû�����¶������ļ���Ҫ��λGBK��־
#endif
                    tasksub_fm_recplay();//���Ÿո�¼�µ��ļ�
#if IS_LCD_DISPLAY
                    t_fm_rec.updata_flag = 1;      // ���ø��±�־
                    // ���¼���ļ�����ʱ����ʾ
                    line_clear();
                    line_update(2);
                    line_update(4);
                    task_fm_disp_init();
#endif
                }
            } else {
                show_no_device();
            }
        }else {  //����/��ͣ
            if(t_fm.mute && sys_ctl.volume){     //����
                t_fm.mute = 0;
                sys_ctl.sleep_cnt = 0xffff;
#if FM_VOUT_THROUGH_MCU
                sys_unmute();
                amux_play();
#else
                fm_set_vol(sys_ctl.volume);
                speaker_unmute();
#endif
            }
            else{               //��ͣ
                t_fm.mute = 1;
                sys_ctl.sleep_cnt = STANDBY_TIME;
#if FM_VOUT_THROUGH_MCU
                amux_stop();
#else
                speaker_mute();
                fm_set_vol(0);
#endif
            }
        }
        break;
        //��һ������һ��
    case KU_PREV:
    case KU_PREV_VOL_DOWN:
        if(t_fm.seek_sta == SEEK_STOP) {
            task_fm_prevch();
            sys_param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
            sys_param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
        }
        //printf("channel %d: %dMHz\n",t_fm.ch_sel,t_fm.freq);
        sys_ctl.sleep_cnt = 0xffff;
        break;

    case KU_NEXT:
    case KU_NEXT_VOL_UP:
        if(t_fm.seek_sta == SEEK_STOP) {
            task_fm_nextch();
            sys_param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
            sys_param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
        }
        //printf("t_fm.freq: %d\n",t_fm.freq);
        //printf("channel %d: %dMHz\n",t_fm.ch_sel,t_fm.freq);
        sys_ctl.sleep_cnt = 0xffff;
        break;

        //��������
/*    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
#if FM_SET_VOL_CHANGE_BANK
        user_set_volume(user_volume_dec(sys_ctl.volume));
#else
        user_set_volume_fm(user_volume_dec_fm(sys_ctl.volume));
#endif
        show_volume();
#if WARNING_VOL_MAXMIN
        if(!sys_ctl.volume){
            fm_play_vol_music();
            t_fm.mute = 1;
        }
#endif
        if(sys_ctl.volume){
            t_fm.mute = 0;
            sys_ctl.sleep_cnt = 0xffff;
#if FM_VOUT_THROUGH_MCU
            sys_unmute();
            amux_play();
#else
            fm_set_vol(sys_ctl.volume);
            speaker_unmute();
#endif
        }
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
#if FM_SET_VOL_CHANGE_BANK
        user_set_volume(user_volume_inc(sys_ctl.volume));
#else
        user_set_volume_fm(user_volume_inc_fm(sys_ctl.volume));
#endif
        show_volume();
#if WARNING_VOL_MAXMIN
        if(sys_ctl.volume == VOLUME_MAX){
            fm_play_vol_music();
        }
#endif
        t_fm.mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
#if FM_VOUT_THROUGH_MCU
        sys_unmute();
        amux_play();
#else
        fm_set_vol(sys_ctl.volume);
        speaker_unmute();
#endif
        break;*/

#if FM_RECORD_EN
        //��ʼ¼��
    case KL_VOL_DOWN:
    case KL_VOL_UP:
        if(t_fm.seek_sta == SEEK_AUTO){     //�Զ���̨ʱ������¼��
            break;
        }
        if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
            tasksub_fm_record();
            if(t_fm_rec.play_flag) {
#if (RECORD_FILE_NAME == SHORT_NAME)
                filename[FILE_NAME_FLAG] |= FB_GBK;     //filenameû�����¶������ļ���Ҫ��λGBK��־
#endif
                tasksub_fm_recplay();//���Ÿո�¼�µ��ļ�
#if IS_LCD_DISPLAY
            // ���¼���ļ�����ʱ����ʾ
            t_rec.updata_flag = 1;      // ���ø��±�־
            line_clear();
            line_update(2);
            line_update(4);
            task_fm_disp_init();
#endif
            }
        } else {
            show_no_device();
        }
        break;
#endif

#if (HARD_IR_EN || SOFT_IR_EN)
    case T_KEY_NUM_0:
    case T_KEY_NUM_1:
    case T_KEY_NUM_2:
    case T_KEY_NUM_3:
    case T_KEY_NUM_4:
    case T_KEY_NUM_5:
    case T_KEY_NUM_6:
    case T_KEY_NUM_7:
    case T_KEY_NUM_8:
    case T_KEY_NUM_9:
        task_fm_select_freq_chan(msg - T_KEY_NUM_0);
        break;
    case KL_SCAN:
    case KU_SCAN:
        if(t_fm.seek_sta == SEEK_AUTO){ //�̰�������SCAN������ֹͣ��̨
            task_fm_auto_stop();        //ֹͣ�Զ���̨
            task_fm_auto_play();
            sys_ctl.sleep_cnt = 0xffff;
        }
        else if(msg == KL_SCAN){        //����SCAN����ʼ��̨
            task_fm_auto_start();       //��ʼ�Զ���̨
            t_fm.seek_sta = SEEK_AUTO;
        }
        break;
#endif
    default:
        deal_msg(msg);
        break;
    }
}

//����¼����������Ϣ����
#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_deal_msg(u8 msg)
{
    switch(msg) {
        //��ͣ¼��
    case KU_PLAY:
        task_fm_record_pause();
        break;

        //ֹͣ¼��
    case KU_MENU:
    case KU_PLAY_POWER:
        t_fm_rec.play_flag = 1;
        task_fm_record_stop();
        break;

    case QSYSTEM_1S:
        task_fm_record_sync();
        break;
        //���ù�����Ϣ����
    default:
        break;
    }
}

//¼������������Ϣ����
#pragma location="TASK_MUSIC_SEG_1"
void tasksub_fm_recplay_deal_msg(u8 msg)
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
    //case KU_MODE:
        music_stop();
        break;

    case K_VOL_UP:
   // case KL_VOL_UP:
    //case KH_VOL_UP:
    //case KH_NEXT_VOL_UP:
        show_volume();
        t_msc.disp_flag = 1;
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_ctl.volume) {
            sys_unmute();
        }
#endif
        break;

    case K_VOL_DOWN:
    //case KL_VOL_DOWN:
    //case KH_VOL_DOWN:
   // case KH_PREV_VOL_DOWN:
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
