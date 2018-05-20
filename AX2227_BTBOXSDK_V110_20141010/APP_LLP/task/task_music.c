/*****************************************************************************
 * Module    : Task
 * File      : task_music.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 音乐播放任务流程
 *****************************************************************************/
#include "include.h"
#if TASK_MUSIC_EN
#include "task.h"
#include "task_music.h"
#include "user_spi.h"
extern type_dir_save dir_save;
#define DEBUG_TASK_MUSIC        0
type_move_ctl move_ctl;

extern type_dir_save dir_save;

extern void amux_init(u8 channel);
extern u16 crc_calculate(void *buf, u16 len);
void task_music_comm_event(void);
void task_music_comm_deal_msg(u8 msg);

void task_music_event(void);
extern u8 music_ibuf[0xa08];

enum {
    PLAYER_NORMAL,                  //正常播放
    PLAYER_REPEAT,                  //复读模式
};
#if TASK_KARAOKE_EN
#define MUSIC_MENU_ITEM_NUM1         (7 + 1)
#define KARAOKE_MENU_ITEM_NUM       10
#else
#define MUSIC_MENU_ITEM_NUM1         (6 + 1)
#endif
#define MUSIC_MENU_ITEM_NUM2         6

#define PLAYMODE_ITEMS_NUM          (MODE_PLAY_NUM + 1)
#define MUSIC_EQ_ITEM_NUM           (MUSIC_EQ_NUM + 1)
#define DEVICE_ITEM_NUM             3

#pragma constseg="LIST_CONTENT"
enum {
#if TASK_KARAOKE_EN
    ITEM_KARAOKE_ENTER,
#endif
    ITEM_MUSIC_EQ,
    ITEM_MUSIC_PLAYMODE,
    ITEM_SELECT_DEVICE,
    ITEM_REPEAT,
    ITEM_MENU_BACK,
    ITEM_MUSIC_EXIT,
};

#if TASK_KARAOKE_EN
enum {
     ITEM_KARAOKE_EXIT, ITEM_KARA_MODE, ITEM_KARA_MIC_VOL, ITEM_KARA_COM_VOL, ITEM_KARA_VOL, ITEM_KARA_EQ, ITEM_KARA_PLAYMODE, ITEM_KARA_NAV, ITEM_KARA_SELECT_DEVICE,
};
#endif

#pragma constseg=default
//task music controller
type_user_music_ctl u_msc;                              //不能复用
IAR_XDATA_A type_task_music_ctl t_msc @ "MUSIC_XDATA";  //跟其他模式复用

void task_music_set_a(void);
void task_music_clear_a(void);
void task_music_repeat_start(u8 repeat_cnt);


IAR_CONST u8 tbl_speed_order[] = {0, 3, 4, 0, 1, 2};

#if TASK_KARAOKE_EN
#pragma location="TASK_MUSIC_SEG_1"
void enter_karaoke(void)
{
   /* mp3en_source_init(RECORD_KARA);
      mp3en_start(false);*/

    amux_init(MIC_CHANNEL_SEL);
    AGCCON0 = 0x15;
    t_msc.karaoke_flag = 1;
    agc_anl_set(sys_ctl.mic_volume);
    AGCDMACON = 0;       //MIC直通
    ATCON5 |= BIT(2);
    at_write(DACCFG, 0x15);
    //imdct_neg();
    WATCHDOG_CLR();
}
#pragma location="TASK_MUSIC_SEG_1"
void exit_karaoke(void)
{
    ATCON5 &= ~BIT(2);
    at_write(DACCFG, 0x01);
#if (SYS_PKG == PKG_SSOP24 || SYS_PKG == PKG_LQFP48_6680E)
    dac_set_mono();                     //设置DAC为单声道
#endif
    sys_ctl.dig_vol = 13;
    change_digital_volume(sys_ctl.dig_vol);
    t_msc.karaoke_flag = 0;
}

#endif
#if BREAK_POINT_EN
#pragma location="TASK_MUSIC_SEG"
//保存断电记忆
void task_music_break_save(void)
{
    if (fs_cur_dev() == DEVICE_UDISK)
    {
        irtcc_write_ram32(USB_BREAK_FRAME, t_msc.break_point.frame);
        irtcc_write_ram32(USB_BREAK_POS, t_msc.break_point.pos);
    }
    else
    {
        irtcc_write_ram32(SD_BREAK_FRAME, t_msc.break_point.frame);
        irtcc_write_ram32(SD_BREAK_POS, t_msc.break_point.pos);
        //printf("fra:%ld,pos:%ld\n",t_msc.break_point.frame,t_msc.break_point.pos);
    }
}

//播放完一曲时，清除断点信息，在music_stop调用
void task_music_break_clr(void)
{
    if(task_ctl.last_work == TASK_MUSIC && !sys_flag.voice_play_flag && !u_msc.music_voice_flag) {
        u32 cur_sec = music_get_mtime()/10;
        //printf("mtime: %04x, alltime: %04x\n", (u16)cur_sec, music_get_alltime());
        if(cur_sec >= 35999) {  //alltime最大值为35999s
            return;             //超过35999s的文件不清断点
        }
        if((u16)cur_sec+2 >= music_get_alltime()) {
            //printf("clr_bp\n");
            if (t_msc.break_dev == DEVICE_UDISK) {  //2014-1-6, 修正清错断点问题
                irtcc_write_ram16(USB_NAME_CRC, ~t_msc.filename_crc);
            } else {
                irtcc_write_ram16(SD_NAME_CRC, ~t_msc.filename_crc);
            }
        }
    }
}

