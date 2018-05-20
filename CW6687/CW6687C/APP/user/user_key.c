/*****************************************************************************
* Module    : User
* File      : user_key.c
* Author    : Hanny
* Function  : 用户按键定义
*****************************************************************************/
#include "include.h"
#include "user_ir.h"
#include "task.h"

#include SET_USER_PATH(CFG_IO_KEY_DIR, /io/io_key.c)

enum {
    ADC_KEY,
    ADC_KEY2,
    ADC_BAT,
    ADC_BANDGAP,
    IO_KEY,
    WKO_KEY,
};

struct {
    u8 mode;
    u8 adc_val;
    u8 key_check;
    u8 scan_cnt;                //扫描次数。每隔几次，需要扫一次电池
    u8 bat_val1;
    u8 bat_val2;
    u8 bat_val;
#if SOFT_POWER_ON_OFF
    u8 power_up_flag;//用于控制软开机，若开机不松开开机键，则此按键暂时无效
#endif
} scan_ctl;

struct {
    u8 adc_sel;                     //选择哪一组ADC
    u8 adc_val;                     //ADC 采样的值
} adc_ctl;

struct {
    u16 adc_val[4];             //一阶滤波器上一个值
} key_ctl;

struct {
    u8 bat_tmp;
    u8 bat_cnt;
    u8 bat_flag;
} bat_ctl;

extern bool paly_twice_flag;
extern u16 twice_cnt;

#if INTERNAL_CHARGE_EN
volatile u8 charge_dc_sta = 1;
#endif
#if SD_DECT_REUSE_ADKEY
volatile u8 sd_online_flag;
#endif
#if AUX_DECT_REUSE_ADKEY
volatile u8 aux_online_flag;
#endif
#if EAR_ADKEY_REUSE
volatile u8 ear_online_flag;
#endif
#if LED_REUSE_ADKEY
volatile u8 adc_chk_flag;
#endif
#if SDCLK_REUSE_ADKEY
volatile u8 adc_get_flag;
volatile u8 adc_kick_flag;
#endif

#define BATVAL_NUM 10
IAR_CONST u8 tbl_bat_val[BATVAL_NUM] = {0x80,0x83,0x87,0x8a,0x8f,0x93,0x97,0x9a,0x9f,0xff};

#pragma location="POWER_CTL_SEG"
u8 get_bat_adcval(void)
{
     WATCHDOG_CLR();
     ADCCON = BIT(7) | BIT(3) | ADBAT_CH;
     while (ADCCON & BIT(7));  //等待采集完成.
     return ADCDATAH;
}

//CW6687不需要判断
#pragma location="POWER_CTL_SEG"
void buck_mode_select(void)
{
#if 0
    u8 cnt = 0;
    u8 cnt1 = 10;
    while(cnt1--)
    {
        if(get_bat_adcval()<0xB0)
        {
            cnt++;
        }
    }
    if(cnt >5)
    {
        buck_init();                    //使用BUCK模式
    }
#else
    buck_init();    //使用BUCK模式
#endif
}

//开机检查
#pragma location="POWER_CTL_SEG"
void power_on_check(void)
{
#if SOFT_POWER_ON_OFF
    //printf("power__on_check\n");
    DCIN_INIT();
    u8 wko_low_cnt = 0;
    u16 wko_high_cnt = 0;
    while(1){
        delay_ms(1);
        if(irtcc_read_cfg(READ_STA) & BIT(5)){
            wko_low_cnt = 0;
            if (wko_high_cnt++ > 300){
                led_mode_on();
                break;   //WKO(PWR/PP)长按开机
            }
        }else{
             wko_high_cnt = 0;
             if(wko_low_cnt++>50){
                 wko_low_cnt = 50+5;
                 //printf("%X ",P1);
                 if(IS_DCIN()){  //假关机,功耗大一点  //接5V电源,没有按PWR时.
                   // printf(">");
#if INTERNAL_CHARGE_EN
                     charge_detect();
#endif

                    continue;    //继续检测WKO
                 }else{
                    //printf("power_down\n");
                    power_down_enter(); //低功耗软关机
                 }
             }
        }
    }
#endif
}

