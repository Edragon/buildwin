/*****************************************************************************
 * Module    : Task
 * File      : task_fm.c
 * Author    : Hanny
 * Function  : ����������
 *****************************************************************************/
#include "include.h"

#if TASK_FM_EN
#include "task.h"
#include "user_fm.h"
#include "task_fm.h"
#if FM_RECORD_EN
#include "task_record.h"
#include "task_music.h"
#endif

#define DEBUG_TASK_FM           0
#define FM_ITEMS_NUM            6
#define MAX_FREQ                10800
#if ((FM_TYPE == FM_RTC6218) && (SYS_CRY == CRY_2M))
#define MIN_RREQ                8740
#else
#define MIN_RREQ                8750
#endif

enum {
    SEEK_STOP,SEEK_AUTO,
};
enum {
    ITEM_AUTO_SEEK,ITEM_DELETE,ITEM_DEL_ALL,ITEM_BACK_MENU,ITEM_EXIT,
};
struct {
    char *ptr;
} disp_fm;

//task fm controller
IAR_XDATA_A TYPE_TASK_FM t_fm           @ "FM_XDATA";
IAR_XDATA_A type_task_fm_rec t_fm_rec   @ "FM_XDATA";

extern IAR_BIT_A volatile bool flag_tm2_fade;
//IAR_BIT_A volatile bool flag_tm2_fade_bak;

#pragma constseg="LIST_CONTENT"
string IAR_CONST tbl_fm_items[FM_ITEMS_NUM+1] = {
    STR_FM_RECIEVE,
    STR_FM_AUTO_SEEK,
    STR_FM_DELETE,
    STR_FM_DEL_ALL,
    STR_BACK_MENU,
    STR_MENU_EXIT,
};
void task_fm_save(void);
extern u32 WriteTotalData;              //�ļ�ϵͳд����ʱ��ʹ��
__near_func void task_fm_auto_stop(void);
__near_func void task_fm_auto_play(void);
__near_func void task_fm_auto_start(void);
__near_func void task_fm_prevch(void);
__near_func bool user_set_volume_fm(u8 volume);
void task_fm_select_freq_chan(u8 num);
static void task_fm_nextch(void);
__near_func u8 user_volume_dec_fm(u8 volume);
__near_func u8 user_volume_inc_fm(u8 volume);
void task_fm_reuse_init(void);
void fm_play_vol_music(void);
void fm_play_continue(void);

#include SET_USER_PATH(CFG_UI_FM_DIR, /ui/ui_fm.c)

#if FM_RECORD_EN
#define TASK_SUB
#include "sub/tasksub_fm_record.c"          //¼��������
#include "sub/tasksub_fm_recplay.c"         //¼��������
#endif

void task_fm_display_init(void);
void task_fm_load(void);
void task_fm_fix(void);

IAR_XDATA_A u16 fm_ch[64] @ "FM_CH_BUF";

extern void task_fm_dispmem_init(void);         //Ԥװ����
extern void task_fm_play_dispmem_init(void);
extern void task_fm_list_dispmem_init(void);
extern void task_fm_display(void);              //��������ʾ�ӿ�
extern void usb_host_reactive(void);
__near_func void dac_anl_fade_wait(void);

#if !FM_VOUT_THROUGH_MCU
//#pragma inline=forced
void dac_pull_down(void)
{
   dac_anl_fade_wait();
   speaker_mute();   //����DACʱ��������MUTEס����
   dac_off(0);
}

//#pragma inline=forced
void dac_recover(void)
{
   speaker_mute();   //����DACʱ��������MUTEס����
   dac_init(0);
#if DAC_MIX_OUT_EN
   dac_left_right_mix_out();
#endif
}
#endif

