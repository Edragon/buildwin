/*****************************************************************************
 * Module    : user
 * File      : user_key.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户按键配置
 *****************************************************************************/
#ifndef USER_KEY_H
#define USER_KEY_H


/*****************************************************************************
 * Module    : ADKEY  配置
 *****************************************************************************/
#define ADBAT_CH                0x06
#define ADBANDGAP_CH            0x07
#define ADKEY_CH                ADKEY_PORT
#define ADKEY2_CH               ADKEY2_PORT

#if (ADKEY_PORT==ADKEY_P33)
    #define ADC_INIT()          P3DIR |= BIT(3)
    #define ADC_PU10K()         P3PU0 |= BIT(6)
    #define ADC_NOPU()          P3PU0 &= ~BIT(6)
    #define ADC_END()           P3DIR &= ~BIT(3)
#elif (ADKEY_PORT==ADKEY_P06)
    #define ADC_INIT()          P0DIR |= BIT(6)
    #define ADC_PU10K()         P0PU0 |= BIT(6)
    #define ADC_NOPU()          P0PU0 &= ~BIT(6)
    #define ADC_END()           P0DIR &= ~BIT(6)
#elif (ADKEY_PORT==ADKEY_P04)
    #define ADC_INIT()          P0DIR |= BIT(4)
    #define ADC_PU10K()         P0PU0 |= BIT(4)
    #define ADC_NOPU()          P0PU0 &= ~BIT(4)
    #define ADC_END()           P0DIR &= ~BIT(4)
#elif (ADKEY_PORT==ADKEY_P05)
    #define ADC_INIT()          P0DIR |= BIT(5)
    #define ADC_PU10K()         P0PU0 |= BIT(5)
    #define ADC_NOPU()          P0PU0 &= ~BIT(5)
    #define ADC_END()           P0DIR &= ~BIT(5)
#elif (ADKEY_PORT==ADKEY_P30)
    #define ADC_INIT()          P3DIR |= BIT(0)
    #define ADC_PU10K()         P3PU0 |= BIT(0);P3PU0 &= ~BIT(1)
    #define ADC_NOPU()          P3PU0 &= ~(BIT(0)|BIT(1))
    #define ADC_END()           P3DIR &= ~BIT(0)
#elif (ADKEY_PORT==ADKEY_P13)
    #define ADC_INIT()          P1DIR |= BIT(3)
    #define ADC_PU10K()         P1PU0 |= BIT(3)
    #define ADC_NOPU()          P1PU0 &= ~BIT(3)
    #define ADC_END()           P1DIR &= ~BIT(3)
#else
    #error "请选择一组ADKEY_PORT"
#endif

#if (ADKEY2_PORT==ADKEY_P33)
    #define ADC2_INIT()         P3DIR |= BIT(3)
    #define ADC2_PU10K()        P3PU0 |= BIT(6)
    #define ADC2_NOPU()         P3PU0 &= ~BIT(6)
#elif (ADKEY2_PORT==ADKEY_P06)
    #define ADC2_INIT()         P0DIR |= BIT(6)
    #define ADC2_PU10K()        P0PU0 |= BIT(6)
    #define ADC2_NOPU()         P0PU0 &= ~BIT(6)
#elif (ADKEY2_PORT==ADKEY_P04)
    #define ADC2_INIT()         P0DIR |= BIT(4)
    #define ADC2_PU10K()        P0PU0 |= BIT(4)
    #define ADC2_NOPU()         P0PU0 &= ~BIT(4)
#elif (ADKEY2_PORT==ADKEY_P05)
    #define ADC2_INIT()         P0DIR |= BIT(5)
    #define ADC2_PU10K()        P0PU0 |= BIT(5)
    #define ADC2_NOPU()         P0PU0 &= ~BIT(5)
#elif (ADKEY2_PORT==ADKEY_P30)
    #define ADC2_INIT()         P3DIR |= BIT(0)
    #define ADC2_PU10K()        P3PU0 |= BIT(0);P3PU0 &= ~BIT(1)
    #define ADC2_NOPU()         P3PU0 &= ~(BIT(0)|BIT(1))
