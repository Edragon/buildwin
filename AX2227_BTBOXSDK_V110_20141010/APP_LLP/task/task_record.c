/*****************************************************************************
 * Module    : Task
 * File      : task_record.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 录音任务流程(作为FM、AUX和MIC的子任务)
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_record.h"
#include "user_fm.h"

#if (AUX_RECORD_EN || TASK_MIC_EN)


#pragma constseg="TASK_RECORD_1_CONST"
IAR_CONST char str_rec_mic[] = "rec_mic\n";
IAR_CONST char str_rec_aux[] = "rec_aux\n";
IAR_CONST char str_rec_fm[] = "rec_fm\n";
IAR_CONST char str_rec_create_ok[] = "Create %s OK\n";
IAR_CONST char str_rec_create_fail[] = "Create %s FAIL\n";
IAR_CONST char str_rec_disk_full[] = "disk full\n";
IAR_CONST char str_rec_err[] = "error\n";
IAR_CONST char str_rec_start[] = "start\n";
IAR_CONST char str_rec_stop[] = "stop\n";

IAR_CONST char str_mic_rec_dir[] = "RECORD";
IAR_CONST char str_aux_rec_dir[] = "RECORDL";
IAR_CONST char str_fm_rec_dir[] = "RECORDFM";

#if (RECORD_FILE_NAME == SHORT_NAME)
IAR_CONST char str_mic_rec_file[] = "RECORD/REC0001.MP3";
IAR_CONST char str_aux_rec_file[] = "RECORDL/LIN0001.MP3";
IAR_CONST char str_fm_rec_file[] = "RECORDFM/FM0001.MP3";

#define REC_FNAME_NUM_LEN           4                               //文件编号长度，4位
#define MIC_FNAME_NUM_OFS           (sizeof(str_mic_rec_file)-8-1)  //文件编号位置，总长减去“0001.MP3”和\0
#define AUX_FNAME_NUM_OFS           (sizeof(str_aux_rec_file)-8-1)  //文件编号位置，总长减去“0001.MP3”和\0
#define FM_FNAME_NUM_OFS            (sizeof(str_fm_rec_file)-8-1)   //文件编号位置，总长减去“0001.MP3”和\0
#else
#define REC_FNAME_NUM_LEN           4                               //文件编号长度，4位
#define MIC_FNAME_NUM_OFS           sizeof(str_mic_rec_dir)         //文件编号位置，在"RECORD/"之后
#define AUX_FNAME_NUM_OFS           sizeof(str_aux_rec_dir)         //文件编号位置，在"RECORDL/"之后
#define FM_FNAME_NUM_OFS            sizeof(str_fm_rec_dir)          //文件编号位置，在"RECORDL/"之后
#endif

#pragma constseg=default


IAR_XDATA_A type_task_rec_ctl t_rec @ "RECORD_XDATA";
extern u32 WriteTotalData;                 //文件系统写数据时候使用


u8 task_record_start(void);
void task_record_stop(void);

void task_recording_display(void);


#pragma location="TASK_RECORD_SEG_1"
bool record_fname_inc(u8 *buf)
{
    u8 i;
    u8 *p = buf + (REC_FNAME_NUM_LEN-1);

    *p += 1;
    for(i=0; i<(REC_FNAME_NUM_LEN-1); i++) {
        if(*p > '9') {
            *p-- = '0';
            *p += 1;
        } else {
            return true;
        }
    }

    return (*p > '9')? false : true;
}


//录音任务初始化
#pragma location="TASK_RECORD_SEG_1"
void task_record_enter(void)
{
    u8 res;
    set_sys_clk(SYS_48M);
    //播放状态
    if(t_rec.rec_src == RECORD_MIC) {
        printf(str_rec_mic);
    } else if(t_rec.rec_src == RECORD_LINEIN) {
        printf(str_rec_aux);
    } else/* if(t_rec.rec_src == RECORD_FM)*/ {
        printf(str_rec_fm);
    }
    t_rec.play_flag = 0;
    t_rec.full_flag = 0;
    t_rec.dir_open = 0;
    t_rec.file_open = 0;
    t_rec.sync_cnt = 0;
    t_rec.rec_sta = REC_STA_STOP;
    t_rec.rec_time = 0;                 //时间清0
    ocx_clear();
    led_busy();
    t_rec.update_flag = 1;
    res = task_record_start();
    flush_msg();                        //清空消息池
    if(res == 2) {
        show_full();
        printf(str_rec_err);
        WATCHDOG_CLR();
        delay_5ms(200);
        return;
    } else if(res == 1) {
        show_err();
        printf(str_rec_err);
        WATCHDOG_CLR();
        delay_5ms(200);
        return;
    } else {
        printf(str_rec_start);
        mp3en_start(true);
        t_rec.rec_sta = REC_STA_RECORD;
        ocx_show_cnt = 0;//控制录音闪烁时间
    }
}

