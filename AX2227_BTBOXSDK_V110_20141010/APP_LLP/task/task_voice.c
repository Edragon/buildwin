/*****************************************************************************
 * Module    : Task
 * File      : task_voice.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
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
void task_voice_event(void);
void task_voice_numget(void);

void task_voice_numget(void)
{
    t_msc.music_num = param_read16(PARAM_VOICE_NUM);        //�򿪵��ļ����
    if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
        t_msc.music_num = 1;
    }
}

//�����ʼ��
void task_voice_enter(void)
{
    set_sys_clk(SYS_24M);

    //������Ϣ��ʼ��
    memset(&t_msc, 0x00, sizeof(t_msc));
    t_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
    u_msc.music_voice_flag = 1;             //����¼����־
    u_msc.pause = 0;
#if K_MODE_CHANGE_DEVICE
    t_msc.mode_flag = 0;
#endif

    ocx_init(task_voice_event, NULL);       //����OCX��Ҫ��Ӧ����Ϣ���¼�
    //sound_ctl_init(3, 0x800, 25, 0x800);
    user_change_volume(sys_ctl.volume);     //��������
    dac_enable();                           //2014-3-7��Ҫ��task_music_num_getǰ��DAC������ͬʱ�����ļ�U�̺�SD����������

    //����ɨ�������
    f_scan_set(SCAN_SUB_FOLDER, &file_filter_voice, &dir_filter_voice);
    //��ȡ����������

    //2014-8-4���޸����ٲ���豸�����¼������ģʽ��bug��(����Ӧ�ý������ֲ���ģʽ�������豸�������ļ�����¼���ļ�)
    //if (!music_mount_device()) {  //����¼������ģʽʱ��ȥ�����豸����ʼ���ļ�ϵͳ�ȣ�ֻ��������ģʽ�����豸����ʼ���ļ�ϵͳ
    if ((!device_is_actived(DEVICE_SDMMC) && !device_is_actived(DEVICE_UDISK)) || !fs_is_mounted() || !music_scan_file()) {
        task_ctl.work_sta = TASK_EXIT;
        return;
    }
    task_voice_numget();
    //��ʼ����
    music_unmute();
#if VOICE_MODE_WARNING_MUSIC
    mp3_res_play_wait(RES_MP3_VOICE_MODE);
#endif
    ocx_init(task_voice_event, NULL);       //����OCX��Ҫ��Ӧ����Ϣ���¼�
}

//�����˳�
void task_voice_exit(void)
{
    music_stop();                           //����Music����
    mp3en_exit();
    dac_disable();
}

//����������
void task_voice_event(void)
{
    comm_event();
    task_music_event();
}

//������Ϣ����
void task_voice_deal_msg(u8 msg)
{
    switch(msg) {

    case KL_PLAY:
#if !KL_PLAY_S10_CHANGE_MODE
    case KL_PLAY_S10:
#endif
        music_stop();
        show_del();
        if (f_delete(t_msc.music_num)) {
            if(t_msc.music_total > 0) {
                t_msc.music_total -= 1;
            }

            if(t_msc.music_total == 0) {
                if(!music_next_device()) {
                    task_ctl.work_sta = TASK_EXIT;
                }
                break;
            }

            if (t_msc.music_num > t_msc.music_total) {
                t_msc.music_num = 1;
            }
            t_msc.file_change = 1;
        }
        break;

        //����MUSIC��Ϣ����
    default:
        task_music_deal_msg(msg);
        break;
    }
}

#pragma constseg="TASK_VOICE_CONST"
IAR_CONST char str_task_voice[] = "task_voice\n";
#pragma constseg=default

//����������
#pragma location="TASK_VOICE_SEG"
void task_voice(void)
{
    printf(str_task_voice);
    task_voice_enter();
    while (task_ctl.work_sta == TASK_VOICE) {
        task_voice_event();
        task_voice_deal_msg(get_msg());
        task_music_display();
    }
    task_voice_exit();
}

#endif