//读取断点记忆
#pragma location="TASK_MUSIC_SEG"
void task_music_break_read(void)
{
    if (fs_cur_dev() == DEVICE_UDISK)
    {
        t_msc.break_point.frame = irtcc_read_ram32(USB_BREAK_FRAME);
        t_msc.break_point.pos = irtcc_read_ram32(USB_BREAK_POS);
        t_msc.filename_crc = irtcc_read_ram16(USB_NAME_CRC);
    }
    else
    {
        t_msc.break_point.frame = irtcc_read_ram32(SD_BREAK_FRAME);
        t_msc.break_point.pos = irtcc_read_ram32(SD_BREAK_POS);
       // printf("frar:%ld,posr:%ld\n",t_msc.break_point.frame,t_msc.break_point.pos);
        t_msc.filename_crc = irtcc_read_ram16(SD_NAME_CRC);
    }
}

#pragma location="TASK_MUSIC_SEG"
void task_music_save_num(void)
{
    if(u_msc.music_voice_flag) {
        param_write16(PARAM_VOICE_NUM, t_msc.music_num);        //保存打开的文件编号
    } else {
        if (fs_cur_dev() == DEVICE_UDISK) {
            param_write16(PARAM_MUSIC_NUM_USB, t_msc.music_num);        //保存打开的文件编号
        } else {
            param_write16(PARAM_MUSIC_NUM_SD, t_msc.music_num);        //保存打开的文件编号
        }
    }
}

//进入音乐任务对断电记忆的操作
#pragma location="TASK_MUSIC_SEG_2"
void task_music_break_init(void)
{
    if(u_msc.music_voice_flag) {
        return;                                 //录音不需要断点
    }
    if(t_msc.break_flag)
    {
        task_music_break_read();
        if(crc_calculate(filename,8) == t_msc.filename_crc)
        {
            music_jump(&t_msc.break_point);
        }
    }
    t_msc.filename_crc = crc_calculate(filename,8);
    t_msc.break_dev = fs_cur_dev();             //2014-1-6, 修正清错断点问题
    if (fs_cur_dev() == DEVICE_UDISK)
    {
        irtcc_write_ram16(USB_NAME_CRC, t_msc.filename_crc);
    }
    else
    {
        irtcc_write_ram16(SD_NAME_CRC, t_msc.filename_crc);
    }
}
#else
void task_music_break_clr(void)
{
}
#endif

#pragma location="TASK_MUSIC_SEG"
void task_music_get_num(void)
{
    if (fs_cur_dev() == DEVICE_UDISK) {
        t_msc.music_num = param_read16(PARAM_MUSIC_NUM_USB);        //打开的文件编号
    } else {
        t_msc.music_num = param_read16(PARAM_MUSIC_NUM_SD);        //打开的文件编号
    }
    if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
        t_msc.music_num = 1;
    }
}

/*****************************************************************************
 * Module    : 音乐任务公共流程
 *****************************************************************************/

//音乐任务初始化
#pragma location="TASK_MUSIC_SEG_1"
void task_music_enter(void)
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

    //播放信息初始化
    u_msc.pause = 0;
    memset(&t_msc, 0x00, sizeof(t_msc));
    t_msc.play_sta = PLAYER_NORMAL;
#if BREAK_POINT_EN
    t_msc.break_flag = 1;
#endif
    t_msc.file_change = 1;                  //需要打开一个新文件
    t_msc.music_num = 1;                    //打开的文件编号
    sys_ctl.dig_vol = 13;
    sys_ctl.mic_volume = 8;
    t_msc.disp_flag = 1;
    ocx_init(task_music_event, NULL);       //设置OCX需要响应的消息及事件

    spi_music_open(0, 0);                   //复位录音位置

    sound_ctl_init(3, 0x800, 25, 0x800);    //连续3次超过0x800认为开始有声；连续25次0x800认为静音
    user_change_volume(sys_ctl.volume);     //模拟音量
    dac_enable();                           //2014-3-7，要在task_music_num_get前打开DAC，修正同时插无文件U盘和SD卡死机问题
    //配置扫描过滤器
    f_scan_set(SCAN_SUB_FOLDER, &file_filter_music, &dir_filter_music);
    if(!music_mount_device()) {
        task_ctl.work_sta = TASK_EXIT;          //设备无效，退出
        return;
    }
    task_music_get_num();
    //开始播放
    music_unmute();
#if MUSIC_MODE_WARNING_MUSIC
    if (work_sta_interrupt == TASK_NULL) {
         mp3_res_play_wait(RES_MP3_MUSIC_MODE);
    }
#endif
    work_sta_interrupt = TASK_NULL;

    led_idle();
}

//根据是否显示歌曲ID3信息选择不同初始化函数
#pragma location="MUSIC_SEG"
bool wma_init_sel(u8 __xdata *const __code *buffer_tbl, u8 total_num)
{
#if WMA_ID3_EN
    return wma_init_sdk(buffer_tbl, total_num);
#else
    return m_wma_init(buffer_tbl, total_num);
#endif
}

//MP3 ID3信息显示
#pragma location="MUSIC_SEG"
bool mp3_id3_sel(void)
{
#if MP3_ID3_EN
    return mp3_get_ID3();
#else
    return false;
#endif
}

//WAV ID3信息显示
#pragma location="MUSIC_SEG"
bool wav_id3_sel(void)
{
#if WAV_ID3_EN
    return wav_get_info();
#else
    return false;
#endif
}
#if MUSIC_LRC_EN
//初始化歌词
#pragma location="MUSIC_SEG"
bool music_lrc_init(void)
{

    if (!f_open_lrcfile()) {
        return false;
    }
    t_msc.lrc_type = lrc_init();
#if (LRC_FILENAME_TIME == 1)
    //直接显示歌词
    if(music_get_lrc(music_lrc_cur_time()))
    {
        t_msc.lrc_change = 1;
    }
#endif
    return true;
}
//刚打开歌时，需要查找并初始化LRC文件
#pragma location="TASK_MUSIC_SEG_1"
void task_music_lrc_init(void)
{
    t_msc.lrc_flag = music_lrc_init();            //打开歌词
    if (!t_msc.lrc_flag) {
        t_msc.filename_change = 1;          //刷新文件名显示
    } else {
        t_msc.lrc_change = 1;               //刷新歌词显示
    }
}
#endif