#elif (ADKEY2_PORT==ADKEY_P13)
    #define ADC2_INIT()         P1DIR |= BIT(3)
    #define ADC2_PU10K()        P1PU0 |= BIT(3)
    #define ADC2_NOPU()         P1PU0 &= ~BIT(5)
#else
    #error "请选择一组ADKEY2_PORT"
#endif

#if ADKEY_INT_PU10K
    #define ADKEY_INIT()        ADC_INIT();ADC_PU10K();
    #define ADKEY_END()         ADC_END();
#else
    #define ADKEY_INIT()        ADC_INIT();ADC_NOPU();
    #define ADKEY_END()         ADC_END();
#endif

#if ADKEY2_INT_PU10K
    #define ADKEY2_INIT()        ADC2_INIT();ADC2_PU10K();
#else
    #define ADKEY2_INIT()        ADC2_INIT();ADC2_NOPU();
#endif


/*****************************************************************************
 * Module    : 按键定义配置
 *****************************************************************************/
#define KEY_SCAN_TIMES          6           //按键防抖的扫描次数
#define KEY_UP_TIMES            6           //抬键防抖的扫描次数
#define KEY_LONG_TIMES          150         //长按键的次数
#define KEY_HOLD_TIMES          35          //连接的频率次数
#define KEY_LONG_HOLD_TIMES     (KEY_LONG_TIMES + KEY_HOLD_TIMES)

#define T_KEY_SHORT             0x00
#define T_KEY_SHORT_UP          0x80
#define T_KEY_LONG              0xA0
#define T_KEY_LONG_UP           0xC0
#define T_KEY_HOLD              0xE0


//定义为32以上(含32)的按键只支持单按
#define NO_KEY                  0
#define T_KEY_1                 0x1
#define T_KEY_2                 0x2
#define T_KEY_3                 0x3
#define T_KEY_4                 0x4
#define T_KEY_5                 0x5
#define T_KEY_6                 0x6
#define T_KEY_7                 0x7
#define T_KEY_8                 0x8
#define T_KEY_9                 0x9
#define T_KEY_9                 0x9
#define T_KEY_10                0xa
#define T_KEY_11                0xb
#define T_KEY_12                0xc
#define T_KEY_13                0xd
#define T_KEY_14                0xe
#define T_KEY_15                0xf
#define T_KEY_16                0x10
#define T_KEY_17                0x11
#define T_KEY_18                0x12
#define T_KEY_19                0x13
#define T_KEY_20                0x14
#define T_KEY_21                0x15
#define T_KEY_22                0x16
#define T_KEY_23                0x17
#define T_KEY_24                0x18
#define T_KEY_25                0x19
#define T_KEY_26                0x1a
#define T_KEY_27                0x1b
#define T_KEY_28                0x1c
#define T_KEY_29                0x1d
#define T_KEY_30                0x1e
#define T_KEY_31                0x1f

//数字按键
#define T_KEY_NUM_0             0x20
#define T_KEY_NUM_1             0x21
#define T_KEY_NUM_2             0x22
#define T_KEY_NUM_3             0x23
#define T_KEY_NUM_4             0x24
#define T_KEY_NUM_5             0x25
#define T_KEY_NUM_6             0x26
#define T_KEY_NUM_7             0x27
#define T_KEY_NUM_8             0x28
#define T_KEY_NUM_9             0x29
//只需单按的功能键
#define T_KEY_LOOP              0x2A
#define T_KEY_EQ                0x2B
#define T_KEY_STOP              0x2C
#define T_KEY_MUTE              0x2D
#define T_KEY_USBSD             0x2E
//双击消息
#define T_KEY_PLAY_TWICE_S10    0x2F
#define K_MIC_VOL_DOWN          0x30
#define K_MIC_VOL_UP            0x31
//系统消息
#define QSYSTEM_1S              0x7e    //系统1秒消息
#define BT_CONN_REQ             0x7d    //蓝牙连接
#define BT_DIS_CONN             0x7c    //蓝牙断开
#define BT_PHONE_VOL_DOWN       0x7b    //手机端减小媒体音量
#define BT_PHONE_VOL_UP         0x7a    //手机端增大媒体音量
#define BT_PHONE_CHANGE_HSHFVOL 0x79    //手机端调节通话音量

