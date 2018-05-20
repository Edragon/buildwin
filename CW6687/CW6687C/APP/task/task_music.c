/*****************************************************************************
 * Module    : Task
 * File      : task_music.c
 * Author    : Hanny
 * Function  : 音乐播放任务流程
 *****************************************************************************/
#include "include.h"

#if TASK_MUSIC_EN
#include "task.h"
#include "task_music.h"
#include "user_spi.h"

//音乐任务调试使能
#define DBG_TASK_MUSIC(...)          //printf(__VA_ARGS__)

//type_move_ctl move_ctl;
extern u8 work_sta_interrupt;   //任务中断标志
#if MUSIC_ENCRYPT_EN
extern u8 music_encrypt;
extern void music_head_decrypt(void);
extern bool lrc_open_encrypt(void);
#endif
extern u16 crc_calculate(void *buf, u16 len);
void task_music_comm_event(void);
void task_music_comm_deal_msg(u8 msg);
void task_music_get_num(void);
void task_music_prev(void);
void task_music_next(void);
void task_music_event(void);
void task_music_break_save(void);
void task_music_select_numsic(u8 num);
extern u8 dec_ibuf[0xa08];
void task_music_repeat_play(void);
bool task_music_auto_next_device(void);
enum {
    PLAYER_NORMAL,                  //正常播放
    PLAYER_REPEAT,                  //复读模式
};

//task music controller
IAR_XDATA_A type_task_music_ctl t_msc @ "MUSIC_XDATA";

void task_music_set_a(void);
void task_music_clear_a(void);
bool task_music_get_total(void);
void task_music_repeat_start(u8 repeat_cnt);
void task_music_lrc_init(void);
void task_music_device_remount(void);
void task_music_play_init(void);
void music_play_vol_music(void);
extern type_music_point pt;
IAR_CONST u8 tbl_speed_order[] = {0, 3, 4, 0, 1, 2};

#define TASK_SUB
#include SET_USER_PATH(CFG_UI_MUSIC_DIR, /ui/ui_music.c)
#include "sub/tasksub_music_menu.c"

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