//初始化第music_num首歌
#pragma location="MUSIC_SEG"
bool task_music_init(u16 music_num)
{
    if (!f_open_opt(music_num)) {
        return false;
    }
    f_get_filename();

#if !MUSIC_ID3_EN
    //make_lfn_to_ansi();            //将GBK或UNICODE转成ANSI
    //printf("lfn: %s\n", filename); //打印
#endif

#if MUSIC_LRC_EN
    task_music_lrc_init();                  //歌词在mp3初始化之前查找，否则可能破坏MP3数据
#endif
    //printf("dir: %d %d\n", t_msc.dir_start, t_msc.dir_total);
    t_msc.music_type = music_init();
#if BT_ALL_TASK
    if (TYPE_MP3 != t_msc.music_type)
#else
    if (TYPE_NONE == t_msc.music_type)
#endif
    {
        return false;
    }
#if MUSIC_ID3_EN
    if (TYPE_WMA != t_msc.music_type) {
        music_decode_ID3();
    }
    id3_transmit();
#endif
    music_eq(sys_ctl.eq_num);
#if MUSIC_FREQ_SHOW
    music_freq_init(FREQ_DAC);
#if LCD_THEME
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
#endif
    return true;
}


//退出Music任务
#pragma location="TASK_MUSIC_SEG_1"
void task_music_exit(void)
{
    //printf("task_music_exit\n");
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
#if TASK_KARAOKE_EN
    if(t_msc.karaoke_flag)
    {
        exit_karaoke();
    }
#endif
    music_stop();                           //结束Music播放
    dac_disable();
}

#if AB_REPEAT_EN
//计算AB时间
#pragma location="TASK_MUSIC_SEG"
u32 task_music_calc_pointab(void)
{
    return (music_calc_mtime(t_msc.point.pb.frame) - music_calc_mtime(t_msc.point.pa.frame));
}
#endif

//自动下一曲到下一设备播放
#pragma location="TASK_MUSIC_SEG"
bool task_music_auto_next_device(void)
{
    if(is_other_device()) {                 //检查是否有其他设备
        //t_msc.music_err = 0;
        if(!music_next_device()) {          //选择下一设备挂载文件系统
            if(!music_next_device()) {
                task_ctl.work_sta = TASK_EXIT;  //设备无效，退出
                return false;
            }
        }
    }

    t_msc.music_num = 1;                    //从第一首歌播起
    t_msc.file_change = 1;                  //需要打开一个新文件
    return true;
}

#pragma location="TASK_MUSIC_SEG"
bool task_music_auto_prev_device(void)
{
    if(is_other_device()) {                 //检查是否有其他设备
        //t_msc.music_err = 0;
        if(!music_next_device()) {          //选择上一设备挂载文件系统
            if(!music_next_device()) {
                task_ctl.work_sta = TASK_EXIT;  //设备无效，退出
                return false;
            }
        }
    }

    t_msc.music_num = t_msc.music_total;    //从第最后一首歌播起
    t_msc.file_change = 1;                  //需要打开一个新文件
    return true;
}

//上一曲
#pragma location="TASK_MUSIC_SEG_1"
void task_music_prev(void)
{
#if AUTO_CHANGE_DEVICE
    if(sys_ctl.mode_play == ITEM_LOOP_ALL)  //全部循环才自动切设备
    {
        if(t_msc.music_num == 1)
        {
            if(task_music_auto_prev_device())
            {
                t_msc.break_flag = 0;
                return;
            }
        }
    }
#endif
    u16 dir_start = f_get_indexbefore() + 1;        //当前目录第一个文件的编号
    switch (sys_ctl.mode_play) {
    case ITEM_LOOP_ALL:
    case ITEM_LOOP_SINGLE:
    //case ITEM_LOOP_NORMAL:
        //全部循环
        t_msc.music_num--;
        if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
            t_msc.music_num = t_msc.music_total;
        }
        break;
    case ITEM_LOOP_FOLDER:
    //case ITEM_LOOP_BROWSE:
        //目录循环
	    music_stop();
        t_msc.dir_last = dir_start + f_get_dirtotal() - 1;
        t_msc.music_num--;
        if (t_msc.music_num < dir_start || t_msc.music_num > t_msc.dir_last) {
            t_msc.music_num = t_msc.dir_last;
        }
        break;
    case ITEM_LOOP_RAMDOM:
        t_msc.music_num = rand(t_msc.music_total) + 1;
        break;
    }
    t_msc.file_change = 1;
    t_msc.file_prev_next = 1;                       //设置方向，供解码出错时使用
}

