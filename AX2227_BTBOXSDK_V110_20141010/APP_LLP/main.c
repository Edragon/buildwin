/*****************************************************************************
* Module    : Main
* File      : main.c
* Author    : Hanny
* Email     : coldney@163.com
* Function  : ������
*****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_spi.h"
#include "btapi.h"
#include "user_fm.h"


/*****************************************************************************
 * �ж����ȼ�˵����
 * ��ߣ�UART1�жϣ���������TX��RX����(���ܱ���������8us��ʹ��λΪIE1��bit6)
 * �θߣ�Timer2 1ms�жϣ�����UART1���յ�����(���ܱ���������1ms)
 * ��ͣ����жϺͽ����ж�
 *
 * �����ж�ע�����
 * 1�������ʱ���ٽ�����ʱ�����Թ����ж�IE_EA���������ָ���
 *    ���岽��ɲο�led_5c7s.c�е�led_7p7s_value_set()
 * 2������ϳ�ʱ����ٽ�����ʱ�����Թص���UART1�ж�������жϣ�Ȼ��ָ���
 *    ���岽��ɲο�led_7p7s.c�е�led_7p7s_value_set()������
 * 3��һ������ֻ��IE_TM2(Timer2�ж�)�������������ȼ���ת�����µ����ȼ�
 *    �����жϺͽ����ж�����Timer2�жϡ�
*****************************************************************************/


#define MSG_POOL_DEPTH      0x20
u8 msg_pool[MSG_POOL_DEPTH];

extern u32 serial_id;
extern IAR_IDATA u8 timer5ms_cnt, timer100ms_cnt;
extern u8 __data banknum;
IAR_BIT_A bool freq_show_update;
IAR_DATA u8 ocx_show_cnt;      //��������һЩ���Ƶ���ʾʱ��
u8 dac_vdd_hp_flag;

void bt_init(void);
void bt_uart_var_init(void);
void bt_param_init(void);
void bt_shutdown(void);
bool sbc_load_code(u16 addr, u8 segnum);
void sd_port_reuse(void);



//��ʼ���������Ȱ�48M����ʼ������ʱ��
#pragma location="INIT_SEG"
void sys_clk_init(void)
{
#if SYS_CRY == CRY_RC
    CLKCON2 &= 0x3f;    //�ر�12M��32K����
    sys_pll_rc();
#elif SYS_CRY == CRY_32K
    CLKCON2 &= 0xbf;    //�ر�12M����
    irtcc_write_cfg(OSC_DRV_WCMD,0xD7); //���������ڲ���������Ϊ21pf������ǿ32.768K������������������Ҫ��ҵ���
    sys_pll_x32k();
#elif SYS_CRY == CRY_12M
    CLKCON2 &= 0x7f;    //�ر�32K����
    irtcc_write_cfg(OSC_DRV_WCMD,0xD7); //���������ڲ���������Ϊ21pf������Ҫ��ҵ���
    sys_pll_x12m();
#else
    #error "��ѡ��һ��ʱ��Դ"
#endif
}

void task_init(void)
{
    task_ctl.last_work = TASK_NULL;
    task_ctl.work_sta = TASK_IDLE;
#if MP3BT_DECT_EN
#if MP3BT_DECT_REUSE_EN
    MP3BT_DECT_INIT();
#endif
    if(MP3BT_DECT_IS_BT()) {
        task_ctl.work_sta = TASK_BT;
    }
    MP3BT_DECT_END();
#endif

#if POWERON_TO_FM
    if(task_ctl.work_sta == TASK_IDLE && fm_init()) {
        fm_off();   //��FM_VOUT�����������Ҳ���SD����U�̿���ʱ�ᵼ��FMû�ص����Ӷ�������ģʽ��FM������һֱ��
        task_ctl.work_sta = TASK_FM;
    }
#endif
#if TASK_BT_EN && !MP3BT_DECT_EN
    if(task_ctl.work_sta == TASK_IDLE) {
        task_ctl.work_sta = TASK_BT;
    }
#endif

    if(task_ctl.work_sta == TASK_BT) {
        led_bt_none();
    } else {
        led_idle();
    }
}