//播放完一曲时，清除断点信息
void task_music_break_clr(void)
{
    if(task_ctl.last_work == TASK_MUSIC && !sys_ctl.voice_play_flag && !t_msc.music_voice_flag) {
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
#pragma location="TASK_MUSIC_SEG_1"
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

#pragma location="TASK_MUSIC_SEG_1"
void task_music_save_num(void)
{
    if(t_msc.music_voice_flag) {
        sys_param_write16(PARAM_VOICE_NUM, t_msc.music_num);        //保存打开的文件编号
    } else {
        if (fs_cur_dev() == DEVICE_UDISK) {
            sys_param_write16(PARAM_MUSIC_NUM_USB, t_msc.music_num);        //保存打开的文件编号
        } else {
            sys_param_write16(PARAM_MUSIC_NUM_SD, t_msc.music_num);        //保存打开的文件编号
        }
    }
}

//进入音乐任务对断电记忆的操作
#pragma location="TASK_MUSIC_SEG_2"
void task_music_break_init(void)
{
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
#endif

//进入音乐任务对歌曲目的获取
#pragma location="TASK_MUSIC_SEG_1"
void task_music_get_num(void)
{
    if (fs_cur_dev() == DEVICE_UDISK) {
        t_msc.music_num = sys_param_read16(PARAM_MUSIC_NUM_USB);        //打开的文件编号
    } else {
        t_msc.music_num = sys_param_read16(PARAM_MUSIC_NUM_SD);        //打开的文件编号
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

/*    if (!device_is_actived(DEVICE_SDMMC)) {
        if (!device_is_actived(DEVICE_UDISK))
        {
            printf("A\n");
            msgbox(STR_NO_DEVICE, NULL, MSGBOX_TIME);
            task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
            return;
        }
    } */
    draw_wait();
    //设置任务
    task_ctl.work_sta = TASK_MUSIC;
    memset(&t_msc,0, sizeof(t_msc));
#if BREAK_POINT_EN
    t_msc.break_flag = 1;
#endif
    //播放信息初始化
    t_msc.file_change = 1;                  //需要打开一个新文件
    t_msc.music_num = 1;                    //打开的文件编号
    sys_ctl.dig_vol = 13;
    sys_ctl.mic_volume = 8;
    ocx_init(task_music_event, NULL);       //设置OCX需要响应的消息及事件
    t_msc.disp_flag = 1; //显示控制
    //change_digital_volume(sys_ctl.dig_vol);
    //sound_ctl_init(3, 0x800, 25, 0x800);     //连续3次超过0x800认为开始有声；连续25次0x800认为静音
    user_change_volume(sys_ctl.volume);//模拟音量
    dac_enable();
    //if (!sys_ctl.mute_flag) {
    //    sys_unmute();
    //}

    //配置扫描过滤器
    f_scan_set(SCAN_SUB_FOLDER|SCAN_SPEED, &file_filter_music, &dir_filter_music);
    set_sys_clk(SYS_48M);
    if(!music_mount_device()) {
        msgbox(STR_NO_MUSIC, NULL, MSGBOX_TIME);
        if(task_ctl.work_sta != TASK_UPDATE_FW)
            task_ctl.work_sta = TASK_EXIT;          //设备无效，退出
        return;
    }
    task_music_get_num();
    //开始播放

#if MUSIC_MODE_WARNING_MUSIC
    if (work_sta_interrupt == TASK_NULL) {
         mp3_res_play_wait(RES_MP3_MUSIC_MODE);
    }
#endif
    work_sta_interrupt = TASK_NULL;
    led_idle();

    dac_dynamic(DYNAMIC_NO);
#if MIC_DIRECT_OUT_EN
    mic_direct_out_enter(MIC_CHANNEL_SEL,11);
#endif
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf = lcd_buf2;   // lcd使用小buf，buf1与读lrc的buf复用了
#endif
}

//初始化第music_num首歌，并查找LRC文件
#pragma location="MUSIC_SEG"
bool task_music_init(u16 music_num)
{
    set_sys_clk(SYS_48M);
    if (!f_open_opt(music_num)) {
        return false;
    }
    t_msc.dir_num = fs_apiinfo.dir_count;
#if MUSIC_ENCRYPT_EN
    music_check_encrypt();//判断是否为加密mp3
#endif
    f_get_opt_filename(filename);
#if MUSIC_ENCRYPT_EN
    music_head_decrypt();
#endif
#if MUSIC_LRC_EN
    task_music_lrc_init();                  //歌词在mp3初始化之前查找，否则可能破坏MP3数据
#endif
    //printf("dir: %d %d\n", t_msc.dir_start, t_msc.dir_total);

    t_msc.music_type = music_init();
#if MUSIC_ID3_EN
    music_decode_ID3();
#endif
    if (t_msc.music_type == TYPE_MP3) {
        set_sys_clk(SYS_24M);               //MP3使用24MHz
    }
    return true;
}

#if MUSIC_LRC_EN
//初始化歌词
#pragma location="MUSIC_SEG"
bool music_lrc_init(void)
{
#if MUSIC_ENCRYPT_EN
    if (!lrc_open_encrypt())
#endif
    {
        if (!f_open_lrcfile()) {
            return false;
        }
    }
    t_msc.lrc_type = lrc_init();

#if (LRC_FILENAME_TIME == 1)
    //直接显示歌词
    t_msc.lrc_change = music_get_lrc(music_lrc_cur_time());
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

//退出Music任务
#pragma location="TASK_MUSIC_SEG_1"
void task_music_exit(void)
{
    //printf("task_music_exit\n");
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
    music_stop();                           //结束Music播放
#if MIC_DIRECT_OUT_EN
    mic_direct_out_exit();
#endif
    sys_unmute();
    dac_disable();
    dac_dynamic(DYNAMIC_NO);
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf = lcd_buf1;   // lcd恢复使用大buf
#endif
}

#pragma location="TASK_MUSIC_SEG_1"
bool task_music_auto_prev_device(void)
{
    if(is_other_device()) {                 //检查是否有其他设备
        //t_msc.music_err = 0;
        if(!music_next_device()) {          //选择上一设备挂载文件系统
            if(!music_next_device()) {      //如果无上一设备返回原设备
                task_ctl.work_sta = TASK_EXIT;  //设备无效，退出
                return false;
            }
        }
    }

    t_msc.music_num = t_msc.music_total;    //从第最后一首歌播起
    t_msc.file_change = 1;                  //需要打开一个新文件
    return true;
}

//自动下一曲到下一设备播放
#pragma location="TASK_MUSIC_SEG"
bool task_music_auto_next_device(void)
{
    if(is_other_device()) {                 //检查是否有其他设备
        //t_msc.music_err = 0;
        if(!music_next_device()) {          //选择下一设备挂载文件系统
            if(!music_next_device()) {      //如果无下一设备返回原设备
                task_ctl.work_sta = TASK_EXIT;  //设备无效，退出
                return false;
            }
        }
    }

    t_msc.music_num = 1;                    //从第一首歌播起
    t_msc.file_change = 1;                  //需要打开一个新文件
    return true;
}

#if AB_REPEAT_EN
//计算AB时间
#pragma location="TASK_MUSIC_SEG_1"
u32 task_music_calc_pointab(void)
{
    return (music_calc_mtime(t_msc.point.pb.frame) - music_calc_mtime(t_msc.point.pa.frame));
}
#endif

//上一文件夹
#if FOLDER_SELECT
#pragma location="TASK_MUSIC_SEG_1"
void task_music_prev_dir(void)
{
    if(t_msc.dir_total <= 1) {
        return;
    }

    if(t_msc.dir_num <= 1) {
        t_msc.dir_num = t_msc.dir_total;
    } else {
        t_msc.dir_num--;
    }

    music_stop();
    f_open_folder(t_msc.dir_num);
    t_msc.file_change = 1;
    t_msc.music_num = fs_apiinfo.file_count;
}

//上一文件夹
#pragma location="TASK_MUSIC_SEG_1"
void task_music_next_dir(void)
{
    if(t_msc.dir_total <= 1) {
        return;
    }

    if(t_msc.dir_num >= t_msc.dir_total) {
        t_msc.dir_num = 1;
    } else {
        t_msc.dir_num++;
    }

    music_stop();
    f_open_folder(t_msc.dir_num);
    t_msc.file_change = 1;
    t_msc.music_num = fs_apiinfo.file_count;
}
#endif

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
#if BREAK_POINT_EN
                t_msc.break_flag = 0;
#endif
                return;
            }
        }
    }
#endif
    u16 dir_start = f_get_indexbefore() + 1;        //当前目录第一个文件的编号
    switch (sys_ctl.mode_play) {
    case ITEM_LOOP_ALL:
    case ITEM_LOOP_SINGLE:
    case ITEM_LOOP_NORMAL:
        //全部循环
        t_msc.music_num--;
        if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
            t_msc.music_num = t_msc.music_total;
        }
        break;
    case ITEM_LOOP_FOLDER:
    case ITEM_LOOP_BROWSE:
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
#if BREAK_POINT_EN
                t_msc.break_flag = 0;
#endif
                return;
            }
        }
    }
