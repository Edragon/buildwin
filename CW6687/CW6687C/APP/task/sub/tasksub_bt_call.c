/*****************************************************************************
 * Module    : TaskSub
 * File      : tasksub_bt_call.c
 * Author    : Hanny
 * Function  : 蓝牙任务 - 通话任务
 *****************************************************************************/
#ifdef TASK_SUB

#define SCO_FARPCM_SIZE         (0x800/2)      // unit 16bits

//未调通的函数
#define mp3en_start(...)
#define mp3en_stop(...)
#define mp3en_exit(...)

extern u8 uartTxQ3[64];

extern s16 *sco_farpcm_wptr;
extern s16 *sco_farpcm_rptr;
extern u16  sco_farpcm_len;
IAR_XDATA_A s16 sco_farpcm[SCO_FARPCM_SIZE] @ "BT_SCOFAR_XDATA";   // sco远端的PCM数据,用于简单抑制回音, BT_SCOFAR_XDATA与BT_AVDTP_XDATA复用

u8 far_vol;
s16 far_noise_pow_cnt;
u8 far_noise_pow;

s16 *sco_farpcm_wptr;
s16 *sco_farpcm_rptr;
u16  sco_farpcm_len;

#define SCO_DELAY (sizeof(sco_spk_vad))
IAR_XDATA_A u8 sco_spk_vad[1] @ "SCO_VAD_BUF";
u8 sco_spk_vad_last, sco_spk_vad_index;
u8 sco_spk_vad_mute;                    //远端停止后，近端MIC再MUTE一小段时间
u8 sco_far_vad, sco_far_vad_last;
u8 sco_near_mic_mute;                       //远端停止后，近端再以低优先级模式MUTE一小段时间
//u8 sco_mic_vad;
u8 sco_spk_stable;

u8 sco_mic_pow, sco_mic_stable;

u8 sco_cal_index;
s8 sco_cal_max, sco_cal_min;

extern u8 aec_status;
extern u8 adc_data_pos;
extern volatile u8 aec_proc_kick;
extern volatile u8 aec_post_kick;
extern __near_func void aec_init(void);
extern __near_func void aec_process(void);

void bt_mic_init(void)
{
    adc_init(SEL_BT_MIC);
    adc_start();
}

#pragma location="BT_SCO_SEG"
__near_func u8 bt_voice_sco(void)
{
    u8 low, high;
    while (sco_farpcm_len && AUCON5 < 0x88) {
        WATCHDOG_CLR();
        s16 sample = *sco_farpcm_rptr++;
        low  = (u8)(sample);
        high = (u8)(sample>>8);
        if (sco_farpcm_rptr >= &sco_farpcm[SCO_FARPCM_SIZE]) {
            sco_farpcm_rptr = sco_farpcm;
        }

        bool ie_ea_bak = IE_EA;
        IE_EA = 0;
        sco_farpcm_len--;
        IE_EA = ie_ea_bak;

        while(!(AUCON7 & BIT(7)));
        AUCON5 = low;
        AUCON5 = high;
        while(!(AUCON7 & BIT(7)));
        AUCON5 = low;
        AUCON5 = high;
    }
    return 0;
}

__near_func void bt_voice_softisr(void)
{
    if (flag_btmode && scoflag) {
#if !BT_CALL_PRIVATE
        //强制私密接听，不需要这一段
        bt_voice_sco();
#endif
        if (aec_proc_kick) {
            aec_proc_kick = 0;
            aec_process();
        }
    }
}

//设置通话音量接口
extern bool send_at_vgs(u8 vol);
bool send_hshfvol_hook(void) @ "BT_RFC_TX"
{
    return send_at_vgs(sys_ctl.hshf_vol);
}

//手机端改变通话音量接口
void hshfvol_change_hook(u8 vol) @ "BT_RXAT_CMD"
{
#if !BT_CALL_PRIVATE
    user_set_hshfvol(vol);
    put_msg(Q_HSHF_VOL_CHANGE);
#endif
}

extern u8 resample_loopnum;
void resample_filter_init(void);

#pragma location="TASK_BT_CALL_SEG"
void tasksub_bt_call_enter(void)
{
#if WARNING_VOL_MAXMIN
    if(sys_ctl.vol_maxmin_music_play == 1) {
        sys_ctl.vol_maxmin_music_play = 0;
        bt_play_continue();
    }
#endif
    task_ctl.work_sub = TASK_BT_CALL;
    bt_enter_call();
#if !BT_CALL_PRIVATE
    user_change_volume(sys_ctl.hshf_vol);
#endif
#if IS_LCD_DISPLAY
    t_bt.updata_flag = 1;
#endif
#if THEME_SELECT==THEME_LCD_DEFAULT
    //lcd_buf=lcd_buf2;   // 蓝牙响铃时使用lcd_buf2，否则会花屏
#endif
}

extern u16 avdtp_sbc_len;
#pragma location="TASK_BT_CALL_SEG"
void tasksub_bt_call_exit(void)
{
#if !BT_CALL_PRIVATE
    user_change_volume(sys_ctl.volume);
#endif
    bt_exit_call();

    task_bt_sta_update();
#if DAC_MIX_OUT_EN
   dac_left_right_mix_out();
#endif
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf3;   // bt播放音乐使用lcd_buf3，否则会花屏
#endif
#if IS_LCD_DISPLAY
    task_bt_call_ended();
    t_bt.updata_flag = 1;
    set_num_len(0);   // 清电话号码的长度
#endif
#if WARNING_VOL_MAXMIN
    if(sys_ctl.vol_maxmin_music_flag == 1) {
        sys_ctl.vol_maxmin_music_flag = 0;
    }
#endif
}

#pragma location="TASK_BT_CALL_SEG"
void tasksub_bt_call_event(void)
{
    WATCHDOG_CLR();
    comm_event();

#if 0//WARNING_LOWPOWER
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        mp3res_play_btmode(RES_MP3_PLEASE_CHARGE);
    }
#endif
#if BT_CALL_PRIVATE
    //强制私密接听
    if (sys_ctl.bt_sta == BT_STA_OUTGOING || sys_ctl.bt_sta == BT_STA_PHONE) {
        bt_sco_disconnect();
    }
#endif

    if (!(sys_ctl.bt_sta == BT_STA_OUTGOING || sys_ctl.bt_sta == BT_STA_PHONE)) {
        task_ctl.work_sub = TASK_BT_NULL;
    }
}

#pragma location="TASK_BT_CALL_SEG"
void tasksub_bt_call_loop(void)
{
    u8 msg;
    while (task_ctl.work_sta == TASK_BT && task_ctl.work_sub == TASK_BT_CALL) {
        tasksub_bt_call_event();
        while (msg = get_msg()) {
            task_bt_call_deal_msg(msg);
        }
#if IS_LCD_DISPLAY
        task_bt_call_display();
#else
        task_bt_display();
#endif
    }
}

#pragma location="TASK_BT_CALL_SEG"
void tasksub_bt_call(void)
{
    //printf("tasksub_bt_call\n");
    tasksub_bt_call_enter();
    tasksub_bt_call_loop();
    tasksub_bt_call_exit();
    //printf("tasksub_bt_call exit\n");
}

#endif
