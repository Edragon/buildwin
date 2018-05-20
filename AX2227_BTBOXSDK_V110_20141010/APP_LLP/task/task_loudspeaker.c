/*****************************************************************************
 * Module    : Task
 * File      : task_record.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 录音任务流程
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_music.h"
#include "user_spi.h"

#if TASK_LOUDSPEAKER_EN
#pragma constseg="TASK_SPEAKER_CONST"
IAR_CONST char str_task_speaker[] = "task_speaker\n";
#pragma constseg=default

void task_speaker_display(void);
void mix_step_set(u8 step);
void lcd_speaker_step_set(void);
IAR_XDATA_A type_ctl_spk t_spk @ "SPEAKER_XDATA";


#pragma location="TASK_SETTING_SEG"
bool task_mix_step_set(u8 step)
{
    if (step == t_spk.step) {
        return false;                             //没有更改
    }
    if (t_spk.step <= MIX_STEP_MAX) {
        t_spk.step = step;
        mix_step_set(step);
        param_write8(PARAM_MIX_STEP, t_spk.step);
    }
    return true;
}

#pragma location="TASK_SPEAKER_SEG"
void dac_out_init(void)
{
    AUCON0 = (BIT(1)|BIT(2));//sync with dac en;
}


//录音控制器初始化
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_mp3en_init(void)
{
    //关闭AMUX模块
    mp3en_exit();
    //初始化AMUX模块
    mp3en_source_init(MIC_LOUD_SPEAKER);
#if AUX_FREQ_SHOW
    mp3en_start(false);
    music_freq_init(FREQ_ADC);
#if LCD_THEME
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
#endif
}

//录音任务初始化
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_enter(void)
{
    led_mode_off();
    led_idle();

    draw_wait();
    set_sys_clk(SYS_24M);
    sound_ctl_init(3, 0x800, 25, 0x800);     //连续3次超过0x800认为开始有声；连续25次0x800认为静音
    user_change_volume(sys_ctl.volume);//数字音量
    WATCHDOG_CLR();

#if LOUDSPEAKER_WARNING_MUSIC
    dac_enable();
    mp3_res_play_wait(RES_MP3_LOUD_SPEAKER_MODE);
    dac_disable();
#endif

    //加载LOUD SPEAKER程序
    load_code(DCODE_LOUDSPK, 1);
    task_speaker_mp3en_init();

    t_spk.step = param_read8(PARAM_MIX_STEP);
    if(t_spk.step > MIX_STEP_MAX) {
        t_spk.step = 3;
    }
    mix_step_set(t_spk.step);   //要先设置step，再开启ADC
    param_write8(PARAM_MIX_STEP, t_spk.step);
    t_spk.updata_flag = 1;
    dac_enable();       //先开启DAC
    mp3en_start(false); //再开启ADC
    AGCCON0 = 0x17;     //agc_disable, ADD_STEP=0.05db  //max gain 84 db*/
    dac_fade_in(1);
    music_unmute();
}

//停止
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_stop(void)
{
    mp3en_stop();
    ATCON5 &= ~BIT(2);
}

#pragma location="TASK_SPEAKER_SEG"
void task_speaker_exit(void)
{
    task_speaker_stop();
    mp3en_exit();
}

//录音任务事务处理
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_event(void)
{
    comm_event();                           //调用公共事件

#if IR_HARD_POWERDOWN_EN
    if((sys_ctl.sleep_cnt == 1 || sys_flag.ir_power_flag) && !sys_flag.lowpower_flag){
        dac_fade_out(1);                        //使能AUX时要先淡出，防止hold_mode时DAC输出噪声
        dac_anl_fade_wait();                    //等待淡出完成
        music_mute();
        mp3en_stop();
        hold_mode_enter();
        mp3en_start(false);
        music_unmute();
        dac_fade_in(1);
    }
#endif
}

//录音任务消息处理
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_deal_msg(u8 msg)
{
    switch(msg) {
#if LCD_THEME
    case KU_PREV:
    case KU_NEXT:
        lcd_speaker_step_set();
        t_spk.updata_flag = 1;
        break;

        //音量加减
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_S10:
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_S10:
        lcd_volume_set();
        t_spk.updata_flag = 1;
        break;
#else
    case KU_PREV:
    case KU_PREV_S10:
        task_mix_step_set(user_mixstep_dec(t_spk.step));
        show_mix_step();
        break;
    case KU_NEXT:
    case KU_NEXT_S10:
        task_mix_step_set(user_mixstep_inc(t_spk.step));
        show_mix_step();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_S10:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_flag.mute_flag) {
            if(sys_ctl.volume) {
                music_unmute();
                dac_fade_in(1);
            }
        }
#endif
        break;
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_S10:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_flag.mute_flag) {
            if(sys_ctl.volume) {
                music_unmute();
                dac_fade_in(1);
            }
        }
#endif
        break;
#endif
        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}

//录音任务
#pragma location="TASK_SPEAKER_SEG"
void task_loud_speaker(void)
{
    printf(str_task_speaker);
    task_speaker_enter();
    ENTER_DCODE();
    while (task_ctl.work_sta == TASK_LOUD_SPEAKER) {
        u8 msg = get_msg();
        task_speaker_event();
        task_speaker_deal_msg(msg);
        task_speaker_display();
    }
    EXIT_DCODE();

    task_speaker_exit();
}

#endif
