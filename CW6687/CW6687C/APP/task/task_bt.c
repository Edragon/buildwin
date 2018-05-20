/*****************************************************************************
 * Module    : Task
 * File      : task_bt.c
 * Author    : Hanny
 * Function  : 蓝牙任务流程
 *****************************************************************************/
#include "include.h"

#if (TASK_BT_EN | TASK_BTHID_EN)
#include "task.h"
#include "user_bt_pbap.h"

#define DEBUG_TASK_BT        0

//蓝牙任务子任务
enum {
    TASK_BT_NULL,
    TASK_BT_TEST,               //测试盒任务
    TASK_BT_RING,               //响铃任务
    TASK_BT_CALL,               //通话任务
    TASK_BT_LOST,               //蓝牙断开，主要做手机防丢
};
bool bt_test_flag = false;
IAR_XDATA_A type_bt t_bt @ "BT_XDATA";
void test_bt(void);
bool btdec_load_code(u16 addr, u8 segnum);
void task_bt_sta_update(void);
extern IAR_BIT_A bool flag_btmode;            //为1时启用完整的蓝牙功能
extern IAR_BIT_A bool scoflag;  //for nokia 5000 use sco play music
extern u8 bt_mic_param[10];
extern u8 work_sta_interrupt;          //电话中断标志
extern void bt_play_vol_music(void);
void bt_play_continue(void);
void lock_pll48m(u8 lock);

#pragma constseg="TASK_BT_MINI_SEG_CONST"
IAR_CONST char str_show_taskbt[] = "task_bt\n";
IAR_CONST char str_show_taskbtexit[] = "task_bt_exit\n";
#pragma constseg=default

#define TASK_SUB
#include SET_USER_PATH(CFG_UI_BT_DIR, /ui/ui_bt.c)          //UI控制
#include "sub/tasksub_bt_ring.c"                            //响铃子任务
#include "sub/tasksub_bt_call.c"                            //通话子任务
#include "sub/tasksub_bt_test.c"                            //测试盒子任务
#include "sub/tasksub_bt_menu.c"                            //菜单子任务
#include "sub/tasksub_bt_lost.c"                            //防丢子任务

//#pragma location="TASK_BT_SEG"
void task_bt_enter(void)
{
    task_ctl.work_sub = TASK_NULL;
    set_sys_clk(SYS_48M);                                   //蓝牙任务的系统时钟
#if !BT_ALL_TASK
    user_bt_init();
    bt_connect();                                           //进入蓝牙任务，发起回连
    sys_ctl.bt_sta = bt_status_get();
    sys_ctl.connected = (sys_ctl.bt_sta >= BT_STA_CONNECTED) ? 1 : 0;
#else
    btinfo_get_mic(bt_mic_param);
    btinfo_get_voice(bt_mic_param+2);
#endif
    sys_ctl.bt_sta = bt_status_get();

    //sound_ctl_init(3, 0x80, 25, 0x40);        //连续3次超过0x80认为开始有声；连续25次0x40认为静音
    memset(&t_bt, 0, sizeof(t_bt));
#if IS_LCD_DISPLAY
    t_bt.updata_flag = 1;   //更新显示标志
#endif
    //先更新显示
    task_bt_display();

    t_bt.disp_bt_sta = 0xff;
    t_bt.disp_dev_bat = 0xff;
    t_bt.add_sco_flag = 0;
    led_bt_none();
    dac_enable();

#if WARNING_TASK_BT
    led_bt_voice();
    if(work_sta_interrupt == TASK_NULL){
        mp3_res_play_wait(RES_MP3_BT_MODE);
    }
#endif

#if BT_DYNAMIC_NOISE
    sound_ctl_init(8, 0x08, 100, 0x03);        //连续10次超过0x08认为开始有声；连续100次0x03以下认为静音
    sound_set_flag(0, 0);
#endif

#if BT_VOLUME_MAX
    user_set_volume(VOLUME_MAX);
#endif

    btdec_load_code(0x10, 3);
    btmode_enter();

#if (DISPLAY_SEL != DISPLAY_NO)
#if IS_LEDSEG_DISPLAY
    ledseg_ocx.disp_flag = 1;
#endif
#endif
    sys_ctl.sleep_cnt = 0xffff;
    sys_ctl.vol_maxmin_music_flag = 0;
    sys_ctl.vol_maxmin_music_play = 0;
    phonebook_total_num = sys_param_read16(PARAM_PHONEBOOK_NUM);
#if IR_HARD_POWERDOWN_EN
    sys_ctl.ir_power_flag = 0;
#endif
    dac_dynamic(BT_DYNAMIC_NOISE);              //启用动态降噪
#if BT_ALL_TASK
    delay_5ms(60);  //需要延时一段时间，动态检测才能获取蓝牙P/P状态
    task_bt_sta_update();
#endif
#if MIC_DIRECT_OUT_EN
    mic_direct_out_enter(MIC_CHANNEL_SEL,11);
    btdec_play();
#endif

#if IS_LCD_DISPLAY
    ocx_init(task_bt_event, NULL);  // 初始化控件，能在调音量时也处理蓝牙事件
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf3;   // bt播放音乐使用lcd_buf3，否则会花屏
#endif
#endif
}

