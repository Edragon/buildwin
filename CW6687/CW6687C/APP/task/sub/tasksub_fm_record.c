/*****************************************************************************
 * Module    : TaskSub
 * File      : tasksub_fm_record.c
 * Author    : Hanny
 * Function  : FM任务 - 录音中任务
 *****************************************************************************/
#ifdef TASK_SUB

//录音任务调试使能
#define DBG_TASK_FMREC(...)          //printf(__VA_ARGS__)

IAR_XDATA_A u8 fm_wav_en_obuf[WAV_OUT_BUF_SIZE] @ "FM_WAV_ENCODE_BUF";
IAR_XDATA_A u8 fm_wav_header_buf[512]           @ "FM_WAV_HEADER_BUF";
IAR_XDATA_A u8 fm_rec_wbuf[512]                 @ "FM_WAV_HEADER_BUF";

#pragma constseg="TASK_FMREC_CONST"
IAR_CONST char str_fm_rec_dir[] = "RECORDF";

#if (RECORD_FILE_NAME == SHORT_NAME)

#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
IAR_CONST char str_fm_rec_file[] = "RECORDF/FM0001.WAV";
#else
IAR_CONST char str_fm_rec_file[] = "RECORDF/FM0001.MP3";
#endif

#define FM_FNAME_NUM_LEN            4                               //文件编号长度，4位
#define FM_FNAME_NUM_OFS            (sizeof(str_fm_rec_file)-8-1)   //文件编号位置，总长减去“0001.MP3”和\0

#else
#define FM_FNAME_NUM_LEN            4                               //文件编号长度，4位
#define FM_FNAME_NUM_OFS            sizeof(str_fm_rec_dir)          //文件编号位置，在"RECORDF/"之后

IAR_CONST u8 long_name_file[] = {
    //短文件名前8个字符不能使用空格“space”
    0x30, '0', '0', '1', 0xBD, 0xA8, 0x7E, 0x31, '.',
#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
    'w', 'a', 'v', '\0',
#else
    'm', 'p', '3', '\0',
#endif
    //长文件名,中文要参照Winhex查看
#if 1 //文件名是“0xxx建荣珠海有限公司-2013年4月2日.MP3”的示例
  //             0          0            1          建          荣          珠          海
    0x30, 0x00, '0',  0x00, '0',  0x00, '1',  0x00, 0xfa, 0x5e, 0x63, 0x83, 0xe0, 0x73, 0x77, 0x6d,
  //有          限          公          司          -           2           0           1
    0x09, 0x67, 0x50, 0x96, 0x6c, 0x51, 0xf8, 0x53, 0x2d, 0x00, 0x32, 0x00, 0x30, 0x00, 0x31, 0x00,
  //3           年          4           月          2           日          .
    0x33, 0x00, 0x74, 0x5e, 0x34, 0x00, 0x08, 0x67, 0x32, 0x00, 0xe5, 0x65, 0x2e, 0x00,

#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
    //W         A           V
    0x57, 0x00, 0x41, 0x00, 0x56, 0x00,
#else
    //M         P           3
    0x4D, 0x00, 0x50, 0x00, 0x33, 0x00,
#endif

#else //文件名是“0xxx建荣.MP3”的示例
//              1            0           0          建          荣          .
    0x30, 0x00, '1',  0x00, '0',  0x00, '0',  0x00, 0xfa, 0x5e, 0x63, 0x83, 0x2e, 0x00,

#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
    //W         A           V
    0x57, 0x00, 0x41, 0x00, 0x56, 0x00,
#else
    //M         P           3
    0x4D, 0x00, 0x50, 0x00, 0x33, 0x00,
#endif

    0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
#endif
};
#endif

IAR_CONST u16 tbl_fm_wav_spr[6] = {48000L, 32000L, 24000L, 16000L, 12000L, 8000L};
#pragma constseg=default

