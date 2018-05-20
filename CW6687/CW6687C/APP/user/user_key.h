/*****************************************************************************
 * Module    : user
 * File      : user_key.h
 * Author    : Hanny
 * Function  : 按键操作的相关函数定义
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
    #define ADC_PU10K()         P3PU |= BIT(3)
    #define ADC_NOPU()          P3PU &= ~BIT(3)
    #define ADC_END()           P3DIR &= ~BIT(3)
    #define ADKEY_DOWN()        !(P3 & BIT(3)) //adkey是否按下
#elif (ADKEY_PORT==ADKEY_P21)
    #define ADC_INIT()          P2DIR |= BIT(1)
    #define ADC_PU10K()         P2PU |= BIT(1)
    #define ADC_NOPU()          P2PU &= ~BIT(1)
    #define ADC_END()           P2DIR &= ~BIT(1)
    #define ADKEY_DOWN()        !(P2 & BIT(1)) //adkey是否按下
#elif (ADKEY_PORT==ADKEY_P14)
    #define ADC_INIT()          P1DIR |= BIT(4)
    #define ADC_PU10K()         P1PU |= BIT(4)
    #define ADC_NOPU()          P1PU &= ~BIT(4)
    #define ADC_END()           P1DIR &= ~BIT(4)
    #define ADKEY_DOWN()        !(P1 & BIT(4)) //adkey是否按下
#elif (ADKEY_PORT==ADKEY_P22)
    #define ADC_INIT()          P2DIR |= BIT(2)
    #define ADC_PU10K()         P2PU |= BIT(2)
    #define ADC_NOPU()          P2PU &= ~BIT(2)
    #define ADC_END()           P2DIR &= ~BIT(2)
    #define ADKEY_DOWN()        !(P2 & BIT(2)) //adkey是否按下
#elif (ADKEY_PORT==ADKEY_P30)
    #define ADC_INIT()          P3DIR |= BIT(0)
    #define ADC_PU10K()         P3PU |= BIT(0)
    #define ADC_NOPU()          P3PU &= ~BIT(0)
    #define ADC_END()           P3DIR &= ~BIT(0)
    #define ADKEY_DOWN()        !(P3 & BIT(0)) //adkey是否按下
#elif (ADKEY_PORT==ADKEY_P13)
    #define ADC_INIT()          P1DIR |= BIT(3)
    #define ADC_PU10K()         P1PU |= BIT(3)
    #define ADC_NOPU()          P1PU &= ~BIT(3)
    #define ADC_END()           P1DIR &= ~BIT(3)
    #define ADKEY_DOWN()        !(P1 & BIT(3)) //adkey是否按下
#else
    #error "请选择一组ADKEY_PORT"
#endif

#if (ADKEY2_PORT==ADKEY_P33)
    #define ADC2_INIT()         P3DIR |= BIT(3)
    #define ADC2_PU10K()        P3PU |= BIT(3)
    #define ADC2_NOPU()         P3PU &= ~BIT(3)
#elif (ADKEY2_PORT==ADKEY_P21)
    #define ADC2_INIT()         P2DIR |= BIT(1)
    #define ADC2_PU10K()        P2PU |= BIT(1)
    #define ADC2_NOPU()         P2PU &= ~BIT(1)
#elif (ADKEY2_PORT==ADKEY_P14)
    #define ADC2_INIT()         P1DIR |= BIT(4)
    #define ADC2_PU10K()        P1PU |= BIT(4)
    #define ADC2_NOPU()         P1PU &= ~BIT(4)
#elif (ADKEY2_PORT==ADKEY_P22)
    #define ADC2_INIT()         P2DIR |= BIT(2)
    #define ADC2_PU10K()        P2PU |= BIT(2)
    #define ADC2_NOPU()         P2PU &= ~BIT(2)
#elif (ADKEY2_PORT==ADKEY_P30)
    #define ADC2_INIT()         P3DIR |= BIT(0)
    #define ADC2_PU10K()        P3PU |= BIT(0)
    #define ADC2_NOPU()         P3PU &= ~BIT(0)
#elif (ADKEY2_PORT==ADKEY_P13)
    #define ADC2_INIT()         P1DIR |= BIT(3)
    #define ADC2_PU10K()        P1PU |= BIT(3)
    #define ADC2_NOPU()         P1PU &= ~BIT(3)
#else
    #error "请选择一组ADKEY2_PORT"
#endif

#if ADKEY_INT_PU10K
    #define ADKEY_INIT()        ADC_INIT();ADC_PU10K()
    #define ADKEY_END()         ADC_END()
#else
    #define ADKEY_INIT()        ADC_INIT();ADC_NOPU()
    #define ADKEY_END()         ADC_END()
#endif

#if ADKEY2_INT_PU10K
    #define ADKEY2_INIT()        ADC2_INIT();ADC2_PU10K()
#else
    #define ADKEY2_INIT()        ADC2_INIT();ADC2_NOPU()
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

//系统消息
#define Q_NOMSG                 0                   //无消息

#define QSYSTEM_1S              0x7e
#define Q_BT_CONN_REQ           0x7d                //蓝牙连接
#define Q_BT_DISCONN_DONE       0x7c                //蓝牙断开
#define Q_VOL_DOWN              0x7b                //系统音量减
#define Q_VOL_UP                0x7a                //系统音量加
#define Q_HSHF_VOL_CHANGE       0x79                //通话音量变化
#define Q_BT_CONN_DONE          0x76                //连接成功
#define Q_BT_PLAY               0x75                //蓝牙开始播放
#define Q_BT_STOP               0x74                //蓝牙停止播放
#define Q_BT_INCOMING           0x73                //蓝牙来电
#define Q_BT_CALL               0x72                //蓝牙通话
#define Q_BT_CONN_LOST          0x71                //蓝牙远距离断开连接
#define QSYSTEM_10S             0x70
#define Q_SPP_MSC_GET_LFN       0x6f                //SPP 获取文件名
#define Q_SPP_MSC_GET_TOTAL     0x6e                //SPP 获取歌曲总数
#define Q_SPP_MSC_GET_NUM       0x6d                //SPP 获取当前曲目

//功能键
#define T_KEY_PREV              T_KEY_1
#define T_KEY_PLAY              T_KEY_2
#define T_KEY_NEXT              T_KEY_3
#define T_KEY_VOL_DOWN          T_KEY_4
#define T_KEY_VOL_UP            T_KEY_5
#define T_KEY_HSF               T_KEY_6
#define T_KEY_MENU              T_KEY_7
#define T_KEY_SCAN              T_KEY_8
#define T_KEY_POWER             T_KEY_9

#define T_KEY_FOLLOW            T_KEY_10
#define T_KEY_COMPARE           T_KEY_11
#define T_KEY_REVIEW            T_KEY_12
#define T_KEY_SPEED             T_KEY_13
#define T_KEY_MODE              T_KEY_14
#define T_KEY_SET_A             T_KEY_15
#define T_KEY_REPEAT            T_KEY_16

#define T_KEY_PREV_VOL_DOWN     T_KEY_17
#define T_KEY_PLAY_POWER        T_KEY_18
#define T_KEY_NEXT_VOL_UP       T_KEY_19


//复用功能键
#define T_KEY_OK                T_KEY_PLAY
#define T_KEY_BACK              T_KEY_HSF
#define T_KEY_TIME_DOWN         T_KEY_NEXT
#define T_KEY_TIME_UP           T_KEY_PREV



/*****************************************************************************
 * Module    : 按键长短按配置
 *****************************************************************************/
