/*****************************************************************************
 * Module    : Task
 * File      : task_mic.c
 * Author    : jingfa
 * Email     :
 * Function  : MIC¼������
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_record.h"

#if TASK_MIC_EN

void task_mic_display(void);

//�����ʼ��
#pragma location="TASK_MIC_SEG"
void task_mic_enter(void)
{
    led_mode_off();
    led_idle();

    set_sys_clk(SYS_24M);
    draw_wait();
    //2014-8-4���޸����ٲ���豸���߲��뻵���豸�����¼��ģʽ��bug
    if ((!device_is_actived(DEVICE_SDMMC) && !device_is_actived(DEVICE_UDISK)) || !fs_is_mounted()) {
        task_ctl.work_sta = TASK_EXIT;      //�豸��Ч���������˵�
        return;
    }

#if MIC_RECORD_WARNING_MUSIC
    dac_enable();
    mp3_res_play_wait(RES_MP3_RECORD_MODE);
    dac_disable();
#endif

    //��ʼ��AMUXģ��
    mp3en_source_init(RECORD_MIC);
#if AUX_FREQ_SHOW
    mp3en_start(false);
    music_freq_init(FREQ_ADC);
#if LCD_THEME
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
#endif
    t_rec.update_flag = 1;
#if KEY_VOICE_EN
    dac_fade_in(0);
    key_voice_init();
#endif
}

//�����˳�
#pragma location="TASK_MIC_SEG"
void task_mic_exit(void)
{
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
    mp3en_exit();
}

#pragma location="TASK_MIC_SEG"
void task_mic_deal_msg(u8 msg)
{
    switch(msg) {
    case KU_PLAY:
    case KU_PLAY_S10:
        if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
            task_record(RECORD_MIC);
        } else {
            show_no_device();
            t_rec.update_flag = 1;
        }
        break;
        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}

#pragma location="TASK_MIC_SEG"
void task_mic_event(void)
{
    WATCHDOG_CLR();
    comm_event();
    //����Ƿ����豸��Ҫ��ʼ��
    if (device_need_activate(DEVICE_SDMMC) || device_need_activate(DEVICE_UDISK)) {
        if(task_ctl.work_sta == TASK_MUSIC) {   //����U�̻�SD��ʱ����ģʽ
            task_ctl.work_sta = task_ctl.last_work;
        }
    }
#if AUX_FREQ_SHOW
    if (music_freq_get()) {//Ƶ�׼������
        /*for(u8 i=0;i<32;i++)
        {
            printf("%04x ",freq_buf[i]);
        }
        printf("\n");*/
        music_freq_kick();//kick��һ��Ƶ�׼���
    }
#endif
    device_change();

    if (!device_is_actived(DEVICE_SDMMC) && !device_is_actived(DEVICE_UDISK)) {
        task_ctl.work_sta = TASK_EXIT;          //�豸��Ч���˳�
    }

#if IR_HARD_POWERDOWN_EN
    if((sys_ctl.sleep_cnt == 1 || sys_flag.ir_power_flag) && !sys_flag.lowpower_flag){
        hold_mode_enter();
    }
#endif

#if LOWPOWER_WARNING
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        dac_enable();
        mp3_res_play_wait(RES_MP3_PLEASE_CHARGE);
        dac_disable();
    }
#endif
}

#pragma constseg="TASK_MIC_CONST"
IAR_CONST char str_task_mic[] = "task_mic\n";
#pragma constseg=default

//����������
#pragma location="TASK_MIC_SEG"
void task_mic(void)
{
    printf(str_task_mic);
    task_mic_enter();
    while (task_ctl.work_sta == TASK_MIC) {
        task_mic_event();
        task_mic_deal_msg(get_msg());
        task_mic_display();
    }
    task_mic_exit();
}

#endif
