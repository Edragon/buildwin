/*****************************************************************************
 * Module    : User
 * File      : user_config_2226_demo.h
 * Author    : jingfa
 * Email     :
 * Function  : �û� USER_2226_DEMO �Զ����������
 *****************************************************************************/
#ifndef USER_CONFIG_2226_DEMO_H
#define USER_CONFIG_2226_DEMO_H


/*****************************************************************************
 * Module    : ϵͳ����
 *****************************************************************************/
#define SYS_PKG                         PKG_SSOP24  //���ط�װѡ��
#define SYS_CRY                         CRY_RC      //ϵͳʱ��Դѡ��
#define MAKECODE_ID                     1           //����ʱʹ��P13
#define SD_UPDATE_ID                    2           //SD����ʹ��P3��


/*****************************************************************************
 * Module    : ���˵��������������
 *****************************************************************************/
#define TASK_MUSIC_EN                   1   //�Ƿ��MUSIC����
#define TASK_AUX_EN                     1   //�Ƿ��LINEIN����
#define TASK_FM_EN                      1   //�Ƿ��FM����
#define TASK_POWEROFF_EN                1   //�Ƿ�򿪹ػ�����
#define TASK_BT_EN                      1   //�Ƿ����������


/*****************************************************************************
 * Module    : ����ѡ��
 *****************************************************************************/
//�������⣬����Ҫ��ȫ�׵��������ѡ��������ֱ��ƾֲ�����
#define THEME_SELECT                    NO_THEME


/*****************************************************************************
 * Module    : Musicģ���������
 *****************************************************************************/
#define AUTO_CHANGE_DEVICE              1   //˫�豸ʱ�Զ��л��豸
#define K_MODE_CHANGE_DEVICE            1   //˫�豸ʱʹ��ģʽ���л��豸
#define MUSIC_MODE_AUTO                 1   //�����豸���Զ��л������ֲ���
#define BREAK_POINT_EN                  1   //�ϵ����


/*****************************************************************************
 * Module    : FMģ���������
 *****************************************************************************/
#define FM_TYPE                         FM_RDA5807  //FM����ѡ��
#define KU_PLAY_TO_SEEKCHANNEL          1           //�Ƿ�ʹ�ö̰����ż�����̨( ����PALY �� PLAY_S10)


/*****************************************************************************
 * Module    : �����������
 *****************************************************************************/
#define BT_TYPE                         BT6639
#define BT_UART_IO                      1   //��������IOѡ��0=P16 P17��1=P36 P37
#define BT_PWK_SEL                      1   //����ʹ��P07����
#define BT_CALL_PRIVATE                 0   //ǿ��˽�ܽ���
#define BT_RING_NUMBER                  1   //�Ƿ���Ҫ���籨��
#define KU_PLAY_ANSWER_AND_HANGUP       1   //�Ƿ�ʹ�ö̰����ż��������͹Ҷϵ绰(����PALY �� PLAY_S10)
#define BT_PHONE_SWITCH		            0   //ͨ��ʱ�����ڲ����绰ʱ�Ƿ���Ҫ�ֻ������������л�
#define BT_NAME_WITH_ADDR               1   //���������Ƿ���ʾ3λ��ַ(�����ã����磺BW-BTBOX-***)


/*****************************************************************************
 * Module    : ϵͳϸ������
 *****************************************************************************/
#define USE_P35_USBPOW_CTL              2   //�Ƿ�ʹ��P35����USB����
#define LED_BLUE_BLINK                  1   //�Ƿ�ʹ��������˸
#define LED_GREEN_BLINK                 1   //�Ƿ�ʹ���̵���˸
#define IIC_REUSE_SD                    1   //IIC�ӿ��Ƿ���Ҫ��SD����P31 P32
#define KL_PLAY_S10_CHANGE_MODE         0   //�Ƿ�ʹ�ó��� PLAY_S10 ���л�ģʽ(������ PLAY ��)
#define POWERON_TO_FM                   1   //�Ƿ���Ҫ����������FM
#define LED_REUSE_ADKEY                 1   //�Ƿ���ADKEY��LED��(��Ҫ����2226 IO �ڲ�����ʱ)
#define AUTO_STANDBY_EN                 0   //��ʱ���޲���(ʱ���ɺ� STANDBY_TIME ����)�Ƿ���Ҫ�Զ��������ߴ���
#define LINEIN_DETECT_EN                1   //�Ƿ���ҪLineIn������
#define SDCMD_REUSE_LINEIN_DET          1   //SDCMD����LineIn������


/*****************************************************************************
 * Module    : ����������IO����
 *****************************************************************************/
//ADKey��ʹ�ã�0Ϊʹ��IOKey��1Ϊ1��ADKey��2Ϊ2��ADKey
#define USE_ADKEY                       1

//ѡ���1��ADKEY��IO
#define ADKEY_PORT                      ADKEY_P13

