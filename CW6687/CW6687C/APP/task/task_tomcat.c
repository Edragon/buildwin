/*****************************************************************************
 * Module    : Task
 * File      : task_tomcat.c
 * Author    : Hanny
 * Function  : ��ķè��������
 *****************************************************************************/
#include "include.h"

#if TASK_TOMCAT_EN
#include "task.h"
#include "task_tomcat.h"
#include "user_spi.h"
__near_func void dac_out(u16 adr, u16 samples, u8 ch);
IAR_XDATA_A u8 tom_adc_buf[TOM_BUF_SIZE]    @ "TOMCAT_ADC_BUF";
IAR_XDATA_A u8 tom_wbuf[512]                @ "TOMCAT_WBUF";
IAR_XDATA_A u8 tom_rbuf[512]                @ "TOMCAT_RBUF";
IAR_XDATA_A type_task_tomcat t_tom          @ "TOMCAT_XDATA";
IAR_XDATA_A u16 tom_output                  @ "TOMCAT_XDATA";
#include SET_USER_PATH(CFG_UI_TOMCAT_DIR, /ui/ui_tomcat.c)
#pragma location="TOMCAT_PROC"
__near_func void task_tomcat_process(void)
{
    if (t_tom.block_cnt >= t_tom.block_lmt) {
        t_tom.block_cnt = 0;
        //printf("err\n");
        //return;
    }
    memcpy((u8*)tom_output, adc_buf+t_tom.pos*128, 128);
    //dac_out((u16)tom_output,TOMCAT_SAMPLES/2, 1);
    u32 v_pow = voice_power((u8*)tom_output, 64);
    sound_detect(v_pow);
    //printf("%08lx ",v_pow);
    tom_output += 128;
    t_tom.block_cnt++;
    if (tom_output >= ((u16)tom_adc_buf+TOM_BUF_SIZE)) {
        tom_output = (u16)tom_adc_buf;
    }
    t_tom.pos ^= 1;//ȡ��
}

//¼��������
#pragma location="TASK_TOMCAT_SEG"
void task_tomcat_record(void)
{
#if IS_LCD_DISPLAY
    t_tom.disp_flag = 1;
    task_tomcat_display();  // ��ʾ¼��ͼ��
#endif
    if (t_tom.block_cnt < 4) {
        return;
    }
    memcpy(tom_wbuf,t_tom.block_adr,512);
    spi_music_write(tom_wbuf);
    t_tom.block_cnt -= 4;
    t_tom.block_adr += 0x200;
    if (t_tom.block_adr == &tom_adc_buf[TOM_BUF_SIZE]) {
        t_tom.block_adr = tom_adc_buf;
    }
}

//����������
#pragma location="TASK_TOMCAT_SEG"
void task_tomcat_play(void)
{
#if IS_LCD_DISPLAY
    t_tom.disp_flag = 1;
    task_tomcat_display();  // ��ʾ����ͼ��
#endif
    spi_music_open(t_tom.rec_addr, t_tom.rec_len);//���ÿ�ʼλ�úͳ���
    while(spi_music_len() && (task_ctl.work_sta == TASK_TOMCAT)) {
        comm_event();
        tempo_calc(tom_adc_buf,tom_rbuf);
        task_tomcat_deal_msg(get_msg());
#if IS_LCD_DISPLAY
        task_tomcat_display();
#endif
    }
    tomcat_record_init();
#if IS_LCD_DISPLAY
    t_tom.disp_flag = 1;    // ��ʾֹͣͼ��
#endif
}

//���ų�ʼ��
#pragma location="TASK_TOMCAT_SEG"
void tomcat_play_init(void)
{
    sound_set_flag(0, 0);
    adc_stop();
    pcm_play();
    t_tom.sta = TOM_PLAY;
    tempo_init();

    t_tom.rec_addr = spi_music_start();     //��ȡ����ǰ��λ��
    t_tom.rec_len = spi_music_len();
    //printf("play:addr=%04x len=%04x\n", t_tom.rec_addr, t_tom.rec_len);
}

