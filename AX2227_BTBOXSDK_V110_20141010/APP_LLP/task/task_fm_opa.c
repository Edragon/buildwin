/*****************************************************************************
 * Module    : Task
 * File      : task_fm_opa.c
 * Author    : junwen
 * Email     : NC
 * Function  : ���������� (FM_VOUT����������)
 *****************************************************************************/

#include "include.h"
#include "task.h"
#include "user_fm.h"
#include "task_fm.h"

#if TASK_FM_EN && !FM_VOUT_THROUGH_MCU

#define DEBUG_TASK_FM           0

#define FM_ITEMS_NUM            5
#define MAX_FREQ                10800

#if FM_TYPE == FM_RTC6218
#define MIN_RREQ                8740
#else
#define MIN_RREQ                8750
#endif

enum {
    SEEK_STOP,SEEK_AUTO,
};
enum {
    ITEM_AUTO_SEEK,ITEM_DELETE,ITEM_DEL_ALL,ITEM_EXIT,
};
struct {
    char *ptr;
} disp_fm;


IAR_XDATA_A u16 fm_ch[64] @ "FM_CH_BUF";
IAR_XDATA_A type_fm_task t_fm @ "FM_XDATA"; //task fm controller

void task_fm_fix(void);
void task_fm_load(void);
void task_fm_save(void);
void task_fm_display_init(void);
__near_func void task_fm_event(void);
__near_func void task_fm_auto_stop(void);

extern u32 WriteTotalData;              //�ļ�ϵͳд����ʱ��ʹ��
extern void task_fm_display(void);              //��������ʾ�ӿ�
extern void task_fm_dispmem_init(void);         //Ԥװ����
extern void task_fm_play_dispmem_init(void);
extern void task_fm_list_dispmem_init(void);


void dac_pull_down(void)
{
    music_mute();   //����DACʱ��������MUTEס����
    ATCON2 |= BIT(2);delay_5ms(20); //disable VCM
    ATCON1 |= BIT(7);delay_5ms(1);  //pull down hp to ground
    ATCON2 |= BIT(0);delay_5ms(1);  //pull VCM to zero
    ATCON0 &= ~BIT(3);delay_5ms(1); //disable DAC
    ATCON0 &= ~BIT(1);delay_5ms(1); //disable hp
    //UART1CON &= ~BIT(4);
    //ATCON3 = 0x1f;          //����迹��170Kohm
    //P3DIR &= ~BIT(6);
    //P3 &= ~BIT(6);          //���˿���Ϊ���0
    //ATCON1 |= 0x0C;         //enable P36,short right channel to P36
    //delay_5ms(10);
    ATCON1 &= ~BIT(7);delay_5ms(1);
}

void dac_recover()
{
    ATCON1 |= BIT(7);               //pull down hp to gnd
    //ATCON1 &= ~0x7f;                //disable P02
    //P3DIR |= BIT(6);               //���˿���Ϊ����������״̬
    //UART1CON |= BIT(4);

    ATCON0 |= BIT(1);delay_5ms(1);  //enable hp
    //ATCON0 |= BIT(3);delay_5ms(1);  //enable dac
    ATCON2 &= ~BIT(0);              // release vcm pops
    ATCON1 &= ~BIT(7);              //disable pops
    ATCON2 &= ~BIT(2);              //enable vcm
    delay_5ms(20);                  // wait for vcm settling
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

#if FMOSC_REUSE_SDCLK
    SD_CLK_DIR_OUT();
    SDCON0 |= BIT(0);               //Enable SDC
  #if FMOSC_FREQ_SEL == FMOSC_32K
    sd_set_rate0(0xb7);             //12MHz / 2 / (SDBAUD + 1) = 32.768K
  #else
    sd_set_rate0(0);                //24MHz / 2 / (SDBAUD + 1) = 12M
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
#if FMOSC_REUSE_SDCLK
    SDCON0 &= ~BIT(1);                   //suspend sdclk
    sd_reactive();
#endif

#if IICCLK_REUSE_USB
    usb_host_reactive();
#endif
}

