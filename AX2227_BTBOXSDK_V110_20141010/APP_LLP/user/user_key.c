/*****************************************************************************
* Module    : User
* File      : user_key.c
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : �û���������
*****************************************************************************/
#include "include.h"
#include "user_ir.h"
#include "btapi.h"

enum {
    ADC_KEY,
    ADC_KEY2,
    ADC_BAT,
    ADC_BANDGAP
};

typedef struct {
    u8 mode;
    u8 adc_val;
    u8 key_check;
    u8 scan_cnt;                //ɨ�������ÿ�����Σ���Ҫɨһ�ε��
    u8 bat_val1;
    u8 bat_val2;
    u8 bat_val;
} type_scan_ctl;

struct {
    u8 adc_sel;                     //ѡ����һ��ADC
    u8 adc_val;                     //ADC ������ֵ
} adc_ctl;

typedef struct {
    //u16 adc_val[4];             //һ���˲�����һ��ֵ
    u8 key_val;                 //��һ������ֵ
    u8 key_cnt;                 //����ֵ��������
    u8 key_up;                  //̧������
} type_key_ctl;
IAR_IDATA type_key_ctl key_ctl;
IAR_IDATA type_scan_ctl scan_ctl;
#if KU_PLAY_S10_TWICE_REJECT || KU_PLAY_S10_TWICE_CALLBACK
bool paly_twice_flag = 0;
u16 twice_cnt = 0;
#endif

#if SD_DECT_REUSE_ADKEY
volatile u8 sd_online_flag;
#endif
#if AUX_DECT_REUSE_ADKEY
volatile u8 aux_online_flag;
#endif
#if LED_REUSE_ADKEY
volatile u8 adc_chk_flag;
#endif
#if SDCLK_REUSE_ADKEY
volatile u8 adc_get_flag;
volatile u8 adc_kick_flag;
#endif

#define BATVAL_NUM      6
IAR_CONST u8 tbl_bat_val[BATVAL_NUM] = {0x80,0x87,0x8f,0x97,0x9f,0xff};

#if USE_ADKEY >= 1
IAR_CONST u8 tbl_key1_adc[KEY1_NUM + 1] = { KEY1_ADC };
IAR_CONST u8 tbl_key1_val[KEY1_NUM + 1] = { KEY1_VAL };
#endif

#if USE_ADKEY >= 2
IAR_CONST u8 tbl_key2_adc[KEY2_NUM + 1] = { KEY2_ADC };
IAR_CONST u8 tbl_key2_val[KEY2_NUM + 1] = { KEY2_VAL };
#endif



void scan_init(void)
{
#if (!USE_ADKEY)
    K_PLAY_INIT();
    K_NEXT_INIT();
    K_PREV_INIT();

    #if USE_MODE_PLAY_KEY
        P1DIR |= BIT(2)|BIT(3);
        P1PU0 |= BIT(2)|BIT(3);
    #endif

    #if POWER_IOKEY_EN
        K_POWERR_INIT();
    #endif

    #if MODE_IOKEY_EN
        K_MODE_INIT();
    #endif

    #if VOL_IOKEY_EN
        K_VOL_DOWN_INIT();
        K_VOL_UP_INIT();
    #endif

#else

    ADKEY_INIT();           //ADKey
    #if (USE_ADKEY == 2)
        ADKEY2_INIT()       //ADKey2
    #endif

    #if (USER_SEL == USER_2226_S10_SIMPLE || USER_SEL == USER_6680E_S10_SIMPLE)
        ADC_PU10K();    //P1PU0 |= BIT(3)
        P1PD0 |= BIT(3);
    #elif (USER_SEL == USER_2227_LCD)
        P2DIR |= BIT(2);//PLAY  POWER
    #endif
#endif

#if LED_REUSE_ADKEY
    /* ����ADKEY��LEDʱ����Ҫռ��timer0
    ��ΪADKEY���ʱIO��Ϊ���룬LED�ᱻ�ⲿ24k������΢����
    ��Ҫ��ADKEYת�����������IO��Ϊ�����������ʹ��ADKEYת����
    ����timer0��ʱ�ж�(ADת��ʱ��Լ25us)��33us����timer0�ж�
    �ر�timer0����IO��Ϊ���������LED��
    */
    adc_chk_flag = 1;
    TMR0CON = 0x05;         //sysclk, 32 div
    TMR0CNT = 0;
    TMR0PR = 50;            //33us*48/32=50
    asm("ORL 0xB9,#0X20");  //IP1�����ȼ�����timer2ͬ���ȼ�
    IE1 |= BIT(5);          //enable interrupt
#elif SDCLK_REUSE_ADKEY
    /* ����SDCLK���ADKEYʱ����Ҫ1ms���һ��ADC�Ƿ�ת����ɣ�
    ��һ��ת�������У����ֶ�������Ҫ��������һ��ADCת����
    */
    adc_get_flag = 0;
    adc_kick_flag = 0;
#endif
    PWRCON1 |= BIT(5);
    memset_i(&scan_ctl, 0, sizeof(scan_ctl));
    scan_ctl.bat_val = 0xff;
    PWKEDGE |= BIT(7);
}