void test_init(void)
{
    PCON1 &= ~(BIT(7) | BIT(6) | BIT(2) | BIT(1) | BIT(0));
    PCON2 &= ~(BIT(6));
    PCON3 &= ~BIT(5);
    PCON1 &= ~BIT(7);
#if LCD_THEME
    LCD_RESET();                //LCD res ��ֹ����ѩ��
#endif
    irtcc_init();

    memset(&sys_ctl, 0, sizeof(sys_ctl));
    sys_flag.all = 0;
    flag_btmode = 0;
    sys_flag.mute_flag = 1;
    sys_ctl.lowpower_cnt = 50;
#if LOWPOWER_WARNING
    sys_ctl.lowpower_warning_flag = 0;
#endif
#if (SYS_PKG == PKG_SSOP24 || SYS_PKG == PKG_LQFP48_6680E || SYS_PKG == PKG_QFN48_6680M)
    dac_vdd_hp_flag = 1;        //DACVDD��DACHP�̽�
#else
    dac_vdd_hp_flag = 0;        //DACVDD��DACHP��������
#endif
    work_sta_interrupt = 0;
    timer5ms_cnt = 0;
    timer100ms_cnt = 0;
    sound_detect_disable();

#if LEDSEG_THEME
    ledseg_init();              //����ܳ�ʼ��������timer2_init֮ǰ��ֹ��Ӱ
    ledseg_disp(MENU_POWERON);
#endif
    led_init();

    bt_uart_var_init();
    timer2_init();              //��ʱ���������ܳ�ʼ�����Ա�������ʼ�����õ��Ķ�ʱ����ʱ������ʱ��֧����չ
#if THEME_SELECT == LEDSEG_THEME_DEFAULT && (LEDSEG_TYPE_SELECT == LEDSEG_7P7S_FM || LEDSEG_TYPE_SELECT == LEDSEG_7P7S_MHZ)
    void timer2_500us_init(void);
    timer2_500us_init();
#endif
#if SOFT_IR_EN
    ir_timer1_init();
#endif
#if LCD_THEME
    set_lcd_buf(LCD_FULL_MODE); //���ó�ʼ���Դ�
    disp_language_numget();
#endif
    spi_param_init();           //�ϵ��ʼ�������豸
    msg_init(msg_pool, MSG_POOL_DEPTH);
#if LCD_THEME
    lcd_init(); //lcd��ʾ��ʼ��
    disp_clear();
    disp_draw(RES_POWERON_WELCOME_BMP, 5, 2);
    disp_update();
    LCD_BACKLIGHT_ON();
#endif

    device_var_init();      //�豸���Ʊ�����ʼ��
#if !USB_GPIO_EN
    usb_init();             //USB��ʼ��
#endif
#if SD_CARD_EN
    sd_init(SD_PORT_SEL);   //SD��ʼ��
#endif

    task_init();

#if SDCMD_REUSE_EARDET || SDCMD_REUSE_LINEIN_DET || SDCLK_REUSE_ADKEY
    sd_port_reuse();
#endif

#if HARD_IR_EN
    ir_hardware_init();
#endif
    device_set_delay(DEVICE_SDMMC, SD_DETECT_DELAY);

    spi_music_init();
    scan_init();                        //��ʼ��ɨ�����
    timer2_hook_enable();               //��ʼ����ϣ�������ʱ����չ���Ա������ȡ�

#if DAC_VCMBUF_EN
    dac_init_vcmbuf();
#else
    dac_init();
#endif

#if (SYS_PKG == PKG_SSOP24 || SYS_PKG == PKG_LQFP48_6680E)
    dac_set_mono();                     //����DACΪ������
#endif
    flush_msg();
    set_sys_clk(SYS_48M);               //��ʼ����ϣ�����ʱ��

#if ALARM_EN
    sys_ctl.next_alarm = irtcc_read_ram32(NEXT_ALARM);
    sys_ctl.alarm_switch = irtcc_read_ram8(ALARM_SWITCH);
    if (sys_ctl.alarm_switch > 1) {
        sys_ctl.alarm_switch = 0;
    }
    if (task_alarm_check()) {
        task_alarm_clear();
    } else {
        task_alarm_set();               //��������
    }
#endif
}

