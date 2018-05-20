/*****************************************************************************
 * Module    : Task
 * File      : task_bt.c
 * Author    : Hanny
 * Function  : ������������
 *****************************************************************************/
#include "include.h"

#if (TASK_BT_EN | TASK_BTHID_EN)
#include "task.h"
#include "user_bt_pbap.h"

#define DEBUG_TASK_BT        0

//��������������
enum {
    TASK_BT_NULL,
    TASK_BT_TEST,               //���Ժ�����
    TASK_BT_RING,               //��������
    TASK_BT_CALL,               //ͨ������
    TASK_BT_LOST,               //�����Ͽ�����Ҫ���ֻ�����
};
bool bt_test_flag = false;
IAR_XDATA_A type_bt t_bt @ "BT_XDATA";
void test_bt(void);
bool btdec_load_code(u16 addr, u8 segnum);
void task_bt_sta_update(void);
extern IAR_BIT_A bool flag_btmode;            //Ϊ1ʱ������������������
extern IAR_BIT_A bool scoflag;  //for nokia 5000 use sco play music
extern u8 bt_mic_param[10];
extern u8 work_sta_interrupt;          //�绰�жϱ�־
extern void bt_play_vol_music(void);
void bt_play_continue(void);
void lock_pll48m(u8 lock);

#pragma constseg="TASK_BT_MINI_SEG_CONST"
IAR_CONST char str_show_taskbt[] = "task_bt\n";
IAR_CONST char str_show_taskbtexit[] = "task_bt_exit\n";
#pragma constseg=default

#define TASK_SUB
#include SET_USER_PATH(CFG_UI_BT_DIR, /ui/ui_bt.c)          //UI����
#include "sub/tasksub_bt_ring.c"                            //����������
#include "sub/tasksub_bt_call.c"                            //ͨ��������
#include "sub/tasksub_bt_test.c"                            //���Ժ�������
#include "sub/tasksub_bt_menu.c"                            //�˵�������
#include "sub/tasksub_bt_lost.c"                            //����������

