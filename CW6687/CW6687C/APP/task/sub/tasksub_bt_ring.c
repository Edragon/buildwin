/*****************************************************************************
 * Module    : TaskSub
 * File      : tasksub_bt_ring.c
 * Author    : Hanny
 * Function  : 蓝牙任务 - 响铃任务
 *****************************************************************************/
#ifdef TASK_SUB

struct {
    u8 num_buf[24]; //响铃BUF //32
    u8 num_len;
    u8 num_cur;
} t_bt_ring;

#pragma constseg="TASK_BT_RING_CONST"
IAR_CONST u8 tbl_ring_mp3[] = {
    RES_MP3_NUM0,
    RES_MP3_NUM1,
    RES_MP3_NUM2,
    RES_MP3_NUM3,
    RES_MP3_NUM4,
    RES_MP3_NUM5,
    RES_MP3_NUM6,
    RES_MP3_NUM7,
    RES_MP3_NUM8,
    RES_MP3_NUM9,
};

#pragma constseg=default


//SECTION(".HSF_BANK_TEXT")
void BT_IncomingCallNumber(u8* pnum, u8 len)
{
    if (t_bt_ring.num_len == 0) {
#if (!BT_CALL_PRIVATE && BT_RING_NUMBER)
        u8 i = 0;
        for(i = 0; i < len; i++){
            //IncomingCallNo[i] = pnum[i];
            t_bt_ring.num_buf[i] = pnum[i]-0x30;
            //bt_voice_put(BT_VOICE_NUM0 + pnum[i]-0x30);
        }
        //IncomingCallNo[i] = 0;

        t_bt_ring.num_cur = 0;
        t_bt_ring.num_len = len;
#if IS_LCD_DISPLAY
        t_bt.updata_flag = 1;
#endif
#endif
    }
}

#pragma location="TASK_BT_RING_SEG"
u8 get_num_len(void)
{
    return t_bt_ring.num_len;
}

#pragma location="TASK_BT_RING_SEG"
void set_num_len(u8 num)
{
    t_bt_ring.num_len = num;
}

#pragma location="TASK_BT_RING_SEG"
u8 *get_num_buf(void) {
    return t_bt_ring.num_buf;
}

#pragma location="TASK_BT_RING_SEG"
void tasksub_bt_ring_enter(void)
{
#if WARNING_VOL_MAXMIN
    if(sys_ctl.vol_maxmin_music_play == 1) {
        sys_ctl.vol_maxmin_music_play = 0;
        bt_play_continue();
    }
#endif
    task_ctl.work_sub = TASK_BT_RING;

    btdec_stop();                               //停止当前的蓝牙音乐播放
#if MIC_DIRECT_OUT_EN
    mic_direct_out_exit();
#endif
    dac_dynamic(DYNAMIC_NO);                    //响铃时，不需要动态降噪
#if !IS_LCD_DISPLAY
    t_bt_ring.num_len = 0;
#endif
    btmode_exit();
    user_change_volume(WARNING_VOLUME);
    sys_ctl.voice_play_flag = 1;
    mp3_res_play(RES_MP3_RING);
#if IS_LCD_DISPLAY
    t_bt.updata_flag = 1;
    t_bt.incoming_call_disp = 1;    // 没接收到电话号码时显示Incoming
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf2;   // lcd使用小buf，否则会花屏
#endif
#endif
}

#pragma location="TASK_BT_RING_SEG"
void tasksub_bt_ring_exit(void)
{
    music_stop();
    memset((u8*)0x4000,0,0x800);
    btdec_load_code(0x10, 3);
    btmode_enter();
    if(!t_bt.add_sco_flag) {
        dac_dynamic(BT_DYNAMIC_NOISE);                  //结束响铃，开启动态降噪
#if MIC_DIRECT_OUT_EN
        mic_direct_out_enter(MIC_CHANNEL_SEL,11);
        btdec_play();
#endif
    }
    t_bt.add_sco_flag = 0;
    sys_ctl.voice_play_flag = 0;
    user_change_volume(sys_ctl.volume);
    task_bt_sta_update();
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf3;   // bt播放音乐使用lcd_buf3，否则会花屏
#endif
#if WARNING_VOL_MAXMIN
    if(sys_ctl.vol_maxmin_music_flag == 1) {
        sys_ctl.vol_maxmin_music_flag = 0;
    }
#endif
}

#pragma location="TASK_BT_RING_SEG"
void tasksub_bt_ring_event(void)
{
    WATCHDOG_CLR();
    comm_event();
#if WARNING_LOWPOWER
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        mp3_res_play_wait(RES_MP3_PLEASE_CHARGE);
    }
#endif
    if (sys_ctl.bt_sta != BT_STA_INCOMING) {
        task_ctl.work_sub = TASK_BT_NULL;
#if IS_LCD_DISPLAY
        if(sys_ctl.bt_sta != BT_STA_PHONE){ //来电手机端拒接时需要清一下号码长度，否则之后来电都不会获取电话号码进行报号
            t_bt.updata_flag = 1;
            t_bt_ring.num_len = 0;          //清电话号码的长度
        }
#endif
        return;
    }
    music_event();
    if (music_get_status() <= STATUS_PLAY_STOPPING && AUCON5 == 0) {
#if BT_RING_NUMBER
        if (t_bt_ring.num_cur < t_bt_ring.num_len) {
            mp3_res_play(tbl_ring_mp3[t_bt_ring.num_buf[t_bt_ring.num_cur]]);
            t_bt_ring.num_cur++;
        } else {
            mp3_res_play(RES_MP3_RING);
        }
#endif
    }
}

#pragma location="TASK_BT_RING_SEG"
void tasksub_bt_ring_loop(void)
{
    u8 msg;
    while (task_ctl.work_sta == TASK_BT && task_ctl.work_sub == TASK_BT_RING) {
        tasksub_bt_ring_event();
        while (msg = get_msg()) {
            task_bt_ring_deal_msg(msg);
        }
#if IS_LCD_DISPLAY
        task_bt_call_display();
#else
        task_bt_display();
#endif
    }
}

#pragma location="TASK_BT_RING_SEG"
void tasksub_bt_ring(void)
{
    //printf("tasksub_bt_ring\n");
    tasksub_bt_ring_enter();
    tasksub_bt_ring_loop();
    tasksub_bt_ring_exit();
    //printf("tasksub_bt_ring exit\n");
}



#endif