void bt_close(void)
{
    void BtMainLoop(void);
    bt_disconnect();
    while (bt_status_get() >= BT_STA_DISCONNECTING) {
        WATCHDOG_CLR();
        user_bt_event();
    }
    bt_set_scan(0, 0);
    btmode_exit();
    user_bt_off();
    sys_ctl.voice_play_flag = 0;
    BTLDO_DIS();
}

//退出蓝牙任务
#pragma location="TASK_BT_SEG_1"
void task_bt_exit(void)
{
    t_bt.hid_enable = 0;
#if TASK_BT_EN
#if BT_ALL_TASK
    if (sys_ctl.bt_sta == BT_STA_PLAYING) {     //后台时切换到其它模式暂停手机播放
        bt_pause_play();
        while (bt_get_status() == BT_STA_PLAYING) {
            bt_event();
            delay_5ms(30);
            WATCHDOG_CLR();
        }
    }
    if(bt_test_flag){
        bt_test_flag = false;
        user_bt_init();
        bt_reset();
    }else{
        btmode_exit();
    }
#else
    if(bt_testmode()) {
        bt_test_flag = false;
        bt_reset();
        user_bt_off(); //关闭蓝牙通信
    } else {
        bt_close();
    }
#endif
    btdec_stop();
#endif
    dac_dynamic(DYNAMIC_NO);                    //退出蓝牙，关闭动态降噪
#if MIC_DIRECT_OUT_EN
    mic_direct_out_exit();
#endif
    sys_unmute();
    dac_disable();
    amux_exit();
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf1;   // 退出时，lcd恢复使用大buf
    work_sta_interrupt = TASK_NULL;
#endif
    if (device_is_online(DEVICE_UDISK)) {
        lock_pll48m(1);                         //U盘在线，则锁定PLL
        usb_host_reactive();
        delay_5ms(80);
    } else {
        lock_pll48m(0);                         //U盘不在线，则不锁定PLL
    }
}

#if BT_AUTO_PLAY
u8 temp_remote_dev_addr[6];
#endif
//蓝牙事件
#pragma location="TASK_BT_MINI_SEG"//"TASK_BTEVENT_SEG"
void task_bt_event(void)
{
    comm_event();
    if (work_sta_interrupt) {
        if(sys_ctl.bt_sta == BT_STA_PLAYING) {
            work_sta_interrupt = TASK_NULL;
        }else if (sys_ctl.bt_sta < BT_STA_INCOMING) {
            task_ctl.work_sta = work_sta_interrupt;
            //printf("ret %d\n", work_sta_interrupt);
        }
    }
#if BT_AUTO_STANDBY_EN
    if (!BT_IS_SLEEP) {
        sys_ctl.sleep_cnt = STANDBY_TIME;
    }
    if (sys_ctl.sleep_cnt == 0) {
        task_hold();
    }
#endif

#if WARNING_LOWPOWER
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        mp3res_play_btmode(RES_MP3_PLEASE_CHARGE);
    }
#endif

#if BT_AUTO_PLAY
    static bool temp_play = 0;
    if(t_bt.bt_sta == BT_STA_CONNECTED && !temp_play){
        temp_play = 1;
        sys_ctl.bt_auto_play_flag = sys_param_read8(PARAM_BT_AUTO_PLAY);//printf("auto: %d\n",sys_ctl.bt_auto_play_flag);
        param_read(temp_remote_dev_addr, PARAM_REMOTE_DEV_ADDR, 6);
        if(sys_ctl.bt_auto_play_flag && 0 == memcmp(temp_remote_dev_addr, Link_BdAddr, 6)){
            //need_send_ppkey = 1;
        }
        sys_ctl.bt_auto_play_flag = 0;
        sys_param_write8(PARAM_BT_AUTO_PLAY, sys_ctl.bt_auto_play_flag);
    }