//sd_portֵ:0: P20 P21 P27,    1: P20 P21 P27, clk���ü��,    2: P30 P31 P32,    3: P30 P31 P32, clk���ü��
#define SD_PORT_SEL                     3

//�Ƿ�ʹ��P01�����ڴ�ӡ����
#define DEBUG_UART_SEL                  1

//ADKEY��ֵ��
#define KEY1_NUM                        4
#define KEY1_ADC                        0x22, 0x36, 0x60, 0x70, 0xFF
#define KEY1_VAL                        T_KEY_NEXT_S10, T_KEY_PLAY_S10, T_KEY_PREV_S10, T_KEY_MODE, NO_KEY

//�Ƿ�Ϊ��������������ֹ����
#define IS_HSF_KEY(k)                   (k == KU_PLAY_S10)


/*****************************************************************************
 * Module    :AUXͨ������
 *****************************************************************************/
//����MIC��ͨ��
#define MIC_CHANNEL_SEL                 MCHANNEL_3

//����FM��ͨ��
#define FM_CHANNEL_SEL                  CHANNEL_11

//����AUX��ͨ��
#define AUX_CHANNEL_SEL                 CHANNEL_11


/*****************************************************************************
 * Module    :IIC IO����
 *****************************************************************************/
#define IIC_IO_SET
#define IIC_DATA                        2       //P32
#define IIC_CLK                         1       //P31
#define IIC_DATA_CLK_OUT()              P3PU0 &= ~(BIT(5)|BIT(4)|BIT(3)|BIT(2)); P3DIR &= ~(BIT(IIC_DATA) | BIT(IIC_CLK)); delay(10)
#define IIC_DATA_IN()                   P3DIR |= BIT(IIC_DATA); P3PU0 |= BIT(4)
#define IIC_DATA_OUT()                  P3PU0 |= BIT(4); P3DIR &= ~BIT(IIC_DATA)
#define IIC_DATA_CLK_HIGH()             P3 |= BIT(IIC_DATA) | BIT(IIC_CLK)
#define IIC_DATA_HIGH()                 P3 |= BIT(IIC_DATA)
#define IIC_DATA_LOW()                  P3 &= ~BIT(IIC_DATA)
#define IIC_CLK_HIGH()                  P3 |= BIT(IIC_CLK)
#define IIC_CLK_LOW()                   P3 &= ~BIT(IIC_CLK)
#define IIC_DATA_IS_HIGH()              (P3 & BIT(IIC_DATA))


/*****************************************************************************
 * Module    :LED IO����
 *****************************************************************************/
//��ɫLED
#define LED_BLUE                        3
#define LED_BLUE_INIT()                 P1 &= ~BIT(LED_BLUE)
#define LED_BLUE_ON()                   P1 |= BIT(LED_BLUE)
#define LED_BLUE_OFF()                  P1 &= ~BIT(LED_BLUE)
#define LED_BLUE_TOG()                  P1 ^= BIT(LED_BLUE)
#define LED_BLUE_IS_ON()                (P1 & BIT(LED_BLUE))
//��ɫLED
#define LED_GREEN_INIT()                P0 &= ~BIT(7); P0DIR &= ~BIT(7)
#define LED_GREEN_ON()                  P0 |= BIT(7)
#define LED_GREEN_OFF()                 P0 &= ~BIT(7)
#define LED_GREEN_TOG()                 P0 ^= BIT(7)
#define LED_GREEN_IS_ON()               (P0 & BIT(7))


/*****************************************************************************
 * Module    : �������IO����
 *****************************************************************************/
#define BT_SLEEP_IO_SET
#define BT_IS_SLEEP()                   (P0 & BIT(7))
#define BT_IS_SLEEP_INIT()              IE_EA = 0; P0DIR |= BIT(7);\
                                        asm("nop");asm("nop");asm("nop");asm("nop");\
                                        asm("nop");asm("nop");asm("nop");asm("nop");
#define BT_IS_SLEEP_END()               P0DIR &= ~BIT(7); IE_EA = 1;

#define BT_RESET_IO_SET
#define BT_RST_INIT()                   P2DIR &= ~BIT(0);
#define BT_RST_LOW()                    P2 &= ~BIT(0);
#define BT_RST_HIGH()                   P2 |= BIT(0);


/*****************************************************************************
 * Module    : ��������IO�������
 *****************************************************************************/
//MUTE���ſ���
#define MUSIC_MUTE()                    P2DIR &= ~BIT(1); P2 |= BIT(1)
#define MUSIC_UNMUTE()                  P2DIR &= ~BIT(1); P2 &= ~BIT(1)


/*****************************************************************************
 * Module    : ���߼�����IO�������
 *****************************************************************************/
#define IO_SLEEP()                      {P1DIR &= ~BIT(3); P1 &= ~BIT(3);}
#define IO_WAKEUP()                     {P1DIR |= BIT(3);}
#define IS_KEY_WAKEUP()                 !(P1 & BIT(3))


#endif