#endif
    u16 dir_start = f_get_indexbefore() + 1;        //当前目录第一个文件的编号
    switch (sys_ctl.mode_play) {
    case ITEM_LOOP_ALL:
    case ITEM_LOOP_SINGLE:
    case ITEM_LOOP_NORMAL:
        //全部循环
        if (t_msc.music_num >= t_msc.music_total) {
            t_msc.music_num = 0;
        }
        t_msc.music_num++;
        break;
    case ITEM_LOOP_FOLDER:
    case ITEM_LOOP_BROWSE:
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

    t_msc.cur_time.min = cur_sec / 60;
    t_msc.cur_time.sec = cur_sec % 60;

#if AB_REPEAT_EN
    u32 a_time = (music_point_a_mtime() + 5) / 10;
    t_msc.rtime = ((cur_sec > a_time) ? (cur_sec - a_time) : 0);
#endif
}

//音乐任务的一些信息初始化
#pragma location="TASK_MUSIC_SEG_1"
void task_music_play_init(void)
{
    t_msc.pause = 0;                        //清除暂停状态
    t_msc.speed = 0;                        //设置播放速度
/*
#if AB_REPEAT_EN
    t_msc.auto_repeat_start = 0;
    umsc_sound_detect(t_msc.auto_repeat);
#endif*/
    //sound_set_flag(0, 0);

    music_set_speed(0);

    t_msc.play_sta = PLAYER_NORMAL;         //正常播放状态
}