//下一曲
#pragma location="TASK_MUSIC_SEG_1"
void task_music_next(void)
{
#if AUTO_CHANGE_DEVICE
    if(sys_ctl.mode_play == ITEM_LOOP_ALL)          //全部循环才自动切设备
    {   //全部循环才自动切设备
        if(t_msc.music_num == t_msc.music_total)
        {
            if(task_music_auto_next_device())
            {
                t_msc.break_flag = 0;
                return;
            }
        }
    }
#endif
    u16 dir_start = f_get_indexbefore() + 1;        //当前目录第一个文件的编号
    switch (sys_ctl.mode_play) {
    case ITEM_LOOP_ALL:
    case ITEM_LOOP_SINGLE:
    //case ITEM_LOOP_NORMAL:
        //全部循环
        if (t_msc.music_num >= t_msc.music_total) {
            t_msc.music_num = 0;
        }
        t_msc.music_num++;
        break;
    case ITEM_LOOP_FOLDER:
    //case ITEM_LOOP_BROWSE:
        //目录循环
		music_stop();
        t_msc.dir_last = dir_start + f_get_dirtotal() - 1;
        t_msc.music_num++;
        if (t_msc.music_num < dir_start || t_msc.music_num > t_msc.dir_last) {
            t_msc.music_num = dir_start;
        }
        break;
    case ITEM_LOOP_RAMDOM:
        t_msc.music_num = rand(t_msc.music_total) + 1;
        break;
    }
    t_msc.file_change = 1;
    t_msc.file_prev_next = 0;                       //设置方向，供解码出错时使用
}

//自动下一曲
#pragma location="TASK_MUSIC_SEG_1"
void task_music_auto_next(void)
{
    if (music_get_mtime() < 3) {
        //播放不足300ms，认为本歌错误
        if (t_msc.file_prev_next) {             //初始化失败，重新读取下一个文件
            task_music_prev();
        } else {
            task_music_next();
        }
    } else {
        t_msc.file_prev_next = 0;                       //清除方向
        t_msc.file_change = 1;
        if (sys_ctl.mode_play != ITEM_LOOP_SINGLE) {
            task_music_next();
        }
    }
}

//计算显示的时间
#pragma location="TASK_MUSIC_SEG"
void task_music_cal_time(void)
{
    u32 cur_sec = music_get_mtime() / 10;           //当前播放时间，精确到0.1s
    u_msc.cur_time.min = cur_sec / 60;
    u_msc.cur_time.sec = cur_sec % 60;

#if AB_REPEAT_EN
    u32 a_time = (music_point_a_mtime() + 5) / 10;
    t_msc.rtime = ((cur_sec > a_time) ? (cur_sec - a_time) : 0);
#endif
}



//音乐任务的一些信息初始化
#pragma location="TASK_MUSIC_SEG_1"
void task_music_play_init(void)
{
#if VOL_MAXMIN_MUSIC
    if(!sys_ctl.vol_maxmin_music_flag)
#endif
    {
        u_msc.pause = 0;                        //清除暂停状态
    }
    t_msc.speed = 0;                        //设置播放速度

#if AB_REPEAT_EN
    t_msc.auto_repeat_start = 0;
    //umsc_sound_detect(t_msc.auto_repeat);
    //sound_set_flag(0, 0);
#endif

    music_set_speed(0);

    t_msc.play_sta = PLAYER_NORMAL;         //正常播放状态
}
//用于在播放歌曲时插入语音菜单的播放，播放完语音菜单后继续播放原来的歌曲
void task_music_menu_music(u8 menu_music_num)
{
#if CONSTANT_WARNING_VOLUME_EN
    user_change_volume(WARNING_VOLUME);
#endif
    sys_flag.voice_play_flag = 1;
    music_unmute();

    type_music_point pt;
    music_get_point(&pt);               //设置返回播放点
    mp3_res_play(menu_music_num);
    while (music_get_status() > STATUS_PLAY_STOPPING);

    sys_flag.voice_play_flag = 0;
    user_set_volume(sys_ctl.volume);

    music_init();
    task_music_play_init();
    music_jump(&pt);                //恢复播放
    if(!u_msc.pause){
        music_play();
    } else {
        music_pause();
    }
}

#pragma location="TASK_MUSIC_SEG_1"
void task_music_new(void)
{
    u16 err_cnt = 0;

    led_music_play();

    while (device_is_actived(fs_cur_dev()) && t_msc.file_change) {
#if DEBUG_TASK_MUSIC
        printf("music_stop\n");
#endif
        music_stop();                               //停止播放器
        t_msc.file_change = 0;
        //播放指定歌曲
        if (task_music_init(t_msc.music_num)) {

            t_msc.disp_flag = 1;
            task_music_play_init();
#if AB_REPEAT_EN
            t_msc.auto_repeat = 0;                  //清除自动领读功能
#endif
            //task_music_clear_a();                   //清除A点
            music_play();

            //delay_5ms(5);                           //延时一小段时间，试解几帧，计算出相应信息
#if LCD_THEME
            t_msc.disp_music_time.sec = 0xff;       //暂不显示总时间
#endif
        } else {
            if (!fs_is_mounted() || ++err_cnt >= t_msc.music_total) {    //所有的音乐文件都无法播放
#if K_MODE_CHANGE_DEVICE
                if(is_other_device() && !t_msc.mode_flag) {
                    t_msc.mode_flag = 1;
                    task_music_auto_next_device();
                } else
#endif
                {
                    task_ctl.work_sta = TASK_EXIT;      //连续错误文件过多，退出目录播放
                }
                break;
            }

            if (t_msc.file_prev_next) {             //初始化失败，重新读取下一个文件
                task_music_prev();
            } else {
                task_music_next();
            }
        }
    }
}