#endif

    if (bt_testmode()) {
        tasksub_bt_test();
    }
}

//蓝牙任务下，1S消息处理
#pragma location="TASK_BT_SEG"
void task_bt_deal_msg_10s(void)
{
    send_bat_to_iphone_hook();
}

#pragma location="BT_API_SEG"
void bt_add_sco(void)
{
    //printf("ADD SCO\n");

    btdec_stop();                               //停止当前的蓝牙音乐播放
    t_bt.add_sco_flag = 1;
    dac_dynamic(DYNAMIC_NO);                    //通话时，不需要动态降噪
    //while(sbc_dec_busy){}   // wait sbc decode complete


    AUCON0 = 0x06;  //RST
    //AUCON9 = 0x04;
    //printf("bt_enter_hshf\n");

    bool ie_sint_bak = IE_SINT;
    IE_SINT = 0;

    memset(sco_spk_vad, 0, sizeof(sco_spk_vad));
    sco_spk_vad_index = 0;
    sco_spk_vad_mute = 0;

    //sco_mic_vad = 1;
    sco_far_vad = 0;
    sco_far_vad_last = 0;
    sco_spk_stable = 0;

    sco_mic_pow = 1;
    sco_mic_stable = 0;

    sco_cal_index = 0;
    sco_cal_max = -128;
    sco_cal_min = 127;

    sco_farpcm_len = 0;
    sco_farpcm_wptr = sco_farpcm;
    sco_farpcm_rptr = sco_farpcm;
    memset(sco_farpcm, 0, sizeof(sco_farpcm));

    far_vol = 255;
    far_noise_pow = 0;
    far_noise_pow_cnt = 0;

    uartTxQ3[0]=0x03;//packet type
    uartTxQ3[1]=0x5C;
    uartTxQ3[2]=0x00;
    uartTxQ3[3]=0x20;//32 bytes, 16 PCM samples

#if 1
    PCON0 &= ~BIT(7);       //DecRAM clk enable
    PCON1 &= ~BIT(1);       //FFT clk enable
    aec_status = 0;
    adc_data_pos = 0;
    aec_proc_kick = 0;
    aec_post_kick = 0;
    aec_init();
#endif

    bt_mic_init();
    pcm_init(S_SPR_8000);
    AUCON10 |= BIT(7);
    FIFO_SPEED = 0x00;

    IE_SINT = ie_sint_bak;
    pcm_play();
}

#pragma location="BT_API_SEG"
void bt_dis_sco(void)
{
    printf("DIS SCO\n");
    adc_stop();
    pcm_stop();
	adc_exit();
    aec_status = 0;
    adc_data_pos = 0;
    aec_proc_kick = 0;
    aec_post_kick = 0;

    FFT1CON = 0;
    FFT1CON1 = 0;

#if !BT_CALL_PRIVATE
    dac_disable();
#endif
    //printf("bt_exit_hshf\n");
    sco_farpcm_len = 0;
#if !BT_CALL_PRIVATE
    amux_exit();
#endif

    t_bt.add_sco_flag = 0;
    dac_enable();
    dac_dynamic(BT_DYNAMIC_NOISE);      //结束通话，开启动态降噪

    sco_farpcm_len = 0;
    sco_farpcm_wptr = sco_farpcm;
    sco_farpcm_rptr = sco_farpcm;
    delay_5ms(25);                      //等待DAC数据完成
    sbc_init();                         //这里会打开软中断

    task_bt_sta_update();
#if MIC_DIRECT_OUT_EN
    mic_direct_out_enter(MIC_CHANNEL_SEL,11);
    btdec_play();
#endif

#if DAC_MIX_OUT_EN
    dac_left_right_mix_out();
#endif

}

#if WARNING_VOL_MAXMIN
void bt_play_vol_music(void)
{
    if(!sys_ctl.vol_maxmin_music_play){
        sys_ctl.vol_maxmin_music_play = 1;
    }
    btmode_exit();
    mp3_res_play_wait(RES_MP3_VOL_MAXMIN);
    flush_dispensable_msg();
}
#endif

#pragma location="TASK_BT_SEG"
void bt_play_continue(void)
{
    btdec_load_code(0x10, 3);
    btmode_enter();
    sbc_init();
    task_bt_sta_update();
}

#pragma location="TASK_BT_SEG"
void mp3res_play_btmode(u8 music_name)
{
    btmode_exit();
    mp3_res_play_wait(music_name);
    bt_play_continue();
}