#pragma constseg="TASK_FMREC_CONST"
#if (REC_TYPE == REC_WAV_ADPCM)
IAR_CONST u8 tbl_fm_wav_head[52] = {
    0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,
    0x14, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x66, 0x61, 0x63, 0x74, 0xC8, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
#elif (REC_TYPE == REC_WAV_PCM)
IAR_CONST u8 tbl_fm_wav_head[52] = {
    0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,
    0x14, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x66, 0x61, 0x63, 0x74, 0xC8, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
#endif
#pragma constseg=default

#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
#pragma location="ADPCM_ENC"
__near_func void task_fmrec_process(void)
{
    if (t_fm_rec.block_cnt >= BLOCK_LIMIT) {
        //printf("err\n");
        return;
    }

    if (!t_fm_rec.pos) {
        adpcm_input = (u16)adc_buf;
    }
#if (REC_TYPE == REC_WAV_ADPCM)
    adpcm_encode(t_fm_rec.slice+t_fm_rec.pos, t_fm_rec.pos);
    if (t_fm_rec.pos) {
        t_fm_rec.block_cnt++;
        if (adpcm_output == ((u16)fm_wav_en_obuf+WAV_OUT_BUF_SIZE)) {
            adpcm_output = (u16)fm_wav_en_obuf;
        }
    }
#else
    else {
        adpcm_input = (u16)adc_buf + t_fm_rec.spl * adc_ch;
    }
    memcpy((u8 *)adpcm_output, (u8 *)adpcm_input, t_fm_rec.spl*adc_ch);
    adpcm_output += t_fm_rec.spl * adc_ch;
    t_fm_rec.block_cnt += adc_ch;
    if (adpcm_output == ((u16)fm_wav_en_obuf+WAV_OUT_BUF_SIZE)) {
        adpcm_output = (u16)fm_wav_en_obuf;
    }
#endif
    t_fm_rec.pos ^= 1;
}

#pragma location="TASK_FMREC_SEG"
bool fm_wav_record_init(void)
{
    //预留512byte写文件头
    my_memset((void *)fm_wav_header_buf, 0, 512);
    if(f_write((void *)fm_wav_header_buf)) {
        msgbox(STR_WRITE_DISABLE, NULL, MSGBOX_TIME);
        DBG_TASK_FMREC("write fail\n");
        return false;
    }
    t_fm_rec.file_ssect = clust2sect(fat_file.org_clust);
    fat_file.sclust = fat_file.org_clust;
    //printf("file_ssect1=%08lx, sclust=%08lx\n", t_fm_rec.file_ssect, fat_file.org_clust);

    t_fm_rec.spr = tbl_fm_wav_spr[SDADC_SPR_FREC];
    t_fm_rec.spl = FMREC_SAMPLES;
    t_fm_rec.pos = 0;
    t_fm_rec.block_cnt = 0;
    t_fm_rec.block_adr = fm_wav_en_obuf;
    t_fm_rec.slice = t_fm_rec.spl >> 4;

    adpcm_channel = adc_ch;
    adpcm_index[0] = 0;
    adpcm_index[1] = 0;
    adpcm_output = (u16)fm_wav_en_obuf;
    return true;
}

#pragma location="TASK_FMREC_SEG"
bool fm_wav_write_header(void *buf)
{
    t_fm_rec.file_size = fat_file.fsize;

    my_memset((void *)buf, 0, 512);
    smemcpy_c((void *)buf, (void __code *)tbl_fm_wav_head, 52);

    u32 *ptr0 = (u32 *)buf;
    ptr0[1] = t_fm_rec.file_size - 8;
    ptr0[6] = t_fm_rec.spr;
    ptr0[7] = (t_fm_rec.spr * BLOCK_SIZE) / t_fm_rec.spl;
    ptr0[12] = ((t_fm_rec.file_size - 512) * t_fm_rec.spl) / BLOCK_SIZE;
    ptr0[126] = 0x61746164;
    ptr0[127] = t_fm_rec.file_size - 512;

    u16 *ptr1 = (u16 *)buf;
    ptr1[11] = adpcm_channel;
    ptr1[16] = BLOCK_SIZE;
    ptr1[19] = t_fm_rec.spl;

    //printf("ssect=%08lx\n", t_fm_rec.file_ssect);
    if (fat_fs.disk_write(buf, t_fm_rec.file_ssect)) {
        return false;
    }
    return true;
}

#pragma location="TASK_FMREC_SEG"
bool fm_wav_close_file(void)
{
    if(!fm_wav_write_header(fm_wav_header_buf)) {
        return false;
    }
    if(!f_close()) {
        return false;
    }
    return true;
}

#pragma location="TASK_FMREC_SEG"
bool fm_sync_file(void)
{
    u8 *buf = fm_wav_header_buf;
    if (!move_window(0)) {
        return false;
    }

    if(!fm_wav_write_header(buf)) {
        return false;
    }
    if(fat_fs.disk_read(buf, fat_file.dir_sect)) {
        return false;
    }
    u8 *dir;
    dir = (u8 *)buf + (fat_file.dir_ptr - fat_fs.win);
    if(fat_fs.fs_type == FS_EXFAT) {
        mkitem_exfat(dir, (char *)fat_file.fname, AM_ARC, fat_file.org_clust, t_fm_rec.file_size);
    } else {
        mkitem_fat(dir, (char *)fat_file.fname, dir[DIR_Attr]|AM_ARC, fat_file.org_clust, t_fm_rec.file_size);
    }
    if(fat_fs.disk_write(buf, fat_file.dir_sect)) {
        return false;
    }
    return true;
}

#pragma location="TASK_FMREC_SEG"
void fm_record_stop(void)
{
    adc_stop();
    if (!fm_wav_close_file()) {
        DBG_TASK_FMREC("close Fail!\n");
    } else {
        DBG_TASK_FMREC("close OK!\n");
    }
}

#pragma location="TASK_RECORD_WRITE_SEG"
void tasksub_fm_record_write(void)
{
    if (t_fm_rec.block_cnt < 4) {
        return;
    }

    memcpy(fm_rec_wbuf, t_fm_rec.block_adr, 512);
    u8 res = f_write(fm_rec_wbuf);
    //printf("r=%02x\n", res);
    if (res) {
        if (res == 6) {
            //磁盘已满，停止录音
            DBG_TASK_FMREC("disk full!\n");
            task_fm_record_stop();
            t_fm_rec.play_flag = 1;
            //t_fm.sta = FM_PLAY;           //磁盘已满，停止录音
        } else {
            //录音出错，停止录音
            DBG_TASK_FMREC("write err!\n");
           // wav_record_abort();
            task_ctl.dev_change = 1;
            task_fm_record_stop();
            //t_fm_rec.file_open = 0;
            //t_fm.sta = FM_PLAY;           //录音出错，停止录音
        }
        return;
    }

    t_fm_rec.block_cnt -= 4;
    t_fm_rec.block_adr += 0x200;
    if (t_fm_rec.block_adr == &fm_wav_en_obuf[WAV_OUT_BUF_SIZE]) {
        t_fm_rec.block_adr = fm_wav_en_obuf;
    }
}

#else

#pragma location="MP3_EN_COM"
__near_func void task_fmrec_process(void)
{
    //printf("rec: %04x\n", (u8 *)0x8000+(t_fm_rec.adc_sample_cnt*2));
    if (!pcm_valid) {
        memcpy((u8 *)0x8000+(t_fm_rec.adc_sample_cnt*2*adc_ch), adc_buf+(t_fm_rec.pos*t_fm_rec.spl*adc_ch), t_fm_rec.spl*adc_ch);
        t_fm_rec.adc_sample_cnt += t_fm_rec.spl/2;
        if (t_fm_rec.adc_sample_cnt >= 576) {
            t_fm_rec.adc_sample_cnt -= 576;
            pcm_valid = 1;
            KICK_SOFT_INT();

        }
    } else {
        //printf("D");
    }
    t_fm_rec.pos ^= 1;
}

#pragma location="TASK_FMREC_SEG"
void mp3_fm_record_init(void)
{
    t_fm_rec.spl = FMREC_SAMPLES;
    t_fm_rec.pos = 0;
    t_fm_rec.adc_sample_cnt = 0;
    mp3en_init();
}

#pragma location="TASK_RECORD_WRITE_SEG"
void tasksub_fm_record_write(void)
{
    u8 try_cnt = 50;
    //printf("L: %04x\n", mp3en_out_len);
    while (mp3en_out_len >= 512 && try_cnt--) {
        u8 res = f_write(mp3en_rptr);
        //printf("t%d\n", res);
        if (res) {
            if (res == 6) {
                //磁盘已满，停止录音
                DBG_TASK_FMREC("disk full!\n");
                task_fm_record_stop();
                t_fm_rec.play_flag = 1;
                //t_fm.sta = FM_PLAY;           //磁盘已满，停止录音
            } else {
                //录音出错，停止录音
                DBG_TASK_FMREC("write err!\n");
                // wav_record_abort();
                task_ctl.dev_change = 1;
                task_fm_record_stop();
                //t_fm_rec.file_open = 0;
                //t_fm.sta = FM_PLAY;           //录音出错，停止录音
            }
            return;
        }

        mp3en_rptr += 512;
        IE_EA = 0;
        mp3en_out_len -= 512;
        IE_EA = 1;
        if (mp3en_rptr >= (u8 *)MP3EN_OUT_END) {
            mp3en_rptr  = (u8 *)MP3EN_BUF_BEG;
        }
    }
}

#pragma location="TASK_FMREC_SEG"
bool fm_sync_file(void)
{
    return f_sync();
}

#pragma location="TASK_FMREC_SEG"
void fm_record_stop(void)
{
    adc_stop();
    if (!f_close()) {
        DBG_TASK_FMREC("close Fail!\n");
    } else {
        DBG_TASK_FMREC("close OK!\n");
    }
}
#endif

//FM录音同步，1s消息处理函数
#pragma location="TASK_FMREC_SEG"
void task_fm_record_sync(void)
{
    t_fm_rec.rec_time++;                           //录音计时
    if (t_fm_rec.sync_cnt++ < REC_SYNC_CNT) {
        return;
    }
    t_fm_rec.sync_cnt = 0;                     //同步
    if (!fm_sync_file()) {
        //printf("file sync Fail!\n");
        adc_stop();
    }
}

#pragma location="TASK_FMREC_SEG"
bool fm_record_fname_inc(u8 *buf)
{
    u8 i;
    u8 *p = buf + (FM_FNAME_NUM_LEN-1);

    *p += 1;
    for(i=0; i<(FM_FNAME_NUM_LEN-1); i++) {
        if(*p > '9') {
            *p-- = '0';
            *p += 1;
        } else {
            return true;
        }
    }

    return (*p > '9')? false : true;
}

#pragma location="TASK_FMREC_SEG"
u8 task_fm_record_copy_name(void)
{
#if (RECORD_FILE_NAME == SHORT_NAME)
    memcpy(filename, (char *)str_fm_rec_file, sizeof(str_fm_rec_file));
    return FM_FNAME_NUM_OFS;
#else
    memcpy(filename, (char *)str_fm_rec_file, sizeof(str_fm_rec_file));
    t_fm_rec.sname_ptr = FM_FNAME_NUM_OFS;
    t_fm_rec.lname_ptr = t_fm_rec.sname_ptr + 13;
    //短文件名使用的是gbk编码，长文件名使用的是unicode编码
    filename[t_fm_rec.sname_ptr-1]='/';
    smemcpy_c((void *)(filename+t_fm_rec.sname_ptr), (void __code *)long_name_file, sizeof(long_name_file));
    return t_fm_rec.sname_ptr;
#endif
}

//根据文件编号新建文件
#pragma location="TASK_FMREC_SEG"
bool task_fm_record_create(void)
{
    u8 file_creat = 0;
    t_fm_rec.file_open = 0;
    u8 rec_fname_ofs = task_fm_record_copy_name();
    while (1) {
#if (RECORD_FILE_NAME == LONG_NAME)
        file_creat = f_create_long_name(filename,t_fm_rec.lname_ptr);
#else
        file_creat = f_create(filename);
#endif
        if (file_creat == 1) {
            //printf("create %s\n", filename);
            break;
        } else if(file_creat == 0) {
            if(!fm_record_fname_inc((u8 *)&filename[rec_fname_ofs])) {
                //printf(str_rec_create_fail, filename);
                return false;
            }
#if (RECORD_FILE_NAME == LONG_NAME)
            filename[t_fm_rec.lname_ptr+0] = filename[t_fm_rec.sname_ptr+0];
            filename[t_fm_rec.lname_ptr+2] = filename[t_fm_rec.sname_ptr+1];
            filename[t_fm_rec.lname_ptr+4] = filename[t_fm_rec.sname_ptr+2];
            filename[t_fm_rec.lname_ptr+6] = filename[t_fm_rec.sname_ptr+3];
#endif
        } else if (file_creat == 2) {
            //printf(str_rec_disk_full);
            return false;
        }
    }
    t_fm_rec.file_open = 1;
    return true;
}

//创建FM录音目录
#pragma location="TASK_FMREC_SEG"
bool task_fm_record_dirmake(void)
{
    char dir_name[sizeof(str_fm_rec_dir)];
    memcpy(dir_name, (char *)str_fm_rec_dir, sizeof(str_fm_rec_dir));
    return f_mkdir(dir_name);
}

//开始录音
#pragma location="TASK_FMREC_SEG"
u8 task_fm_record_start(void)
{
    if(!record_mount_device()) {
        return 1;       //err
    }
    if(!t_fm_rec.dir_open) {
#if IS_LCD_DISPLAY
        draw_wait();            // 这样做就只会出现一次"请稍后"，符合常理
#endif
        t_fm_rec.dir_open = task_fm_record_dirmake();
        if(!t_fm_rec.dir_open) {
            return 1;   //make dir fail
        }
    }
    if (!fs_writable()) {
        return 1;
    }
    if (!t_fm_rec.file_open) {
        if(!task_fm_record_create()) {
            return 2;   //disk full
        }
#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
        if (!fm_wav_record_init()) {
            return 1;
        }
#endif
        t_fm_rec.file_open = 1;
    }
#if (REC_TYPE == REC_MP3)
    mp3_fm_record_init();
#endif
    return 0;
}

//暂停录音
#pragma location="TASK_FMREC_SEG"
void task_fm_record_pause(void)
{
    adc_stop();
    fm_sync_file();
    t_fm_rec.pos = 0;
#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
    t_fm_rec.block_cnt = 0;
    t_fm_rec.block_adr = fm_wav_en_obuf;
    adpcm_index[0] = 0;
    adpcm_index[1] = 0;
    adpcm_output = (u16)fm_wav_en_obuf;
#endif
    t_fm_rec.sync_cnt = 0;
    t_fm.fm_sta = FM_REC_PAUSE;
    task_fm_reuse_init();
    load_code(0x28, 3);
    task_fm_load();
}

//停止录音
#pragma location="TASK_FMREC_SEG"
void task_fm_record_stop(void)
{
    if(t_fm_rec.file_open) {
    	fm_record_stop();
	}
    t_fm_rec.file_open = 0;
    t_fm_rec.dir_open = 0;
    t_fm_rec.rec_time = 0;                                 //时间清0
    t_fm.fm_sta = FM_REC_PLAY;
    load_code(0x28, 3);
    task_fm_load();
}

//录音中事务处理
#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_event(void)
{
    WATCHDOG_CLR();
    comm_event();

    tasksub_fm_record_write();

#if MUSIC_MODE_AUTO
    if (device_need_activate(DEVICE_SDMMC) || device_need_activate(DEVICE_UDISK)) {
        if(task_ctl.work_sta == TASK_MUSIC) {           //插入U盘或SD卡时不切模式
            task_ctl.work_sta = task_ctl.last_work;
        }
    }
#endif
    if (device_change()) {
        if(t_fm.fm_sta == FM_RECORD) {
            t_fm_rec.play_flag = 0;
            task_fm_record_stop();
            t_fm.fm_sta = FM_PLAY;           //设备变动，停止录音
        }
    }
}

//录音控制器初始化
#pragma location="TASK_FMREC_SEG"
void task_fm_record_init(void)
{
    //关闭预分配
    fat_fs.limit_clust = 0;
    memset(&t_fm_rec, 0, sizeof(t_fm_rec));

    //录音控制器初始化
    adc_ch = ((FM_CHANNEL_SEL & 0x80) >> 7) + 1;
    adc_init(SEL_RECFM);
    sys_ctl.sleep_cnt = 0xffff;
#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
    wav_load_code(0x60);
#else
    mp3en_load_code(0x80);
#endif
}

//录音任务初始化
#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_enter(void)
{
    u8 res;
    set_sys_clk(SYS_24M);
    //播放状态
    ocx_clear();
    led_busy();
#if !IS_LCD_DISPLAY
    draw_wait();
#endif
    task_fm_reuse_exit();
    if (t_fm.fm_sta != FM_REC_PAUSE) {
        task_fm_record_init();
    }

    res = task_fm_record_start();
    flush_msg();                        //清空消息池
    if(res == 2) {
        show_full();
        //printf(str_rec_err);
        WATCHDOG_CLR();
        delay_5ms(200);
        return;
    } else if(res == 1) {
        show_err();
        t_fm.fm_sta = FM_PLAY;
        //printf(str_rec_err);
        WATCHDOG_CLR();
        delay_5ms(200);
        return;
    } else {
        //printf("rec_start\n");
        adc_start();
        t_fm.fm_sta = FM_RECORD;
        ocx_show_cnt = 0;//控制录音闪烁时间
#if IS_LCD_DISPLAY
#if THEME_SELECT==THEME_LCD_DEFAULT
        lcd_buf = lcd_buf4;   // lcd使用小buf，否则会花屏
#endif
        // 清除"请稍候"字样
        draw_clear();                                       //清空屏显
        line_clear();
        line_update(2);
        line_update(4);
        t_fm_rec.updata_flag = 1;      // 设置更新标志,用于显示正在录音图标
#endif
    }
}

//退出录音任务
#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_exit(void)
{
    if (t_fm.fm_sta != FM_RECORD && t_fm.fm_sta != FM_REC_PAUSE) {
        task_fm_record_stop();
    }
    if(t_fm_rec.play_flag) {
        memcpy(filename, &filename[sizeof(str_fm_rec_dir)], 13);
    }
#if IS_LCD_DISPLAY
    t_fm_rec.updata_flag = 1;      // 设置更新标志,用于显示暂停或者停止图标
#endif
    led_idle();
    ocx_clear();
}

#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record_loop(void)
{
    u8 msg;
    while ((task_ctl.work_sta == TASK_FM) && (t_fm.fm_sta == FM_RECORD)) {
        tasksub_fm_record_event();
        while (msg = get_msg()) {
            tasksub_fm_record_deal_msg(msg);
        }
        tasksub_fm_record_display();
    }
}

#pragma location="TASK_FMREC_SEG"
void tasksub_fm_record(void)
{
    //printf("tasksub_fm_record\n");
    tasksub_fm_record_enter();
    tasksub_fm_record_loop();
    tasksub_fm_record_exit();
    //printf("tasksub_fm_record exit\n");
}
#endif
