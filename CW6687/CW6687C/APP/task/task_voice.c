/*****************************************************************************
 * Module    : Task
 * File      : task_voice.c
 * Author    : Hanny
 * Function  : ����¼���ļ�
 *****************************************************************************/
#include "include.h"
#if TASK_VOICE_EN
#include "task.h"
#include "task_music.h"

#define DEBUG_TASK_VOICE            0

extern type_task_music_ctl t_msc;

void task_music_event(void);
void task_music_deal_msg(u8);
void task_music_display(void);
void task_music_exit(void);
void task_voice_numget(void);

void task_voice_numget(void)
{
    t_msc.music_num = sys_param_read16(PARAM_VOICE_NUM);        //�򿪵��ļ����
    if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
        t_msc.music_num = 1;
    }
}

//���������ʼ��
void task_voice_enter(void)
{
    led_mode_off();
    led_idle();
    set_sys_clk(SYS_48M);
    draw_wait();
#if FIRST_PLAY_UDISK                                  //2014-1-2, ����U������ʱ��������ģʽ����SD���Բ�U��
    if(device_is_actived(DEVICE_SDMMC) && device_is_actived(DEVICE_UDISK)) { //U�̺�SD��ͬʱ����ʱ���Ȳ���U��
        if(fs_cur_dev() != DEVICE_UDISK){
            fs_sel(DEVICE_UDISK);
        }
    }
#endif

 /*   if (!device_is_actived(DEVICE_SDMMC)) {
        if (!device_is_actived(DEVICE_UDISK))
        {
            msgbox(STR_NO_DEVICE, NULL, MSGBOX_TIME);
            task_ctl.work_sta = TASK_EXIT;      //�豸��Ч���������˵�
            return;
        }
    } */
    draw_wait();
    //��������
    task_ctl.work_sta = TASK_VOICE;
    memset(&t_msc,0,sizeof(t_msc));
    //������Ϣ��ʼ��
    t_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
    t_msc.disp_flag = 1; //��ʾ����
    t_msc.music_voice_flag = 1;
    //sound_ctl_init(3, 0x800, 25, 0x800);     //����3�γ���0x800��Ϊ��ʼ����������25��0x800��Ϊ����
    //����ɨ�������
    user_change_volume(sys_ctl.volume);//��������
    dac_enable();
    f_scan_set(SCAN_SUB_FOLDER|SCAN_SPEED, &file_filter_voice, &dir_filter_voice);
    if(!music_mount_device()) {
        msgbox(STR_NO_VOICE, NULL, MSGBOX_TIME);
        task_ctl.work_sta = TASK_EXIT;          //�豸��Ч���˳�
        return;
    }
#if WARNING_TASK_VOICE
    if (work_sta_interrupt == TASK_NULL) {
        mp3_res_play_wait(RES_MP3_PLAY_RECORDING);
    }
#endif
    work_sta_interrupt = TASK_NULL;
    //��ȡ����������
    task_voice_numget();
    //��ʼ����
    ocx_init(task_music_event, NULL);       //����OCX��Ҫ��Ӧ����Ϣ���¼�
    dac_dynamic(DYNAMIC_NO);
}


//�����˳�
void task_voice_exit(void)
{
    music_stop();                           //����Music����
    sys_unmute();
    dac_disable();
}

//����������
//#pragma location="TASK_MUSIC_SEG"
void task_voice(void)
{
    u8 msg;
#if DEBUG_TASK_VOICE
    printf("task_voice\n");
#endif
    task_voice_enter();
    while (task_ctl.work_sta == TASK_VOICE) {
#if IS_LCD_DISPLAY
        task_music_event();
        while(msg = get_msg()) { // lcd��ʾ��ʱ̫���ȼ��д�����Ϣ�������������ʾ
            task_music_deal_msg(msg);
        }
        task_music_display();
#else
        msg = get_msg();
        task_music_event();
        task_music_deal_msg(msg);
        task_music_display();
#endif
    }
    task_voice_exit();
}

#endif