//音乐任务事务处理
#pragma location="TASK_MUSIC_SEG"
void task_music_event(void)
{
    task_music_comm_event();                        //调用公共事件
	
#if(VOL_MAXMIN_MUSIC && !VOL_MUSIC_ONCE)
    if(sys_ctl.vol_maxmin_music_flag){  //处于播放音量调节提示音状态
        return;
    }
#endif

    //检查设备状态
    if (!device_is_actived(fs_cur_dev())) {
        music_stop();                           //停止当前音乐
        task_ctl.dev_change = 1;                //当前设备无效，设备发生变动
    }

    if (device_change()) {
        if (!music_remount_device()) {
            if (!music_next_device()) {
                task_ctl.work_sta = TASK_EXIT;  //设备无效，退出
                return;
            }
        }
        task_music_get_num();
        t_msc.file_change = 1;                  //需要打开一个新文件
#if BREAK_POINT_EN
        t_msc.break_flag = 1;
#endif
#if K_MODE_CHANGE_DEVICE
        t_msc.mode_flag = 0;
#endif
    }
#if MUSIC_LRC_EN
    //获取歌词
    if (t_msc.lrc_flag == LRC_FILENAME_TIME) {
        if(music_get_lrc(music_get_mtime() + 3))  //歌词故意提前300ms显示
        {
            t_msc.lrc_change = 1;
        }
    }
#endif
    //打开新文件
    if (t_msc.file_change) {
        //set_sys_clk(SYS_24M);
        task_music_new();
        sys_ctl.sleep_cnt = 0;
#if BREAK_POINT_EN
        task_music_break_init();
        task_music_save_num();
#endif
#if AB_REPEAT_EN
        t_msc.repeat_flag = 0;
        task_music_set_a();
#endif
#if (LCDSEG_THEME || LEDSEG_THEME)
        show_music_num();         //LCD段码屏及LED屏显示歌曲序号
#endif
    }
    //计算当前显示的时间
    task_music_cal_time();
#if LCD_THEME
    if (t_msc.disp_music_time.sec == 0xff) {
        u16 all_time = music_get_alltime();
        if (all_time != 0xffff) {
            t_msc.disp_music_time.min = all_time / 60;      //总时间
            t_msc.disp_music_time.sec = all_time % 60;
            t_msc.disp_status_flag = 1;                     //更新状态显示
        }
    }
#endif
    //判断音乐的播放状诚
    if (music_get_status() <= STATUS_PLAY_STOPPING) {
#if BREAK_POINT_EN
        if(t_msc.break_flag)
        {
            t_msc.break_flag = 0;
            t_msc.music_num = 0;
            t_msc.file_change = 1;
        }
#endif
        /*if(sys_ctl.mode_play == ITEM_LOOP_NORMAL)
        {
            if(t_msc.music_num >= t_msc.music_total)
            {
                return;
            }
        }*/
        task_music_auto_next();                      //本曲播放完毕，播放下一曲
    }
#if BREAK_POINT_EN
    t_msc.break_flag = 0;
#endif

#if AB_REPEAT_EN
    //复读状态控制
    switch (t_msc.play_sta) {
    case PLAYER_NORMAL:
        /*if (t_msc.auto_repeat) {
            task_music_autorepeat();
            t_msc.disp_status_flag = 1;
        }*/
        break;
    case PLAYER_REPEAT:
        if (music_get_repeat() == 0) {
            t_msc.play_sta = PLAYER_NORMAL;
            task_music_set_a();
            t_msc.disp_status_flag = 1;             //更新状态显示
        }
        break;
    }
#endif

    //浏览播放
    /*if(sys_ctl.mode_play == ITEM_LOOP_BROWSE) {
        if(music_get_mtime() > 100) {
            task_music_next();
        }
    }*/

#if AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((sys_ctl.sleep_cnt == 1 || sys_flag.ir_power_flag) && !sys_flag.lowpower_flag){
        u8 music_sta = music_get_status();
        if(music_sta == STATUS_PLAY_PAUSE) {
        } else if(music_sta >= STATUS_PLAY_PLAYING) {
            music_pause();
        }
        dac_anl_fade_wait();                        //等待淡出完成
        music_mute();
        hold_mode_enter();
        music_unmute();                             //V095退出hold_mode时不会解MUTE

        if(music_sta == STATUS_PLAY_PAUSE) {
        } else if(music_sta >= STATUS_PLAY_PLAYING) {
            music_play();
        }
    }
#endif

#if LOWPOWER_WARNING
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        dac_fade_out(1);
        task_music_menu_music(RES_MP3_PLEASE_CHARGE);
        dac_fade_in(1);
    }
#endif
}

#if AB_REPEAT_EN
/*****************************************************************************
 * Module    : 音乐任务AB复读，追述的流程
 *****************************************************************************/
//清除A点
#pragma location="TASK_MUSIC_SEG_1"
void task_music_clear_a(void)
{
    memset(&t_msc.point.pa, 0, sizeof(t_msc.point.pa));
    music_set_a_point(&t_msc.point.pa);
}

//设置A点
#pragma location="TASK_MUSIC_SEG"
void task_music_set_a(void)
{
    music_get_point(&t_msc.point.pa);
    music_set_a_point(&t_msc.point.pa);
    //printf("a fra:%d\n",t_msc.point.pa.frame);
   // printf("a pos:%d\n",t_msc.point.pa.pos);
}

//开始复读
#pragma location="TASK_MUSIC_SEG_2"
void task_music_repeat_start(u8 repeat_cnt)
{
#if DEBUG_TASK_MUSIC
    printf("repeat\n");
#endif
    music_get_point(&t_msc.point.pb);
    music_jump(&t_msc.point.pa);
  //printf("b fra:%d\n",t_msc.point.pb.frame);
 // printf("b pos:%d\n",t_msc.point.pb.pos);
    music_repeat(repeat_cnt, &t_msc.point);
    t_msc.play_sta = PLAYER_REPEAT;
}

//复读/播放
#pragma location="TASK_MUSIC_SEG_2"
void task_music_repeat_play(void)
{
    if (u_msc.pause) {
        return;
    }
    if (t_msc.play_sta == PLAYER_NORMAL) {
        if (t_msc.auto_repeat) {
            t_msc.auto_repeat = 0;
            //umsc_sound_detect(0);
            task_music_set_a();                     //更新A点
            return;
        }
        task_music_repeat_start(REPEAT_LOOP);
    } /*else {
#if DEBUG_TASK_MUSIC
        printf("play\n");
#endif
        t_msc.auto_repeat = 0;                      //关闭自动复读
        //umsc_sound_detect(0);
        music_jump(&t_msc.point.pb);
        task_music_set_a();                         //更新A点
        t_msc.play_sta = PLAYER_NORMAL;
    }*/
}
#endif