//�����ʼ��
#pragma location="TASK_FM_INIT"
void task_fm_init(void)
{
#if FMOSC_REUSE_SDCLK && FMOSC_FREQ_SEL == FMOSC_32K
    set_sys_clk(SYS_12M);           //��Ҫͨ�� SDCLK ��32.728Kʱ��ʱ��FMģʽ�¹�����12Mϵͳʱ��
#else
    set_sys_clk(SYS_24M);           //FMģʽ�¹�����24Mϵͳʱ��
#endif
    user_set_volex(0);              //FM��Դ�ϴ�һ�㲻��Ҫ3DB��������
    led_mode_off();
    led_idle();
    draw_wait();
    music_mute();   //FM_VOUT����������ֱ�����ӹ���ʱFM��ʼ������������������MUTEһ�¹���

    task_fm_reuse_init();
    user_change_volume(sys_ctl.volume); //��������

    if (!fm_init()) {
        task_ctl.work_sta = TASK_EXIT;  //FMģ�����
        return;
    }

    fm_off();                       //�ȹ�һ��FM����ֹFM������ʾ��
    delay_5ms(100);

    //��ʾ��
    music_unmute();
#if FM_WARNING_MUSIC
    if (work_sta_interrupt == TASK_NULL) {
        dac_enable();
        mp3_res_play_wait(RES_MP3_FM_MODE);
    }
#endif
    work_sta_interrupt = TASK_NULL;
    dac_disable();

    dac_pull_down();
    delay_5ms(10);
    fm_init();                      //����ر�FM���ڴ��ٳ�ʼ��һ��FM

    OPA_CTL_LOW();
    t_fm.disp_flag = 1;
    t_fm.disp_ch_flag = 1;
    t_fm.disp_freq = 0xffff;
    task_fm_load();
    t_fm.ch_sel = param_read8(PARAM_FM_CHSEL);              //��ǰ̨��
    if(t_fm.ch_cnt >= 100 || t_fm.ch_sel > 100)
    {
        t_fm.ch_cnt = 0;
        t_fm.ch_sel = 0;
    }
    if (t_fm.ch_sel > t_fm.ch_cnt || t_fm.ch_sel == 0) {
        t_fm.ch_sel = t_fm.ch_cnt;
    }

    t_fm.freq = param_read16(PARAM_FM_FREQ);                //��ʼƵ��
    t_fm.seek_sta = SEEK_STOP;
    t_fm.fm_sta = FM_PLAY;
    task_fm_fix();
    fm_set_freq(t_fm.freq);

    WATCHDOG_CLR();

    //��ʼ����
    fm_set_vol(sys_ctl.volume); //����FMģ������Ϊϵͳ��������ʼ����
    music_unmute();             //����Ϊ��ֹ����ͨ�Ÿ��������Ƶ����MUTE�˹��ţ��ڴ��ٽ�MUTE
#if FM_FREQ_SHOW
    mp3en_start(false);
    music_freq_init(FREQ_ADC);
#if LCD_THEME
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
#endif
#if KEY_VOICE_EN
    key_voice_init();
#endif
    draw_clear();
}

#pragma location="TASK_FM_SEG_1"
void task_fm_menu_music(u8 menu_music_num)
{
    fm_off();       //������ʾ��ʱ��Ҫ�ر�FM������FM���DAC��������
    dac_recover();
    mp3en_exit();
    dac_enable();

#if VOL_MAXMIN_MUSIC && !VOL_MUSIC_ONCE
    while(sys_ctl.vol_maxmin_music_flag != 1)
#endif
    {
        mp3_res_play_wait(menu_music_num);
    }

    dac_disable();
#if FM_FREQ_SHOW
    mp3en_start(false);
    music_freq_init(FREQ_ADC);
  #if LCD_THEME
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
  #endif
#endif
    load_code(DCODE_FM, 3);
    task_fm_load(); //����fm_ch[]�Ǳ�����DCODE_RAM�ģ�������ʾ����ᱻ���㣬��Ҫ����LOADһ��

    fm_init();                                  //FM��ʼ��
    t_fm.ch_sel = param_read8(PARAM_FM_CHSEL);  //��ȡ��ǰ̨��
    t_fm.freq = param_read16(PARAM_FM_FREQ);    //��ȡ��ǰƵ��
    task_fm_fix();                              //У��Ƶ��
    fm_set_freq(t_fm.freq);                     //����Ƶ��

    bool sys_mute_flag_temp = sys_flag.mute_flag;
    dac_pull_down();
    fm_set_vol(sys_ctl.volume);
    if (!sys_mute_flag_temp){
        music_unmute();
    }
}