//¼����ʼ��
#pragma location="TASK_TOMCAT_SEG"
void tomcat_record_init(void)
{
    t_tom.pos = 0;
    t_tom.block_cnt = 0;
    t_tom.block_adr = tom_adc_buf;
    t_tom.block_lmt = TOM_BUF_SIZE / TOM_BLOCK_SIZE;
    tom_output = (u16)tom_adc_buf;
    t_tom.sta = TOM_NULL;

    spi_record_start();
    adc_start();
    //printf("adc_start\n");
}

//�����ʼ��
#pragma location="TASK_TOMCAT_SEG"
void task_tomcat_enter(void)
{
    led_mode_off();
    led_idle();
    set_sys_clk(SYS_48M);
    draw_wait();
    user_change_volume(sys_ctl.volume);//��������
    dac_enable();
#if WARNING_TASK_TOMCAT
    if (work_sta_interrupt == TASK_NULL) {
        mp3_res_play_wait(RES_MP3_TOMCAT_MODE);
    }
#endif
    work_sta_interrupt = TASK_NULL;

    memset(&t_tom,0,sizeof(t_tom));
    sound_ctl_init(6, 0x200, 50, 0x200);     //����6�γ���0x200L��Ϊ��ʼ����������50�ε���0x200��Ϊ����
    sound_set_flag(0, 0);
    cr_speed = 54;       //����ٶ� {54, 63, 72, 81, 90}, 72Ϊ�����
    tomcat_load_code(0x78);

    adc_init(SEL_TOMCAT);
    pcm_init(S_SPR_16000);
    spi_music_init();
    tomcat_record_init();  //¼����ʼ��
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf2;       // tomcatʹ��lcd_buf2������Ứ��
    t_tom.last_sta = 0xff;  // ��ʼ��Ϊ��Ч״̬
    line_clear();
    disp_menu(STR_MAIN_TOMCAT,0,0, 0,ME_CENTER);
    line_update(0);
    t_tom.picture_disp_flag = 1;
    t_tom.disp_flag = 1;             //���ø��±�־
#endif
}

//�����˳�
#pragma location="TASK_TOMCAT_SEG"
void task_tomcat_exit(void)
{
    adc_stop();
    pcm_stop();
    adc_exit();
    amux_exit();
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf=lcd_buf1;   // �˳�tomcat���ָ�ʹ�ô��Դ�
#endif
}

//����������
#pragma location="TASK_TOMCAT_SEG"
void task_tomcat_event(void)
{
    comm_event();
    if (t_tom.auto_play) {
        if (!sound_flag()) {
            t_tom.auto_play = 0;
            tomcat_play_init();
        }
    } else {
        if (sound_flag()) {
            t_tom.auto_play = 1;
            t_tom.sta = TOM_REC;
        }
    }

    switch(t_tom.sta) {
    case TOM_REC:
        task_tomcat_record();
        break;
    case TOM_PLAY:
        task_tomcat_play();
        break;
    case TOM_NULL:
        break;
    }
}

#pragma constseg="TASK_TOMCAT_CONST"
IAR_CONST char str_show_tasktomcat[] = "task_tomcat\n";
#pragma constseg=default

//��ķè����
#pragma location="TASK_TOMCAT_SEG"
void task_tomcat(void)
{
    printf(str_show_tasktomcat);
    task_tomcat_enter();
    while (task_ctl.work_sta == TASK_TOMCAT) {
        u8 msg = get_msg();
        task_tomcat_event();
        task_tomcat_deal_msg(msg);
#if IS_LCD_DISPLAY
        if(t_tom.disp_flag) {
#else
        if(msg) {
#endif
            task_tomcat_display();
        }
    }
    task_ctl.work_sta = TASK_EXIT;
    task_tomcat_exit();
}

#endif