//#pragma location="TASK_BT_SEG"
void task_bt_enter(void)
{
    task_ctl.work_sub = TASK_NULL;
    set_sys_clk(SYS_48M);                                   //���������ϵͳʱ��
#if !BT_ALL_TASK
    user_bt_init();
    bt_connect();                                           //�����������񣬷������
    sys_ctl.bt_sta = bt_status_get();
    sys_ctl.connected = (sys_ctl.bt_sta >= BT_STA_CONNECTED) ? 1 : 0;
#else
    btinfo_get_mic(bt_mic_param);
    btinfo_get_voice(bt_mic_param+2);
#endif
    sys_ctl.bt_sta = bt_status_get();

    //sound_ctl_init(3, 0x80, 25, 0x40);        //����3�γ���0x80��Ϊ��ʼ����������25��0x40��Ϊ����
    memset(&t_bt, 0, sizeof(t_bt));
#if IS_LCD_DISPLAY
    t_bt.updata_flag = 1;   //������ʾ��־
#endif
    //�ȸ�����ʾ
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
    sound_ctl_init(8, 0x08, 100, 0x03);        //����10�γ���0x08��Ϊ��ʼ����������100��0x03������Ϊ����
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
    dac_dynamic(BT_DYNAMIC_NOISE);              //���ö�̬����
#if BT_ALL_TASK
    delay_5ms(60);  //��Ҫ��ʱһ��ʱ�䣬��̬�����ܻ�ȡ����P/P״̬
    task_bt_sta_update();
#endif
#if MIC_DIRECT_OUT_EN
    mic_direct_out_enter(MIC_CHANNEL_SEL,11);
    btdec_play();
#endif

#if IS_LCD_DISPLAY
    ocx_init(task_bt_event, NULL);  // ��ʼ���ؼ������ڵ�����ʱҲ���������¼�
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf3;   // bt��������ʹ��lcd_buf3������Ứ��
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

//�˳���������
#pragma location="TASK_BT_SEG_1"
void task_bt_exit(void)
{
    t_bt.hid_enable = 0;
#if TASK_BT_EN
#if BT_ALL_TASK
    if (sys_ctl.bt_sta == BT_STA_PLAYING) {     //��̨ʱ�л�������ģʽ��ͣ�ֻ�����
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
        user_bt_off(); //�ر�����ͨ��
    } else {
        bt_close();
    }
#endif
    btdec_stop();
#endif
    dac_dynamic(DYNAMIC_NO);                    //�˳��������رն�̬����
#if MIC_DIRECT_OUT_EN
    mic_direct_out_exit();
#endif
    sys_unmute();
    dac_disable();
    amux_exit();
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf1;   // �˳�ʱ��lcd�ָ�ʹ�ô�buf
    work_sta_interrupt = TASK_NULL;
#endif
    if (device_is_online(DEVICE_UDISK)) {
        lock_pll48m(1);                         //U�����ߣ�������PLL
        usb_host_reactive();
        delay_5ms(80);
    } else {
        lock_pll48m(0);                         //U�̲����ߣ�������PLL
    }
}

#if BT_AUTO_PLAY
u8 temp_remote_dev_addr[6];
#endif
//�����¼�
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

//���������£�1S��Ϣ����
#pragma location="TASK_BT_SEG"
void task_bt_deal_msg_10s(void)
{
    send_bat_to_iphone_hook();
}

#pragma location="BT_API_SEG"
void bt_add_sco(void)
{
    //printf("ADD SCO\n");

    btdec_stop();                               //ֹͣ��ǰ���������ֲ���
    t_bt.add_sco_flag = 1;
    dac_dynamic(DYNAMIC_NO);                    //ͨ��ʱ������Ҫ��̬����
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
    dac_dynamic(BT_DYNAMIC_NOISE);      //����ͨ����������̬����

    sco_farpcm_len = 0;
    sco_farpcm_wptr = sco_farpcm;
    sco_farpcm_rptr = sco_farpcm;
    delay_5ms(25);                      //�ȴ�DAC�������
    sbc_init();                         //���������ж�

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

//�ֻ��˸ı�ý�������ӿڣ�1ΪUP, 0ΪDOWN
#pragma location="BT_AVCTP_RX"
void a2dpvol_change_hook(u8 mode)
{
    if (mode == 0) {
        put_msg(Q_VOL_DOWN);
    } else {
        put_msg(Q_VOL_UP);
    }
}

//A2DP���������ӿ�
#pragma location="BT_AVCTP_RX"
void a2dpvol_set_hook(u8 vol)
{
    //iphone�˵��豸����������iphone������Χ��0~0x7f��Ҫת��Ϊ�豸������VOLUME_MAX����ܱ���������16��32
    vol = (vol+2)/8;
    if(vol != sys_ctl.volume) {
        user_set_volume(vol);
        show_volume();
    }
}

//A2DP��ȡ���������ӿ�
#pragma location="BT_AVCTP_RX"
u8 a2dpvol_get_hook(void)
{
    u8 vol = sys_ctl.volume*8+2; //iphone��ȡ�豸������Ҫת��Ϊiphone������VOLUME_MAX����ܱ�����
    if(vol < 7) {
        return 0;
    } else if(vol > 0x77) {
        return 0x7f;
    } else {
	    return vol;
    }
}

//����������֪ͨ��Ϣ
#pragma location="TASK_BT_SEG"
void task_bt_deal_notice_msg(u8 msg)
{
    switch (msg) {
    case Q_BT_CONN_DONE:
        //����������
#if WARNING_BT_CONNECT
        if (!bt_testmode()) {                           //����ģʽ�²�Ҫ������ʾ��
            mp3res_play_btmode(RES_MP3_BT_CONNECT);
        }
#endif
        task_bt_sta_update();
        break;

    case Q_BT_DISCONN_DONE:
        //�����ѶϿ�
#if WARNING_BT_DISCONNECT
        if (!bt_testmode()) {                           //����ģʽ�²�Ҫ�Ͽ���ʾ��
            mp3res_play_btmode(RES_MP3_BT_DISCONNECT);
        }
#endif
        break;

    case Q_BT_CONN_LOST:
        //�������Ӷ�ʧ��Զ����Ͽ���
        //printf("���˲�Ҫ�����Ұ�\n");
#if BT_ANTI_LOST
        if (sys_ctl.bt_sta < BT_STA_CONNECTED) {
            tasksub_bt_lost();
        }
#endif
        break;

    case Q_BT_PLAY:
        //������ʼ����
        btdec_play();
        break;

    case Q_BT_STOP:
        //����ֹͣ����
#if !MIC_DIRECT_OUT_EN
        if(!t_bt.add_sco_flag) {                        //�Ѿ���add scoʱֹͣ���ֲ���
            btdec_stop();
        }
#endif
        break;

    case Q_BT_INCOMING:
        //��������
        tasksub_bt_ring();
        break;

    case Q_BT_CALL:
        //������ʼͨ��
        tasksub_bt_call();
        break;

        //�ֻ��˵�ϵͳ����
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
#if WARNING_LOWPOWER    // �͵�ʱ�����ֻ�����
            if(task_ctl.work_sta == TASK_POWEROFF) {
                break;
            }
#endif

        }
        task_bt_display();
    }
}

//��������
#pragma location="TASK_BT_MINI_SEG"//"TASK_BT_SEG"
void task_bt(void)
{
    printf(str_show_taskbt);
    task_bt_enter();
    task_bt_loop();
    task_bt_exit();
    printf(str_show_taskbtexit);
}
#else          //���ڹر������ᵼ����������ֱ�����󣬹���ʱ����һ����������õı����ͺ�����������
u16  sco_farpcm_len;
s16 *sco_farpcm_wptr;
s16 *sco_farpcm_rptr;
#define SCO_FARPCM_SIZE         (0x800/2)      // unit 16bits
IAR_XDATA_A s16 sco_farpcm[SCO_FARPCM_SIZE] @ "BT_SCOFAR_XDATA";
#endif