#if VOL_MAXMIN_MUSIC
#pragma location="TASK_FM_SEG_1"
void fm_play_vol_music(void)
{
#if !VOL_MUSIC_ONCE
    if(!sys_ctl.vol_maxmin_music_flag){
        sys_ctl.vol_maxmin_music_flag = 2;
    }
    task_fm_menu_music(RES_MP3_VOL_MAXMIN);
    flush_msg();
#else
    if(!sys_ctl.vol_maxmin_music_flag){
        sys_ctl.vol_maxmin_music_flag = 2;
        task_fm_menu_music(RES_MP3_VOL_MAXMIN);
    }
#endif
}
#endif

//����Ƶ��
#pragma location="TASK_FM_INIT"
void task_fm_save(void)
{
    param_write8(PARAM_FM_CH, t_fm.ch_cnt);
    param_write(fm_ch, PARAM_FM_CH+2, sizeof(fm_ch));
}

//�����˳�
#pragma location="TASK_FM_SEG_1"
void task_fm_exit(void)
{
#if IICCLK_REUSE_USB
    USB_DPDM_GPIO_EN(); //��ֹ�˳�FMʱ��IIC_CLK��USB_DM���õ���IICͨ���쳣���Ӷ�����FMû�йص�һֱ���������������
#endif

    music_mute();   //����DAC���˳�FMʱ����������MUTEס����
    fm_set_vol(0);
    fm_off();

#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
    fm_off();

    if(t_fm.seek_sta == SEEK_AUTO) {
        task_fm_auto_stop();
    }
    param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
    param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��

    OPA_CTL_HIGH();

    task_fm_reuse_exit();
    dac_recover();
}

//����Ƶ��
#pragma location="TASK_FM_INIT"
void task_fm_load(void)
{
    t_fm.ch_cnt = param_read8(PARAM_FM_CH);
    param_read(fm_ch, PARAM_FM_CH+2, sizeof(fm_ch));
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

    music_mute();   //QN8065��̨������MUTEQN8065��Ȼ��žž������MUTEס���ţ�ֹͣ��̨ʱ�ٴ�
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
    fm_set_vol(sys_ctl.volume);
    music_unmute();  //QN8065����̨��MUTEס����
}

//��һƵ��
#pragma location="TASK_FMPLAY_SEG"
__near_func bool task_fm_auto_nextfreq(void)
{
    if (t_fm.freq < MAX_FREQ) {
#if FMOSC_REUSE_SDCLK && (FM_TYPE == FM_QN8065)
        t_fm.freq += 5;
#elif FM_TYPE == FM_RTC6218
        t_fm.freq += 5;
#else
        t_fm.freq += 10;
#endif
        return true;
    }
    task_fm_auto_stop();
    task_fm_auto_play();

    param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
    param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
    return false;
}

//У��Ƶ��
#pragma location="TASK_FM_SEG"
void task_fm_fix(void)
{
    if (t_fm.freq < MIN_RREQ) {
        t_fm.freq = MAX_FREQ;
    } else if (t_fm.freq > MAX_FREQ) {
        t_fm.freq = MIN_RREQ;
    }
}