//按键详细定义 -> K_: Key Short Down;   KU_: Key Short Up;   KL_: Key Long Down;   KLU_: Key Long Up;   KH_: Key Hold;

#define K_LOOP                      (T_KEY_LOOP | T_KEY_SHORT)
#define KU_LOOP                     (T_KEY_LOOP | T_KEY_SHORT_UP)
#define KL_LOOP                     (T_KEY_LOOP | T_KEY_LONG)
#define KLU_LOOP                    (T_KEY_LOOP | T_KEY_LONG_UP)
#define KH_LOOP                     (T_KEY_LOOP | T_KEY_HOLD)

#define K_EQ                        (T_KEY_EQ | T_KEY_SHORT)
#define KU_EQ                       (T_KEY_EQ | T_KEY_SHORT_UP)
#define KL_EQ                       (T_KEY_EQ | T_KEY_LONG)
#define KLU_EQ                      (T_KEY_EQ | T_KEY_LONG_UP)
#define KH_EQ                       (T_KEY_EQ | T_KEY_HOLD)

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

#define K_MENU                  (T_KEY_MENU | T_KEY_SHORT)
#define KU_MENU                 (T_KEY_MENU | T_KEY_SHORT_UP)
#define KL_MENU                 (T_KEY_MENU | T_KEY_LONG)
#define KLU_MENU                (T_KEY_MENU | T_KEY_LONG_UP)
#define KH_MENU                 (T_KEY_MENU | T_KEY_HOLD)

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