//���ؿ��������
void soft_power_check(void)
{
    u16 cnt;
    for (cnt = 500; cnt!=0; cnt--) {
        if (!SOFT_POWER_CHK_IS_ON()) {
            //printf("power_off\n");
            WDTCON = 0;
            SOFT_POWER_CTL_OFF();
            while (1);
        }
        delay(2800);
    }

    //printf("power_on\n");
    SOFT_POWER_CTL_ON();
}

void test_bt(void)
{
    bt_uart_init();
    //P3DIR |= BIT(3);  // CTS
    memset((u8 *)0x8900, 0, 0x1700);        //���BT������
    bt_power_up();
    BT_STATE = StateIdle;
}

//V099�Ľ���SD��������Զ���λ�����ҿ���show_sdupd��ʾ�������
//��Ϊͨ��SD�������̹��ű�MUTEס���޷�ȷ���Ƿ������ɹ�
void show_sdupd(void)
{
    u8 tmp, tmp1, tmp2;
    tmp1 = *(u8 *)0x3efc;
    *(u8 *)0x3efc = 0x00;
    tmp1 ^= *(u8 *)0x3efd;
    tmp2 = *(u8 *)0x3efe;
    tmp2 ^= *(u8 *)0x3eff;
    tmp = tmp1 & tmp2;
    tmp1 = *(u8 *)0x3efd;
    tmp2 = *(u8 *)0x3efe;
    if(tmp == 0xff && (tmp1 == 0xaa || tmp1 == 0x99) && tmp2 < 10) {
        //printf("sdupd: %02x\n", tmp2);
        switch(tmp2) {
        case 0x00://�����ɹ�
        case 0x03://�����ļ�����ϳ�����ͬ
        case 0x05://ǿ�������ļ�����ϳ�����ͬ
        case 0x07://ǿ�������ɹ�
            //printf("sdupd ok\n");
            //dac_enable();
            //mp3_res_play_wait(RES_SDUPD_OK);
            break;
        case 0x01://�����ļ���С����
        case 0x02://�����ļ�һ���Դ���
        case 0x04://����ʧ��
        case 0x06://ǿ������ʧ��
            //printf("sdupd fail\n");
            //dac_enable();
            //mp3_res_play_wait(RES_SDUPD_FAIL);
            break;
        case 0x08://�����ļ�������������Ϣ
            //printf("no btinf\n");
            //dac_enable();
            //mp3_res_play_wait(RES_SDUPD_FAIL);
            break;
        case 0x09://�������������ļ�
            //printf("two files\n");
            //dac_enable();
            //mp3_res_play_wait(RES_SDUPD_FAIL);
            break;
        }
    }
}

#pragma constseg="INIT_CONST"
IAR_CONST char str_showsid[] = "SID:%lu\n";
IAR_CONST char str_start[] = "\nStart\n";
#pragma constseg=default

#pragma location="INIT_SEG"
void main(void)
{
#if !SD_CARD_EN
    sd_init_none();         //SD�ճ�ʼ�����˺�SD���ſɵ�GPIO��
#endif
    user_io_init();         //IO�ϵ��ʼ��

    printf_io_select(1);    //��ӡ��ѡ��P01
#if !DEBUG_UART_SEL
    uart_disable();
#endif
    printf(str_start);
    printf(str_showsid, serial_id);

#if SOFT_POWER_ON_OFF
    soft_power_check();     //���������
#endif

    IE0 = 0;                //�ر������ж�
    IE1 = 0;
    DPCON = 0;
    IE_EA = 1;

    bt_init();              // ����һЩ��Ҫ��timerǰ��ʼ����ȫ�ֱ���
    test_init();

    sbc_load_code(DCODE_BT_SBC, 3);
    ENTER_DCODE();

#if POWER_ON_VOICE
    dac_enable();
    mp3_res_play_wait(RES_MP3_START);
#endif

    show_sdupd();           //��ʾSD�������

    run_task();
    while (1) {
    }
}