//退出录音任务
#pragma location="TASK_RECORD_SEG_1"
void task_record_exit(void)
{
    if (t_rec.rec_sta != REC_STA_STOP) {
        task_record_stop();
    }

    if(!device_is_actived(fs_cur_dev())) {
        t_rec.play_flag = 0;
        t_rec.full_flag = 0;
        device_change();
    }


#if REC_PLAY_EN
    if(t_rec.play_flag) {
        mp3en_exit();
#if LCD_THEME
        if (t_rec.rec_src == RECORD_MIC) {
            memcpy(filename, &filename[sizeof(str_mic_rec_dir)],13);
        } else if(t_rec.rec_src == RECORD_LINEIN) {
            memcpy(filename, &filename[sizeof(str_aux_rec_dir)],13);
        } else/* if(t_rec.rec_src == RECORD_FM)*/ {
            memcpy(filename, &filename[sizeof(str_fm_rec_dir)],13);
        }
#if (RECORD_FILE_NAME == SHORT_NAME)
        filename[FILE_NAME_FLAG] |= FB_GBK;
#endif
#endif
        set_sys_clk(SYS_24M);
        task_recplay();
        mp3en_source_init(t_rec.rec_src);
    }
#else
    if(t_rec.rec_src == RECORD_FM) {
        dac_fade_out(0);        //FM切时钟会使收音效果变差，这里进行淡入淡出处理
        dac_anl_fade_wait();
    #if FMOSC_REUSE_SDCLK && FMOSC_FREQ_SEL == FMOSC_32K
        set_sys_clk(SYS_12M);
    #else
        set_sys_clk(SYS_24M);
    #endif
        dac_fade_in(0);
    }
#endif

    if(t_rec.full_flag){                            //当前设备已录满
        if(is_other_device()){                      //检查是否有其他设备
            if(!record_next_device()){              //选择下一设备挂载文件系统
                if(!record_next_device()){          //下一个设备挂载失败则重新挂载回当前已录满的设备，若也挂载失败则退出录音模式
                    task_ctl.work_sta = TASK_EXIT;  //设备无效，退出
                    return;
                }
            }
        }
    }

#if AUX_FREQ_SHOW || FM_FREQ_SHOW
    mp3en_start(false);
#if LCD_THEME
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
#endif
    t_rec.update_flag = 1;
    led_idle();
    ocx_clear();
}