//��ȡADC��ֵ
#pragma inline=forced
#pragma location="SCAN_COM"
__near_func bool adc_get(void)
{
#if LED_REUSE_ADKEY
    if(adc_chk_flag == 0) {
        scan_ctl.adc_val = ADCDATAH;
        adc_chk_flag = 1;       //ADKEYת�����
        return true;
    } else {
        return false;
    }
#else
#if SDCLK_REUSE_ADKEY
    if(scan_ctl.mode == ADC_KEY) {
        if(adc_get_flag) {
            adc_get_flag = 0;
            return true;
        } else {
            return false;
        }
    }
#endif
    if (ADCCON & BIT(7)) {
        return false;
    }
    scan_ctl.adc_val = ADCDATAH;
    //printf("%02x ", scan_ctl.adc_val);
    return true;
#endif
}

#if LED_REUSE_ADKEY
#pragma location="SCAN_COM"
__near_func void adc_chk(void)
{
    if(adc_chk_flag) {
        if (ADCCON & BIT(7)) {      //ADKEYδ��ɣ��������
            return;
        } else {
            adc_chk_flag = 0;
            TIMER0_DIS();           //disable timer0
            ADKEY_END();            //ʹ����ADKEY����IO��Ϊ���������LED
        }
    }
}

#pragma vector = VECTOR_OFFSET(TIMER0_INT)
#pragma register_bank=2
__near_func __interrupt void timer0_isr(void)
{
    ISR_ENTER();

    TMR0CON &= ~BIT(7);             //clear pending
    adc_chk();

    ISR_EXIT();
}
#elif SDCLK_REUSE_ADKEY
__near_func void adc_chk(void)
{
    if(scan_ctl.mode == ADC_KEY) {
        if(SD_CLK_IS_IN()) {
            if ((ADCCON & BIT(7)) == 0) {      //ADKEYת�����
                if(adc_kick_flag) {
                    adc_kick_flag = 0;
                    ADCCON = BIT(7) | BIT(3) | ADKEY_CH;
                } else {
                    scan_ctl.adc_val = ADCDATAH;
                    adc_get_flag = 1;
                }
            }
        }
    }
}

__near_func void adc_key_kick(void)
{
    if(scan_ctl.mode == ADC_KEY) {
        adc_kick_flag = 1;              //�˴�ת�����ɿ�����Ҫ����kick
    }
}
#endif

#if !USE_ADKEY
#pragma inline=forced
__near_func u8 iokey_get(void)
{
    u8 key_tmp;
    if(IS_K_PLAY()) {
        key_tmp = T_KEY_PLAY_S10;
#if !VOL_IOKEY_EN
    } else if(IS_K_NEXT()) {
        key_tmp = T_KEY_NEXT_S10;
    } else if (IS_K_PREV()) {
        key_tmp = T_KEY_PREV_S10;
#else
    } else if(IS_K_NEXT()) {
        key_tmp = T_KEY_NEXT;
    } else if (IS_K_PREV()) {
        key_tmp = T_KEY_PREV;
    } else if(IS_K_VOL_UP()) {
        key_tmp = T_KEY_VOL_UP;
    } else if (IS_K_VOL_DOWN()) {
        key_tmp = T_KEY_VOL_DOWN;
#endif

#if USE_MODE_PLAY_KEY
    } else if(!(P1 & BIT(2))) {
        key_tmp = T_KEY_PLAY;
    } else if(!(P1 & BIT(3))) {
        key_tmp = T_KEY_MODE;
#endif

#if POWER_IOKEY_EN
    } else if(IS_K_POWERR()) {
        key_tmp = T_KEY_POWER;
#endif

#if MODE_IOKEY_EN
    } else if(IS_K_MODE()) {
        key_tmp = T_KEY_MODE;
#endif

    } else {
        key_tmp = NO_KEY;
    }
    return key_tmp;
}

#else