#if AB_REPEAT_EN
//自动领读
#pragma location="TASK_MUSIC_SEG_1"
void task_music_msg_autorepeat(void)
{
    if (u_msc.pause) {
        return;
    }
    task_music_set_a();
    t_msc.auto_repeat = 1;
    //umsc_sound_detect(1);                 //无歌词的话，采用静音检测
    //sound_set_flag(0, 0);
    t_msc.next_sentence = music_lrc_next_time();
    //printf("next:%ld\n", t_msc.next_sentence);
    t_msc.disp_status_flag = 1;
}
#endif

#if TASK_KARAOKE_EN

/*****************************************************************************
 * Module    : 音乐任务消息处理
 *****************************************************************************/
//卡拉ok菜单
#pragma location="TASK_MUSIC_SEG_2"
void task_music_kara_menu(void)
{
    u8 index = 0;;
    //index = listbox(0, KARAOKE_MENU_ITEM_NUM, NULL, (u16)tbl_karaoke_menu, LIST_TIME);
    switch (index) {
    case ITEM_KARA_MODE:
        //index = listbox(0, 3, NULL, (u16)tbl_kara_select_mode, LIST_TIME);
        if(index == 0)
        {
            at_write(DACCFG, 0x01);
#if (SYS_PKG == PKG_SSOP24 || SYS_PKG == PKG_LQFP48_6680E)
            dac_set_mono();                     //设置DAC为单声道
#endif
        }
        else if(index == 1)
        {
            at_write(DACCFG, 0x15);
        }
        break;
    case ITEM_KARA_MIC_VOL:
        //task_music_mic_volume();
        break;
    case ITEM_KARA_COM_VOL:
        //task_music_digi_volume();
        break;
    case ITEM_KARA_VOL:
        //task_music_volume_set();
        break;
    case ITEM_KARA_EQ:
        //index = listbox(sys_ctl.eq_num, MUSIC_EQ_ITEM_NUM, LB_EQ, (u16)tbl_music_eq, LIST_TIME);
        user_set_eq(index);
        music_eq(sys_ctl.eq_num);
        break;

    case ITEM_KARA_PLAYMODE:
        //index = listbox(sys_ctl.mode_play, PLAYMODE_ITEMS_NUM, NULL, (u16)tbl_playmode_items, LIST_TIME);
        user_set_mode_play(index);
        break;
    case ITEM_KARAOKE_EXIT:
        exit_karaoke();
        break;
    case ITEM_KARA_SELECT_DEVICE:
        //task_music_device_select();
        break;
    }
    t_msc.disp_flag = 1;
}
#endif


//数字点歌
#pragma location="TASK_MUSIC_SEG_1"
void task_music_select_numsic(u8 num)
{
    u16 f_index = numbox(num, NULL);
    if (f_index != 0 && f_index <= t_msc.music_total) {
        t_msc.music_num = f_index;                //打开播放音乐
        t_msc.file_change = 1;
        t_msc.file_prev_next = 0;               //清除方向
#if (LCDSEG_THEME || LEDSEG_THEME)
        show_music_num();         //LCD段码屏及LED屏显示歌曲序号
#endif
    }
    draw_clear();
}

#if(VOL_MAXMIN_MUSIC && !VOL_MUSIC_ONCE)
type_music_point pt;
#endif

#if VOL_MAXMIN_MUSIC
#pragma location="TASK_MUSIC_SEG"
void music_play_vol_music(void)
{
#if !VOL_MUSIC_ONCE
    if(!sys_ctl.vol_maxmin_music_flag){
        sys_ctl.vol_maxmin_music_flag = 2;
        music_get_point(&pt);               //设置返回播放点
    }
    mp3_res_play_wait(RES_MP3_VOL_MAXMIN);
    flush_msg();
#else
    if(!sys_ctl.vol_maxmin_music_flag){
        sys_ctl.vol_maxmin_music_flag = 2;
        dac_fade_out(1);
        task_music_menu_music(RES_MP3_VOL_MAXMIN);
        dac_fade_in(1);
    }
#endif
}
#endif

/*----------------------------------------------------------------------------*/
/*@brief: 上下文件夹选择播放函数接口：
//@param: direct =1为下一个文件夹选择；direct =0为上一个文件夹选择;
//@return:无
//@note
*/
/*----------------------------------------------------------------------------*/
#pragma location="TASK_MUSIC_SEG_2"
void task_music_folder_select(u8 direct)
{
    //printf("next dir\n");
    music_stop();
    if(direct)
    {
      dir_save.dir_count ++;
      if(dir_save.dir_count>dir_save.dir_total)
        dir_save.dir_count = 1;
    }
    else
    {
      if(dir_save.dir_count>1)
        dir_save.dir_count --;
      else
        dir_save.dir_count = dir_save.dir_total;
    }
    t_msc.music_num = fat_folder_select(dir_save.dir_count);
    //文件夹的文件名存放在filename这个buf里面，故在打开新的文件之前要显示，打开音乐文件之后filename会变成音乐文件的文件名
    //printf_array(filename, FILE_NAME_LEN);
   // printf("dir:%d\n",dir_save.dir_count);
    if(!t_msc.music_num)
        t_msc.music_num = 1;
   // printf("num:%d\n",t_msc.music_num);
    t_msc.file_change = 1;
}