#if (RECORD_FILE_NAME == LONG_NAME)
//若文件名小于等于8个字节，则RECORD_FILE_NAME可定义为 SHORT_NAME，大于8字节，则定义为LONG_NAME
#pragma location="TASK_RECORD_SEG_1"
void task_record_filename_make(void)
{
    //注：短文件名使用的是gbk编码，长文件名使用的是unicode编码
    filename[t_rec.short_name_ptr]='/';
#if 1
    //长文件名是“0xxx建荣珠海有限公司-2013年4月2日.MP3”的示例
    //对应短文件“0xxx建~1.MP3”，7-8位置一定要“~1”，不能更改
    //short name
    filename[t_rec.short_name_ptr+1]='0';   //数字编号，会自动生成
    filename[t_rec.short_name_ptr+2]='0';
    filename[t_rec.short_name_ptr+3]='0';
    filename[t_rec.short_name_ptr+4]='1';
    filename[t_rec.short_name_ptr+5]=0xBD;  //5-6为'建'，gbk编码为0xBD 0xA8，中文可参照Winhex查看
    filename[t_rec.short_name_ptr+6]=0xA8;
    filename[t_rec.short_name_ptr+7]='~';   //7-8为“~1”，不能改
    filename[t_rec.short_name_ptr+8]='1';
    filename[t_rec.short_name_ptr+9] ='.';
    filename[t_rec.short_name_ptr+10]='m';
    filename[t_rec.short_name_ptr+11]='p';
    filename[t_rec.short_name_ptr+12]='3';
    filename[t_rec.short_name_ptr+13]='\0';
    //longname
    my_memset((void *)&filename[t_rec.long_name_ptr], 0xFF, 26*2);
    //数字编号，会自动生成
    filename[t_rec.long_name_ptr] = '0';    //0
    filename[t_rec.long_name_ptr+1] = 0x00;
    filename[t_rec.long_name_ptr+2] = '0';  //0
    filename[t_rec.long_name_ptr+3] = 0x0;
    filename[t_rec.long_name_ptr+4] = '0';  //0
    filename[t_rec.long_name_ptr+5] = 0x0;
    filename[t_rec.long_name_ptr+6] = '1';  //1
    filename[t_rec.long_name_ptr+7] = 0x0;
    //需要使用Winhex来查看对应填写其余文件名
    filename[t_rec.long_name_ptr+8] = 0xfa; //建
    filename[t_rec.long_name_ptr+9] = 0x5e;
    //6-11
    filename[t_rec.long_name_ptr+10] = 0x63;//荣
    filename[t_rec.long_name_ptr+11] = 0x83;
    filename[t_rec.long_name_ptr+12] = 0xe0;//珠
    filename[t_rec.long_name_ptr+13] = 0x73;
    filename[t_rec.long_name_ptr+14] = 0x77;//海
    filename[t_rec.long_name_ptr+15] = 0x6d;
    filename[t_rec.long_name_ptr+16] = 0x09;//有
    filename[t_rec.long_name_ptr+17] = 0x67;
    filename[t_rec.long_name_ptr+18] = 0x50;//限
    filename[t_rec.long_name_ptr+19] = 0x96;
    filename[t_rec.long_name_ptr+20] = 0x6c;//公
    filename[t_rec.long_name_ptr+21] = 0x51;
    //12-13
    filename[t_rec.long_name_ptr+22] = 0xf8;//司
    filename[t_rec.long_name_ptr+23] = 0x53;
    filename[t_rec.long_name_ptr+24] = 0x2d;//-
    filename[t_rec.long_name_ptr+25] = 0x00;
    //14-18
    filename[t_rec.long_name_ptr+26] = 0x32;//2
    filename[t_rec.long_name_ptr+27] = 0x0;
    filename[t_rec.long_name_ptr+28] = 0x30;//0
    filename[t_rec.long_name_ptr+29] = 0x00;
    filename[t_rec.long_name_ptr+30] = 0x31;//1
    filename[t_rec.long_name_ptr+31] = 0x0;
    filename[t_rec.long_name_ptr+32] = 0x33;//3
    filename[t_rec.long_name_ptr+33] = 0x0;
    filename[t_rec.long_name_ptr+34] = 0x74;//年
    filename[t_rec.long_name_ptr+35] = 0x5e;
    //19-24
    filename[t_rec.long_name_ptr+36] = 0x34;//4
    filename[t_rec.long_name_ptr+37] = 0x0;
    filename[t_rec.long_name_ptr+38] = 0x08;//月
    filename[t_rec.long_name_ptr+39] = 0x67;
    filename[t_rec.long_name_ptr+40] = 0x32;//2
    filename[t_rec.long_name_ptr+41] = 0x0;
    filename[t_rec.long_name_ptr+42] = 0xe5;//日
    filename[t_rec.long_name_ptr+43] = 0x65;
    filename[t_rec.long_name_ptr+44] = 0x2e;//.
    filename[t_rec.long_name_ptr+45] = 0x0;
    filename[t_rec.long_name_ptr+46] = 0x6d;//m
    filename[t_rec.long_name_ptr+47] = 0x00;
    //25-26
    filename[t_rec.long_name_ptr+48] = 0x70;//p
    filename[t_rec.long_name_ptr+49] = 0x0;
    filename[t_rec.long_name_ptr+50] = 0x33;//3
    filename[t_rec.long_name_ptr+51] = 0x0;
#else
    //长文件名是“0xxx建荣.MP3”的示例
    //对应短文件“0xxx建~1.MP3”，7-8位置一定要“~1”，不能更改
    //short name
    filename[t_rec.short_name_ptr+1]='0';   //数字编号，会自动生成
    filename[t_rec.short_name_ptr+2]='0';
    filename[t_rec.short_name_ptr+3]='0';
    filename[t_rec.short_name_ptr+4]='1';
    filename[t_rec.short_name_ptr+5]=0xBD;  //5-6为'建'，gbk编码为0xBD 0xA8，中文可参照Winhex查看
    filename[t_rec.short_name_ptr+6]=0xA8;
    filename[t_rec.short_name_ptr+7]='~';   //7-8为“~1”，不能改
    filename[t_rec.short_name_ptr+8]='1';
    filename[t_rec.short_name_ptr+9] ='.';
    filename[t_rec.short_name_ptr+10]='m';
    filename[t_rec.short_name_ptr+11]='p';
    filename[t_rec.short_name_ptr+12]='3';
    filename[t_rec.short_name_ptr+13]='\0';
    //longname
    my_memset((void *)&filename[t_rec.long_name_ptr], 0xFF, 26*2);
    /*数字编号，会自动生成*/
    filename[t_rec.long_name_ptr] = '0';
    filename[t_rec.long_name_ptr+1] = 0x00;
    filename[t_rec.long_name_ptr+2] = '0'; //阿拉伯数字
    filename[t_rec.long_name_ptr+3] = 0x0;
    filename[t_rec.long_name_ptr+4] = '0';
    filename[t_rec.long_name_ptr+5] = 0x0;
    filename[t_rec.long_name_ptr+6] = '1';
    filename[t_rec.long_name_ptr+7] = 0x0;
    //需要使用Winhex来查看对应填写其余文件名
    filename[t_rec.long_name_ptr+8] = 0xfa;
    filename[t_rec.long_name_ptr+9] = 0x5e;
    //6-11
    filename[t_rec.long_name_ptr+10] = 0x63;
    filename[t_rec.long_name_ptr+11] = 0x83;
    filename[t_rec.long_name_ptr+12] = 0x2e;
    filename[t_rec.long_name_ptr+13] = 0x0;
    filename[t_rec.long_name_ptr+14] = 0x6d;
    filename[t_rec.long_name_ptr+15] = 0x0;
    filename[t_rec.long_name_ptr+16] = 0x70;
    filename[t_rec.long_name_ptr+17] = 0x0;
    filename[t_rec.long_name_ptr+18] = 0x33;
    filename[t_rec.long_name_ptr+19] = 0x0;
    //没有使用也要按照Winhex填写
    filename[t_rec.long_name_ptr+20] = 0x00;
    filename[t_rec.long_name_ptr+21] = 0x00;
    //12-13 没有使用也要按照Winhex填写
    filename[t_rec.long_name_ptr+22] = 0xff;
    filename[t_rec.long_name_ptr+23] = 0xff;
    filename[t_rec.long_name_ptr+24] = 0xff;
    filename[t_rec.long_name_ptr+25] = 0xff;
#endif
}
#endif