void scan_init(void)
{
    PWRCON3 |= (BIT(1)|BIT(0));
    memset(&scan_ctl, 0, sizeof(scan_ctl));
    scan_ctl.bat_val = 0xff;
    scan_ctl.mode = ADC_BAT;
#if USE_ADKEY
    ADKEY_INIT();           //ADKey
#if SDCLK_REUSE_ADKEY
    /* 复用SDCLK检测ADKEY时，需要1ms检测一次ADC是否转换完成，
    若一次转换过程中，出现读卡，需要重新启动一次ADC转换。
    */
    adc_get_flag = 0;
    adc_kick_flag = 0;
#endif
#endif
#if USE_IOKEY
    io_key_init();
#endif
#if USE_WKOKEY
    u8 wko_sfr = irtcc_read_cfg(READ_WKO);
    wko_sfr |= BIT(2)|BIT(3);   //wko pin input enable
    wko_sfr &= ~BIT(7);  //wko pin 60K pulldown enalbe,注意要打开BIT3,60K下拉才会有效.
    wko_sfr &= ~(BIT(0) |BIT(1)|BIT(5)); //如果闹钟唤醒，WKO的BIT(0)会永远为1，但是再写回1时，则会打开RTC的测试模式，唤醒后的RTC会走得很快，所以这里要把BIT(0)灭掉
    //printf("wko_sfr=%02x",wko_sfr);
    irtcc_write_cfg(WRITE_WKO,wko_sfr);
#endif
#if LONG_PRESS_POWERON
    if(!sys_ctl.alarm_wakeup_flag){  //闹钟唤醒的，直接跳过开关判断。
        power_on_check();
    }
#endif
}

//ADKey初始化
#pragma location="INIT_SEG_1"
void adc_scan_init(void)
{
    ADCCON = BIT(7) | BIT(3) | ADKEY_CH;        //adkey init
    ADCBAUD = 23;                               //system clk / 2(ADCBAUD + 1)
    memset(&adc_ctl, 0, sizeof(adc_ctl));
    memset(&key_ctl, 0, sizeof(key_ctl));
    memset(&bat_ctl, 0, sizeof(bat_ctl));
}

//获取ADC的值
#pragma inline=forced
#pragma location="SCAN_COM"
__near_func bool adc_get(void)
{
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
   // printf("%02x ", scan_ctl.adc_val);

    return true;
}

#if LED_REUSE_ADKEY
#pragma location="SCAN_COM"
__near_func void adc_chk(void)
{
    if(adc_chk_flag) {
        if (ADCCON & BIT(7)) {      //ADKEY未完成，继续检测
            return;
        } else {
            adc_chk_flag = 0;
            TIMER0_DIS();           //disable timer0
            ADKEY_END();            //使用完ADKEY，把IO设为输出，控制LED
        }
    }
}

#pragma vector = VECTOR_OFFSET(TIMER0_INT)
#pragma register_bank=2  //0优先级(最低优先级)必须用bank1 //2优先级必须用bank2 //3优先级必须用bank3
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
            if ((ADCCON & BIT(7)) == 0) {      //ADKEY转换完成
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
        adc_kick_flag = 1;              //此次转换不可靠，需要重新kick
    }
}
#endif

#if USE_ADKEY
//根据ADKey检测按键值
#pragma location="SCAN_COM"
__near_func u8 key_get(void)
{
    u8 adc_val;
    u8 IAR_CONST *pkey;
    u8 knum;
    //printf("%02x-", scan_ctl.adc_val);

    adc_val = key_filter(scan_ctl.key_check, scan_ctl.adc_val);
    /*if(adc_val < 0xf0) {
        printf("%02x\n", adc_val);
    }*/
    knum = 0;
    pkey = tbl_key1;
#if (USE_ADKEY == 2)
    if (scan_ctl.key_check == 1) {
        pkey = tbl_key2;
    }
#endif
    while (adc_val > pkey[knum]) {
        knum+=2;
    }
    return pkey[knum+1];
}
#endif