//ѡ��Ƶ�������Ƶ��
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_set_freq(void)
{
    fm_set_vol(0);

    task_fm_fix();
    fm_set_freq(t_fm.freq);
    delay_5ms(20);//QN8065����Ƶ���Ҫ��ʱ��������ɳɳ��

    fm_set_vol(sys_ctl.volume);
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

#if (LCDSEG_THEME || LEDSEG_THEME)
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


#if (LCDSEG_THEME || LEDSEG_THEME)
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
    switch (t_fm.fm_sta) {
    case FM_AUTO_SEEK:
        fm_chk = fm_check_freq(t_fm.freq);
        if(fm_chk) {
#if FM_TYPE == FM_RTC6218
            t_fm.freq += 5*(fm_chk - 1);
#endif
            fm_ch[t_fm.ch_cnt++] = t_fm.freq;

#if DEBUG_TASK_FM
            printf("true channel: %d\n",t_fm.freq);
#endif
            set_timeout(200);
            t_fm.fm_sta = FM_AUTO_PLAY;
#if SEEK_DISPLAY_CHANNEL_NUM
            t_fm.ch_sel = t_fm.ch_cnt;
            show_freq_chan();
#endif

            fm_unmute();
            fm_set_vol(sys_ctl.volume);
            music_unmute();     //QN8065�ѵ���̨��MUTE,�����Զ�����һС��ʱ��
            led_idle();
            break;
        }
#if DEBUG_TASK_FM
        printf("t_fm.freq: %d\n",t_fm.freq);
#endif
        task_fm_auto_nextfreq();
        break;
    case FM_AUTO_PLAY:
        if (!time_out_flag) {
            break;
        }
#if SEEK_DISPLAY_CHANNEL_NUM
        ocx_clear();
#endif
        led_busy();
        fm_set_vol(0);
        music_mute();   //QN8065��̨ʱMUTEס����
        t_fm.fm_sta = FM_AUTO_SEEK;
        task_fm_auto_nextfreq();
        break;
    }

#if FM_AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((sys_ctl.sleep_cnt == 1 || sys_flag.ir_power_flag) && !sys_flag.lowpower_flag){
        music_mute();
        fm_off();                               //�ر�FMģʽ�Խ��͹���
        //task_fm_reuse_exit();
        hold_mode_enter();
        task_fm_reuse_init();

        fm_init();      //FMģ���ʼ��
        task_fm_load(); //����Ƶ������
        t_fm.ch_sel = param_read8(PARAM_FM_CHSEL);  //��ȡ��ǰ̨��
        t_fm.freq = param_read16(PARAM_FM_FREQ);    //��ȡ��ǰƵ��
        task_fm_fix();          //У��Ƶ��
        fm_set_freq(t_fm.freq); //����Ƶ��
        fm_set_vol(sys_ctl.volume);
        music_unmute();         //FM��ʼ����Ž�MUTE����ֹž��
    }
#endif

#if FM_SOUND_DETECT_EN
    if (sound_flag()) {
        //printf("1 ");
    } else {
        //printf("0 ");
    }
#endif

#if FM_FREQ_SHOW
    if (music_freq_get()) {//Ƶ�׼������
        /*for(u8 i=0;i<16;i++)
        {
            printf("%04x ",freq_buf[i]);
        }
        printf("\n");*/
        music_freq_kick();//kick��һ��Ƶ�׼���
    }
#endif

#if LOWPOWER_WARNING
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        task_fm_menu_music(RES_MP3_PLEASE_CHARGE);
    }
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
        //param_write8(PARAM_VOLUME, volume);
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

#if (LCDSEG_THEME || LEDSEG_THEME)
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
#if LCD_THEME
    task_fm_display_init();
#endif
}