//�����ʼ��
#pragma location="TASK_FM_INIT"
void task_fm_enter(void)
{
    if(device_is_online(DEVICE_UDISK)){
        //usb_host_suspend();
    }
#if (FM_CLK_SEL == FMOSC_REUSE_32KSDCLK)
    set_sys_clk(SYS_12M);           //��Ҫͨ�� SDCLK ��32.728Kʱ��ʱ��FMģʽ�¹�����12Mϵͳʱ��
#else
    set_sys_clk(SYS_24M);           //FMģʽ�¹�����24Mϵͳʱ��
#endif
   // user_set_volex(0);              //FM��Դ�ϴ�һ�㲻��Ҫ3DB��������
    led_fm_play();
    draw_wait();

    task_ctl.work_sta = TASK_FM;
    memset(&t_fm,0,sizeof(t_fm));

#if (((FM_CHANNEL_SEL & 0x07) == CHANNEL_01) && FM_VOUT_THROUGH_MCU)
    uart_disable();
    P0DIR |= BIT(1);
#endif

    P3DRV0 &= ~BIT(3);
#if (FM_CLK_SEL == P33_OUT_32K)
    P3DIR &= ~BIT(3);
    PCON3 |= 0x80;   // make sure 32k xosc run
    PMUXCON5 = (PMUXCON5 & 0xF0) | BIT(0);    //XOSC
#elif (FM_CLK_SEL == P33_OUT_12M)
    P3DIR &= ~BIT(3);
    PMUXCON5 = (PMUXCON5 & 0xF0) | BIT(0) | BIT(2);  //PLL 12M
#endif

    task_fm_reuse_init();
    user_change_volume(sys_ctl.volume); //��������
    if (!fm_init()) {
        task_ctl.work_sta = TASK_EXIT;  //FMģ�����
        return;
    }
#if !FM_VOUT_THROUGH_MCU
    fm_off();                       //�ȹ�һ��FM����ֹFM������ʾ��
    delay_5ms(100);
#endif

    //��ʾ��

#if MP3BT_DECT_EN
    work_sta_interrupt = TASK_NULL;
#endif

#if WARNING_TASK_FM
    if (work_sta_interrupt == TASK_NULL) {
        dac_enable();
        mp3_res_play_wait(RES_MP3_FM_MODE);
        dac_disable();
    }
#endif
    task_fm_reuse_init();
    work_sta_interrupt = TASK_NULL;
#if !FM_VOUT_THROUGH_MCU
    dac_pull_down();
    delay_5ms(10);
    fm_init();                      //����ر�FM���ڴ��ٳ�ʼ��һ��FM
#endif
    t_fm.disp_flag = 1;
    t_fm.disp_ch_flag = 1;
    t_fm.disp_freq = 0xffff;

    load_code(0x28, 3);
    task_fm_load();
    t_fm.ch_sel = sys_param_read8(PARAM_FM_CHSEL);              //��ǰ̨��
    if(t_fm.ch_cnt >= 100 || t_fm.ch_sel > 100)
    {
        t_fm.ch_cnt = 0;
        t_fm.ch_sel = 0;
    }
    if (t_fm.ch_sel > t_fm.ch_cnt || t_fm.ch_sel == 0) {
        t_fm.ch_sel = t_fm.ch_cnt;
    }

    t_fm.freq = sys_param_read16(PARAM_FM_FREQ);                //��ʼƵ��
    t_fm.seek_sta = SEEK_STOP;
    t_fm.fm_sta = FM_PLAY;
    task_fm_fix();
    fm_set_freq(t_fm.freq);
    WATCHDOG_CLR();
    //��ʼ����

#if FM_VOUT_THROUGH_MCU
    amux_init(FM_CHANNEL_SEL);
#else
    fm_set_vol(sys_ctl.volume); //����FMģ������Ϊϵͳ��������ʼ����
#endif
#if KEY_VOICE_EN
    key_voice_init();
#endif
    draw_clear();
    ocx_clear();

#if IS_LCD_DISPLAY
    task_fm_disp_init();
#endif

#if MIC_DIRECT_OUT_EN && FM_VOUT_THROUGH_MCU
    mic_direct_out_enter(MIC_CHANNEL_SEL,11);
#endif
    if (!sys_ctl.mute_flag) {
#if FM_VOUT_THROUGH_MCU
        amux_play();
#else
        speaker_unmute();
#endif
    }
    //printf("fm_init finished\n");
}

#pragma location="TASK_FM_SEG_1"
void mp3res_play_fmmode(u8 music_num)
{
    dac_enable();
    mp3_res_play_wait(music_num);
    task_fm_reuse_init();
    dac_disable();
    load_code(0x28, 3);
    task_fm_load();     //����fm_ch[]�Ǳ�����DCODE_RAM�ģ�������ʾ����ᱻ���㣬��Ҫ����LOADһ��
}