//手机端改变媒体音量接口，1为UP, 0为DOWN
#pragma location="BT_AVCTP_RX"
void a2dpvol_change_hook(u8 mode)
{
    if (mode == 0) {
        put_msg(Q_VOL_DOWN);
    } else {
        put_msg(Q_VOL_UP);
    }
}

//A2DP设置音量接口
#pragma location="BT_AVCTP_RX"
void a2dpvol_set_hook(u8 vol)
{
    //iphone端调设备音量，其中iphone音量范围是0~0x7f，要转换为设备音量，VOLUME_MAX最好能被整除，如16或32
    vol = (vol+2)/8;
    if(vol != sys_ctl.volume) {
        user_set_volume(vol);
        show_volume();
    }
}

//A2DP获取当音音量接口
#pragma location="BT_AVCTP_RX"
u8 a2dpvol_get_hook(void)
{
    u8 vol = sys_ctl.volume*8+2; //iphone获取设备音量，要转换为iphone音量，VOLUME_MAX最好能被整除
    if(vol < 7) {
        return 0;
    } else if(vol > 0x77) {
        return 0x7f;
    } else {
	    return vol;
    }
}

//处理蓝牙的通知消息
#pragma location="TASK_BT_SEG"
void task_bt_deal_notice_msg(u8 msg)
{
    switch (msg) {
    case Q_BT_CONN_DONE:
        //蓝牙已连接
#if WARNING_BT_CONNECT
        if (!bt_testmode()) {                           //测试模式下不要连接提示音
            mp3res_play_btmode(RES_MP3_BT_CONNECT);
        }
#endif
        task_bt_sta_update();
        break;

    case Q_BT_DISCONN_DONE:
        //蓝牙已断开
#if WARNING_BT_DISCONNECT
        if (!bt_testmode()) {                           //测试模式下不要断开提示音
            mp3res_play_btmode(RES_MP3_BT_DISCONNECT);
        }
#endif
        break;

    case Q_BT_CONN_LOST:
        //蓝牙连接丢失（远距离断开）
        //printf("主人不要丢下我啊\n");
#if BT_ANTI_LOST
        if (sys_ctl.bt_sta < BT_STA_CONNECTED) {
            tasksub_bt_lost();
        }
#endif
        break;

    case Q_BT_PLAY:
        //蓝牙开始播放
        btdec_play();
        break;

    case Q_BT_STOP:
        //蓝牙停止播放
#if !MIC_DIRECT_OUT_EN
        if(!t_bt.add_sco_flag) {                        //已经在add sco时停止音乐播放
            btdec_stop();
        }
#endif
        break;

    case Q_BT_INCOMING:
        //蓝牙来电
        tasksub_bt_ring();
        break;

    case Q_BT_CALL:
        //蓝牙开始通话
        tasksub_bt_call();
        break;

        //手机端调系统音量
    case Q_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
        break;
    case Q_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        break;
    }
}

#pragma location="TASK_BTEVENT_SEG"
void task_bt_loop(void)
{
    u8 msg;
    while ((task_ctl.work_sta == TASK_BT || sys_ctl.bt_sta >= BT_STA_INCOMING)
           && (task_ctl.work_sta != TASK_POWEROFF))
    {
        task_bt_event();
        while (msg = get_msg()) {
#if BT_HID_MANU
			if (t_bt.hid_enable == 1) {
                task_bt_hid_deal_msg(msg);
			}else
#endif
            {
                task_bt_deal_msg(msg);
            }
#if WARNING_LOWPOWER    // 低电时跳出手机响铃
            if(task_ctl.work_sta == TASK_POWEROFF) {
                break;
            }
#endif

        }
        task_bt_display();
    }
}

//蓝牙任务
#pragma location="TASK_BT_MINI_SEG"//"TASK_BT_SEG"
void task_bt(void)
{
    printf(str_show_taskbt);
    task_bt_enter();
    task_bt_loop();
    task_bt_exit();
    printf(str_show_taskbtexit);
}
#else          //由于关闭蓝牙会导致蓝牙库出现编译错误，故暂时定义一下蓝牙库调用的变量和函数，待整理
u16  sco_farpcm_len;
s16 *sco_farpcm_wptr;
s16 *sco_farpcm_rptr;
#define SCO_FARPCM_SIZE         (0x800/2)      // unit 16bits
IAR_XDATA_A s16 sco_farpcm[SCO_FARPCM_SIZE] @ "BT_SCOFAR_XDATA";
#endif