//用于在播放歌曲时插入语音菜单的播放，播放完语音菜单后继续播放原来的歌曲
void mp3res_play_musicmode(u8 music_name)
{
    type_music_point pt;
    music_get_point(&pt);                       //设置返回播放点
    music_stop();                               //停止当彰音乐播放

    mp3_res_play_wait(music_name);              //播放提示音

    music_init();
#if MUSIC_LRC_EN
    if (t_msc.lrc_flag) {
        t_msc.lrc_type = lrc_init();            //再次初始化歌词
    }
#endif
    task_music_play_init();
    music_jump(&pt);                            //恢复播放
    music_play();
}

#pragma location="TASK_MUSIC_SEG_1"
void task_music_new(void)
{
    u16 err_cnt = 0;

    led_music_play();

    while (device_is_actived(fs_cur_dev()) && t_msc.file_change) {
        DBG_TASK_MUSIC("music_stop\n");
        music_stop();                               //停止播放器
        t_msc.file_change = 0;
#if BT_SPP_MSC_EN
        spp_tx_msc_num(t_msc.music_num);            //切歌，通知SPP曲目变化
#endif
        //播放指定歌曲
        if (task_music_init(t_msc.music_num)) {
            task_music_play_init();
#if AB_REPEAT_EN
            t_msc.auto_repeat = 0;                  //清除自动领读功能
#endif
#if BREAK_POINT_EN
            if(!t_msc.music_voice_flag) {           //放在music_play前，防止wav断点记忆时先播放一小段开头再跳到记忆点播放
                task_music_break_init();
            }
            task_music_save_num();
#endif
            //task_music_clear_a();                   //清除A点
            music_play();
            //delay_5ms(5);                           //延时一小段时间，试解几帧，计算出相应信息
            t_msc.disp_music_time.sec = 0xff;       //暂不显示总时间
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
                    msgbox(STR_FILE_ERROR, NULL, MSGBOX_TIME);
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
    if(sys_ctl.vol_maxmin_music_play){  //处于播放音量调节提示音状态
        return;
    }
    //检查设备状态
    if (!device_is_actived(fs_cur_dev())) {
        music_stop();                           //停止当前音乐
        task_ctl.dev_change = 1;                //当前设备无效，设备发生变动
    }

    if (device_need_activate(DEVICE_SDMMC) || device_need_activate(DEVICE_UDISK)) {
        music_stop();       //播放当前设备时插入另一设备，立即停止当前音乐
        task_ctl.dev_change = 1;
        t_msc.file_change = 1;
#if BREAK_POINT_EN
        t_msc.break_flag = 1;
#endif
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
        t_msc.disp_flag = 1;
        sys_ctl.sleep_cnt = 0xffff;
#if PLAY_DEVICE_SAVE_EN
        sys_param_write8(PARAM_PLAY_DEVICE,fs_cur_dev());
#endif
#if AB_REPEAT_EN
        t_msc.repeat_flag = 0;
        task_music_set_a();
#endif
#if (IS_LCDSEG_DISPLAY || IS_LEDSEG_DISPLAY)
        show_num(t_msc.music_num);         //LCD段码屏及LED屏显示歌曲序号
#endif
    }
    //计算当前显示的时间
    task_music_cal_time();

    if (t_msc.disp_music_time.sec == 0xff) {
        u16 all_time = music_get_alltime();
        //printf("t:%d\n",all_time);
        if (all_time != 0xffff) {
            t_msc.disp_music_time.min = all_time / 60;      //总时间
            t_msc.disp_music_time.sec = all_time % 60;
            t_msc.disp_status_flag = 1;                     //更新状态显示
        }
    }

    //判断音乐的播放状诚
    if (music_get_status() <= STATUS_PLAY_STOPPING) {
#if BREAK_POINT_EN
        if(t_msc.break_flag) {
            t_msc.break_flag = 0;
            t_msc.music_num = 0;
            t_msc.file_change = 1;
        }
        task_music_break_clr();
#endif
        if(sys_ctl.mode_play == ITEM_LOOP_NORMAL)
        {
            if(t_msc.music_num >= t_msc.music_total)
            {
                return;
            }
        }
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
    if(sys_ctl.mode_play == ITEM_LOOP_BROWSE) {
        if(music_get_mtime() > 100) {
            task_music_next();
        }
    }

#if MUSIC_AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((((!sys_ctl.sleep_cnt) && BT_IS_SLEEP) || sys_ctl.ir_power_flag) && !sys_ctl.lowpower_flag){
        if (music_get_status() < STATUS_PLAY_PLAYING) {
            task_hold();
            put_msg(KU_PLAY);   //U盘或SD卡唤醒后直接播放
        }
    }
#endif

#if WARNING_LOWPOWER
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        mp3res_play_musicmode(RES_MP3_PLEASE_CHARGE);
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
#pragma location="TASK_MUSIC_SEG_1"
void task_music_set_a(void)
{
    music_get_point(&t_msc.point.pa);
    music_set_a_point(&t_msc.point.pa);
    //printf("a fra:%08lx\n",t_msc.point.pa.frame);
    //printf("a pos:%08lx\n",t_msc.point.pa.pos);
}

//开始复读
#pragma location="TASK_MUSIC_SEG_2"
void task_music_repeat_start(u8 repeat_cnt)
{
    DBG_TASK_MUSIC("repeat\n");
    music_get_point(&t_msc.point.pb);
    music_jump(&t_msc.point.pa);
    //printf("b fra:%08lx\n",t_msc.point.pb.frame);
    //printf("b pos:%08lx\n",t_msc.point.pb.pos);
    music_repeat(repeat_cnt, &t_msc.point);
    t_msc.play_sta = PLAYER_REPEAT;
}

//复读/播放
#pragma location="TASK_MUSIC_SEG_2"
void task_music_repeat_play(void)
{
    if (t_msc.pause) {
        return;
    }
    if (t_msc.play_sta == PLAYER_NORMAL) {
        if (t_msc.auto_repeat) {
            t_msc.auto_repeat = 0;
            umsc_sound_detect(0);
            task_music_set_a();                     //更新A点
            return;
        }
        task_music_repeat_start(REPEAT_LOOP);
    } /*else {
        DBG_TASK_MUSIC("play\n");
        t_msc.auto_repeat = 0;                      //关闭自动复读
        umsc_sound_detect(0);
        music_jump(&t_msc.point.pb);
        task_music_set_a();                         //更新A点
        t_msc.play_sta = PLAYER_NORMAL;
    }*/
}
#endif


//复读接口，加在解码过程中
__near_func bool music_repeat_hook(void)
{
#if AB_REPEAT_EN
    return music_repeat_hook2();
#else
    return true;
#endif
}

#if AB_REPEAT_EN
//自动领读
#pragma location="TASK_MUSIC_SEG_1"
void task_music_msg_autorepeat(void)
{
    if (t_msc.pause) {
        return;
    }
    task_music_set_a();
    t_msc.auto_repeat = 1;
#if MUSIC_LRC_EN
    if (t_msc.lrc_flag) {
        t_msc.next_sentence = music_lrc_next_time();
    } else
#endif
    {
        umsc_sound_detect(1);                 //无歌词的话，采用静音检测
    }
    t_msc.next_sentence = music_lrc_next_time();
    //printf("next:%ld\n", t_msc.next_sentence);
    //sound_set_flag(0, 0);
    t_msc.disp_status_flag = 1;
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
#if (IS_LCDSEG_DISPLAY || IS_LEDSEG_DISPLAY)
        show_num(t_msc.music_num);         //LCD段码屏及LED屏显示歌曲序号
#endif
    }
}

#if(WARNING_VOL_MAXMIN)
type_music_point pt;
#endif

#if WARNING_VOL_MAXMIN
#pragma location="TASK_MUSIC_SEG_1"
void music_play_vol_music(void)
{
    if(!sys_ctl.vol_maxmin_music_play){
        sys_ctl.vol_maxmin_music_play = 1;
        music_get_point(&pt);               //设置返回播放点
    }
    mp3_res_play_wait(RES_MP3_VOL_MAXMIN);
    flush_msg();
}
#endif

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
    DBG_TASK_MUSIC("Total: %d\n", t_msc.music_total);
    while (task_ctl.work_sta == TASK_MUSIC) {
#if IS_LCD_DISPLAY
        WATCHDOG_CLR();
        task_music_event();
        while(msg = get_msg()) {  // lcd显示耗时太大，先集中处理消息，处理完后再显示
            task_music_deal_msg(msg);
        }
        task_music_display();
#else
        msg = get_msg();
        WATCHDOG_CLR();
        task_music_event();
        task_music_deal_msg(msg);
        task_music_display();

#endif
    }
    task_music_exit();
}
#endif