#pragma location="TASK_FM_SEG_1"
void fm_play_exit(void)
{
#if FM_VOUT_THROUGH_MCU
    amux_exit();
#else
    speaker_mute();
    fm_set_vol(0);
    fm_off();       //������ʾ��ʱ��Ҫ�ر�FM������FM���DAC��������
    dac_recover();
    speaker_unmute();
#endif
}

#pragma location="TASK_FM_SEG_1"
void fm_play_continue(void)
{
#if FM_VOUT_THROUGH_MCU
    amux_init(FM_CHANNEL_SEL);
    if(!t_fm.mute){
        sys_unmute();
        amux_play();
    }
#else                          //У��Ƶ��
    dac_pull_down();
    delay_5ms(10);
    fm_init();
    load_code(0x28, 3);
    task_fm_load();     //����fm_ch[]�Ǳ�����DCODE_RAM�ģ�������ʾ����ᱻ���㣬��Ҫ����LOADһ��
    fm_set_freq(t_fm.freq);                     //����Ƶ��
    if(t_fm.mute){
        speaker_mute();
        fm_set_vol(0);
    }else{
        fm_set_vol(sys_ctl.volume);
        speaker_unmute();
    }
    WATCHDOG_CLR();
#endif
}

#pragma location="TASK_FM_SEG_1"
void fm_play_vol_music(void)
{
    if(SEEK_AUTO == t_fm.seek_sta){
        task_fm_save();
    }
#if FM_VOUT_THROUGH_MCU
    sys_unmute();
#else
    speaker_unmute();
#endif
    if(!sys_ctl.vol_maxmin_music_play) { //����hold��Ϣʱ��ִֻ�������һ��
        fm_play_exit();
        sys_ctl.vol_maxmin_music_play = 1;
    }
    mp3res_play_fmmode(RES_MP3_VOL_MAXMIN);
    flush_msg();
}

#pragma location="TASK_FM_SEG_1"
void task_fm_menu_music_continue(u8 menu_music_num)
{
    fm_play_exit();
    mp3res_play_fmmode(menu_music_num);
    fm_play_continue();
}

//����Ƶ��
#pragma location="TASK_FM_INIT"
void task_fm_save(void)
{
    sys_param_write8(PARAM_FM_CH, t_fm.ch_cnt);
    sys_param_write(fm_ch, PARAM_FM_CH+2, sizeof(fm_ch));
}

#pragma location="TASK_FM_INIT"
void task_fm_reuse_init(void)
{
    sd_stop(1);

#if IIC_REUSE_SD                    //����SDʱ�����һ��SDCLK��ʹSD���ͷ�SDDAT
    SDCON0 |= BIT(1);               //Keep Clk Outing
    delay_5ms(2);
    SDCON0 &= ~(BIT(1) | BIT(0));   //Disable SDC & CLK
#endif

#if ((FM_CLK_SEL == FMOSC_REUSE_32KSDCLK) || (FM_CLK_SEL == FMOSC_REUSE_12MSDCLK))
    SD_CLK_DIR_OUT();
    SDCON0 |= BIT(0);               //Enable SDC
  #if (FM_CLK_SEL == FMOSC_REUSE_32KSDCLK)
    SDBAUD = 0xB6;        //12MHz / 2 / (SDBAUD + 1) = 32.768K  //  sd_set_rate0(0xb7);
  #else
    SDBAUD = 0x00;        //24MHz / 2 / (SDBAUD + 1) = 12M //sd_set_rate0(0);
  #endif
    SDCON0 |= BIT(1);               //Keep Clk Outing
    delay_5ms(2);                   //����SDʱ�����һ��SDCLK��ʹSD���ͷ�SDDAT
#endif
}

