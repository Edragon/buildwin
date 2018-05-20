/*****************************************************************************
 * Module    : Task
 * File      : task_poweroff.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �ػ�����
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "rtc.h"


extern void poweroff_display(void);
/*****************************************************************************
 * holdmode�ػ�ע�����
 * 1����Ϊ�ػ�ģʽ���SPI���޷���bank������
 *    syspll_resume��io_sleep3��io_wakeup��3�����������"#pragma inline=forced"������
 * 2�����й���MUTE�����Դ�POWER_OFF_DAC����ȫ�ر�DAC���ɼ�С���ġ�
 * 3�����ػ�ʱ�ر�������Դ����Ҫ��RST/RX/TXҪ��Ϊ�͵�ƽ����������������IO©��
*****************************************************************************/


#define POWER_OFF_DAC       0       //�Ƿ���Ҫ���׹ر�DAC����ž����ҪMUTE����


#pragma inline=forced
void syspll_resume(void)
{
#if SYS_CRY == CRY_RC
    PLLCON |= 0x0f;     //pll source is rc, pll enable
    PLLINTH = 0x00;
    PLLINTL = ((*(u8 __code *)0x7ff4)>>5) + 3; //���ֵ+3;
    PLLFRACH = *(u8 __code *)0x7ff6;
    PLLFRACL = *(u8 __code *)0x7ff5;
    delay(0x12);        //��Ҫ��ʱ1ms
    PLLCON |= 0x0d;     //pll source is rc, pll analog enable
    delay(0x12);        //��Ҫ��ʱ1ms
    PLLCON |= 0x02;     //pll digital enable
    CLKCON1 &= 0xf0;    //PLL output 48M,ϵͳ��pll
#elif SYS_CRY == CRY_12M
    PLLCON = 0x00;    //0x08: pll digital & analog disable, pll clock source sel x32k
    PLLFRACH = 0x00;
    PLLFRACL = 0x00;
    PLLINTH = 0x00;//(5>>8) & 0xFF;    //1831
    PLLINTL = 0x05;//5 & 0xFF;         //1831

    PLLCON |= 0x02; // pll digital enable
    PLLCON |= 0x01; // pll analog enable
    delay(0x12);    //��Ҫ��ʱ1ms
    PLLCON |= 0x02; // pll digital enable(�ظ�����һ��)
    delay(0xC0);    //��Ҫ��ʱ10ms
#elif SYS_CRY == CRY_32K
    PLLCON = 0x0b;
#endif
    delay(20);
}