//����ADKey��ⰴ��ֵ
#pragma location="SCAN_COM"
__near_func u8 adkey_get(void)
{
    u8 adc_val;
    u8 IAR_CONST *padc, *pval;
    u8 knum;
    //printf("%02x ", scan_ctl.adc_val);
    adc_val = scan_ctl.adc_val;
    knum = 0;
    if (scan_ctl.key_check == 0) {
        padc = tbl_key1_adc;
        pval = tbl_key1_val;
    }
#if (USE_ADKEY == 2)
    else {
        padc = tbl_key2_adc;
        pval = tbl_key2_val;
    }
#endif
    while (adc_val > padc[knum]) {
        knum++;
    }
#if SD_DECT_REUSE_ADKEY && !AUX_DECT_REUSE_ADKEY
    if(pval[knum] == NO_KEY) {
        if(adc_val < ADC_SD_IN) {
            sd_online_flag = 1;
        } else {
            sd_online_flag = 0;
        }
    }
#elif !SD_DECT_REUSE_ADKEY && AUX_DECT_REUSE_ADKEY
    if(pval[knum] == NO_KEY) {
        if(adc_val < ADC_LINE_IN) {
            aux_online_flag = 1;
        } else {
            aux_online_flag = 0;
        }
    }
#elif SD_DECT_REUSE_ADKEY && AUX_DECT_REUSE_ADKEY
    if(pval[knum] == NO_KEY) {
        if(adc_val < ADC_SD_LINE_IN) {
            sd_online_flag = 1;
            aux_online_flag = 1;
        }
    #if ADC_LINE_IN < ADC_SD_IN
        else if(adc_val < ADC_LINE_IN) {
            sd_online_flag = 0;
            aux_online_flag = 1;
        }
        else if(adc_val < ADC_SD_IN) {
            sd_online_flag = 1;
            aux_online_flag = 0;
        }
    #else
        else if(adc_val < ADC_SD_IN) {
            sd_online_flag = 1;
            aux_online_flag = 0;
        }
        else if(adc_val < ADC_LINE_IN) {
            sd_online_flag = 0;
            aux_online_flag = 1;
        }
    #endif
        else{
            sd_online_flag = 0;
            aux_online_flag = 0;
        }
    }
#endif
    return pval[knum];
}
#endif