#pragma location="TASK_FM_INIT"
void task_fm_reuse_exit(void)
{
#if IIC_REUSE_SD
    sd_reactive();
#endif
#if ((FM_CLK_SEL == FMOSC_REUSE_32KSDCLK) || (FM_CLK_SEL == FMOSC_REUSE_12MSDCLK))
    SDCON0 &= ~BIT(1);                   //suspend sdclk
    sd_reactive();
#endif

#if IICCLK_REUSE_USB
    usb_host_reactive();
#endif
}
//�����˳�
#pragma location="TASK_FM_SEG_1"
void task_fm_exit(void)
{
#if FM_RECORD_EN
    if((t_fm.sta == FM_REC_PAUSE) && t_fm_rec.file_open) {
        task_fm_record_stop();
    }
	adc_exit();
    adc_ch = 1;
#endif
    amux_exit();
#if FM_VOUT_THROUGH_MCU
    sys_unmute();
#else
    speaker_mute();
    fm_set_vol(0);
#endif

#if MIC_DIRECT_OUT_EN && FM_VOUT_THROUGH_MCU
    mic_direct_out_exit();
#endif
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
    fm_off();

    if(t_fm.seek_sta == SEEK_AUTO) {
        task_fm_auto_stop();
    }
    if(t_fm.freq) {  //��ֹfm��ʼ��ʧ��дflash
    sys_param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
    sys_param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
    }

#if (DEBUG_UART_SEL && ((FM_CHANNEL_SEL & 0x07) == CHANNEL_01) && FM_VOUT_THROUGH_MCU)
    P0DIR &= ~BIT(1);  //LQP48 S10�ϲ���,������,DAC����������ú�С
    uart_enable();
#endif

#if ((FM_CLK_SEL == P33_OUT_32K) || (FM_CLK_SEL == P33_OUT_12M))  //�ر�P33ʱ�����.
    PMUXCON5 &= 0xF0;
#endif
    task_fm_reuse_exit();
#if !FM_VOUT_THROUGH_MCU
    dac_recover();
#endif
    if(device_is_online(DEVICE_UDISK)){
        //usb_host_resume();
    }
#if IS_LCD_DISPLAY
    lcd_buf = lcd_buf1;   // �˳�ʱ��lcd�ָ�ʹ�ô�buf
#endif
}

//����Ƶ��
#pragma location="TASK_FM_INIT"
void task_fm_load(void)
{
    t_fm.ch_cnt = sys_param_read8(PARAM_FM_CH);
    sys_param_read(fm_ch, PARAM_FM_CH+2, sizeof(fm_ch));
}

//ֹͣ��̨
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_auto_stop(void)
{
    t_fm.fm_sta = FM_PLAY;
    task_fm_save();
    t_fm.ch_sel  = 1;
    t_fm.disp_ch_flag = 1;
    t_fm.seek_sta = SEEK_STOP;

    led_idle();
    ocx_clear();
#if DEBUG_TASK_FM
    printf("t_fm.ch_cnt: %d\n",t_fm.ch_cnt);
#endif
}

//ֹͣ��̨�󲥷�
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_auto_play(void)
{
    if(t_fm.ch_sel) {
        t_fm.freq = fm_ch[t_fm.ch_sel-1];
        task_fm_fix();
        fm_set_freq(t_fm.freq);
    }
    t_fm.mute = 0;
#if FM_VOUT_THROUGH_MCU
    if(sys_ctl.volume){
        sys_unmute();
        amux_play();
    }
#else
    speaker_unmute();
    fm_set_vol(sys_ctl.volume);
#endif
}

//��ʼ��̨
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_auto_start(void)
{
    t_fm.freq = MIN_RREQ;
    t_fm.ch_cnt = 0;
    t_fm.ch_sel = 0;
    t_fm.fm_sta = FM_AUTO_SEEK;
    led_busy();
    ocx_clear();
#if FM_VOUT_THROUGH_MCU
    sys_mute();   //QN8065��̨ʱMUTEס����
#else
    speaker_mute();
    fm_set_vol(0);
#endif
}

//ѡ��Ƶ�������Ƶ��
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_set_freq(void)
{
#if !FM_VOUT_THROUGH_MCU
    fm_set_vol(0);
#else
    sys_mute();
#endif
    task_fm_fix();
    fm_set_freq(t_fm.freq);
    delay_5ms(20);//QN8065����Ƶ���Ҫ��ʱ��������ɳɳ��

#if FM_VOUT_THROUGH_MCU
    if (sys_ctl.mute_flag && sys_ctl.volume) {
        t_fm.mute = 0;
        sys_unmute();
        amux_play();
    }
#else
    if(sys_ctl.spk_mute && sys_ctl.volume){
        t_fm.mute = 0;
        fm_set_vol(sys_ctl.volume);
        speaker_unmute();
    }
#endif
}

