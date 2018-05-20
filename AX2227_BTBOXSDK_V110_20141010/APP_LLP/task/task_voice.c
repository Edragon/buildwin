/*****************************************************************************
 * Module    : Task
 * File      : task_voice.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
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
void task_voice_event(void);
void task_voice_numget(void);

void task_voice_numget(void)
{
    t_msc.music_num = param_read16(PARAM_VOICE_NUM);        //打开的文件编号
    if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
        t_msc.music_num = 1;
    }
}

//任务初始化
void task_voice_enter(void)
{
    set_sys_clk(SYS_24M);

    //播放信息初始化
    memset(&t_msc, 0x00, sizeof(t_msc));
    t_msc.file_change = 1;                  //需要打开一个新文件
    u_msc.music_voice_flag = 1;             //播放录音标志
    u_msc.pause = 0;
#if K_MODE_CHANGE_DEVICE
    t_msc.mode_flag = 0;
#endif

    ocx_init(task_voice_event, NULL);       //设置OCX需要响应的消息及事件
    //sound_ctl_init(3, 0x800, 25, 0x800);
    user_change_volume(sys_ctl.volume);     //数字音量
    dac_enable();                           //2014-3-7，要在task_music_num_get前打开DAC，修正同时插无文件U盘和SD卡死机问题

    //配置扫描过滤器
    f_scan_set(SCAN_SUB_FOLDER, &file_filter_voice, &dir_filter_voice);
    //获取歌曲的总数

    //2014-8-4：修复快速插拔设备会进入录音播放模式的bug。(正常应该进入音乐播放模式，除非设备无音乐文件而有录音文件)
    //if (!music_mount_device()) {  //进入录音播放模式时不去激活设备及初始化文件系统等，只能在音乐模式激活设备及初始化文件系统
    if ((!device_is_actived(DEVICE_SDMMC) && !device_is_actived(DEVICE_UDISK)) || !fs_is_mounted() || !music_scan_file()) {
        task_ctl.work_sta = TASK_EXIT;
        return;
    }
    task_voice_numget();
    //开始播放
    music_unmute();
#if VOICE_MODE_WARNING_MUSIC
    mp3_res_play_wait(RES_MP3_VOICE_MODE);
#endif
    ocx_init(task_voice_event, NULL);       //设置OCX需要响应的消息及事件
}

//任务退出
void task_voice_exit(void)
{
    music_stop();                           //结束Music播放
    mp3en_exit();
    dac_disable();
}

//任务事务处理
void task_voice_event(void)
{
    comm_event();
    task_music_event();
}

//任务消息处理
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

        //调用MUSIC消息函数
    default:
        task_music_deal_msg(msg);
        break;
    }
}

#pragma constseg="TASK_VOICE_CONST"
IAR_CONST char str_task_voice[] = "task_voice\n";
#pragma constseg=default

//任务主流程
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