/*#pragma location="TASK_MUSIC_SEG"
void task_music_prev_folder()
{

}
#pragma location="TASK_MUSIC_SEG"
void task_music_next_folder()
{
    u16 dir_start = f_get_indexbefore() + 1;        //当前目录第一个文件的编号
    t_msc.dir_last = dir_start + f_get_dirtotal() - 1;
    if(t_msc.dir_last < t_msc.music_total){
        t_msc.music_num = t_msc.dir_last + 1;
    }
    else{
        t_msc.music_num = 1;
    }
    t_msc.file_change = 1;
}*/


//音乐任务消息处理
#pragma location="TASK_MUSIC_SEG"
void task_music_deal_msg(u8 msg)
{
#if VOL_MAXMIN_MUSIC
    if(sys_ctl.vol_maxmin_music_flag == 1){
        #if !VOL_MUSIC_ONCE
            music_init();
            task_music_play_init();
            music_jump(&pt);    //恢复播放
            if(!u_msc.pause){
                music_play();
            } else {
                music_pause();
            }
        #endif

        sys_ctl.vol_maxmin_music_flag = 0;
        flush_msg();
        return;
    }
#endif

    if (msg == NULL) {
        return;
    }
    switch(msg) {

    /*case KU_HSF:
        //task_music_next_folder();
        task_music_select_dir(1);
        break;
    case T_KEY_STOP:
        task_music_select_dir(0);
        break;*/
#if TASK_KARAOKE_EN
    case KU_HSF:
        if(u_msc.music_voice_flag) {
            break;
        }
        if(t_msc.karaoke_flag)
            exit_karaoke();
        else
            enter_karaoke();
        break;
#endif
#if TASK_KARAOKE_EN
    case K_MIC_VOL_DOWN:
        change_mic_volume(user_volume_dec(sys_ctl.mic_volume));
        break;
    case K_MIC_VOL_UP:
        change_mic_volume(user_volume_inc(sys_ctl.mic_volume));
        break;
#endif
        //上一曲，下一曲
    case KU_PREV:
    case KU_PREV_S10:
#if AB_REPEAT_EN
        if(t_msc.repeat_flag)
        {
            if(t_msc.repeat_flag == 2)
            {
                t_msc.repeat_flag = 1;
                t_msc.disp_status_flag = 1;
            }
            else if(t_msc.repeat_flag == 3)
            {
                t_msc.repeat_flag = 1;
                t_msc.disp_status_flag = 1;
                t_msc.play_sta = PLAYER_NORMAL;
                music_get_point(&t_msc.point.pb);
                music_jump(&t_msc.point.pb);
            }
            break;
        }
#endif
#if DEBUG_TASK_MUSIC
        printf("Prev\n");
#endif
        task_music_prev();
        break;
    case KU_NEXT:
    case KU_NEXT_S10:
#if AB_REPEAT_EN
        if(t_msc.repeat_flag == 1)
        {
            t_msc.repeat_flag = 2;
            t_msc.disp_status_flag = 1;
            task_music_set_a();//更新A点
            break;
        }
        else if(t_msc.repeat_flag == 2)
        {
            task_music_repeat_play();
            t_msc.repeat_flag = 3;
            t_msc.disp_status_flag = 1;
            break;
        }
        else if(t_msc.repeat_flag == 3)
        {
            t_msc.repeat_flag = 1;
            t_msc.disp_status_flag = 1;
            t_msc.play_sta = PLAYER_NORMAL;
            music_get_point(&t_msc.point.pb);
            music_jump(&t_msc.point.pb);
            break;
        }
#endif
#if DEBUG_TASK_MUSIC
        printf("Next\n");
#endif
        task_music_next();
        break;

        //播放、暂停
    case KU_PLAY:
    case KU_PLAY_S10:
        if (music_start_pause()) {
            u_msc.pause = 0;                //更新显示状态
            sys_ctl.sleep_cnt = 0;
#if AB_REPEAT_EN
            if (t_msc.play_sta == PLAYER_NORMAL) {
                task_music_set_a();         //正常播放模式下，重设A点
            }
#endif
#if DEBUG_TASK_MUSIC
            printf("play\n");
#endif
            led_music_play();
        } else {
            u_msc.pause = 1;
            sys_ctl.sleep_cnt = STANDBY_TIME;        //默认暂停10分钟后自动进入休眠模式
#if DEBUG_TASK_MUSIC
            printf("pause\n");
#endif
            led_idle();
        }
        break;

        //快进快退，复读模式下不支持
    case KL_PREV:
    case KH_PREV:
        if (t_msc.play_sta == PLAYER_NORMAL) {
#if DEBUG_TASK_MUSIC
            printf("P");
#endif
            t_msc.speed = 0;                            //快进快退恢复会清空变速信息
            music_fast(MUSIC_FAST_BACKWORD, 1);            //快退
#if AB_REPEAT_EN
            task_music_set_a();
#endif
        }

        break;
    case KL_NEXT:
    case KH_NEXT:
        if (t_msc.play_sta == PLAYER_NORMAL) {
#if DEBUG_TASK_MUSIC
            printf("N");
#endif
           // t_msc.speed = 0;                            //快进快退恢复会清空变速信息
           // music_set_speed(tbl_speed_order[t_msc.speed]);
            /*不想快进快退自动切换下一曲，则打开此段
            if(music_get_alltime() - (music_get_mtime/10) == 1)
            {
                if(!u_msc.pause)
                {
                    music_fast_end();
                    if (!music_start_pause())
                    {
                        u_msc.pause = 1;
                        return;
                    }
                }
            }*/
            music_fast(MUSIC_FAST_FORWORD, 1);             //快进
#if AB_REPEAT_EN
            task_music_set_a();
#endif
        }
        break;
    case KLU_PREV:
    case KLU_NEXT:
        if (t_msc.play_sta == PLAYER_NORMAL) {
#if DEBUG_TASK_MUSIC
            printf("E");
#endif
            music_fast_end();                           //快进快退结束
#if AB_REPEAT_EN
            task_music_set_a();                         //更新A点
#endif
            //music_set_speed(tbl_speed_order[t_msc.speed]);
            //t_msc.speed = 0;                            //快进快退恢复会清空变速信息
            //music_set_speed(tbl_speed_order[t_msc.speed]);
        }
        break;
#if LCD_THEME
        //音量加减
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
        //音量加减
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

#if VOL_MAXMIN_MUSIC
        if(u_msc.pause){
            //music_play();
            //u_msc.pause = 0;
        }
        if(!sys_ctl.volume){
            music_play_vol_music();
        }
#endif
        break;
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

#if VOL_MAXMIN_MUSIC
        if(u_msc.pause){
            //music_play();
            //u_msc.pause = 0;
        }
        if(sys_ctl.volume == VOLUME_MAX){
            music_play_vol_music();
        }
#endif
        break;
#endif

#if AB_REPEAT_EN
        //AB复读
   /*case KU_SET_A:
        task_music_set_a();
        t_msc.disp_status_flag = 1;
        break;
    case KU_REPEAT:
        task_music_repeat_play();
        t_msc.disp_status_flag = 1;
        break;*/
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
        task_music_select_numsic(msg - T_KEY_NUM_0);
        break;

#if THEME_SELECT != NO_THEME
    case T_KEY_LOOP:
        if(sys_ctl.mode_play < MODE_PLAY_NUM - 1){
            sys_ctl.mode_play ++;
        }
        else{
            sys_ctl.mode_play = 0;
        }
        param_write8(PARAM_MODE_PLAY, sys_ctl.mode_play);
        show_mode_play();
        //printf("PM: %d\n",sys_ctl.mode_play);
        break;
    case T_KEY_EQ:
        if(sys_ctl.eq_num < (MUSIC_EQ_NUM - 1)){
            sys_ctl.eq_num ++;
        }
        else{
            sys_ctl.eq_num = 0;
        }

        param_write8(PARAM_EQ_NUM, sys_ctl.eq_num);
        music_eq(sys_ctl.eq_num);
        show_eq();
        //printf("sys_ctl.eq_num: %d\n",sys_ctl.eq_num);
        break;
#endif

    case T_KEY_USBSD:
        if(is_other_device()) {
            task_music_auto_next_device();
        }
        break;
#endif
        //调用公共消息函数
    default:
        task_music_comm_deal_msg(msg);
        break;
    }
}

