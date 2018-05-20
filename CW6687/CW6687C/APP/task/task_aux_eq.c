/*****************************************************************************
 * Module    : Task
 * File      : task_aux_eq.c
 * Author    : Hanny
 * Function  : 音源输入任务流程
 *****************************************************************************/
#include "include.h"

#if TASK_AUX_EQ_EN
#include "task.h"
#include "task_aux_eq.h"

IAR_XDATA_A type_task_auxeq t_auxeq @ "AUX_EQ_XDATA";

#include SET_USER_PATH(CFG_UI_AUX_EQ_DIR, /ui/ui_aux_eq.c)

#pragma location="AUX_PROC"
__near_func void task_aux_eq_process(void)
{
    if (AUCON5 >= 130) {
        FIFO_SPEED = 0;
    } else {
        if (AUCON5 <= 64) {
            FIFO_SPEED = (u8)-1;
        }
    }

    if (!t_auxeq.pos) {
        t_auxeq.aux_input = (u16)adc_buf;
    } else {
        t_auxeq.aux_input = (u16)adc_buf + AUX_EQ_SAMPLES * adc_ch;
    }
    dac_direct_out(t_auxeq.aux_input, AUX_EQ_SAMPLES/2, adc_ch);
    t_auxeq.pos ^= 1;
}

#pragma location="TASK_AUX_SEG"
void aux_eq_continue(void)
{
    adc_stop();
	auxeq_load_code(0x70);
    t_auxeq.pos = 0;
    t_auxeq.mute = 0;
    sys_ctl.sleep_cnt = 0xffff;
    pcm_init(S_SPR_48000);
    adc_init(SEL_AUX_EQ);
    adc_start();
    pcm_play();
    if (!sys_ctl.volume) {
        t_auxeq.mute = 1;
        sys_mute();
    }
}

#pragma location="TASK_AUX_SEG"
void mp3res_play_auxeqmode(u8 music_name)
{
    adc_stop();
    mp3_res_play_wait(music_name);
    aux_eq_continue();
}

#pragma location="TASK_AUX_SEG"
void auxeq_play_vol_music(void)
{
    if(!sys_ctl.vol_maxmin_music_play){
        sys_ctl.vol_maxmin_music_play = 1;
        adc_stop();
        sys_unmute();
    }
    mp3_res_play_wait(RES_MP3_VOL_MAXMIN);
    flush_msg();
}

//AUX EQ控制MUTE
#pragma location="TASK_AUX_SEG"
void auxeq_mute_control(void)
{
    if (t_auxeq.mute && sys_ctl.volume) {
        t_auxeq.mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
        sys_unmute();
    } else {
        t_auxeq.mute = 1;
        sys_ctl.sleep_cnt = STANDBY_TIME;
        sys_mute();
    }
}

//AUX EQ减小音量
#pragma location="TASK_AUX_SEG"
void auxeq_vol_down(void)
{
    user_set_volume(user_volume_dec(sys_ctl.volume));
    show_volume();
#if WARNING_VOL_MAXMIN
    if(!sys_ctl.volume){
        auxeq_play_vol_music();
        return;
    }
#endif
    if (t_auxeq.mute && sys_ctl.volume) {
        t_auxeq.mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
        sys_unmute();
    }
}

//AUX EQ增加音量
#pragma location="TASK_AUX_SEG"
void auxeq_vol_up(void)
{
    user_set_volume(user_volume_inc(sys_ctl.volume));
    show_volume();
#if WARNING_VOL_MAXMIN
    if(sys_ctl.volume == VOLUME_MAX){
        auxeq_play_vol_music();
        return;
    }
#endif
    if (t_auxeq.mute) {
        t_auxeq.mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
        sys_unmute();
    }
}

