/*****************************************************************************
 * Module    : Task
 * File      : task_recplay.c
 * Author    : jingfa
 * Email     :
 * Function  : ������¼������(��Ϊtask_record��������)
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_music.h"

#if REC_PLAY_EN

void task_music_cal_time(void);
void task_music_display(void);

bool f_open_exist(void);
void task_recplay_event(void);
bool play_record_file(void)
{
    if (!f_open_exist()) {
        return false;
    }
    if (TYPE_NONE == music_init()) {
        return false;
    }
    return true;
}

//�����ʼ��
void task_recplay_enter(void)
{
    ocx_clear();
    draw_wait();
    led_music_play();
    dac_enable();
    user_change_volume(sys_ctl.volume);     //��������
    music_unmute();                         //��ʼ����
#if LCD_THEME
#if MUSIC_FREQ_SHOW
    music_freq_init(FREQ_DAC);
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
    t_msc.disp_music_time.sec = 0xff;
    t_msc.disp_flag = 1;
    t_msc.music_type = TYPE_MP3;
    t_msc.music_total = 1;
    t_msc.music_num = 1;
#endif
    //������Ϣ��ʼ��
    u_msc.pause = 0;
    if (play_record_file()) {
        music_play();
    } else {
        //printf("file Not valid!\n");
        music_stop();
    }
    ocx_init(task_recplay_event, NULL);       //����OCX��Ҫ��Ӧ����Ϣ���¼�
}

//�����˳�
void task_recplay_exit(void)
{
    music_stop();
    ocx_clear();
#if LCD_THEME
    music_freq_init(FREQ_ADC);
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
}

void task_recplay_deal_msg(u8 msg)
{
    switch(msg) {
    case KU_PLAY:
    case KU_PLAY_S10:
    case KU_MODE:
        music_stop();
        break;
#if LCD_THEME
        //�����Ӽ�
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_S10:
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_S10:
        lcd_volume_set();
        t_msc.disp_flag = 1;
        break;
#else
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_S10:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_flag.mute_flag) {
            if(sys_ctl.volume) {
                music_unmute();
                dac_fade_in(1);
            }
        }
#endif
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_S10:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_flag.mute_flag) {
            if(sys_ctl.volume) {
                music_unmute();
                dac_fade_in(1);
            }
        }
#endif
        break;
#endif
    default:
        deal_msg(msg);
        break;
    }
}

#pragma location="TASK_MUSIC_SEG"
void task_recplay_event(void)
{
    WATCHDOG_CLR();
    music_event();
    if (!device_is_actived(fs_cur_dev())) {
        music_stop();
    }

    task_music_cal_time();
#if LCD_THEME
    if (t_msc.disp_music_time.sec == 0xff) {
        u16 all_time = music_get_alltime();
        if (all_time != 0xffff) {
            t_msc.disp_music_time.min = all_time / 60;      //��ʱ��
            t_msc.disp_music_time.sec = all_time % 60;
            t_msc.disp_status_flag = 1;                     //����״̬��ʾ
        }
    }
#endif
    if((sys_ctl.sleep_cnt == 1 || sys_flag.ir_power_flag) && !sys_flag.lowpower_flag){
        music_stop();
    }
}

//����������
#pragma location="TASK_MUSIC_SEG"
void task_recplay(void)
{
    u8 work_sta = task_ctl.work_sta;
    task_recplay_enter();
    while (music_get_status() > STATUS_PLAY_STOPPING && work_sta == task_ctl.work_sta) {
        task_recplay_event();
        task_recplay_deal_msg(get_msg());
        task_music_display();
    }
    task_recplay_exit();
}

#endif