//功能键
#define T_KEY_PREV              T_KEY_1
#define T_KEY_PLAY              T_KEY_2
#define T_KEY_NEXT              T_KEY_3
#define T_KEY_VOL_DOWN          T_KEY_4
#define T_KEY_VOL_UP            T_KEY_5
#define T_KEY_HSF               T_KEY_6
#define T_KEY_PLAY_S10          T_KEY_7
#define T_KEY_PREV_S10          T_KEY_8
#define T_KEY_NEXT_S10          T_KEY_9
#define T_KEY_MODE              T_KEY_10
#define T_KEY_POWER             T_KEY_11
#define T_KEY_SCAN              T_KEY_12
#define T_KEY_SPEED             T_KEY_13
#define T_KEY_SET_A             T_KEY_14
#define T_KEY_REPEAT            T_KEY_15

//复用功能键
#define T_KEY_OK                T_KEY_PLAY
#define T_KEY_BACK              T_KEY_HSF
#define T_KEY_USBVOL_DOWN       T_KEY_VOL_DOWN
#define T_KEY_USBVOL_UP         T_KEY_VOL_UP


/*****************************************************************************
 * Module    : 按键长短按配置
 *****************************************************************************/
//按键详细定义 -> K_: Key Short Down;   KU_: Key Short Up;   KL_: Key Long Down;   KLU_: Key Long Up;   KH_: Key Hold;

#define K_PREV_S10                  (T_KEY_PREV_S10 | T_KEY_SHORT)
#define KU_PREV_S10                 (T_KEY_PREV_S10 | T_KEY_SHORT_UP)
#define KL_PREV_S10                 (T_KEY_PREV_S10 | T_KEY_LONG)
#define KLU_PREV_S10                (T_KEY_PREV_S10 | T_KEY_LONG_UP)
#define KH_PREV_S10                 (T_KEY_PREV_S10 | T_KEY_HOLD)

#define K_PLAY_S10                  (T_KEY_PLAY_S10 | T_KEY_SHORT)
#define KU_PLAY_S10                 (T_KEY_PLAY_S10 | T_KEY_SHORT_UP)
#define KL_PLAY_S10                 (T_KEY_PLAY_S10 | T_KEY_LONG)
#define KLU_PLAY_S10                (T_KEY_PLAY_S10 | T_KEY_LONG_UP)
#define KH_PLAY_S10                 (T_KEY_PLAY_S10 | T_KEY_HOLD)

#define K_NEXT_S10                  (T_KEY_NEXT_S10 | T_KEY_SHORT)
#define KU_NEXT_S10                 (T_KEY_NEXT_S10 | T_KEY_SHORT_UP)
#define KL_NEXT_S10                 (T_KEY_NEXT_S10 | T_KEY_LONG)
#define KLU_NEXT_S10                (T_KEY_NEXT_S10 | T_KEY_LONG_UP)
#define KH_NEXT_S10                 (T_KEY_NEXT_S10 | T_KEY_HOLD)

#define K_SCAN                      (T_KEY_SCAN | T_KEY_SHORT)
#define KU_SCAN                     (T_KEY_SCAN | T_KEY_SHORT_UP)
#define KL_SCAN                     (T_KEY_SCAN | T_KEY_LONG)
#define KLU_SCAN                    (T_KEY_SCAN | T_KEY_LONG_UP)
#define KH_SCAN                     (T_KEY_SCAN | T_KEY_HOLD)

#define K_PREV                  (T_KEY_PREV | T_KEY_SHORT)
#define KU_PREV                 (T_KEY_PREV | T_KEY_SHORT_UP)
#define KL_PREV                 (T_KEY_PREV | T_KEY_LONG)
#define KLU_PREV                (T_KEY_PREV | T_KEY_LONG_UP)
#define KH_PREV                 (T_KEY_PREV | T_KEY_HOLD)

