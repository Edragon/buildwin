/*****************************************************************************
 * Module    : Task
 * File      : task_fm_opa.c
 * Author    : junwen
 * Email     : NC
 * Function  : 收音机任务 (FM_VOUT不经过主控)
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

extern u32 WriteTotalData;              //文件系统写数据时候使用
extern void task_fm_display(void);              //收音机显示接口
extern void task_fm_dispmem_init(void);         //预装数据
extern void task_fm_play_dispmem_init(void);
extern void task_fm_list_dispmem_init(void);


void dac_pull_down(void)
{
    music_mute();   //配置DAC时有杂音，MUTE住功放
    ATCON2 |= BIT(2);delay_5ms(20); //disable VCM
    ATCON1 |= BIT(7);delay_5ms(1);  //pull down hp to ground
    ATCON2 |= BIT(0);delay_5ms(1);  //pull VCM to zero
    ATCON0 &= ~BIT(3);delay_5ms(1); //disable DAC
    ATCON0 &= ~BIT(1);delay_5ms(1); //disable hp
    //UART1CON &= ~BIT(4);
    //ATCON3 = 0x1f;          //最大化阻抗到170Kohm
    //P3DIR &= ~BIT(6);
    //P3 &= ~BIT(6);          //将端口设为输出0
    //ATCON1 |= 0x0C;         //enable P36,short right channel to P36
    //delay_5ms(10);
    ATCON1 &= ~BIT(7);delay_5ms(1);
}

void dac_recover()
{
    ATCON1 |= BIT(7);               //pull down hp to gnd
    //ATCON1 &= ~0x7f;                //disable P02
    //P3DIR |= BIT(6);               //将端口设为正常在输入状态
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

#if IIC_REUSE_SD                    //复用SD时先输出一段SDCLK，使SD卡释放SDDAT
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
    delay_5ms(2);                   //复用SD时先输出一段SDCLK，使SD卡释放SDDAT
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

//任务初始化
#pragma location="TASK_FM_INIT"
void task_fm_init(void)
{
#if FMOSC_REUSE_SDCLK && FMOSC_FREQ_SEL == FMOSC_32K
    set_sys_clk(SYS_12M);           //需要通过 SDCLK 推32.728K时钟时，FM模式下工作在12M系统时钟
#else
    set_sys_clk(SYS_24M);           //FM模式下工作在24M系统时钟
#endif
    user_set_volex(0);              //FM音源较大，一般不需要3DB音量补偿
    led_mode_off();
    led_idle();
    draw_wait();
    music_mute();   //FM_VOUT不经过主控直接连接功放时FM初始化会引入杂音，故先MUTE一下功放

    task_fm_reuse_init();
    user_change_volume(sys_ctl.volume); //数字音量

    if (!fm_init()) {
        task_ctl.work_sta = TASK_EXIT;  //FM模块错误
        return;
    }

    fm_off();                       //先关一下FM，防止FM拉低提示音
    delay_5ms(100);

    //提示音
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
    fm_init();                      //上面关闭FM后在此再初始化一下FM

    OPA_CTL_LOW();
    t_fm.disp_flag = 1;
    t_fm.disp_ch_flag = 1;
    t_fm.disp_freq = 0xffff;
    task_fm_load();
    t_fm.ch_sel = param_read8(PARAM_FM_CHSEL);              //当前台号
    if(t_fm.ch_cnt >= 100 || t_fm.ch_sel > 100)
    {
        t_fm.ch_cnt = 0;
        t_fm.ch_sel = 0;
    }
    if (t_fm.ch_sel > t_fm.ch_cnt || t_fm.ch_sel == 0) {
        t_fm.ch_sel = t_fm.ch_cnt;
    }

    t_fm.freq = param_read16(PARAM_FM_FREQ);                //起始频率
    t_fm.seek_sta = SEEK_STOP;
    t_fm.fm_sta = FM_PLAY;
    task_fm_fix();
    fm_set_freq(t_fm.freq);

    WATCHDOG_CLR();

    //开始播放
    fm_set_vol(sys_ctl.volume); //设置FM模块音量为系统音量，开始播放
    music_unmute();             //上面为防止蓝牙通信干扰引入高频噪声MUTE了功放，在此再解MUTE
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
    fm_off();       //播放提示音时需要关闭FM，否则FM会把DAC声音拉低
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
    task_fm_load(); //由于fm_ch[]是保存在DCODE_RAM的，播放提示音后会被清零，需要重新LOAD一下

    fm_init();                                  //FM初始化
    t_fm.ch_sel = param_read8(PARAM_FM_CHSEL);  //读取当前台号
    t_fm.freq = param_read16(PARAM_FM_FREQ);    //读取当前频率
    task_fm_fix();                              //校正频道
    fm_set_freq(t_fm.freq);                     //设置频率

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

//保存频道
#pragma location="TASK_FM_INIT"
void task_fm_save(void)
{
    param_write8(PARAM_FM_CH, t_fm.ch_cnt);
    param_write(fm_ch, PARAM_FM_CH+2, sizeof(fm_ch));
}

//任务退出
#pragma location="TASK_FM_SEG_1"
void task_fm_exit(void)
{
#if IICCLK_REUSE_USB
    USB_DPDM_GPIO_EN(); //防止退出FM时因IIC_CLK与USB_DM复用导致IIC通信异常，从而导致FM没有关掉一直有声音输出的问题
#endif

    music_mute();   //配置DAC后退出FM时有杂音，需MUTE住功放
    fm_set_vol(0);
    fm_off();

#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
    fm_off();

    if(t_fm.seek_sta == SEEK_AUTO) {
        task_fm_auto_stop();
    }
    param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //保存当前台号
    param_write16(PARAM_FM_FREQ, t_fm.freq);                //保存起始频率

    OPA_CTL_HIGH();

    task_fm_reuse_exit();
    dac_recover();
}

//载入频道
#pragma location="TASK_FM_INIT"
void task_fm_load(void)
{
    t_fm.ch_cnt = param_read8(PARAM_FM_CH);
    param_read(fm_ch, PARAM_FM_CH+2, sizeof(fm_ch));
}

//开始搜台
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_auto_start(void)
{
    t_fm.freq = MIN_RREQ;
    t_fm.ch_cnt = 0;
    t_fm.ch_sel = 0;
    t_fm.fm_sta = FM_AUTO_SEEK;
    led_busy();
    ocx_clear();

    music_mute();   //QN8065搜台过程中MUTEQN8065仍然有啪啪声，故MUTE住功放，停止搜台时再打开
}

//停止搜台
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

//停止搜台后播放
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_auto_play(void)
{
    if(t_fm.ch_sel) {
        t_fm.freq = fm_ch[t_fm.ch_sel-1];
        task_fm_fix();
        fm_set_freq(t_fm.freq);
    }
    fm_set_vol(sys_ctl.volume);
    music_unmute();  //QN8065搜完台后不MUTE住功放
}

//下一频率
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

    param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //保存当前台号
    param_write16(PARAM_FM_FREQ, t_fm.freq);                //保存起始频率
    return false;
}

//校正频道
#pragma location="TASK_FM_SEG"
void task_fm_fix(void)
{
    if (t_fm.freq < MIN_RREQ) {
        t_fm.freq = MAX_FREQ;
    } else if (t_fm.freq > MAX_FREQ) {
        t_fm.freq = MIN_RREQ;
    }
}

//选好频点后设置频点
#pragma location="TASK_FMPLAY_SEG"
__near_func void task_fm_set_freq(void)
{
    fm_set_vol(0);

    task_fm_fix();
    fm_set_freq(t_fm.freq);
    delay_5ms(20);//QN8065设置频点后要延时，否则有沙沙声

    fm_set_vol(sys_ctl.volume);
}

//上一频道
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
        show_freq_chan();   //LCDSEG及LEDSEG时显示正在频道切换时的频道号
#endif

    } else {
        t_fm.freq -= 10;
    }

    task_fm_set_freq();
}

//下一频道
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
        show_freq_chan();   //LCDSEG及LEDSEG时显示正在频道切换时的频道号
#endif

    } else {
        t_fm.freq += 10;
    }

    task_fm_set_freq();
}

//任务事务处理
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
            music_unmute();     //QN8065搜到真台后不MUTE,让其自动播放一小段时间
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
        music_mute();   //QN8065搜台时MUTE住功放
        t_fm.fm_sta = FM_AUTO_SEEK;
        task_fm_auto_nextfreq();
        break;
    }

#if FM_AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((sys_ctl.sleep_cnt == 1 || sys_flag.ir_power_flag) && !sys_flag.lowpower_flag){
        music_mute();
        fm_off();                               //关闭FM模式以降低功耗
        //task_fm_reuse_exit();
        hold_mode_enter();
        task_fm_reuse_init();

        fm_init();      //FM模块初始化
        task_fm_load(); //载入频道总数
        t_fm.ch_sel = param_read8(PARAM_FM_CHSEL);  //读取当前台号
        t_fm.freq = param_read16(PARAM_FM_FREQ);    //读取当前频率
        task_fm_fix();          //校正频道
        fm_set_freq(t_fm.freq); //设置频率
        fm_set_vol(sys_ctl.volume);
        music_unmute();         //FM初始化后才解MUTE，防止啪声
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
    if (music_freq_get()) {//频谱计算完成
        /*for(u8 i=0;i<16;i++)
        {
            printf("%04x ",freq_buf[i]);
        }
        printf("\n");*/
        music_freq_kick();//kick下一次频谱计算
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