/*****************************************************************************
 * Module    : 音乐任务的公共消息与事件
 *****************************************************************************/
//音乐任务的公共事件
#pragma location="TASK_MUSIC_SEG"
void task_music_comm_event(void)
{
    comm_event();                           //调用公共事件
    music_event();                          //解码需要的事件
}

//音乐任务的公共消息
#pragma location="TASK_MUSIC_SEG"
void task_music_comm_deal_msg(u8 msg)
{
    switch (msg) {
    case QSYSTEM_1S:
#if MUSIC_LRC_EN
        if (t_msc.lrc_flag && t_msc.lrc_flag != LRC_FILENAME_TIME) {
            t_msc.lrc_flag++;
            if (t_msc.lrc_flag == LRC_FILENAME_TIME) {
                if (music_lrc_first_time() <= music_get_mtime()) {
                    if(music_get_lrc(music_lrc_first_time()))
                    {
                        t_msc.lrc_change = 1;
                    }
                } else {
                    t_msc.lrc_flag = LRC_FILENAME_TIME - 1;
                }
            }
        }
#endif
#if BREAK_POINT_EN
        if(!u_msc.music_voice_flag) {   //音乐播放才记忆断点，录音播放不记忆断点
            music_get_point(&t_msc.break_point);
            task_music_break_save();
        }
#endif
        deal_msg(msg);
        break;

#if K_MODE_CHANGE_DEVICE
    case KU_MODE:
#if KL_PLAY_S10_CHANGE_MODE
    case KL_PLAY_S10:
#endif
#if K_HSF_CHANGE_MODE
  #if BT_CALL_PRIVATE
    case KU_HSF:
  #else
    case KL_HSF:
  #endif
#endif
        if(t_msc.mode_flag) {
            deal_msg(msg);
        } else {
            if (music_next_device()) {
                //task_music_get_num();         //若要恢复断点，还需恢复曲目
                //t_msc.break_flag = 1;         //设置恢复断点标志
                t_msc.break_flag = 0;           //不用恢复断点
                t_msc.file_change = 1;          //需要打开一个新文件
            } else {
                deal_msg(msg);
            }
        }
        t_msc.mode_flag = 1;
        break;
#endif
    default:
        deal_msg(msg);
        break;
    }
}

/*****************************************************************************
 * Module    : 音乐任务入口
 *****************************************************************************/
//音乐播放任务
#pragma constseg="TASK_MUSIC_SEG_CONST"
IAR_CONST char str_show_taskmusic[] = "task_music\n";
#pragma constseg=default

#pragma location="TASK_MUSIC_SEG"
void task_music(void)
{
    u8 msg;
    printf(str_show_taskmusic);
    task_music_enter();
#if DEBUG_TASK_MUSIC
    printf("Total: %d\n", t_msc.music_total);
#endif
    while (task_ctl.work_sta == TASK_MUSIC) {
        msg = get_msg();
        task_music_event();
        task_music_deal_msg(msg);
        task_music_display();
    }
    task_music_exit();
}

#else

void task_music_break_clr(void)
{
}

#endif