#define K_PLAY                  (T_KEY_PLAY | T_KEY_SHORT)
#define KU_PLAY                 (T_KEY_PLAY | T_KEY_SHORT_UP)
#define KL_PLAY                 (T_KEY_PLAY | T_KEY_LONG)
#define KLU_PLAY                (T_KEY_PLAY | T_KEY_LONG_UP)
#define KH_PLAY                 (T_KEY_PLAY | T_KEY_HOLD)

#define K_NEXT                  (T_KEY_NEXT | T_KEY_SHORT)
#define KU_NEXT                 (T_KEY_NEXT | T_KEY_SHORT_UP)
#define KL_NEXT                 (T_KEY_NEXT | T_KEY_LONG)
#define KLU_NEXT                (T_KEY_NEXT | T_KEY_LONG_UP)
#define KH_NEXT                 (T_KEY_NEXT | T_KEY_HOLD)

#define K_VOL_DOWN              (T_KEY_VOL_DOWN | T_KEY_SHORT)
#define KU_VOL_DOWN             (T_KEY_VOL_DOWN | T_KEY_SHORT_UP)
#define KL_VOL_DOWN             (T_KEY_VOL_DOWN | T_KEY_LONG)
#define KLU_VOL_DOWN            (T_KEY_VOL_DOWN | T_KEY_LONG_UP)
#define KH_VOL_DOWN             (T_KEY_VOL_DOWN | T_KEY_HOLD)

#define K_VOL_UP                (T_KEY_VOL_UP | T_KEY_SHORT)
#define KU_VOL_UP               (T_KEY_VOL_UP | T_KEY_SHORT_UP)
#define KL_VOL_UP               (T_KEY_VOL_UP | T_KEY_LONG)
#define KLU_VOL_UP              (T_KEY_VOL_UP | T_KEY_LONG_UP)
#define KH_VOL_UP               (T_KEY_VOL_UP | T_KEY_HOLD)

#define K_MODE                  (T_KEY_MODE | T_KEY_SHORT)
#define KU_MODE                 (T_KEY_MODE | T_KEY_SHORT_UP)
#define KL_MODE                 (T_KEY_MODE | T_KEY_LONG)
#define KLU_MODE                (T_KEY_MODE | T_KEY_LONG_UP)
#define KH_MODE                 (T_KEY_MODE | T_KEY_HOLD)

#define K_HSF                   (T_KEY_HSF | T_KEY_SHORT)
#define KU_HSF                  (T_KEY_HSF | T_KEY_SHORT_UP)
#define KL_HSF                  (T_KEY_HSF | T_KEY_LONG)
#define KLU_HSF                 (T_KEY_HSF | T_KEY_LONG_UP)
#define KH_HSF                  (T_KEY_HSF | T_KEY_HOLD)

#define K_SPEED                 (T_KEY_SPEED | T_KEY_SHORT)
#define KU_SPEED                (T_KEY_SPEED | T_KEY_SHORT_UP)
#define KL_SPEED                (T_KEY_SPEED | T_KEY_LONG)
#define KLU_SPEED               (T_KEY_SPEED | T_KEY_LONG_UP)
#define KH_SPEED                (T_KEY_SPEED | T_KEY_HOLD)

#define K_SET_A                 (T_KEY_SET_A | T_KEY_SHORT)
#define KU_SET_A                (T_KEY_SET_A | T_KEY_SHORT_UP)
#define KL_SET_A                (T_KEY_SET_A | T_KEY_LONG)
#define KLU_SET_A               (T_KEY_SET_A | T_KEY_LONG_UP)
#define KH_SET_A                (T_KEY_SET_A | T_KEY_HOLD)