#pragma location="TASK_RECORD_SEG_1"
u8 task_record_copy_name(void)
{
#if (RECORD_FILE_NAME == SHORT_NAME)
    if(t_rec.rec_src == RECORD_MIC) {
        memcpy(filename, (char *)str_mic_rec_file, sizeof(str_mic_rec_file));
        return MIC_FNAME_NUM_OFS;
    } else if(t_rec.rec_src == RECORD_LINEIN) {
        memcpy(filename, (char *)str_aux_rec_file, sizeof(str_aux_rec_file));
        return AUX_FNAME_NUM_OFS;
    } else/* if(t_rec.rec_src == RECORD_FM)*/ {
        memcpy(filename, (char *)str_fm_rec_file, sizeof(str_fm_rec_file));
        return FM_FNAME_NUM_OFS;
    }
#else
    if(t_rec.rec_src == RECORD_MIC) {
        memcpy(filename, (char *)str_mic_rec_dir, sizeof(str_mic_rec_dir));
        t_rec.short_name_ptr = sizeof(str_mic_rec_dir) - 1;
    } else if(t_rec.rec_src == RECORD_LINEIN) {
        memcpy(filename, (char *)str_aux_rec_dir, sizeof(str_aux_rec_dir));
        t_rec.short_name_ptr = sizeof(str_aux_rec_dir) - 1;
    } else/* if(t_rec.rec_src == RECORD_FM)*/ {
        memcpy(filename, (char *)str_aux_fm_file, sizeof(str_aux_fm_file));
        t_rec.short_name_ptr = sizeof(str_fm_rec_dir) - 1;
    }
    t_rec.long_name_ptr = t_rec.short_name_ptr + 14;

    task_record_filename_make();
    return t_rec.short_name_ptr+1;
#endif
}