//用于在蓝牙模式下,快速连接两次KU_PLAY,则发送一个KU_HSF,用于回拨电话.
__near_func u8 double_click_filter(u8 key_val)  //5ms调用一次.
{
    static u8 key_double_click_cnt = 0;
    if (TASK_BT == task_ctl.work_sta)
    {
        if (key_double_click_cnt > 0){  //双击超时计时.
            //if(key_double_click_cnt%20==0) {printf("*%X",key_double_click_cnt);}
             key_double_click_cnt--;
        }

        if(1 == key_double_click_cnt){
            return DOUBLE_CLICK_KEY;
        }

        if(DOUBLE_CLICK_KEY == key_val){
            if(key_double_click_cnt){
                key_double_click_cnt = 0;
                return KU_HSF;   //两次 DOUBLE_FILTER_KEY,则发送一个KU_HSF,用于回拨电话.
            }else {
                key_double_click_cnt = 400/5;  //延时400ms,判断双击,
                return NO_KEY;
            }
        }
    }
    return key_val;
}

#if USE_WKOKEY
//因为irtcc_read_cfg函数在bank区不能直接用
#pragma location="SCAN_COM"
__near_func bool get_wko_pin_state(void)
{
    u8 irtcc_cfg;
    IRTCON |= 0x01;                  //irtcc communicate en
    irtcc_write(READ_STA);
    irtcc_cfg = irtcc_read();
    IRTCON &= ~(0x01);
    //printf("irtcc_cfg=%02x\n",irtcc_cfg);
    return (irtcc_cfg & BIT(5));
}
#endif

#pragma location="SCAN_COM"
__near_func u8 get_scan_adc_val(void)
{
    return scan_ctl.adc_val;
}