//��һƵ��
#pragma location="TASK_FMPLAY_SEG"
__near_func bool task_fm_auto_nextfreq(void)
{
    if (t_fm.freq < MAX_FREQ) {
//#if (((FM_CLK_SEL != SHARE_CRYSTAL_32K)  && (FM_TYPE == FM_QN8065)) || (FM_TYPE == FM_RTC6218))
#if FM_TYPE == FM_RTC6218
        t_fm.freq += 5;
#else
        t_fm.freq += 10;
#endif
        return true;
    }
    led_fm_play();
    task_fm_auto_stop();
    task_fm_auto_play();  //����FM����̨��������.
    t_fm.freq = fm_ch[t_fm.ch_sel-1];
    task_fm_fix();
    fm_set_freq(t_fm.freq);
    sys_param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
    sys_param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
    return false;
}

//У��Ƶ��
#pragma location="TASK_FM_SEG"
void task_fm_fix(void)
{
    if (t_fm.freq < MIN_RREQ) {
        t_fm.freq = MAX_FREQ;//MIN_RREQ;
    } else if (t_fm.freq > MAX_FREQ) {
        t_fm.freq = MIN_RREQ;//MAX_FREQ;
    }
}

//��һƵ��
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_prevch(void)
{
    if (t_fm.ch_cnt) {
        t_fm.ch_sel--;
        if (!t_fm.ch_sel) {
            t_fm.ch_sel = t_fm.ch_cnt;
        }
        t_fm.freq = fm_ch[t_fm.ch_sel-1];
#if (IS_LCDSEG_DISPLAY || IS_LEDSEG_DISPLAY)
        show_freq_chan();   //LCDSEG��LEDSEGʱ��ʾ����Ƶ���л�ʱ��Ƶ����
#endif

    } else {
        t_fm.freq -= 10;
    }

    task_fm_set_freq();
}

