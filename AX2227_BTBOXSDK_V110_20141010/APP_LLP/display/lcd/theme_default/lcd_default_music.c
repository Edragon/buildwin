/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_music.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ���ֲ���������ʾ����
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_MUSIC == LCD_THEME_DEFAULT)
#include "task.h"
#include "task_music.h"
extern u8 id3_disp_buf[];

//��ʾ�ļ���
#pragma location="TASK_MUSIC_SEG_2"
void task_music_disp_filename(void)
{
    if (t_msc.filename_change) {
        t_msc.filename_change = 0;
#if MUSIC_ID3_EN
        move_ctl.name_wide=name_width((char *)id3_disp_buf, 0);//��ȡ�ļ�������ʾ����
#else
        move_ctl.name_wide=name_width((char *)filename, filename[FILE_NAME_FLAG]&FB_GBK);//��ȡ�ļ�������ʾ����
#endif
        if(move_ctl.name_wide>128)
        {
            move_ctl.move_flag=1;
        }
        else
            move_ctl.move_flag=0;
        move_ctl.name_x=0;
    }
    if(move_ctl.ms_flag==1)
    {
        move_ctl.ms_flag=0;
        move_ctl.ms3_flag++;
        if(move_ctl.ms3_flag>=3)
        {
            move_ctl.ms3_flag=0;
            if(move_ctl.move_flag)
            {
                if(move_ctl.name_x+(s16)move_ctl.name_wide>106)
                {
                    move_ctl.name_x--;
                }
                else
                {
                    move_ctl.name_x=0;
                }
            }
            line_clear();
#if MUSIC_ID3_EN
            disp_text_u((char *)id3_disp_buf, move_ctl.name_x, 0, 1);
            line_update_clear(4);
#else
            if (filename[FILE_NAME_FLAG] & FB_GBK) {
                  disp_text((char *)filename, move_ctl.name_x, 0, 1);
              } else {
                    disp_text_u((char *)filename, move_ctl.name_x, 0, 1);
              }
            line_update_clear(4);
#if MUSIC_LRC_EN
            if (t_msc.lrc_flag) {
                line_update(6);
            }
#endif
#endif
        }
    }
}

#if MUSIC_FREQ_SHOW
//��ʾƵ��
#pragma location="TASK_MUSIC_SEG_2"
void task_music_disp_freq(void)
{
    if (!freq_show_update) {
        return;
    }
    freq_show_update = 0;
#if MUSIC_LRC_EN
    if(!t_msc.lrc_flag)
#endif
    {      //�и��ʱ����ʾƵ��
        if (music_freq_get()) {
            line_clear();
            disp_freq_line(0);
            music_freq_kick();
            line_update(6);
        }
    }
}
#endif
#if MUSIC_LRC_EN
//��ʾ���
#pragma location="TASK_MUSIC_SEG_2"
void task_music_disp_lrc(void)
{

    char *temp;


    temp = (char *)filename;
    if((*temp == 0)&&(*(temp+1) == 0))
    {
        if(!t_msc.lrc_change)
        {
            return;
        }
    }

    if(!t_msc.lrc_change) {
        return;
    }
    t_msc.lrc_change = 0;
    line_clear();

    if(t_msc.lrc_type == TYPE_GBK)
    {
        temp = disp_text(temp, 0, 0, 0);
        line_update_clear(2);

        temp = disp_text(temp, 0, 0, 0);
        line_update_clear(4);

        disp_text(temp, 0, 0, 0);
        line_update(6);
    }
    else
    {
        temp = disp_text_u(temp, 0, 0, 0);
        line_update_clear(2);

        temp = disp_text_u(temp, 0, 0, 0);
        line_update_clear(4);

        disp_text_u(temp, 0, 0, 0);
        line_update(6);
    }
}
#endif
//��ʾ����״̬��
#pragma location="TASK_MUSIC_SEG_2"
void task_music_disp_status(void)
{
    u8 flag_icon;
    u8 disp_bat_status_flag = 0;
    u8 disp_time_status_flag = 0;	
    u32 bit_rate = 0;
    bit_rate = music_get_bitrate();	
    if(bit_rate != t_msc.bit_rate)
    {
        t_msc.disp_status_flag = 1;
    }
    //�ж��Ƿ�Ҫ����ʱ��
    if (t_msc.disp_time.min != u_msc.cur_time.min || t_msc.disp_time.sec != u_msc.cur_time.sec) {
        //printf("%02d:%02d - %02d:%02d\n", play_time.min, play_time.sec, music_time.min, music_time.sec);
        t_msc.disp_time.min = u_msc.cur_time.min;
        t_msc.disp_time.sec = u_msc.cur_time.sec;
        t_msc.disp_status_flag = 1;//���ø��±�־
    }

    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        //t_msc.disp_status_flag = 1;
        disp_bat_status_flag = 1; //���ø��±�־
    }