#define K_FOLLOW                (T_KEY_FOLLOW | T_KEY_SHORT)
#define KU_FOLLOW               (T_KEY_FOLLOW | T_KEY_SHORT_UP)
#define KL_FOLLOW               (T_KEY_FOLLOW | T_KEY_LONG)
#define KLU_FOLLOW              (T_KEY_FOLLOW | T_KEY_LONG_UP)
#define KH_FOLLOW               (T_KEY_FOLLOW | T_KEY_HOLD)

#define K_COMPARE               (T_KEY_COMPARE | T_KEY_SHORT)
#define KU_COMPARE              (T_KEY_COMPARE | T_KEY_SHORT_UP)
#define KL_COMPARE              (T_KEY_COMPARE | T_KEY_LONG)
#define KLU_COMPARE             (T_KEY_COMPARE | T_KEY_LONG_UP)
#define KH_COMPARE              (T_KEY_COMPARE | T_KEY_HOLD)

#define K_REVIEW                (T_KEY_REVIEW | T_KEY_SHORT)
#define KU_REVIEW               (T_KEY_REVIEW | T_KEY_SHORT_UP)
#define KL_REVIEW               (T_KEY_REVIEW | T_KEY_LONG)
#define KLU_REVIEW              (T_KEY_REVIEW | T_KEY_LONG_UP)
#define KH_REVIEW               (T_KEY_REVIEW | T_KEY_HOLD)

#define K_POWER                 (T_KEY_POWER | T_KEY_SHORT)
#define KU_POWER                (T_KEY_POWER | T_KEY_SHORT_UP)
#define KL_POWER                (T_KEY_POWER | T_KEY_LONG)
#define KLU_POWER               (T_KEY_POWER | T_KEY_LONG_UP)
#define KH_POWER                (T_KEY_POWER | T_KEY_HOLD)

#define K_OK                    (T_KEY_OK | T_KEY_SHORT)
#define KU_OK                   (T_KEY_OK | T_KEY_SHORT_UP)
#define KL_OK                   (T_KEY_OK | T_KEY_LONG)
#define KLU_OK                  (T_KEY_OK | T_KEY_LONG_UP)
#define KH_OK                   (T_KEY_OK | T_KEY_HOLD)

#define K_BACK                  (T_KEY_BACK | T_KEY_SHORT)
#define KU_BACK                 (T_KEY_BACK | T_KEY_SHORT_UP)
#define KL_BACK                 (T_KEY_BACK | T_KEY_LONG)
#define KLU_BACK                (T_KEY_BACK | T_KEY_LONG_UP)
#define KH_BACK                 (T_KEY_BACK | T_KEY_HOLD)

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

#define K_TIME_SET              (T_KEY_REPEAT | T_KEY_SHORT)
#define KU_TIME_SET             (T_KEY_REPEAT | T_KEY_SHORT_UP)
#define KL_TIME_SET             (T_KEY_REPEAT | T_KEY_LONG)
#define KLU_TIME_SET            (T_KEY_REPEAT | T_KEY_LONG_UP)
#define KH_TIME_SET             (T_KEY_REPEAT | T_KEY_HOLD)