//������Ϣ����
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_deal_msg(u8 msg)
{
#if VOL_MAXMIN_MUSIC
    if(sys_ctl.vol_maxmin_music_flag == 1){
        sys_ctl.vol_maxmin_music_flag = 0;
        flush_msg();
        return;
    }
#endif

    switch(msg) {
        //�Զ���̨
    case KU_PLAY:
    case KU_PLAY_S10:
        if(t_fm.seek_sta == SEEK_AUTO){
            task_fm_auto_stop();        //ֹͣ�Զ���̨
            task_fm_auto_play();
        } else {
#if KU_PLAY_TO_SEEKCHANNEL
            task_fm_auto_start();       //S10��ʼ�Զ���̨
            t_fm.seek_sta = SEEK_AUTO;
#elif KU_PLAY_FM_MUTE_EN
            if(sys_flag.mute_flag) {
                if(sys_ctl.volume) {
                    music_unmute();
                    sys_ctl.sleep_cnt = 0;
                }
            } else {
                music_mute();
            #if FM_AUTO_STANDBY_EN
                sys_ctl.sleep_cnt = STANDBY_TIME;
            #endif
            }
#endif
        }
        break;

#if !KU_PLAY_TO_SEEKCHANNEL
        //�Զ���̨
    case KL_PLAY:
    case KL_PLAY_S10:
        if(t_fm.seek_sta == SEEK_AUTO){
            task_fm_auto_stop();        //ֹͣ�Զ���̨
            task_fm_auto_play();
        } else {
            task_fm_auto_start();
            t_fm.seek_sta = SEEK_AUTO;
            sys_ctl.sleep_cnt = 0;
        }
        break;
#endif

        //��һ������һ��
    case KU_PREV:
    case KU_PREV_S10:
        if(t_fm.seek_sta == SEEK_STOP)
        {
            task_fm_prevch();
            param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
            param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
#if DEBUG_TASK_FM
            printf("channel %d: %dMHz\n",t_fm.ch_sel,t_fm.freq);
#endif
        }
        sys_ctl.sleep_cnt = 0;
        break;
    case KU_NEXT:
    case KU_NEXT_S10:
        if(t_fm.seek_sta == SEEK_STOP)
        {
            task_fm_nextch();
            param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //���浱ǰ̨��
            param_write16(PARAM_FM_FREQ, t_fm.freq);                //������ʼƵ��
#if DEBUG_TASK_FM
            printf("channel %d: %dMHz\n",t_fm.ch_sel,t_fm.freq);
#endif
        }
        sys_ctl.sleep_cnt = 0;
        break;
#if LCD_THEME
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_S10:
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_S10:
        lcd_volume_set();
        task_fm_display_init();
        break;
#else
        //��������
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_S10:
#if FM_SET_VOL_CHANGE_BANK
        user_set_volume(user_volume_dec(sys_ctl.volume));
#else
		user_set_volume_fm(user_volume_dec_fm(sys_ctl.volume));
#endif
        fm_set_vol(sys_ctl.volume);
        show_volume();task_fm_display();

#if VOL_MAXMIN_MUSIC
        if(!sys_ctl.volume){
            fm_play_vol_music();
        }
#endif

#if KU_PLAY_FM_MUTE_EN || HARD_IR_EN || SOFT_IR_EN
    #if VOL_MAXMIN_MUSIC
        if(!sys_ctl.vol_maxmin_music_flag)
    #endif
        {
            if(sys_flag.mute_flag) {
                if(sys_ctl.volume) {
                    music_unmute();
                    sys_ctl.sleep_cnt = 0;
                }
            }
        }
#endif
        break;
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_S10:
#if FM_SET_VOL_CHANGE_BANK
        user_set_volume(user_volume_inc(sys_ctl.volume));
#else
		user_set_volume_fm(user_volume_inc_fm(sys_ctl.volume));
#endif
        fm_set_vol(sys_ctl.volume);
        show_volume();task_fm_display();

#if VOL_MAXMIN_MUSIC
        if(sys_ctl.volume == VOLUME_MAX){
            fm_play_vol_music();
        }
#endif

#if KU_PLAY_FM_MUTE_EN || HARD_IR_EN || SOFT_IR_EN
    #if VOL_MAXMIN_MUSIC
        if(!sys_ctl.vol_maxmin_music_flag)
    #endif
        {
            if(sys_flag.mute_flag) {
                if(sys_ctl.volume) {
                    music_unmute();
                    sys_ctl.sleep_cnt = 0;
                }
            }
        }
#endif
        break;
#endif
#if !FM_SET_VOL_CHANGE_BANK
    case KLU_VOL_DOWN:
    case KLU_VOL_UP:
    case KLU_PREV_S10:
    case KLU_NEXT_S10:
      if (sys_ctl.volume <= VOLUME_MAX) {
            param_write8(PARAM_VOLUME, sys_ctl.volume);
        }
        break;
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
        task_fm_select_freq_chan(msg - T_KEY_NUM_0);
        break;
    case KL_SCAN:
    case KU_SCAN:
        if(t_fm.seek_sta == SEEK_AUTO){ //�̰�������SCAN������ֹͣ��̨
            task_fm_auto_stop();        //ֹͣ�Զ���̨
            task_fm_auto_play();
            sys_ctl.sleep_cnt = 0;
        }
        else if(msg == KL_SCAN){        //����SCAN����ʼ��̨
            task_fm_auto_start();       //��ʼ�Զ���̨
            t_fm.seek_sta = SEEK_AUTO;
        }
        break;
#endif
    default:
        deal_msg(msg);
        break;
    }
}

//FM����������
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_play(void)
{
    task_fm_disp_clear();

    t_fm.disp_flag = 1;
    task_fm_load();
#if LCD_THEME
    task_fm_dispmem_init();
    task_fm_display_init();
    task_fm_play_dispmem_init();
#endif
    while (task_ctl.work_sta == TASK_FM) {
        u8 msg = get_msg();
        task_fm_event();
        task_fm_deal_msg(msg);
        task_fm_display();
    }
}
#pragma constseg="TASK_FM_SEG_CONST"
IAR_CONST char str_show_taskfm[] = "task_fm\n";
#pragma constseg=default

//����������
#pragma location="TASK_FM_SEG"
void task_fm(void)
{
    printf(str_show_taskfm);
    task_fm_init();
    load_code(DCODE_FM, 3);
    ENTER_DCODE();
    task_fm_play();
    EXIT_DCODE();
    task_fm_exit();
}

#endif
