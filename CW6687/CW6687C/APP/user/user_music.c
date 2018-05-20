/*****************************************************************************
 * Module    : User
 * File      : user_music.c
 * Author    : Hanny
 * Function  : �û����ֲ��ſ���
 *****************************************************************************/
#include "include.h"
#include "user_music.h"
#include "task.h"
#include "task_music.h"
#include "task_record.h"

struct {
    u8 sound_detect;                //�Ƿ���Ҫ�������
} umsc_ctl;

//�Զ�����Ľӿ�
__near_func void mp3_auto_repeat_hook(void)
{
    /*
    if (!umsc_ctl.sound_detect) {
        return;
    }
    u32 v_pow;
    v_pow = voice_power((void *)0x4000, 576);
    sound_detect(v_pow);
    */
}

//�����Ƿ����MP3��������Զ�����
void umsc_sound_detect(u8 sound_detect)
{
    umsc_ctl.sound_detect = sound_detect;
}

#pragma location="MP3_SEG"
void mp3_res_play_wait(u8 music_name)
{
    u8 dynamic_bak = dac_dynamic(DYNAMIC_NO);           //������ʾ���رն�̬����

    dac_fade_out();
    user_change_volume(WARNING_VOLUME);
    sys_ctl.voice_play_flag = 1;

    mp3_res_play(music_name);
    while (music_get_status() > STATUS_PLAY_STOPPING || AUCON5 != 0) {
        comm_event();
        music_event();
        WATCHDOG_CLR();
    }
    music_stop();

    sys_ctl.voice_play_flag = 0;
    user_change_volume(sys_ctl.volume);
    dac_dynamic(dynamic_bak);
}

#pragma location="MOUNT_SEG"
bool device_mount(void)
{
    //printf("mount, %d\n", fs_cur_dev());
    if (fs_need_mounted()) {
        fs_init();
    }
    if (!fs_is_mounted()) {
        //device_set_aborted(fs_cur_dev());   //�����豸�޷�����
        return false;
    }
    if (!device_is_actived(fs_cur_dev())) {
        //device_set_aborted(fs_cur_dev());   //�����豸�޷�����
        return false;
    }

    return true;
}

//����Ƿ��������豸
//#pragma location="TASK_MUSIC_SEG"
bool is_other_device(void)
{
    if(fs_cur_dev() == DEVICE_SDMMC) {
        return device_is_online(DEVICE_UDISK);
    } else {
        return device_is_online(DEVICE_SDMMC);
    }
}

//ɨ���豸�ļ�����ȡ�����ļ��ĸ���
#pragma location="TASK_MUSIC_SEG_1"
bool music_scan_file(void)
{
    led_busy();

#if WARNING_USB_SD
    if(!t_msc.music_voice_flag && work_sta_interrupt == TASK_NULL) {
        if(fs_cur_dev() == DEVICE_SDMMC){
            mp3_res_play_wait(RES_MP3_SD);
        }
        else if(fs_cur_dev() == DEVICE_UDISK){
            mp3_res_play_wait(RES_MP3_USB);
        }
        flush_msg();                        //��������ʱ�䳤������̫�ఴ����Ϣδ����
    }
#endif

    //��ȡ����������
    t_msc.music_total = 0;
    t_msc.music_total = music_total();
    t_msc.dir_total = fs_apiinfo.dir_count;
    //t_msc.dir_total = t_fat.dir_count;
    //printf("total:%d\n",t_msc.music_total);
    led_idle();

    if(t_msc.music_total == 0) {
        //device_set_aborted(fs_cur_dev());   //�����豸�޷�����
        return false;
    }
    return true;
}

//ѡ����/��һ�������豸
#pragma location="TASK_MUSIC_SEG_1"
bool music_next_device(void)
{
    u8 cur_dev;
    if(fs_cur_dev() == DEVICE_SDMMC) {
        cur_dev = DEVICE_UDISK;
    } else {
        cur_dev = DEVICE_SDMMC;
    }
    if (device_is_online(cur_dev)) {                 //����п����豸����ѡ�����豸
        music_stop();                               //ֹͣ��ǰ����
        fs_sel(cur_dev);
        if(device_mount() && music_scan_file()) {   //�����ļ�ϵͳ
            t_msc.music_num = 1;                    //�򿪵��ļ����
            t_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
            return true;
        }
    }
    return false;
}

//����һ�������豸
#pragma location="TASK_MUSIC_SEG_1"
bool music_mount_device(void)
{
    device_change();
#if PLAY_DEVICE_SAVE_EN
    u8 dev_num = sys_param_read8(PARAM_PLAY_DEVICE);
    if(dev_num<2)
    {
        if(device_is_actived(dev_num))
            fs_sel(dev_num);
    }
#endif
    if(device_mount() && music_scan_file()) {       //�����ļ�ϵͳ
        return true;
    }
    if(music_next_device()) {
        return true;
    }

    return false;
}