#define K_FNAV_DEL              (T_KEY_FNAV_DEL | T_KEY_SHORT)
#define KU_FNAV_DEL             (T_KEY_FNAV_DEL | T_KEY_SHORT_UP)
#define KL_FNAV_DEL             (T_KEY_FNAV_DEL | T_KEY_LONG)
#define KLU_FNAV_DEL            (T_KEY_FNAV_DEL | T_KEY_LONG_UP)
#define KH_FNAV_DEL             (T_KEY_FNAV_DEL | T_KEY_HOLD)

#define K_PREV_VOL_DOWN             (T_KEY_PREV_VOL_DOWN | T_KEY_SHORT)
#define KU_PREV_VOL_DOWN            (T_KEY_PREV_VOL_DOWN | T_KEY_SHORT_UP)
#define KL_PREV_VOL_DOWN            (T_KEY_PREV_VOL_DOWN | T_KEY_LONG)
#define KLU_PREV_VOL_DOWN           (T_KEY_PREV_VOL_DOWN | T_KEY_LONG_UP)
#define KH_PREV_VOL_DOWN            (T_KEY_PREV_VOL_DOWN | T_KEY_HOLD)

#define K_PLAY_POWER                (T_KEY_PLAY_POWER | T_KEY_SHORT)
#define KU_PLAY_POWER               (T_KEY_PLAY_POWER | T_KEY_SHORT_UP)
#define KL_PLAY_POWER               (T_KEY_PLAY_POWER | T_KEY_LONG)
#define KLU_PLAY_POWER              (T_KEY_PLAY_POWER | T_KEY_LONG_UP)
#define KH_PLAY_POWER               (T_KEY_PLAY_POWER | T_KEY_HOLD)

#define K_NEXT_VOL_UP               (T_KEY_NEXT_VOL_UP | T_KEY_SHORT)
#define KU_NEXT_VOL_UP              (T_KEY_NEXT_VOL_UP | T_KEY_SHORT_UP)
#define KL_NEXT_VOL_UP              (T_KEY_NEXT_VOL_UP | T_KEY_LONG)
#define KLU_NEXT_VOL_UP             (T_KEY_NEXT_VOL_UP | T_KEY_LONG_UP)
#define KH_NEXT_VOL_UP              (T_KEY_NEXT_VOL_UP | T_KEY_HOLD)
/*****************************************************************************
 * Module    : 函数变量声明配置
 *****************************************************************************/
#if SD_DECT_REUSE_ADKEY
extern volatile u8 sd_online_flag;
//使用ADKEY检测SD卡时，在配置头文件里面不用自定义以下两个宏，会在此定义
#define SD_DETECT_INIT()        sd_online_flag = 0
#define IS_SD_IN()              sd_online_flag
#endif

#if AUX_DECT_REUSE_ADKEY
extern volatile u8 aux_online_flag;
//使用ADKEY检测LINE IN时，在配置头文件里面不用自定义以下两个宏，会在此定义
#define LINEIN_DETECT_INIT()    aux_online_flag = 0
#define IS_LINEIN_IN()          aux_online_flag
#endif

#if EAR_ADKEY_REUSE
extern volatile u8 ear_online_flag;
//使用ADKEY检测EAR IN时，在配置头文件里面不用自定义以下两个宏，会在此定义
#define EAR_DETECT_INIT()       ear_online_flag = 0
#define EAR_IS_IN()             ear_online_flag
#endif

#if LED_REUSE_ADKEY
#define TIMER0_EN()             TMR0CON |= 0x10; TMR0CNT = 0;
#define TIMER0_DIS()            TMR0CON &= 0x0f;
#endif

void scan_init(void);
__near_func void adc_chk(void);
__near_func void key_scan(void);
__near_func u8 get_bat_val(void);
__near_func u8 get_scan_adc_val(void);
__near_func void task_battery_check(void);
__near_func void charge_detect(void);
__near_func u8 key_filter(u8 group, u8 adc_val);        //4组滤波器
__near_func void key_msg_flags(u8 key_val);
void charge_init(void);
void buck_mode_select(void);

#endif