//扫描按键
#pragma location="SCAN_COM"
__near_func void key_scan(void)
{

    u8 key_val = NO_KEY;
    if (!adc_get()) {
        return;
    }
    switch (scan_ctl.mode) {
#if (USE_ADKEY)
    case ADC_KEY:
        scan_ctl.key_check = 0;
        //printf("%02x ", scan_ctl.adc_val);
        key_val = key_get();
        if (NO_KEY == key_val) {
            OTHER_ADC_CHECK();
            if (++scan_ctl.scan_cnt >= 10) {
                scan_ctl.scan_cnt = 0;
                scan_ctl.mode = ADC_BAT;                                //累计10次无按键，扫描电池
                ADCCON = BIT(7) | BIT(3) | ADBAT_CH;
                break;
            }
  #if (USE_ADKEY == 2)
            scan_ctl.mode = ADC_KEY2;                                   //第1组按键无按键，扫描第2组
            ADCCON = BIT(7) | BIT(3) | ADKEY2_CH;
            break;
  #elif (USE_IOKEY || USE_WKOKEY)
            scan_ctl.mode = IO_KEY;   //NO_KEY,则扫描下一组IO KEY.
  #endif
        }else{
            ADCCON = BIT(7) | BIT(3) | ADKEY_CH;  //有ADC_KEY按下，继续扫描本组按键
        }
        break;
#endif

#if (USE_ADKEY == 2)
    case ADC_KEY2:
        scan_ctl.key_check = 1;
        key_val = key_get();
        if (key_val == NO_KEY) {
             OTHER_ADC2_CHECK();
    #if (USE_IOKEY || USE_WKOKEY)
             scan_ctl.mode = IO_KEY;   //NO_KEY,则扫描下一组IO KEY.
    #else
            scan_ctl.mode = ADC_KEY;                                    //第2组按键无按键，扫描第1组
            ADCCON = BIT(7) | BIT(3) | ADKEY_CH;
    #endif
        }else{
            ADCCON = BIT(7) | BIT(3) | ADKEY2_CH;  //有ADC_KEY2按下，继续扫描本组按键
        }
        break;
#endif

#if (USE_IOKEY || USE_WKOKEY)
    case WKO_KEY:
    case IO_KEY:
  #if USE_IOKEY
        key_val = get_io_key();
  #endif

  #if USE_WKOKEY
        if (NO_KEY == key_val){
            static u8 dcin_cnt = 0; //避免DC_IN插入脉冲触发按键。
            if(IS_DCIN()){
                if(dcin_cnt < 50){
                    dcin_cnt++;
                }
            }else{
                dcin_cnt = 0;
            }
            if ((NO_KEY == key_val) && (!(IRTCON & 0x01))){  //!(IRTCON&0x01)： 主循环中也可能与rtc通信，这里加判断互斥一下
                if(get_wko_pin_state()) {
                    if((0 == dcin_cnt) || (dcin_cnt >= 50)){
        #if SOFT_POWER_ON_OFF
                        if(scan_ctl.power_up_flag)
        #endif
                            key_val = T_KEY_PLAY_POWER;
                    }
                }
        #if SOFT_POWER_ON_OFF
                else{
                    scan_ctl.power_up_flag = 1;
                }
        #endif
            }
        }
  #endif //end #if USE_WKOKEY
       if (NO_KEY == key_val) {
            if (++scan_ctl.scan_cnt >= 10){
                scan_ctl.scan_cnt = 0;
                scan_ctl.mode = ADC_BAT;                                //累计10次无按键，扫描电池
                ADCCON = BIT(7) | BIT(3) | ADBAT_CH;
            }
        }else{
            scan_ctl.mode = IO_KEY;  //有IO KEY按下，继续扫描本组按键
        }
        break;
#endif  //end #if USE_WKOKEY

    case ADC_BAT:
        scan_ctl.bat_val1 = scan_ctl.adc_val;
       // printf("%02x:",scan_ctl.adc_val);
        scan_ctl.mode = ADC_BANDGAP;
        ADCCON = BIT(7) | BIT(3) | ADBANDGAP_CH;
        return;                                             //不处理按键
    case ADC_BANDGAP:
        //printf("%02x ",scan_ctl.adc_val);
        scan_ctl.bat_val2 = scan_ctl.bat_val1 - scan_ctl.adc_val + 0x5a;
        if(scan_ctl.bat_val2 > scan_ctl.bat_val) {
            scan_ctl.bat_val++;
        } else if(scan_ctl.bat_val2 < scan_ctl.bat_val) {
            scan_ctl.bat_val--;
        }
#if USE_ADKEY
        scan_ctl.mode = ADC_KEY;
        ADCCON = BIT(7) | BIT(3) | ADKEY_CH;
#elif (USE_IOKEY || USE_WKOKEY)
        scan_ctl.mode = IO_KEY;
#endif
        return;                                             //不处理按键
    }

#if HARD_IR_EN         //红外硬解
    if(key_val == NO_KEY) {
        key_val = ir_hardware_scan();
    }
#elif SOFT_IR_EN
    if(key_val == NO_KEY) {
        if(32 == ir.state){
            key_val = ir_key_get(ir.data & 0xff);
        }
    }
#endif
    key_val = key_deal(key_val, KEY_SCAN_TIMES, KEY_UP_TIMES, KEY_LONG_TIMES, KEY_LONG_HOLD_TIMES);     //处理按键值

#if KEY_DOUBLE_CLICK_EN
    key_val = double_click_filter(key_val); //KU_HSF;   //处理蓝牙下双击回拨电话
#endif
    if (key_val != NO_KEY) {
        //printf("%X>>",key_val);
        put_msg(key_val);
        key_msg_flags(key_val);
#if IS_LCD_DISPLAY
        reset_backlight();
#endif
    }
}

#pragma inline=forced
__near_func void key_msg_flags(u8 key_val)
{
#if WARNING_VOL_MAXMIN
        if (key_val == KLU_VOL_DOWN ||key_val == KLU_VOL_UP|| key_val == KU_VOL_DOWN
            || key_val == KU_VOL_UP || key_val == KLU_PREV_VOL_DOWN || key_val == KLU_NEXT_VOL_UP)
        {
            if(!sys_ctl.volume || sys_ctl.volume >= VOLUME_MAX){
                //printf("maxmin\n");
                sys_ctl.vol_maxmin_music_flag = 1;
            }
        }
#endif
}