//任务事务处理
#pragma location="TASK_AUX_SEG"
void task_aux_eq_event(void)
{
    comm_event();                           //调用公共事件
#if LINEIN_DETECT_EN
    if (!device_activate(DEVICE_LINEIN)) {
        task_ctl.work_sta = TASK_EXIT;
    }
#endif
#if AUX_AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((((!sys_ctl.sleep_cnt) && BT_IS_SLEEP) || sys_ctl.ir_power_flag) && !sys_ctl.lowpower_flag){
        task_hold();
        sys_ctl.sleep_cnt = 0xffff;
        sys_unmute();
        aux_eq_continue();
    }
#endif
#if WARNING_LOWPOWER
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        mp3res_play_auxeqmode(RES_MP3_PLEASE_CHARGE);
    }
#endif
}

//任务初始化
#pragma location="TASK_AUX_SEG"
void task_aux_eq_enter(void)
{
#if LINEIN_DETECT_EN
    if (!device_activate(DEVICE_LINEIN)) {      //检查linein
        msgbox(STR_NO_CONNECTED_AUX, NULL, MSGBOX_TIME);
        task_ctl.work_sta = TASK_EXIT;          //设备无效，返回主菜单
        return;
    }
#endif
    memset(&t_auxeq, 0, sizeof(t_auxeq));
    led_aux_play();
    draw_wait();
    set_sys_clk(SYS_24M);
#if (DEBUG_UART_SEL && ((AUX_CHANNEL_SEL & 0x07) == CHANNEL_01))
    uart_disable();
    P0DIR |= BIT(1);
#endif

/*#if MP3BT_DECT_EN
    work_sta_interrupt = TASK_NULL;
#endif*/

    user_change_volume(sys_ctl.volume);//数字音量
    dac_enable();

#if WARNING_TASK_AUX
    if (work_sta_interrupt == TASK_NULL) {
        mp3_res_play_wait(RES_MP3_AUX_MODE);
    }
#endif
    work_sta_interrupt = TASK_NULL;

	auxeq_load_code(0x70);
    pcm_init(S_SPR_48000);
    AUCON10 |= BIT(7);
    FIFO_SPEED = 0;
    adc_ch = ((AUX_CHANNEL_SEL & 0x80) >> 7) + 1;
    adc_init(SEL_AUX_EQ);
    adc_start();
    pcm_play();
#if MIC_DIRECT_OUT_EN
    mic_direct_out_enter(MIC_CHANNEL_SEL,11);
#endif
#if IS_LCD_DISPLAY
    //set_lcd_buf(LCD_FULL_MODE);
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf = lcd_buf2;   // lcd使用小buf，否则会花屏
#endif
    draw_clear();                                       //清空屏显
    t_auxeq.updata_flag = 1;
    // 清除"请稍候"字样
    line_clear();
    line_update(2);
    line_update(4);
#endif
}

//任务退出
#pragma location="TASK_AUX_SEG"
void task_aux_eq_exit(void)
{
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
    adc_stop();
    pcm_stop();
    adc_exit();
    adc_ch = 1;
#if (DEBUG_UART_SEL && ((AUX_CHANNEL_SEL & 0x07) == CHANNEL_01))
    P0DIR &= ~BIT(1);
    uart_enable();
#endif
#if MIC_DIRECT_OUT_EN
    mic_direct_out_exit();
#endif
    if(sys_ctl.mute_flag) {
        sys_unmute();
    }
}

#pragma constseg="TASK_AUX_CONST"
IAR_CONST char str_show_taskauxeq[] = "task_aux_eq\n";
#pragma constseg=default

//音源输入任务
#pragma location="TASK_AUX_SEG"
void task_aux_eq(void)
{
    printf(str_show_taskauxeq);
    task_aux_eq_enter();
    while (task_ctl.work_sta == TASK_AUX_EQ) {
        u8 msg = get_msg();
        task_aux_eq_event();
        task_aux_eq_deal_msg(msg);
        task_aux_eq_display();
    }
    task_aux_eq_exit();
}
#endif
