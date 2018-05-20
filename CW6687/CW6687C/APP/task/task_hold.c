/*****************************************************************************
 * Module    : Task
 * File      : task_hold.c
 * Author    : Hanny
 * Function  : HOLDģʽ��ϵͳ�������״̬
 *****************************************************************************/
#include "include.h"
#include "task.h"

#if TASK_HOLD_EN
#include "task_hold.h"

#include SET_USER_PATH(CFG_IO_HOLD_DIR, /io/io_hold.c)

void save_sfr(u8 hold_type);
void load_sfr(void);
void sfr_hold_set(void);
void dac_sfr_hold_load(void);
void dac_sfr_hold_set(void);
void lock_pll48m(u8 lock);
__near_func bool get_wko_pin_state(void);
IAR_XDATA_A u8 key_cnt @ 0x152A;

#pragma location="TASK_HOLD_SEG"
void wkup_config(void)
{
    PWKEN = 0x1F;
#if (WKUP_EDGE == WKUP_RISING_EDGE)
    PWKEDGE &= 0xE0;
    WKUP_PORT_INIT();
    WKUP_PORT_PULLDOWN();
#elif (WKUP_EDGE == WKUP_FALLING_EDGE)
    PWKEDGE = (PWKEDGE & 0xE0) | BIT(WKUP_PIN);
    WKUP_PORT_INIT();
    WKUP_PORT_PULLUP();
#endif
    PWKEN = 0x1F & (~BIT(WKUP_PIN));
}

#pragma location="TASK_HOLD_SEG"
void hold_led_scan(void)
{
    static u8 led_cnt = 0;
    led_cnt++;
    if(1 == led_cnt){
        LED_BLUE_ON();
    } else if(2 == led_cnt){
        LED_BLUE_OFF();
    } else if(3 == led_cnt){
        LED_BLUE_ON();
    } else if(4 == led_cnt){
        LED_BLUE_OFF();
    } else if(led_cnt > 30){
        led_cnt = 0;
    }
}

//����������
#pragma location="TASK_HOLD_SEG"
void task_hold(void)
{
    //printf("task_hold\n");
#if BT_DISCONNECT_POWEROFF_TIME
    u16 hold_cnt = 0;
#endif
    WDTCON = 0;
    save_sfr(HOLD_POWER_TYPE | (SYS_CRY<<1)); //����bit0�Ǹߵ͵�ѹ�ؿ��ƣ� bit[3:1]�ǵ�ǰʹ�õ�ʱ�ӣ�����Ϊ0ʱ���ر�PLL,����ΪCRY_2M/32Kʱ����ر�PLL(��ʡԼ300uA��Դ)
    IE_EA = 0;
    task_hold_io_sleep();
    SPEAKER_MUTE();
    u8 sys_clk = get_sys_clk();             //���ݵ�ǰϵͳʱ��
    set_sys_clk(SYS_RC_4M);                 //����RC

#if TASK_HOLD_DAC_CLOSE   //���׹ص�dac,����ʱ����ž������Ҫmute����
    dac_sfr_hold_set();
#else
    dac_disable();  //�����׹ص�dac���Ļ�Ƚϴ�
#endif

#if IS_LCD_DISPLAY
    lcd_off();  // �ر�lcd��
#endif
    sfr_hold_set();     //����hold mode�µļĴ���
    wkup_config();      //BT wakeup

    WDTCON = 0x13;      //ʹ�ÿ��Ź���ÿ128ms����һ��
    while(1) {
#if HOLD_POWER_TYPE
        PCON5 |= BIT(7);    //����ģʽ:0.8v, ����ģʽ:1.25v
#else
        PCON0 |= BIT(1);    //��ѹ:1.35v
#endif
        delay_ms(1);
        //��ʱ���Ѳ�ѯ״̬
        //if (IS_KEY_WAKEUP || !BT_IS_SLEEP) {
        if (!BT_IS_SLEEP) {
            break;
        }
#if USE_WKOKEY
        if(get_wko_pin_state()) {
            break;
        }
#endif
        hold_led_scan();

#if BT_DISCONNECT_POWEROFF_TIME
        if(task_ctl.work_sta==TASK_BT && sys_ctl.bt_sta<BT_STA_CONNECTED) {
            hold_cnt++;
            if(hold_cnt > (BT_DISCONNECT_POWEROFF_TIME*1000UL/128)) {
                task_ctl.work_sta = TASK_POWEROFF;
                break;
            }
        }
#endif
#if INTERNAL_CHARGE_EN
        charge_detect();
#endif
    }

    PCON0 = 0;
#if TASK_HOLD_DAC_CLOSE
    dac_sfr_hold_load();
#else
    if(task_ctl.work_sta != TASK_FM) {
        dac_enable();
    }
#endif
    load_sfr();//��ԭ�Ĵ���
    //���ݲ�ͬ���񣬻�ԭ��Ӧģ��
    task_hold_io_reset();                         //�������Ź�
    if(!sys_ctl.spk_mute) {
        SPEAKER_UNMUTE();
    }
    WDTCON = 0;
    set_sys_clk(sys_clk);                //��ԭ��ǰϵͳʱ��
    LED_BLUE_ON();
    IE_EA = 1;                  //�����ж�
    if(task_ctl.work_sta !=TASK_BT){
        if (device_is_online(DEVICE_UDISK)) {
            lock_pll48m(1);                         //U�����ߣ�������PLL
            usb_host_reactive();
            delay_5ms(80);
        } else {
            lock_pll48m(0);                         //U�̲����ߣ�������PLL
        }
    }
    WATCHDOG_INIT();
    key_cnt = 0;
    flush_msg();
   // printf("task_hold_exit\n");
    if(!BT_IS_SLEEP) {
        sys_ctl.sleep_cnt = 0xffff;
    } else {
        sys_ctl.sleep_cnt = STANDBY_TIME;
    }

#if IS_LCD_DISPLAY
    lcd_on();  // ��lcd��
#endif
}
#endif