#if 1
//获取电量
#pragma inline=forced
__near_func u8 get_bat_val(void)
{
    u8 bat_val=0;
    while (scan_ctl.bat_val > tbl_bat_val[bat_val]) {
        bat_val++;
    }
    return bat_val;
}
#else
//获取电量
#pragma inline=forced
__near_func u8 get_bat_val(void)
{
    u8 bat_val=0;
    u8 bat_return = sys_ctl.bat_val;
    while (scan_ctl.bat_val > tbl_bat_val[bat_val]) {
        bat_val++;
    }

    //printf("f=%d v=%d t1=%d s=%d", bat_ctl.bat_flag, bat_val, bat_ctl.bat_tmp, sys_ctl.bat_val);
    if (!bat_ctl.bat_flag) {
        bat_ctl.bat_tmp = bat_val;
        bat_ctl.bat_cnt = 0;
        if (bat_val < sys_ctl.bat_val) {
            bat_ctl.bat_flag = 1;
        } else if (bat_val > sys_ctl.bat_val) {
            bat_ctl.bat_flag = 2;
        }
    } else {
        if (bat_val == bat_ctl.bat_tmp) {
            bat_ctl.bat_cnt++;
            //printf("c=%d ", bat_ctl.bat_cnt);
            if (bat_ctl.bat_cnt == 20) {
                bat_ctl.bat_flag = 0;
                bat_ctl.bat_cnt = 0;
                bat_return = bat_val;
            }
        } else if (bat_val < bat_ctl.bat_tmp && bat_ctl.bat_flag==1) {
            bat_ctl.bat_flag = 0;
            bat_return = bat_val;
        } else if (bat_val > bat_ctl.bat_tmp && bat_ctl.bat_flag==2) {
            bat_ctl.bat_flag = 0;
            bat_return = bat_val;
        } else {
            bat_ctl.bat_flag = 0;
        }
    }
    //printf("r=%d\n", bat_return);
    return bat_return;
}
#endif

//电池电量检测，中断中调用
__near_func void task_battery_check(void)
{
#if IS_LCD_DISPLAY
    static u8 dc_det_time = 0;  // 记录充电检测次数
    if (IS_DCIN()) {
        dc_det_time++;
        if(dc_det_time > 8) {   // 大概1s电量变一次
            dc_det_time = 0;
            if (sys_ctl.bat_val >= 5) {
                sys_ctl.bat_val = 0;
            }
            sys_ctl.bat_val++;
            if(get_bat_val()==5)
            {
                sys_ctl.bat_val = 5;
            }
        }
    } else
#endif
    {
        sys_ctl.bat_val = get_bat_val();
        //printf("%d ",sys_ctl.bat_val);
        if (task_ctl.work_sta != TASK_BT) {
            sys_ctl.bat_val = (sys_ctl.bat_val + 1) >>1;
        }
#if LED_STATUS_BLINK
        static u8 status_cnt = 0;
        if(scan_ctl.bat_val <= 0x80){
            if(status_cnt < 50) {
                status_cnt++;
            } else {
                sys_ctl.led_status_blink_flag = 1;
            }
        } else {
            if(scan_ctl.bat_val > 0x82) {
                sys_ctl.led_status_blink_flag = 0;
            }
            status_cnt = 0;
            LED_STATUS_OFF();
        }
#endif
#if TASK_POWEROFF_EN
#if WARNING_LOWPOWER
        //低电预报循环
        if(2 == sys_ctl.lowpower_warning_flag){
            sys_ctl.lowpower_warning_cycle++;
            if(sys_ctl.lowpower_warning_cycle > LOWPOWER_WARNING_CYCLE){   //100*100ms = 10s提示一次低电
                sys_ctl.lowpower_warning_flag = 1;
                sys_ctl.lowpower_warning_cycle = 0;
            }
            if(scan_ctl.bat_val > LOWPOWER_WARNING_VOLTAGE){
                 sys_ctl.lowpower_warning_flag = 0;
            }
        }
        //低电预报判断
        if(!sys_ctl.lowpower_warning_flag)
        {
            if(scan_ctl.bat_val <= LOWPOWER_WARNING_VOLTAGE){
                if(sys_ctl.lowpower_cnt > 0) {
                    sys_ctl.lowpower_cnt--;
                } else {
                    sys_ctl.lowpower_warning_flag = 1;
                }
            } else {
                sys_ctl.lowpower_cnt = 50;          //lowpower_cnt*100ms
            }
        }else
#endif
        {
            //printf("%02x ", scan_ctl.bat_val);
            if(scan_ctl.bat_val <= LOWPOWER_VOLTAGE){
                if(sys_ctl.lowpower_cnt > 0) {
                    sys_ctl.lowpower_cnt--;
                } else {
                    sys_ctl.lowpower_flag = 1;
                    task_ctl.work_sta = TASK_POWEROFF;
                    //printf("****lowpower_flag\n");
                }
            } else {
                sys_ctl.lowpower_cnt = 50;          //50*100ms
            }
        }
#endif
    }
}

