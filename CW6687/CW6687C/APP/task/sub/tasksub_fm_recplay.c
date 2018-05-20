/*****************************************************************************
 * Module    : TaskSub
 * File      : tasksub_fm_recplay.c
 * Author    : Hanny
 * Function  : FM���� - ¼������
 *****************************************************************************/
#ifdef TASK_SUB

//�����ʼ��
void tasksub_fm_recplay_enter(void)
{
    adc_exit();
    amux_exit();
    ocx_clear();
    draw_wait();
    led_music_play();
   // dac_enable();
    user_change_volume(sys_ctl.volume);     //��������

    dac_enable();
    t_fm.mute = 0;

    t_msc.disp_music_time.sec = 0xff;
    t_msc.disp_flag = 1;
    t_msc.music_type = TYPE_WAV;
    t_msc.music_total = 1;
    t_msc.music_num = 1;
    //������Ϣ��ʼ��
    t_msc.pause = 0;
    if (play_record_file()) {
        music_play();
    } else {
        //printf("file Not valid!\n");
        music_stop();
    }
    ocx_init(tasksub_recplay_event, NULL);       //����OCX��Ҫ��Ӧ����Ϣ���¼�
}

//�����˳�
void tasksub_fm_recplay_exit(void)
{
    music_stop();
    dac_disable();
    ocx_clear();
    task_fm_reuse_init();
    t_fm.fm_sta = FM_PLAY;
    load_code(0x28, 3);
    task_fm_load();     //����fm_ch[]�Ǳ�����DCODE_RAM�ģ�������ʾ����ᱻ���㣬��Ҫ����LOADһ��
    amux_init(FM_CHANNEL_SEL);
    led_fm_play();
    if(!t_fm.mute){
        sys_unmute();
        amux_play();
    }
    t_fm.ch_sel = sys_param_read8(PARAM_FM_CHSEL); //���µ�ǰ̨�ţ���ֹ�ڴ渴�����Ƶ������
    t_fm.freq = sys_param_read16(PARAM_FM_FREQ);
}

#pragma location="TASK_MUSIC_SEG"
void tasksub_fm_recplay_loop(void)
{
    u8 msg;

    while ((task_ctl.work_sta == TASK_FM) && (music_get_status() > STATUS_PLAY_STOPPING)) {
        tasksub_recplay_event();
        while (msg = get_msg()) {
            tasksub_fm_recplay_deal_msg(msg);
        }
        task_music_display();
    }
}

//����������
void tasksub_fm_recplay(void)
{
    tasksub_fm_recplay_enter();
    tasksub_fm_recplay_loop();
    tasksub_fm_recplay_exit();
}
#endif