#define K_REPEAT                (T_KEY_REPEAT | T_KEY_SHORT)
#define KU_REPEAT               (T_KEY_REPEAT | T_KEY_SHORT_UP)
#define KL_REPEAT               (T_KEY_REPEAT | T_KEY_LONG)
#define KLU_REPEAT              (T_KEY_REPEAT | T_KEY_LONG_UP)
#define KH_REPEAT               (T_KEY_REPEAT | T_KEY_HOLD)

#define K_POWER                 (T_KEY_POWER | T_KEY_SHORT)
#define KU_POWER                (T_KEY_POWER | T_KEY_SHORT_UP)
#define KL_POWER                (T_KEY_POWER | T_KEY_LONG)
#define KLU_POWER               (T_KEY_POWER | T_KEY_LONG_UP)
#define KH_POWER                (T_KEY_POWER | T_KEY_HOLD)

#define K_TIME_DOWN             (T_KEY_TIME_DOWN | T_KEY_SHORT)
#define KU_TIME_DOWN            (T_KEY_TIME_DOWN | T_KEY_SHORT_UP)
#define KL_TIME_DOWN            (T_KEY_TIME_DOWN | T_KEY_LONG)
#define KLU_TIME_DOWN           (T_KEY_TIME_DOWN | T_KEY_LONG_UP)
#define KH_TIME_DOWN            (T_KEY_TIME_DOWN | T_KEY_HOLD)

#define K_TIME_UP               (T_KEY_TIME_UP | T_KEY_SHORT)
#define KU_TIME_UP              (T_KEY_TIME_UP | T_KEY_SHORT_UP)
#define KL_TIME_UP              (T_KEY_TIME_UP | T_KEY_LONG)
#define KLU_TIME_UP             (T_KEY_TIME_UP | T_KEY_LONG_UP)
#define KH_TIME_UP              (T_KEY_TIME_UP | T_KEY_HOLD)

#define K_USBVOL_DOWN           (T_KEY_USBVOL_DOWN | T_KEY_SHORT)
#define KU_USBVOL_DOWN          (T_KEY_USBVOL_DOWN | T_KEY_SHORT_UP)
#define KL_USBVOL_DOWN          (T_KEY_USBVOL_DOWN | T_KEY_LONG)
#define KLU_USBVOL_DOWN         (T_KEY_USBVOL_DOWN | T_KEY_LONG_UP)
#define KH_USBVOL_DOWN          (T_KEY_USBVOL_DOWN | T_KEY_HOLD)

#define K_USBVOL_UP             (T_KEY_USBVOL_UP | T_KEY_SHORT)
#define KU_USBVOL_UP            (T_KEY_USBVOL_UP | T_KEY_SHORT_UP)
#define KL_USBVOL_UP            (T_KEY_USBVOL_UP | T_KEY_LONG)
#define KLU_USBVOL_UP           (T_KEY_USBVOL_UP | T_KEY_LONG_UP)
#define KH_USBVOL_UP            (T_KEY_USBVOL_UP | T_KEY_HOLD)


/*****************************************************************************
 * Module    : 函数变量声明配置
 *****************************************************************************/
#if SD_DECT_REUSE_ADKEY
extern volatile u8 sd_online_flag;
//使用ADKEY检测SD卡时，在配置头文件里面不用自定义以下两个宏，会在此定义
#define SD_DETECT_INIT()        sd_online_flag = 0;
#define IS_SD_IN()              sd_online_flag
#endif

#if AUX_DECT_REUSE_ADKEY
extern volatile u8 aux_online_flag;
//使用ADKEY检测LINE IN时，在配置头文件里面不用自定义以下两个宏，会在此定义
#define LINEIN_DETECT_INIT()    aux_online_flag = 0;
#define IS_LINEIN_IN()          aux_online_flag
#endif

#if LED_REUSE_ADKEY
#define TIMER0_EN()             TMR0CON |= 0x10; TMR0CNT = 0;
#define TIMER0_DIS()            TMR0CON &= 0x0f;
#endif


void scan_init(void);
__near_func void adc_chk(void);
__near_func void key_scan(void);
__near_func u8 get_bat_val(void);
__near_func u8 key_deal(u8 key_val);

#endif