//按键滤波
#pragma location="SCAN_COM"
__near_func u8 key_filter(u8 group, u8 adc_val)
{
    //一阶滤波
    u8 tmp = (key_ctl.adc_val[group] >> 9) - (adc_val >> 1);
    if (tmp & 0x80) {
        tmp = -tmp;
    }
    if (tmp <= 0x08) {
        key_ctl.adc_val[group] = (u16)adc_val * 48 + (((u32)key_ctl.adc_val[group] * 208 + 128) >> 8);
    } else {
        key_ctl.adc_val[group] = (u16)adc_val << 8;
    }
    return (key_ctl.adc_val[group] >> 8);
}

#if INTERNAL_CHARGE_EN
//充电初始化
#pragma location="POWER_CTL_SEG"
void charge_init(void)
{
    LED_POWER_OFF();
    CHAGCON0 = (CHAGCON0 & 0x0F) | BIT(6);    //software control stop charge
    CHAGCON1 = 0x04;        //充电电流 160mA; 配置为电池容量的一半多
    CHAGCON0 |= 0x01 << 4;  //充电终止检测电流：00--20mA; 01--40mA; 10--60mA; 11--80mA
    CHAGCON0 = (CHAGCON0 & 0xF0) | 0x09;   //BG trim
    //printf("CHA0=%02x\n", CHAGCON0);
}

#define STOP_CHARGE_TIME        10  //20s
#define START_CHAGRE_TIME       5  //5s

//充电检测
__near_func void charge_detect(void)
{
    static u8 charge_cnt = 0;
    //printf("CHAG1=%02x\n", CHAGCON1);
    if (IS_DCIN()) {
        if ((CHAGCON1 & 0x30) == 0x30) {    //电池电压 >= 4.1v且充电电流 < 40mA, 停止充电
            if (!(CHAGCON0 & BIT(7))) {
                charge_cnt++;
                //printf("stop=%02x\n", charge_cnt);
                if (charge_cnt >= STOP_CHARGE_TIME) {
                    charge_cnt = 0;
                    CHAGCON0 |= BIT(7);
                    charge_dc_sta = 0;
                }
            }
        } else if (!(CHAGCON1 & BIT(5))) {  //电池电压 < 4.1v, 开始充电
            if (CHAGCON0 & BIT(7)) {
                charge_cnt++;
                //printf("start=%02x\n", charge_cnt);
                if (charge_cnt >= START_CHAGRE_TIME) {
                    charge_cnt = 0;
                    CHAGCON0 &= ~BIT(7);
                }
            }
        }else {
           CHAGCON0 |= BIT(7);     //停止充电
        }
    } else {
        //printf("dc_in_remove\n");
        charge_cnt = 0;
        CHAGCON0 |= BIT(7);     //停止充电
        charge_dc_sta = 1;
    }

#if LED_POWER_BLINK  //更改led指示，充满电后关闭LED不再指示直到dc状态改变
    if (CHAGCON0 & BIT(7)) {
        LED_POWER_OFF();
    } else if (charge_dc_sta && !(CHAGCON0 & BIT(7))){
        LED_POWER_ON();
    }
#endif
/*
#if 0//LED_POWER_BLINK
    if (CHAGCON0 & BIT(7)) {
        LED_POWER_OFF();
    } else {
        LED_POWER_ON();
    }
#endif
*/
}
#endif