#pragma location="TASK_POWER"
void hold_mode_enter(void)
{
    u8 usbcon0, usbcon1, usbdpdm;
    u8 clkcon, sdcon0, adccon;
#if POWER_OFF_DAC
    u8 atcon0, atcon1, atcon2;
#endif

    IE_EA = 0;                  //�ر����ж�
    MUSIC_MUTE();               //MUTE����
    adccon = ADCCON;
    usbcon0 = USBCON0;
    usbcon1 = USBCON1;
    usbdpdm = USBDPDM;
    clkcon = CLKCON;
    sdcon0 = SDCON0;
    dac_disable();              //�ر�DAC
    ADCCON = 0;                 //�ر�ADC
    at_write(AUADCON1, 0x00);
    USBCON0 = 0x40;             //�ر�USB
    USBCON1 = 0x00;
    USBDPDM = 0x00;
    SDCON0 = 0;                 //�ر�SD
    SPI0CON = 0;                //�ر�SPI
    SPI1CON = 0;
    FMAMCON0 = 0;

#if POWER_OFF_DAC
    atcon0 = ATCON0;            //���׹ر�DAC����ҪMUTE����
    atcon1 = ATCON1;
    atcon2 = ATCON2;
    ATCON0 = 0;
    ATCON1 = 0;
    ATCON2 = 0X05;
#endif

    IO_SLEEP();                 //����IOΪʹ���������ڵ͹��ĵ�״̬

#if POWER_OFF_VPG33
    PWRCON1 &= ~BIT(4);         //�ر�VPG33
    P3DIR &= ~BIT(6);           //P36��VPG33��ͬ�����ţ�����Ҫ��Ϊ�͵�ƽ
    P3 &= ~BIT(6);
    PWKEN = 0x0f;               //�ر�IO����
#else
    if(!sys_flag.lowpower_flag && STANDBY_WAKEUP_EN && task_ctl.last_work == TASK_BT){
        PWK_INIT();             //�Զ�����ʱ������P34�½��ػ���
    }
#endif

    CLKCON &= ~0x03;            //��PLL֮ǰ, �е��ڲ�rc
    asm("nop");asm("nop");asm("nop");
    PLLCON = 0;                 //�ر�PLL
    asm("nop");asm("nop");asm("nop");

    if(!sys_flag.lowpower_flag && STANDBY_WAKEUP_EN) {
        WDTCON = 0x13;          //�Զ�����ʱ�����Ź�128ms�жϻ���
    } else {
        WDTCON = 0;             //�͵������ػ�ʱ�رտ��Ź�
    }

    while(1) {

        PCON0 |= BIT(1);        //����hold_mode
        asm("nop");asm("nop");asm("nop");

        WATCHDOG_CLR();

#if IR_HARD_POWERDOWN_EN
        if(IRCON0 & BIT(2)){
            IRCON0 &= ~BIT(2);
            if(IRDAT2 == IR_PWR_DAT){
                break;
            }
        }
#endif

        if((IS_KEY_WAKEUP() || PWK_IS_PEND()) && STANDBY_WAKEUP_EN){
            break;
        }
    }

#if POWER_OFF_VPG33
    PWRCON1 |= BIT(4);          //�ָ�VPG33
#endif
#if POWER_OFF_DAC
    ATCON0 = atcon0;
    ATCON1 = atcon1;
    ATCON2 = atcon2;
#endif

    syspll_resume();            //�ָ�ϵͳʱ��
    CLKCON = clkcon;

    IO_WAKEUP();                //�ָ�IO����������

    PCON0 = 0x00;
    PWKEN = 0x0f;               //�ر�IO����
    SPI0CON = 0x80;
    SPI1CON = 0xb1;
    ADCCON = adccon;
    USBCON0 = usbcon0;
    USBCON1 = usbcon1;
    USBDPDM = usbdpdm;
    SDCON0 = sdcon0;

    WATCHDOG_INIT();
    user_io_init();
    led_init();
#if DEBUG_UART_SEL
    printf_io_select(1);        //��ӡ��ѡ��P01
#endif
#if LEDSEG_THEME
    ledseg_init();
#endif
    if(task_ctl.last_work == TASK_BT) {
#if LED_BLUE_BLINK              //����ģʽ��������
        LED_BLUE_ON();
#elif LED_GREEN_BLINK
        LED_GREEN_ON();
#endif
    } else {
#if LED_GREEN_BLINK             //����ģʽ�̵�����
        LED_GREEN_ON();
#elif LED_BLUE_BLINK
        LED_BLUE_ON();
#endif
    }

    dac_enable();
    user_change_volume(sys_ctl.volume);
    task_ctl.work_sta = task_ctl.last_work;
    sys_flag.need_mute = 0;     //���Ѻ�Ҫ�ٶ�̬����Ƿ���ҪMUTE
    sys_flag.ir_power_flag = 0;
    sys_ctl.sleep_cnt = 0;

#if IR_HARD_POWERDOWN_EN || STANDBY_WAKEUP_EN
    sys_ctl.wakeup_delay = KEY_LONG_TIMES+2;
#endif

    IE_EA = 1;                  //�����ж�
#if !USB_GPIO_EN
    usb_host_reactive();        //���U�����ߣ�����Ҫ����ö�ٲ�������ʹ��
#endif
#if SD_CARD_EN
    sd_reactive();
#endif
    flush_msg();                //�����Ϣ��
}