//ɨ�谴��
#pragma location="SCAN_COM"
__near_func void key_scan(void)
{
    u8 key_val;

#if KU_PLAY_S10_TWICE_REJECT || KU_PLAY_S10_TWICE_CALLBACK
    if(paly_twice_flag)  twice_cnt ++;
    else  twice_cnt = 0;
#endif

    if (!adc_get()) {
        return;
    }

    switch (scan_ctl.mode) {
    case ADC_KEY:
        scan_ctl.key_check = 0;
#if (!USE_ADKEY)
        key_val = iokey_get();
#else   //USE_ADKEY
        key_val = adkey_get();
#endif  //USE_ADKEY

        if (key_val == NO_KEY) {
#if (USE_ADKEY == 2)
            scan_ctl.mode = ADC_KEY2;                   //��1�鰴���ް�����ɨ���2��
            ADCCON = BIT(7) | BIT(3) | ADKEY2_CH;
            break;
#else
            if (++scan_ctl.scan_cnt >= 10) {
                scan_ctl.scan_cnt = 0;
                scan_ctl.mode = ADC_BAT;                //�ۼ�10���ް�����ɨ����
                ADCCON = BIT(7) | BIT(3) | ADBAT_CH;
                break;
            }
#endif
        }
#if LED_REUSE_ADKEY
        ADKEY_INIT();
        TIMER0_EN();
#endif
        ADCCON = BIT(7) | BIT(3) | ADKEY_CH;
        break;
#if (USE_ADKEY == 2)
    case ADC_KEY2:
        scan_ctl.key_check = 1;
        key_val = adkey_get();
        if (key_val == NO_KEY) {
            if (++scan_ctl.scan_cnt >= 10) {
                scan_ctl.scan_cnt = 0;
                scan_ctl.mode = ADC_BAT;                //�ۼ�10���ް�����ɨ����
                ADCCON = BIT(7) | BIT(3) | ADBAT_CH;
                break;
            }
        #if LED_REUSE_ADKEY
            ADKEY_INIT();
            TIMER0_EN();
        #endif
            scan_ctl.mode = ADC_KEY;                    //��2�鰴���ް�����ɨ���1��
            ADCCON = BIT(7) | BIT(3) | ADKEY_CH;
            break;
        }
        ADCCON = BIT(7) | BIT(3) | ADKEY2_CH;
        break;
#endif

    case ADC_BAT:
        scan_ctl.bat_val1 = scan_ctl.adc_val;
        scan_ctl.mode = ADC_BANDGAP;
        ADCCON = BIT(7) | BIT(3) | ADBANDGAP_CH;
        return;                                         //��������
    case ADC_BANDGAP:
        scan_ctl.bat_val2 = scan_ctl.bat_val1 - scan_ctl.adc_val + 0x42;
        if(scan_ctl.bat_val2 > scan_ctl.bat_val) {
            scan_ctl.bat_val++;
        } else if(scan_ctl.bat_val2 < scan_ctl.bat_val) {
            scan_ctl.bat_val--;
        }
#if LED_REUSE_ADKEY
        ADKEY_INIT();
        TIMER0_EN();
#endif
        scan_ctl.mode = ADC_KEY;
        ADCCON = BIT(7) | BIT(3) | ADKEY_CH;
        return;                                         //��������
    }

#if HARD_IR_EN         //����Ӳ��
    if(key_val == NO_KEY) {
        key_val = ir_hardware_scan();
    }
#endif

    if (key_val == NO_KEY) {
#if USER_SEL == USER_2227
        if (RTCON1 & BIT(7)) {
            key_val = T_KEY_PLAY;
        }
#elif USER_SEL == USER_2227_LCD
        if (P2 & BIT(2)) {
            key_val = T_KEY_PLAY;
        }
#endif
    }

    key_val = key_deal(key_val);                        //������ֵ
#if SOFT_IR_EN
    if(key_val == NO_KEY) {
        key_val = key_deal_ir();                        //������ʱ������ֵ
    }
#endif

#if IR_HARD_POWERDOWN_EN || STANDBY_WAKEUP_EN
    if(sys_ctl.wakeup_delay) {
        sys_ctl.wakeup_delay--;                         //�ջ���ʱҪ��ʱС��ʱ�䣬���򿪻����ᱻ��Ϊ�ػ���
        return;                                         //��������
    }
#endif

    if(key_val==KLU_PREV || key_val== KLU_NEXT) {
        flush_msg();//��հ�����Ϣ
    }

    if (key_val != NO_KEY) {
#if KU_PLAY_S10_TWICE_REJECT || KU_PLAY_S10_TWICE_CALLBACK
    #if(KU_PLAY_S10_TWICE_REJECT)
        if(HSFRingActive){  //�绰��������ʱ
    #elif(KU_PLAY_S10_TWICE_CALLBACK)
        if(BT_STATE == StateConnected || BT_STATE == StatePlay){  //����������ͣ�򲥷�״̬ʱ
    #endif
            if(key_val == KU_PLAY_S10){
                if(!paly_twice_flag){
                    paly_twice_flag = 1;
                }
                else{
                    put_msg(T_KEY_PLAY_TWICE_S10);
                    paly_twice_flag = 0;
                    twice_cnt= 0;
                }
            }
            else
                put_msg(key_val);
        }
        else{
            put_msg(key_val);
            paly_twice_flag = 0;
            twice_cnt= 0;
        }
#else
        if((BT_STATE != StateNone) && HSFRingActive && IS_HSF_KEY(key_val)){  //�����绰��Ϣ
            bt_voice_abort = 1;
        }
        put_msg(key_val);
#endif

#if VOL_MAXMIN_MUSIC
        if(key_val == KLU_PREV_S10 || key_val == KLU_NEXT_S10 || key_val == KLU_VOL_DOWN ||
           key_val == KLU_VOL_UP   || key_val == KU_VOL_DOWN  || key_val == KU_VOL_UP){
            if(!sys_ctl.volume || sys_ctl.volume == VOLUME_MAX){
                sys_ctl.vol_maxmin_music_flag = 1;
            }
        }
#endif

#if KEY_VOICE_EN
        if((key_val & 0xE0) == T_KEY_SHORT_UP || (key_val & 0xE0) == T_KEY_LONG_UP || (key_val & 0xE0) == T_KEY_HOLD)
        {
            if(((key_val & 0x1F) == T_KEY_VOL_UP || (key_val & 0x1F) == T_KEY_VOL_DOWN || key_val == KH_PREV_S10 || key_val == KH_NEXT_S10)\
                && (sys_ctl.volume >= VOLUME_MAX || sys_ctl.volume <= 0))
            {
                ;//�������������С��Ҫ������
            } else
            {
                if(!(KVCON1 & BIT(0))){
                    KVCON1 |= 0x01;
                }
            }
        }
#endif

       //printf("key_val: 0x%02x\n",key_val);
       //reset_backlight();
    }
#if KU_PLAY_S10_TWICE_REJECT || KU_PLAY_S10_TWICE_CALLBACK
    else{   //ʹ��˫������ʱ���Ѿ�������һ�β���500ms��û���ٴε�������ֻ��һ��������Ϣ
        if(paly_twice_flag == 1 && twice_cnt > 100){
            put_msg(KU_PLAY_S10);
            paly_twice_flag = 0;
            twice_cnt= 0;

            #if(KU_PLAY_S10_TWICE_REJECT)
            bt_voice_abort = 1;
            #endif
        }
    }
#endif
}

//��ȡ����
#pragma inline=forced
__near_func u8 get_bat_val(void)
{
    u8 bat_val=0;
    while (scan_ctl.bat_val > tbl_bat_val[bat_val]) {
        bat_val++;
    }
    return bat_val;
}

//��ص�����⣬�ж��е���
__near_func void task_battery_check(void)
{
    /*if (IS_DCIN()) {
        if (sys_ctl.bat_val >= 5) {
            sys_ctl.bat_val = 0;
        }
        sys_ctl.bat_val++;
        if(get_bat_val()==5)
        {
            sys_ctl.bat_val = 5;
        }
    } else */{
        sys_ctl.bat_val = get_bat_val();

        //printf("%02x ", scan_ctl.bat_val);
#if LOWPOWER_WARNING
        if(!sys_ctl.lowpower_warning_flag){
            if(scan_ctl.bat_val <= LOWPOWER_WARNING_VOLTAGE){
                if(sys_ctl.lowpower_cnt > 0) {//printf("- ");
                    sys_ctl.lowpower_cnt--;
                } else {
                    sys_ctl.lowpower_warning_flag = 1;//printf("!!! lowpower_warning !!!\n ");
                    sys_ctl.lowpower_cnt = 50;      //50*100ms
                }
            } else {
                sys_ctl.lowpower_cnt = 50;          //50*100ms
            }
        } else
#endif
        {
            if(scan_ctl.bat_val <= LOWPOWER_POWEROFF_VOLTAGE){
                if(sys_ctl.lowpower_cnt > 0) {
                    sys_ctl.lowpower_cnt--;
                } else {
                    sys_flag.lowpower_flag = 1;
                    //printf("****lowpower_flag\n");
                }
            } else {
                sys_ctl.lowpower_cnt = 50;          //50*100ms
                //printf("\n");
            }
        }
    }
}

//ADKey��ʼ��
#pragma location="INIT_SEG_1"
void adc_scan_init(void)
{
    ADCCON = BIT(7) | BIT(3) | ADKEY_CH;        //adkey init
    ADCBAUD = 23;                               //system clk / 2(ADCBAUD + 1)

    memset(&adc_ctl, 0, sizeof(adc_ctl));
    memset_i(&key_ctl, 0, sizeof(key_ctl));
    key_ctl.key_val = 0;
    key_ctl.key_cnt = 0;
    key_ctl.key_up = 0;
}

//�����¼�
#pragma location="SCAN_COM"
__near_func u8 key_deal(u8 key_val)
{
    u8 key_return = NO_KEY;
    u8 key_tmp = NO_KEY;
    if (key_val == NO_KEY || key_val != key_ctl.key_val) {
        if (key_ctl.key_up < KEY_UP_TIMES) {
            key_ctl.key_up++;
        } else {
            if (key_ctl.key_cnt >= KEY_LONG_TIMES) {                //����̧��
                key_return = key_ctl.key_val | T_KEY_LONG_UP;
            } else if (key_ctl.key_cnt >= KEY_SCAN_TIMES) {         //�̰�̧��
                key_return = key_ctl.key_val | T_KEY_SHORT_UP;
            }
            key_tmp = key_ctl.key_val;
            key_ctl.key_val = key_val;
            key_ctl.key_cnt = 0;
        }
    } else {
        key_ctl.key_cnt++;
        if (key_ctl.key_cnt == KEY_SCAN_TIMES) {                    //ȥ��
            key_ctl.key_up = 0;
            key_return = key_ctl.key_val;
        } else if (key_ctl.key_cnt == KEY_LONG_TIMES) {             //����
            key_return = key_ctl.key_val | T_KEY_LONG;
        } else if (key_ctl.key_cnt == KEY_LONG_HOLD_TIMES) {        //����
            key_return = key_ctl.key_val | T_KEY_HOLD;
            key_ctl.key_cnt = KEY_LONG_TIMES;
        }
        key_tmp = key_ctl.key_val;
    }
    if(key_tmp > 0x1f && key_return >= 0x80)         //32-127��Щ��ֵ��֧�ֳ���
    {
        key_return = NO_KEY;
    }
    return key_return;
}