#if AB_REPEAT_EN
    if (t_msc.repeat_flag)
    {
        if(ocx_show_cnt >= 5)
        {
            ocx_show_cnt = 0;
            t_msc.repeat_show = ~t_msc.repeat_show;
            t_msc.disp_status_flag = 1;
        }
    }
#endif
    if ((!t_msc.disp_status_flag)&&(!disp_time_status_flag)&&(!disp_bat_status_flag)) {
        return;
    }

    if((t_msc.disp_status_flag) ||disp_bat_status_flag){
        //ȷ����Ҫ��ʾ����LCD������ʾ�������

        line_clear();	
#if TASK_KARAOKE_EN
        if (t_msc.karaoke_flag) {
            flag_icon = RES_PLAY_RECODEFA_BMP;       //����okģʽ
        }
        else
#endif
        {
            flag_icon = RES_PLAY_CP_LRC_0_BMP;       //
        }
        disp_draw(flag_icon, 1, 0);
        disp_draw(RES_PLAY_PLAY_MODE0_BMP + sys_ctl.mode_play, 16, 0);
#if AB_REPEAT_EN
        if(t_msc.repeat_flag)
        {
            if(t_msc.repeat_flag != 3)
            {
                if(t_msc.repeat_show)
                {
                    if(t_msc.repeat_flag == 1)
                    {
                        disp_draw(RES_ICON_A_BMP, 16+14, 0);
                    }
                    else
                    {
                        disp_draw(RES_ICON_B_BMP, 16+14, 0);
                    }
                }
                else
                {
                    disp_draw(RES_ICON_AB_BMP, 16+14, 0);
                }
            }
            else
            {
                disp_draw(RES_ICON_AB_BMP, 16+14, 0);
            }
        }
        else
#endif
        {
            disp_num(t_msc.music_num, 16+13, 0, DN_RIGHT | DN_ZERO | DN_SMALL | 4);
            disp_num(t_msc.music_total, 16+13, 1, DN_RIGHT | DN_ZERO | DN_SMALL | 4);
        }
        disp_draw(RES_PLAY_EQ_0_BMP + sys_ctl.eq_num, 16+13+29, 0);
        disp_draw(RES_PLAY_M_TYPE1_BMP + t_msc.music_type-1, 16+13+29+17, 0);
        disp_num(bit_rate/1000, 16+13+29+19, 0, DN_RIGHT | DN_ZERO | DN_SSMALL | 3);
        disp_draw(RES_SMALLNUM_SS_BMP, 16+13+29+17+20, 0);
        if(fs_cur_dev() == DEVICE_SDMMC )
            disp_draw( RES_PLAY_CP_SDCARD_BMP, 16+13+29+17+21, 0);
        else if(fs_cur_dev() == DEVICE_UDISK )
            disp_draw( RES_PLAY_CP_UDISK_BMP, 16+13+29+17+21, 0);
        if (sys_ctl.bat_val >= 1) {
            disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 110, 0);
        }
        line_update(0);
    }
#if MUSIC_LRC_EN
    if(t_msc.lrc_flag != LRC_FILENAME_TIME)
#endif
    {
        if((t_msc.disp_status_flag) || disp_time_status_flag){
            line_clear();
            disp_medium_time1 (u_msc.cur_time.min, u_msc.cur_time.sec, 21, 0);
            disp_draw(RES_MEDIUMNUM_CP_SPRIT_BMP, 60, 0);
            if(t_msc.disp_music_time.sec != 0xff) {
                disp_medium_time1(t_msc.disp_music_time.min, t_msc.disp_music_time.sec,67 , 0);
            }
            line_update(2);	
        }
    }
    t_msc.disp_status_flag = 0;
}

//����������ʾ����
#pragma location="TASK_MUSIC_SEG_2"
void task_music_display(void)
{
    if (t_msc.disp_flag) {
        t_msc.disp_flag = 0;
        t_msc.disp_status_flag = 1;
        t_msc.filename_change = 1;
        t_msc.lrc_change = 1;
        move_ctl.ms_flag = 1;
        move_ctl.ms3_flag = 2;
    }
    task_music_disp_status();
#if MUSIC_LRC_EN
    if(t_msc.lrc_flag != LRC_FILENAME_TIME)
#endif
    {
        task_music_disp_filename();
#if MUSIC_FREQ_SHOW
        task_music_disp_freq();
#endif
    }
#if MUSIC_LRC_EN
    else {
        task_music_disp_lrc();
    }
#endif
}
#endif