#if TASK_POWEROFF_EN
//����������
#pragma location="TASK_POWER"
void task_poweroff(void)
{
#if LCD_THEME
    set_lcd_buf(LCD_FULL_MODE);
    LCD_BACKLIGHT_OFF();
#endif
#if LOWER_POWER_DISPLAY_OFF
    poweroff_display();
    #if !LOWPOWER_MUSIC         //û�е͵���ʾ��ʱ������ʱ��ʾOFF
        u8 temp = 3;
        while(temp--){
            WATCHDOG_CLR();
            delay_5ms(200);
        }
    #endif
#endif

#if LOWPOWER_MUSIC
    if(sys_flag.lowpower_flag){
        dac_enable();
        mp3_res_play_wait(RES_MP3_PLEASE_CHARGE);
        dac_disable();
    }
#endif

#if SOFT_POWER_ON_OFF
    IE_EA = 0;
    dac_disable();
    MUSIC_MUTE();
    draw_clear();
    WDTCON = 0;
    SOFT_POWER_CTL_OFF();       //������ʱ�����ضϵ�
    while(1);
#else
    hold_mode_enter();          //������ʱ������holdmode
#endif
}

#endif


/*****************************************************************************
 * powerdown�ػ�ע�����
 * 1����Ϊ�ػ�ģʽ���SPI���޷���bank������
 *    irtcc_write_cfg_temp���������"#pragma inline=forced"������
 * 2���ػ�ʱ���ر�VDDIO����ʱLDOIN���벻����4.2V�����������IC�ķ��ա�
*****************************************************************************/
//������inline��������ֹ��bank
#pragma inline=forced
void irtcc_write_cfg_temp(u8 cmd,u8 irtcc_cfg)
{
    RTCON |= 0x01;          //enable IRTCC communications
    RTCDAT = cmd;           //RTCC communication command
    while (RTCON & 0x02);   //�ȴ���������
    RTCDAT = irtcc_cfg;     //RTCC communication data
    while (RTCON & 0x02);   //�ȴ����ݴ������
    RTCON &= ~(0x01);       //disable IRTCC communications
}

//�ر��ڲ�LDO������͹��Ĳ���Ҫ����
#pragma location="TASK_POWER"
void power_down_enter(void)
{
    IE_EA = 0;                  //�ر����ж�
    MUSIC_MUTE();               //MUTE����
    dac_disable();              //�ر�DAC
    ADCCON = 0;                 //�ر�ADC
    at_write(AUADCON1, 0x00);
    USBCON0 = 0x40;             //�ر�USB
    USBCON1 = 0x00;
    USBDPDM = 0x00;
    SDCON0 = 0;                 //�ر�SD
    SPI0CON = 0;                //�ر�SPI
    SPI1CON = 0;
    FMAMCON0 = 0;

    ATCON0 = 0;
    ATCON1 = 0;
    ATCON2 = 0X05;

    IO_SLEEP();                 //����IOΪʹ���������ڵ͹��ĵ�״̬

    PWRCON1 &= ~BIT(4);         //�ر�VPG33
    P3DIR &= ~BIT(6);           //P36��VPG33��ͬ�����ţ�����Ҫ��Ϊ�͵�ƽ
    P3 &= ~BIT(6);
    PWKEN = 0x0f;               //�ر�IO����

    //����IRTWKO���Ż���(�½���)������˯��ʱIRTWKO���ŻῪ�ڲ�����ά�ֵ͵�ƽ������ʱ�ⲿ�����ߵ�ƽ���ɿ����ɲ����½���
    //irtcc_write_cfg_temp(RTC_CFG_WCMD,0x88);//X32K En //Normal RTC //bit4=0, Wake up Pin out put0, �൱��������.
    //irtcc_write_cfg_temp(WK_EN_WCMD,0x02);  //bit1=1: WKO_WK_EN(Hight Wake_Up)

    CLKCON &= ~0x03;            //��PLL֮ǰ, �е��ڲ�rc
    asm("nop");asm("nop");asm("nop");
    PLLCON = 0;                 //�ر�PLL
    asm("nop");asm("nop");asm("nop");
    WDTCON = 0;                 //�رտ��Ź�

    while(1){
        irtcc_write_cfg_temp(PWR_CTL_WCMD,0xBF);    //�ػ�(0xBF:��VDDIO,��ʱLDOIN���벻����4.2V�����������IC�ķ���)
        //irtcc_write_cfg_temp(WK_EN_WCMD,0x00);      //Clear wakeup pending
        //irtcc_write_cfg_temp(WK_EN_WCMD,0x02);      //bit1=1: WKO_WK_EN(Hight Wake_Up)
    }
}
