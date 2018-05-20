/*****************************************************************************
 * Module    : Task
 * File      : task_aux.c
 * Author    : jingfa
 * Email     :
 * Function  : AUX任务
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_record.h"
#include "user_fm.h"

#if TASK_AUX_EN

void task_aux_display(void);
void task_aux_event(void);

#pragma location="TASK_AUX_SEG"
void task_aux_mp3en_init(void)
{
    //选择LineIN录音
    mp3en_source_init(RECORD_LINEIN);
#if AUX_AGC_AMPLIFY
    AGCANLCON |= 0x70;
    ATCON5 |= BIT(2);
#endif

#if AUX_SOUND_DETECT_EN
    memset(mp3_adc_buf, 0x00, 0xd80);
    sound_ctl_init(10, 0x200, 100, 0x200);        //连续10次超过0x200认为开始有声；连续100次0x200认为静音
    sound_detect_init(mp3_adc_buf, 576);
    sound_detect_enable(0);
    mp3en_start(false);
#endif

#if AUX_FREQ_SHOW
    mp3en_start(false);
    music_freq_init(FREQ_ADC);
#if LCD_THEME
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
#endif

    delay_5ms(50);  //延时500ms再淡入DAC以消除AUX通道配置带来的噪声
    dac_fade_in(0); //淡入DAC以播放AUX的声音
}

//任务初始化
#pragma location="TASK_AUX_SEG"
void task_aux_enter(void)
{
#if LINEIN_DETECT_EN
    if (!device_activate(DEVICE_LINEIN)) {      //检查linein
        task_ctl.work_sta = TASK_EXIT;          //设备无效，返回主菜单
        return;
    }
#endif

    draw_wait();
    led_mode_off();
    led_aux_play();

    set_sys_clk(SYS_24M);
    user_set_volex(0);                          //若AUX声音较小可注释此行，可增加3DB音量补偿

#if ((AUX_CHANNEL_SEL & 0x07) == CHANNEL_1)
#if DEBUG_UART_SEL
    uart_disable();
#endif
    P0DIR |= BIT(1);
#endif


#if AUX_WARNING_MUSIC
    if (work_sta_interrupt == TASK_NULL) {
        dac_enable();
        mp3_res_play_wait(RES_MP3_AUDIO_INPUT);
    }
#endif
    work_sta_interrupt = TASK_NULL;
    dac_disable();

    //录音控制器初始化
    task_aux_mp3en_init();
    music_unmute();

#if KEY_VOICE_EN
    key_voice_init();
#endif
#if LCD_THEME
    t_rec.update_flag = 1;
#endif
}

//任务退出
#pragma location="TASK_AUX_SEG"
void task_aux_exit(void)
{
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif

    mp3en_exit();

#if (DEBUG_UART_SEL && ((AUX_CHANNEL_SEL & 0x07) == CHANNEL_1))
    P0DIR &= ~BIT(1);
    uart_enable();
#endif
}



#if VOL_MAXMIN_MUSIC
#pragma location="TASK_RECORD_SEG_1"
void record_play_vol_music(void)
{
#if !VOL_MUSIC_ONCE
    mp3en_exit();   //关闭AUX输出
    dac_enable();
    mp3_res_play_wait(RES_MP3_VOL_MAXMIN);
    flush_msg();
    dac_disable();
#else
    if(!sys_ctl.vol_maxmin_music_flag){
        sys_ctl.vol_maxmin_music_flag = 2;
        mp3en_exit();   //关闭AUX输出
        dac_enable();
        mp3_res_play_wait(RES_MP3_VOL_MAXMIN);
        dac_disable();
        task_aux_mp3en_init();
    }
#endif
}
#endif

#pragma location="TASK_AUX_SEG"
void task_aux_deal_msg(u8 msg)
{
#if VOL_MAXMIN_MUSIC
    if(sys_ctl.vol_maxmin_music_flag == 1){
        #if !VOL_MUSIC_ONCE
            task_aux_mp3en_init();
        #endif

        sys_ctl.vol_maxmin_music_flag = 0;
        flush_msg();
        return;
    }
#endif

    switch(msg) {
    case KU_PLAY:
    case KU_PLAY_S10:
#if KU_PLAY_LINEIN_MUTE_EN
        //控制MUTE
        if(sys_flag.mute_flag) {
            led_aux_play();
            if(sys_ctl.volume) {
                dac_fade_in(0);
                music_unmute();
                sys_ctl.sleep_cnt = 0;
            }
        } else {
            led_aux_pause();
            dac_fade_out(0);
            dac_anl_fade_wait();
            music_mute();
        #if AUX_AUTO_STANDBY_EN
            sys_ctl.sleep_cnt = STANDBY_TIME;
        #endif
        }
#elif AUX_RECORD_EN
        //开始录音
        if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
            task_record(RECORD_LINEIN);
            dac_fade_in(0); //淡入DAC以播放AUX声音
            led_aux_play();
        #if AUX_SOUND_DETECT_EN
            mp3en_start(false);
        #endif
        } else {
            show_no_device();
#if LCD_THEME
            t_rec.update_flag = 1;
#endif
        }
#endif
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
        t_rec.update_flag = 1;
        break;
#else
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
#if AUX_KU_NEXT_PREV_VOL
    case KU_NEXT:
    case KU_NEXT_S10:
#endif
    case KH_NEXT_S10:
        user_set_volume(user_volume_inc(sys_ctl.volume));
#if KU_PLAY_LINEIN_MUTE_EN || HARD_IR_EN || SOFT_IR_EN
        if(sys_flag.mute_flag) {
            if(sys_ctl.volume) {
                led_aux_play();
                music_unmute();
                dac_fade_in(0);
                sys_ctl.sleep_cnt = 0;
            }
        }
#endif
        show_volume();

#if VOL_MAXMIN_MUSIC
        if(sys_ctl.volume == VOLUME_MAX){
            record_play_vol_music();
        }
#endif
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
#if AUX_KU_NEXT_PREV_VOL
    case KU_PREV:
    case KU_PREV_S10:
#endif
    case KH_PREV_S10:
        user_set_volume(user_volume_dec(sys_ctl.volume));
#if KU_PLAY_LINEIN_MUTE_EN || HARD_IR_EN || SOFT_IR_EN
        if(sys_flag.mute_flag) {
            if(sys_ctl.volume) {
                led_aux_play();
                music_unmute();
                dac_fade_in(0);
                sys_ctl.sleep_cnt = 0;
            }
        }
#endif
        show_volume();

#if VOL_MAXMIN_MUSIC
        if(!sys_ctl.volume){
            record_play_vol_music();
        }
#endif
        break;
#endif
#if (USER_SEL == USER_S10 || USER_SEL == USER_2226_S10)
    case KL_PLAY_S10:
        if(device_is_online(DEVICE_SDMMC) || fm_online_flag) {
            task_ctl.work_sta = TASK_EXIT;
        }
        break;
#endif
        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}
#pragma location="TASK_AUX_SEG"
void task_aux_event(void)
{
    WATCHDOG_CLR();

    comm_event();

#if LINEIN_DETECT_EN
    if (!device_activate(DEVICE_LINEIN)) {
        task_ctl.work_sta = TASK_EXIT;
    }
#endif

#if AUX_AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((sys_ctl.sleep_cnt == 1 || sys_flag.ir_power_flag) && !sys_flag.lowpower_flag){
        dac_fade_out(0);                        //使能AUX时要先淡出，防止hold_mode时DAC输出噪声
        dac_anl_fade_wait();                    //等待淡出完成
        music_mute();
        hold_mode_enter();
        dac_fade_in(0);
        music_unmute();
    }
#endif
#if AUX_FREQ_SHOW
    if (music_freq_get()) {//频谱计算完成
        /*for(u8 i=0;i<32;i++)
        {
            printf("%04x ",freq_buf[i]);
        }
        printf("\n");*/
        music_freq_kick();//kick下一次频谱计算
    }
#endif
#if AUX_SOUND_DETECT_EN
    if (sound_flag()) {
        //printf("1 ");
    } else {
        //printf("0 ");
    }
#endif

#if LOWPOWER_WARNING
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        mp3en_exit();   //关闭AUX输出
        dac_enable();
        mp3_res_play_wait(RES_MP3_PLEASE_CHARGE);
        dac_disable();
        task_aux_mp3en_init();
    }
#endif
}

#pragma constseg="TASK_AUX_CONST"
IAR_CONST char str_task_aux[] = "task_aux\n";
#pragma constseg=default

//任务主流程
#pragma location="TASK_AUX_SEG"
void task_aux(void)
{
    printf(str_task_aux);
    task_aux_enter();
    while (task_ctl.work_sta == TASK_AUX) {
        task_aux_event();
        task_aux_deal_msg(get_msg());
        task_aux_display();
    }
    task_aux_exit();
}

#endif