//���¹����ļ�ϵͳ
#pragma location="TASK_MUSIC_SEG_1"
bool music_remount_device(void)
{
    music_stop();                                   //ֹͣ��ǰ����

    if(!device_mount() || !music_scan_file()) {
        return false;
    }

    return true;
}

bool record_next_device(void)
{
    u8 cur_dev;
    if(fs_cur_dev() == DEVICE_SDMMC) {
        cur_dev = DEVICE_UDISK;
    } else {
        cur_dev = DEVICE_SDMMC;
    }
    if (device_is_online(cur_dev)) {                //����п����豸����ѡ�����豸
        fs_sel(cur_dev);
        if(device_mount()) {                        //�����ļ�ϵͳ
            return true;
        }
    }
    return false;
}

//����һ��¼���豸
bool record_mount_device(void)
{
    if(device_mount()) {                            //�����ļ�ϵͳ
        return true;
    }
    if(record_next_device()) {
        return true;
    }

    return false;
}

//ϵͳMUTEס���ȵ���򵭳�dac����mute����
#pragma location="DAC_API"
void sys_mute(void)
{
    sys_ctl.mute_flag = 1;
    dac_e.mute = 1;
    dac_fade_out();
    speaker_mute();
}

#pragma location="DAC_API"
void sys_unmute(void)
{
    sys_ctl.mute_flag = 0;
    dac_e.mute = 0;
    //printf("dac_e.out = %d\n",dac_e.out);
    if (dac_e.out) {
        dac_fade_in();                  //�رն�̬����ʱ����������ѵ��룬����ģ��
    }
}

//mute����
__near_func void speaker_mute(void)
{
    bool temp_ea = IE_EA;
    IE_EA = 0;
    sys_ctl.spk_mute = 1;
    SPEAKER_MUTE();
    IE_EA = temp_ea;
}

__near_func void speaker_unmute(void)
{
    bool temp_ea = IE_EA;
    IE_EA = 0;
    sys_ctl.spk_mute = 0;
#if EAR_DETECT_EN
    if (!device_is_online(DEVICE_EAR))
#endif
    {
        SPEAKER_UNMUTE();
    }
    IE_EA = temp_ea;
}

__near_func void dac_fade_auto(void)
{
   // printf("f:%d,s:%02x,c:%02x\n",dac_ctl.fade,dac_ctl.anl_volset,dac_ctl.anl_volcur);
    dac_fade_auto_step();
    if (dac_ctl.anl_volcur) {
        ATCON3 &= ~BIT(3);
        ATCON4 = tbl_anl_vol[dac_ctl.anl_volcur];
        //printf("-X%d-e%d-",dac_ctl.anl_volcur,dac_e.mute);
        if(!dac_e.mute) {
            speaker_unmute();
        }
    } else {
        ATCON4 = 0;
        ATCON3 |= BIT(3);                   //����Ϊ0ʱ��MUTE DAC
        if (!dac_ctl.anl_vol || dac_e.dynamic == DYNAMIC_NO || dac_e.dynamic == DYNAMIC_DAC_SPK) {
            //printf("-a:%d,d:%02x-",dac_ctl.anl_vol,dac_e.dynamic);
            //printf("m");
            speaker_mute();
        }
    }
}

//MP3 ID3��Ϣ��ʾ
#pragma location="MUSIC_SEG"
bool mp3_id3_sel(void)
{
#if MP3_ID3_EN
    return mp3_get_ID3();
#else
    return false;
#endif
}

//WAV ID3��Ϣ��ʾ
#pragma location="MUSIC_SEG"
bool wav_id3_sel(void)
{
#if WAV_ID3_EN
    return wav_get_info();
#else
    return false;
#endif
}

//WMA ID3��Ϣ��ʾ
#pragma location="INIT_SEG"
bool wma_id3_sel(void)
{
    return WMA_ID3_EN;
}

bool play_record_file(void)
{
    fat_file.sclust = fat_file.org_clust;
    memcpy(&fat_file_opt,&fat_file,sizeof(fat_file));
    pfs = &fat_fs;

    //printf("f:%08lx %08lx\n",fat_file_opt.fsize, fat_file_opt.sclust);
    if (f_open_exist(&fat_file_opt)) {
        return false;
    }
   // printf_array(&fat_file_opt,sizeof(fat_file));
    if (TYPE_NONE == music_init()) {
        return false;
    }
   // printf_array(&fat_file_opt,sizeof(fat_file));
    return true;
}

#pragma location="TASK_MUSIC_SEG"
void tasksub_recplay_event(void)
{
    comm_event();                           //���ù����¼�
    music_event();

    if (!device_is_actived(fs_cur_dev())) {
        music_stop();
    }

    task_music_cal_time();
    if (t_msc.disp_music_time.sec == 0xff) {
        u16 all_time = music_get_alltime();
        if (all_time != 0xffff) {
            t_msc.disp_music_time.min = all_time / 60;      //��ʱ��
            t_msc.disp_music_time.sec = all_time % 60;
            t_msc.disp_status_flag = 1;                     //����״̬��ʾ
        }
    }
}