//��һƵ��
#pragma location="TASK_FM_SEG"
static void task_fm_nextch(void)
{
    if (t_fm.ch_cnt) {
        t_fm.ch_sel++;
        if (t_fm.ch_sel > t_fm.ch_cnt) {
            t_fm.ch_sel = 1;
        }
        t_fm.freq = fm_ch[t_fm.ch_sel-1];


#if (IS_LCDSEG_DISPLAY || IS_LEDSEG_DISPLAY)
        show_freq_chan();   //LCDSEG��LEDSEGʱ��ʾ����Ƶ���л�ʱ��Ƶ����
#endif

    } else {
        t_fm.freq += 10;
    }

    task_fm_set_freq();
}
//����������
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_event(void)
{
    u8 fm_chk;

    comm_event();
#if FM_RECORD_EN
    if (device_change()) {
        if(t_fm.fm_sta == FM_REC_PAUSE) {
            t_fm_rec.play_flag = 0;
            task_fm_record_stop();
        }
    }
#endif
#if WARNING_LOWPOWER
    if(sys_ctl.lowpower_warning_flag == 1){
        task_fm_menu_music_continue(RES_MP3_PLEASE_CHARGE);
        sys_ctl.lowpower_warning_flag = 2;
    }
#endif
    switch (t_fm.fm_sta) {
    case FM_AUTO_SEEK:
        fm_chk = fm_check_freq(t_fm.freq);
        if(fm_chk) {
            led_fm_play();
#if FM_TYPE == FM_RTC6218
            fm_ch[t_fm.ch_cnt++] = t_fm.freq + 5*(fm_chk - 1);
#else
            fm_ch[t_fm.ch_cnt++] = t_fm.freq;
#endif
            set_timeout(200);
            t_fm.fm_sta = FM_AUTO_PLAY;
#if SEEK_DISPLAY_CHANNEL_NUM
            t_fm.ch_sel = t_fm.ch_cnt;
            show_freq_chan();
#endif
            fm_unmute();
            t_fm.mute = 0;
#if FM_VOUT_THROUGH_MCU
            if(sys_ctl.volume){
                sys_unmute();
                amux_play();
            }
#else
            fm_set_vol(sys_ctl.volume);
            if (!sys_ctl.mute_flag) {
                speaker_unmute();
            }
#endif
            break;
        }
		//printf("t_fm.freq: %d\n",t_fm.freq);
        task_fm_auto_nextfreq();
        break;
    case FM_AUTO_PLAY:
        if (!time_out_flag) {
            break;
        }
        led_fm_scan();
#if SEEK_DISPLAY_CHANNEL_NUM
        ocx_clear();
#endif
#if FM_VOUT_THROUGH_MCU
        sys_mute();   //QN8065��̨ʱMUTEס����
#else
        speaker_mute();
        fm_set_vol(0);
#endif
        t_fm.fm_sta = FM_AUTO_SEEK;
        task_fm_auto_nextfreq();
        break;
    }
#if FM_AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((((!sys_ctl.sleep_cnt) && BT_IS_SLEEP) || sys_ctl.ir_power_flag) && !sys_ctl.lowpower_flag){
#if FM_VOUT_THROUGH_MCU
        sys_mute();
#else
        speaker_mute();
#endif
        fm_off();                               //�ر�FMģʽ�Խ��͹���
        //task_fm_reuse_exit();
        task_hold();
        dac_disable();
        task_fm_reuse_init();

#if (DEBUG_UART_SEL && ((FM_CHANNEL_SEL & 0x07) == CHANNEL_01) && FM_VOUT_THROUGH_MCU)
        uart_disable();
        P0DIR |= BIT(1);
#endif

        fm_init();      //FMģ���ʼ��
        task_fm_load(); //����Ƶ������
        t_fm.ch_sel = sys_param_read8(PARAM_FM_CHSEL);  //��ȡ��ǰ̨��
        t_fm.freq = sys_param_read16(PARAM_FM_FREQ);    //��ȡ��ǰƵ��
        task_fm_fix();          //У��Ƶ��
        fm_set_freq(t_fm.freq); //����Ƶ��
        sys_ctl.sleep_cnt = 0xffff;

#if FM_VOUT_THROUGH_MCU
        if (sys_ctl.mute_flag && sys_ctl.volume) {
            t_fm.mute = 0;
            sys_unmute();
            amux_play();
        }
#else
        if(sys_ctl.spk_mute && sys_ctl.volume){
            t_fm.mute = 0;
            fm_set_vol(sys_ctl.volume);
            speaker_unmute();
        }
#endif
    }
#endif
}

