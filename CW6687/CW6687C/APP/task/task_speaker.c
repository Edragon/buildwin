/*****************************************************************************
 * Module    : Task
 * File      : task_speaker.c
 * Author    : Hanny
 * Function  : 扩音器任务流程
 *****************************************************************************/
#include "include.h"

#if TASK_SPEAKER_EN
#include "task.h"
#include "task_speaker.h"

#pragma data_alignment=2
IAR_XDATA_A u16 echo_input              @ "SPEAKER_XDATA";
IAR_XDATA_A u16 volatile echo_read      @ "SPEAKER_XDATA";
IAR_XDATA_A u16 volatile echo_write     @ "SPEAKER_XDATA";
IAR_XDATA_A u8  echo_factor[2]          @ "SPEAKER_XDATA";
IAR_XDATA_A type_task_speaker t_speaker @ "SPEAKER_XDATA";
IAR_XDATA_A u8 echo_buf[ECHO_BUF_LEN]   @ "ECHO_BUF";

#include SET_USER_PATH(CFG_UI_SPEAKER_DIR, /ui/ui_speaker.c)

#pragma location="TASK_SPEAKER_SEG"
void speaker_continue(void)
{
    speaker_load_code(0x68);
    t_speaker.pos = 0;
    echo_read  = ECHO_BUF_SATRT;
    echo_write = ECHO_BUF_SATRT;
    pcm_init(S_SPR_8000);
    adc_init(SEL_SPEAKER);
    adc_start();
    pcm_play();
}

#pragma location="TASK_SPEAKER_SEG"
void mp3res_play_speakermode(u8 music_name)
{
    adc_stop();
    mp3_res_play_wait(music_name);
    speaker_continue();
}

#pragma location="TASK_SPEAKER_SEG"
void speaker_play_vol_music(void)
{
    if(!sys_ctl.vol_maxmin_music_play){
        sys_ctl.vol_maxmin_music_play = 1;
        adc_stop();
    }
    mp3_res_play_wait(RES_MP3_VOL_MAXMIN);
    flush_msg();
}

//AGC中断处理函数
#pragma location="MIX_PROC"
__near_func void task_speaker_process(void)
{
    if (AUCON5 >= 130) {
        FIFO_SPEED = 2;
    } else {
        if (AUCON5 <= 64) {
            FIFO_SPEED = 0;
        }
    }

    if (!t_speaker.pos) {
        echo_input = (u16)adc_buf;
    }
    mix_proc(ECHO_SAMPLES);
    dac_direct_out(echo_write-SPEAKER_SAMPLES, ECHO_SAMPLES, 1);
    if (echo_read >= ECHO_BUF_END) {
        echo_read = ECHO_BUF_SATRT;
    }
    if (echo_write >= ECHO_BUF_END) {
        echo_write = ECHO_BUF_SATRT;
    }
    t_speaker.pos ^= 1;
}

#pragma location="TASK_SPEAKER_SEG"
void mix_step_set(u8 step)
{
    IE_AGC = 0;
    if (!step) {
        echo_factor[0] = 108;
        echo_factor[1] = 0;
        IE_AGC = 1;
        return;
    }
    if (step == ECHO_MAX) {
        echo_write = echo_read;
    } else {
        echo_write = echo_read + (step * ECHO_STEP);
        if (echo_write > ECHO_BUF_END) {
            echo_write -= (ECHO_MAX * ECHO_STEP);
        }
    }
    //printf("r=%04x w=%04x\n", echo_read, echo_write);
    echo_factor[0] = 90;
    echo_factor[1] = 48;
    IE_AGC = 1;
}

//任务事务处理
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_event(void)
{
    comm_event();                           //调用公共事件
#if WARNING_LOWPOWER
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        mp3res_play_speakermode(RES_MP3_PLEASE_CHARGE);
    }
#endif
}

//任务初始化
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_enter(void)
{
    led_mode_off();
    led_idle();
    set_sys_clk(SYS_24M);
    draw_wait();

    //sound_ctl_init(3, 0x800, 25, 0x800);     //连续3次超过0x800认为开始有声；连续25次0x800认为静音
    user_change_volume(sys_ctl.volume);//数字音量
    dac_enable();

#if WARNING_TASK_SPEAKER
    if (work_sta_interrupt == TASK_NULL) {
        mp3_res_play_wait(RES_MP3_SPEAKER_MODE);
    }
#endif
    work_sta_interrupt = TASK_NULL;

    speaker_load_code(0x68);
    t_speaker.mix_step = sys_param_read8(PARAM_MIX_STEP);
    if(t_speaker.mix_step > ECHO_MAX) {
        t_speaker.mix_step = ECHO_MAX;
    }
    sys_param_write8(PARAM_MIX_STEP, t_speaker.mix_step);
    t_speaker.pos = 0;
    echo_read  = ECHO_BUF_SATRT;
    echo_write = ECHO_BUF_SATRT;
    mix_step_set(t_speaker.mix_step);

    pcm_init(S_SPR_8000);
    AUCON10 |= BIT(7);
    FIFO_SPEED = 0;
    adc_init(SEL_SPEAKER);
    adc_start();
    pcm_play();
#if IS_LCD_DISPLAY
    t_speaker.picture_update_flag = 1;
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf3;   // 使用lcd_buf3，否则会花屏
#endif
#endif
}

//任务退出
#pragma location="TASK_SPEAKER_SEG"
void task_speaker_exit(void)
{
    adc_stop();
    pcm_stop();
	adc_exit();
    amux_exit();
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf1;   // 退出时，lcd恢复使用大buf
#endif
}

#pragma constseg="TASK_SPEAKER_CONST"
IAR_CONST char str_show_taskspeaker[] = "task_speaker\n";
#pragma constseg=default

//扩音器任务
#pragma location="TASK_SPEAKER_SEG"
void task_speaker(void)
{
    printf(str_show_taskspeaker);
    task_speaker_enter();
    while (task_ctl.work_sta == TASK_SPEAKER) {
        u8 msg = get_msg();
        task_speaker_event();
        task_speaker_deal_msg(msg);
        task_speaker_display();
    }
    task_speaker_exit();
}
#endif