//根据文件编号新建文件
#pragma location="TASK_RECORD_SEG_1"
bool task_record_create(u16 file_num)
{
    u8 file_creat;
    u8 rec_fname_ofs;
    memset(filename,0,sizeof(filename));
    rec_fname_ofs = task_record_copy_name();
    while (1) {
#if (RECORD_FILE_NAME == LONG_NAME)
        file_creat = f_create_long_name(filename,t_rec.long_name_ptr);
#else
        file_creat = f_create(filename);
#endif
        if (file_creat == 1) {
            printf(str_rec_create_ok, filename);
            break;
        } else if(file_creat == 0) {
            if(!record_fname_inc((u8 *)&filename[rec_fname_ofs])) {
                printf(str_rec_create_fail, filename);
                return false;
            }
#if (RECORD_FILE_NAME == LONG_NAME)
            filename[t_rec.long_name_ptr+0] = filename[t_rec.short_name_ptr+1];
            filename[t_rec.long_name_ptr+2] = filename[t_rec.short_name_ptr+2];
            filename[t_rec.long_name_ptr+4] = filename[t_rec.short_name_ptr+3];
            filename[t_rec.long_name_ptr+6] = filename[t_rec.short_name_ptr+4];
#endif
        } else if (file_creat == 2) {
            printf(str_rec_disk_full);
            return false;
        }
    }
    return true;
}

#pragma location="TASK_RECORD_SEG_1"
bool task_record_dirmake(void)
{
    char dir_name[sizeof(str_mic_rec_dir)];
    memcpy(dir_name, (char *)str_mic_rec_dir, sizeof(str_mic_rec_dir));
    return f_mkdir(dir_name);
}

#pragma location="TASK_RECORD_SEG_1"
bool task_line_dirmake(void)
{
    char dir_name[sizeof(str_aux_rec_dir)];
    memcpy(dir_name, (char *)str_aux_rec_dir, sizeof(str_aux_rec_dir));
    return f_mkdir(dir_name);
}

#pragma location="TASK_RECORD_SEG_1"
bool task_fm_dirmake(void)
{
    char dir_name[sizeof(str_fm_rec_dir)];
    memcpy(dir_name, (char *)str_fm_rec_dir, sizeof(str_fm_rec_dir));
    return f_mkdir(dir_name);
}

//开始录音
#pragma location="TASK_RECORD_SEG_1"
u8 task_record_start(void)
{
    if(!record_mount_device()) {
        return 1;       //err
    }

    if(!t_rec.dir_open) {
        if (t_rec.rec_src == RECORD_MIC) {
            t_rec.dir_open = task_record_dirmake();
        } else if(t_rec.rec_src == RECORD_LINEIN) {
            t_rec.dir_open = task_line_dirmake();
        } else/* if(t_rec.rec_src == RECORD_FM)*/ {
            t_rec.dir_open = task_fm_dirmake();
        }
        if(!t_rec.dir_open) {
            return 1;   //make dir fail
        }
    }
    if (!fs_writable()) {
        return 1;
    }
    if (!t_rec.file_open) {
        if(!task_record_create(0)) {
            return 2;   //disk full
        }
        t_rec.file_open = 1;
    }
    return 0;
}

//暂停录音
#pragma location="TASK_RECORD_SEG_1"
void task_record_pause(void)
{
    mp3en_stop();
    f_sync();
    led_idle();
    t_rec.rec_sta = REC_STA_PAUSE;
    //printf("wr total: %08lx\n", WriteTotalData);
}

//继续录音
#pragma location="TASK_RECORD_SEG_1"
void task_record_continue(void)
{
    task_record_start();
    mp3en_start(true);
    led_busy();
    t_rec.sync_cnt = 0;      //同步
    t_rec.rec_sta = REC_STA_RECORD;
}