//保存设置音量
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

//保存设置音量
#pragma location="TASK_FMPLAY_SEG"
__near_func bool user_set_volume_fm(u8 volume)
{
    user_change_volume_fm(volume);

    if (volume == sys_ctl.volume) {
        return false;                             //没有更改
    }
    if (volume <= VOLUME_MAX) {
        sys_ctl.volume = volume;

        //改到长按键松开才记忆
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

    if(freq_chan_index < MIN_RREQ/10) {         //小于最小频点时，用于选频道
        if(freq_chan_index != 0 && freq_chan_index <= t_fm.ch_cnt){
            t_fm.ch_sel = freq_chan_index;
            t_fm.freq = fm_ch[t_fm.ch_sel-1];

#if (LCDSEG_THEME || LEDSEG_THEME)
            show_freq_chan();                   //LCDSEG及LEDSEG时显示正在频道切换时的频道号
#endif
            task_fm_set_freq();
        }
    } else {                                    //大于时，用于选频点
        if(freq_chan_index >= MIN_RREQ/10 && freq_chan_index <= MAX_FREQ/10){
            t_fm.freq  = freq_chan_index * 10;
            task_fm_set_freq();
        }
    }
#if LCD_THEME
    task_fm_display_init();
#endif
}

//任务消息处理
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
        //自动搜台
    case KU_PLAY:
    case KU_PLAY_S10:
        if(t_fm.seek_sta == SEEK_AUTO){
            task_fm_auto_stop();        //停止自动搜台
            task_fm_auto_play();
        } else {
#if KU_PLAY_TO_SEEKCHANNEL
            task_fm_auto_start();       //S10开始自动搜台
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
        //自动搜台
    case KL_PLAY:
    case KL_PLAY_S10:
        if(t_fm.seek_sta == SEEK_AUTO){
            task_fm_auto_stop();        //停止自动搜台
            task_fm_auto_play();
        } else {
            task_fm_auto_start();
            t_fm.seek_sta = SEEK_AUTO;
            sys_ctl.sleep_cnt = 0;
        }
        break;
#endif

        //上一曲，下一曲
    case KU_PREV:
    case KU_PREV_S10:
        if(t_fm.seek_sta == SEEK_STOP)
        {
            task_fm_prevch();
            param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //保存当前台号
            param_write16(PARAM_FM_FREQ, t_fm.freq);                //保存起始频率
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
            param_write8(PARAM_FM_CHSEL, t_fm.ch_sel);              //保存当前台号
            param_write16(PARAM_FM_FREQ, t_fm.freq);                //保存起始频率
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
        //调节音量
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
        if(t_fm.seek_sta == SEEK_AUTO){ //短按、长按SCAN键均可停止搜台
            task_fm_auto_stop();        //停止自动搜台
            task_fm_auto_play();
            sys_ctl.sleep_cnt = 0;
        }
        else if(msg == KL_SCAN){        //长按SCAN键开始搜台
            task_fm_auto_start();       //开始自动搜台
            t_fm.seek_sta = SEEK_AUTO;
        }
        break;
#endif
    default:
        deal_msg(msg);
        break;
    }
}

//FM播放主流程
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

//任务主流程
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