#pragma location="TASK_FM_SEG_1"
void task_fm_list(void)
{
        u8 index = listbox(0, FM_ITEMS_NUM, NULL, (u16)tbl_fm_items, LIST_TIME);
        switch (index) {
      /*  case ITEM_AUTO:
            t_fm.seekway = ITEM_AUTO;
             task_fm_auto_start();
            break;
        case ITEM_HALF_AUTO:
            t_fm.seekway = ITEM_HALF_AUTO;
            task_fm_auto_nextfreq();
            t_fm.fm_sta = FM_AUTO_SEEK;
            break;
        case ITEM_HAND:
            t_fm.seekway = ITEM_HAND;
            //task_ctl.work_sta = TASK_MENU;      //�˳�
            break;
        case ITEM_SAVE:
            break;*/
        case ITEM_AUTO_SEEK:
              task_fm_auto_start();
              t_fm.seek_sta = SEEK_AUTO;
            break;
        case ITEM_DELETE:
        if(!t_fm.ch_cnt)
        {
            break;
        }
        for(u8 k=t_fm.ch_sel-1;k<t_fm.ch_cnt-1;k++)
        {
            fm_ch[k] = fm_ch[k+1];
        }
        t_fm.ch_cnt -=1;
        if (t_fm.ch_sel>1) {
            t_fm.ch_sel--;
        }
        else
        {
            t_fm.ch_sel = t_fm.ch_cnt;
        }
        if(t_fm.ch_sel)
        {
            task_fm_nextch();
        }
        task_fm_save();
        sys_param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
        sys_param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
        break;
    case ITEM_DEL_ALL:
        t_fm.ch_cnt = 0;
        t_fm.ch_sel = 0;
        task_fm_save();
        sys_param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
        break;
    case ITEM_EXIT:
        /* task_ctl.work_sta = TASK_MENU;              //�������˵�
        task_fm_save();
        sys_param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
        sys_param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��*/
        break;
    case ITEM_BACK_MENU:
        task_ctl.work_sta = TASK_MENU;              //�������˵�
        break;
    default:
        break;
    }
#if IS_LCD_DISPLAY
    //task_fm_list_dispmem_init();
    task_fm_disp_init();
#endif
}
#pragma location="TASK_FM_SEG_1"
void task_fm_volume_set(void)
{
    //������������������ʾ1.5��
    progressbox(sys_ctl.volume, 0, VOLUME_MAX, (u32)&tbl_pg_volume, USE_VOL_KEY, PROGRESS_TIME);
    //������ʾ
#if IS_LCD_DISPLAY
    //task_fm_display_init();
    task_fm_disp_init();
#endif
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
#if !FM_SET_VOL_CHANGE_BANK
extern u8 const tbl_volume[VOLUME_MAX+1];


#pragma location="TASK_FMPLAY_SEG"
__near_func u8 user_volume_inc_fm(u8 volume)
{
    volume++;
    if(volume > VOLUME_MAX)
        volume = VOLUME_MAX;
    return volume;
}

#pragma location="TASK_FMPLAY_SEG"
__near_func u8 user_volume_dec_fm(u8 volume)
{
    if(volume > 0)
        volume--;
    return volume;
}

//������������
#pragma location="TASK_FMPLAY_SEG"
__near_func bool user_change_volume_fm(u8 volume)
{
    if (volume <= VOLUME_MAX) {//printf("tbl_volume_fm[%d]: %d\n",volume,tbl_volume_fm[volume]);
        u8 dac_vol = tbl_volume[volume] + sys_ctl.vol_ex;
        if (volume == 0) {
            dac_vol = 0;
        }
        if (dac_vol >= 128) {
            dac_vol = 0;
        } else if (dac_vol > 65) {
            dac_vol = 65;
        }
        dac_set_volume(dac_vol);
    }
    return true;
}

//������������
#pragma location="TASK_FMPLAY_SEG"
__near_func bool user_set_volume_fm(u8 volume)
{
    user_change_volume_fm(volume);

    if (volume == sys_ctl.volume) {
        return false;                             //û�и���
    }
    if (volume <= VOLUME_MAX) {
        sys_ctl.volume = volume;

        //�ĵ��������ɿ��ż���
        //sys_param_write8(PARAM_VOLUME, volume);
    }
    return true;
}
#endif
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

#pragma location="TASK_FM_SEG"
void task_fm_select_freq_chan(u8 num)
{
    u16 freq_chan_index = numbox(num, NULL);

    if(freq_chan_index < MIN_RREQ/10) {         //С����СƵ��ʱ������ѡƵ��
        if(freq_chan_index != 0 && freq_chan_index <= t_fm.ch_cnt){
            t_fm.ch_sel = freq_chan_index;
            t_fm.freq = fm_ch[t_fm.ch_sel-1];

#if (IS_LCDSEG_DISPLAY || IS_LEDSEG_DISPLAY)
            show_freq_chan();                   //LCDSEG��LEDSEGʱ��ʾ����Ƶ���л�ʱ��Ƶ����
#endif
            task_fm_set_freq();
        }
    } else {                                    //����ʱ������ѡƵ��
        if(freq_chan_index >= MIN_RREQ/10 && freq_chan_index <= MAX_FREQ/10){
            t_fm.freq  = freq_chan_index * 10;
            task_fm_set_freq();
        }
    }
#if IS_LCD_DISPLAY
    //task_fm_display_init();
    task_fm_disp_init();
#endif
}

#pragma constseg="TASK_FM_SEG_CONST"
IAR_CONST char str_show_taskfm[] = "task_fm\n";
#pragma constseg=default

//����������
#pragma location="TASK_FM_SEG"
void task_fm(void)
{
    printf(str_show_taskfm);
    task_fm_enter();
    while (task_ctl.work_sta == TASK_FM) {
        u8 msg = get_msg();
        task_fm_event();
        task_fm_deal_msg(msg);
        task_fm_display();
    }
    task_fm_exit();
}
#endif