//停止录音
#pragma location="TASK_RECORD_SEG_1"
void task_record_stop(void)
{
    mp3en_stop();
    f_close();
    t_rec.file_open = 0;
    t_rec.dir_open = 0;
    t_rec.rec_sta = REC_STA_STOP;
}

//录音中事务处理
#pragma location="TASK_RECORD_SEG"
void task_recording_event(void)
{
    WATCHDOG_CLR();

    if(t_rec.rec_sta == REC_STA_RECORD) {
        u8 res = mp3en_write();
        if(res == 2) {
            show_full();
            WATCHDOG_CLR();
            delay_5ms(200);
            t_rec.rec_sta = REC_STA_STOPPING;           //磁盘已满，停止录音
            t_rec.play_flag = 1;
            t_rec.full_flag = 1;
        } else if(!res) {
            task_ctl.dev_change = 1;
            t_rec.rec_sta = REC_STA_STOPPING;           //录音出错，停止录音
        }
    }

#if LINEIN_DETECT_EN
    if(t_rec.rec_src == RECORD_LINEIN) {
        if (!device_activate(DEVICE_LINEIN)) {
            task_ctl.work_sta = TASK_EXIT;
            if(t_rec.rec_sta == REC_STA_RECORD) {
                t_rec.rec_sta = REC_STA_STOPPING;       //LINEIN拔掉，停止录音
            }
            return;
        }
    }
    else if(task_ctl.work_sta == TASK_AUX){             //插入LINE IN不切模式
        task_ctl.work_sta = task_ctl.last_work;
    }
#endif

    comm_event();
#if MUSIC_MODE_AUTO
    if (device_need_activate(DEVICE_SDMMC) || device_need_activate(DEVICE_UDISK)) {
        if(task_ctl.work_sta == TASK_MUSIC) {           //插入U盘或SD卡时不切模式
            task_ctl.work_sta = task_ctl.last_work;
        }
    }
#endif
    if (device_change()) {
        if(t_rec.rec_sta == REC_STA_RECORD) {
            t_rec.rec_sta = REC_STA_STOPPING;           //设备变动，停止录音
        }
    }

#if AUTO_STANDBY_EN || AUX_AUTO_STANDBY_EN || FM_AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((sys_ctl.sleep_cnt == 1 || sys_flag.ir_power_flag) && !sys_flag.lowpower_flag){
        if(t_rec.rec_sta == REC_STA_RECORD) {
            t_rec.rec_sta = REC_STA_STOPPING;           //低电/关机，停止录音
        }
    }
#endif
#if AUX_FREQ_SHOW
    if(t_rec.rec_src != RECORD_FM)
    {
        if (music_freq_get()) {//频谱计算完成
            music_freq_kick();//kick下一次频谱计算
        }
    }
#endif
}

//正在录音中任务消息处理
#pragma location="TASK_RECORD_SEG"
void task_recording_deal_msg(u8 msg)
{
    switch(msg) {
    //开始/暂停录音
    case KL_PLAY:
    case KL_PLAY_S10:
        if(t_rec.rec_sta == REC_STA_RECORD){
            task_record_pause();
        } else if(t_rec.rec_sta == REC_STA_PAUSE){
            task_record_continue();
        }
        break;

    //停止录音
    case KU_PLAY:
    case KU_PLAY_S10:
        t_rec.play_flag = 1;
        task_record_stop();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_S10:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_S10:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
        break;

    case QSYSTEM_1S:
        if(t_rec.rec_sta == REC_STA_RECORD){
            t_rec.rec_time++;                           //录音计时
            t_rec.sync_cnt++;
            t_rec.update_flag = 1;
            if (t_rec.sync_cnt >= RECORD_SYNC_CNT) {
                t_rec.sync_cnt = 0;                     //同步
                f_sync();
            }
        }
        deal_msg(msg);
        break;

        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}

//录音任务
#pragma location="TASK_RECORD_SEG"
void task_record(u8 record_source)
{
    t_rec.rec_src = record_source;
    task_record_enter();
    while (t_rec.rec_sta == REC_STA_RECORD || t_rec.rec_sta == REC_STA_PAUSE) {
        u8 msg = get_msg();
        task_recording_event();
        task_recording_deal_msg(msg);
        task_recording_display();
    }
    task_record_exit();
}

#endif
