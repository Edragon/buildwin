/*****************************************************************************
 * Module    : Task
 * File      : task_voice.c
 * Author    : Hanny
 * Function  : 播放录音文件
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
    t_msc.music_num = sys_param_read16(PARAM_VOICE_NUM);        //打开的文件编号
    if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
        t_msc.music_num = 1;
    }
}

//音乐任务初始化
void task_voice_enter(void)
{
    led_mode_off();
    led_idle();
    set_sys_clk(SYS_48M);
    draw_wait();
#if FIRST_PLAY_UDISK                                  //2014-1-2, 避免U盘在线时，在其他模式插入SD卡仍播U盘
    if(device_is_actived(DEVICE_SDMMC) && device_is_actived(DEVICE_UDISK)) { //U盘和SD卡同时在线时优先播放U盘
        if(fs_cur_dev() != DEVICE_UDISK){
            fs_sel(DEVICE_UDISK);
        }
    }
#endif

 /*   if (!device_is_actived(DEVICE_SDMMC)) {
        if (!device_is_actived(DEVICE_UDISK))
        {
            msgbox(STR_NO_DEVICE, NULL, MSGBOX_TIME);
            task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
            return;
        }
    } */
    draw_wait();
    //设置任务
    task_ctl.work_sta = TASK_VOICE;
    memset(&t_msc,0,sizeof(t_msc));
    //播放信息初始化
    t_msc.file_change = 1;                  //需要打开一个新文件
    t_msc.disp_flag = 1; //显示控制
    t_msc.music_voice_flag = 1;
    //sound_ctl_init(3, 0x800, 25, 0x800);     //连续3次超过0x800认为开始有声；连续25次0x800认为静音
    //配置扫描过滤器
    user_change_volume(sys_ctl.volume);//数字音量
    dac_enable();
    f_scan_set(SCAN_SUB_FOLDER|SCAN_SPEED, &file_filter_voice, &dir_filter_voice);
    if(!music_mount_device()) {
        msgbox(STR_NO_VOICE, NULL, MSGBOX_TIME);
        task_ctl.work_sta = TASK_EXIT;          //设备无效，退出
        return;
    }
#if WARNING_TASK_VOICE
    if (work_sta_interrupt == TASK_NULL) {
        mp3_res_play_wait(RES_MP3_PLAY_RECORDING);
    }
#endif
    work_sta_interrupt = TASK_NULL;
    //获取歌曲的总数
    task_voice_numget();
    //开始播放
    ocx_init(task_music_event, NULL);       //设置OCX需要响应的消息及事件
    dac_dynamic(DYNAMIC_NO);
}


//任务退出
void task_voice_exit(void)
{
    music_stop();                           //结束Music播放
    sys_unmute();
    dac_disable();
}

//任务主流程
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
        while(msg = get_msg()) { // lcd显示耗时太大，先集中处理消息，处理完后再显示
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
