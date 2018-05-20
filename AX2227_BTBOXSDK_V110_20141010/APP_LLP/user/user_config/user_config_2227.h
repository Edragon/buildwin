/*****************************************************************************
 * Module    : User
 * File      : user_config_2227.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �û� USER_2227 �Զ����������
 *****************************************************************************/
#ifndef USER_CONFIG_2227_H
#define USER_CONFIG_2227_H


/*****************************************************************************
 * Module    : ���˵��������������
 *****************************************************************************/
#define TASK_MUSIC_EN                   1   //�Ƿ��MUSIC����
#define TASK_AUX_EN                     1   //�Ƿ��LINEIN����
#define TASK_FM_EN                      1   //�Ƿ��FM����
#define TASK_USBDEV_EN                  1   //�Ƿ��USB�豸����
#define TASK_POWEROFF_EN                1   //�Ƿ�򿪹ػ�����
#define TASK_BT_EN                      1   //�Ƿ����������
#define TASK_LOUDSPEAKER_EN             1   //�Ƿ��������ģ��
#define TASK_MIC_EN                     1   //�Ƿ��MIC¼��ģ��
#define TASK_VOICE_EN                   1   //�Ƿ��¼������ģ��
#define TASK_RTC_EN                     1   //�Ƿ��RTC����
#define TASK_KARAOKE_EN                 0   //�Ƿ�򿪿���ok����


/*****************************************************************************
 * Module    : ����ѡ��
 *****************************************************************************/
//�������⣬����Ҫ��ȫ�׵��������ѡ��������ֱ��ƾֲ�����
#define THEME_SELECT                    LEDSEG_THEME_DEFAULT
#define LEDSEG_TYPE_SELECT              LEDSEG_5C7S_MHZ     //ledseg������ѡ��


/*****************************************************************************
 * Module    : ϵͳ��ʾ���������
 *****************************************************************************/
#define POWER_ON_VOICE                  1   //�Ƿ���Ҫ������ʾ��


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
#define FM_TYPE                         FM_QN8035   //FM����ѡ��
#define SEEK_DISPLAY_CHANNEL_NUM        1           //��̨��������ʾ�ѵ���Ƶ��
#define FM_RECORD_EN                    1           //�Ƿ���ҪFM¼��


/*****************************************************************************
 * Module    : �����������������
 *****************************************************************************/
#define MIX_MODE_DECAY_NOISE            0   //����������


/*****************************************************************************
 * Module    : AUX¼�������������
 *****************************************************************************/
#define AUX_RECORD_EN                   1   //AUX�Ƿ���Ҫ¼��
#define KU_PLAY_LINEIN_MUTE_EN          0   //LINEINģʽ���Ƿ�ʹ�ö̰����ż�MUTE����(����PALY �� PLAY_S10)


/*****************************************************************************
 * Module    : USB�豸ģ���������
 *****************************************************************************/
//USB�豸ö������
#define USBDEV_ENUM_TYPE                (USB_DEV_MASS | USB_DEV_SPEAKER | USB_DEV_MIC | USB_DEV_HID)


/*****************************************************************************
 * Module    : RTC�������
 *****************************************************************************/
#define ALARM_EN                        1


/*****************************************************************************
 * Module    : �����������
 *****************************************************************************/
#define BT_TYPE                         BT6639
#define BT_CALL_PRIVATE                 0   //ǿ��˽�ܽ���
#define BT_RING_NUMBER                  1   //�Ƿ���Ҫ���籨��
#define KL_PLAY_DISCONNECT              1   //�Ƿ�ʹ�ó��� PLAY ���Ͽ�����(������ PLAY_S10 ��)
#define BT_NAME_WITH_ADDR               1   //���������Ƿ���ʾ3λ��ַ(�����ã����磺BW-BTBOX-***)
#define NUMBER_KEY_TO_CALL              1   //�Ƿ�ʹ�����ְ������Ź���
#define BT_DISP_NUM_TIME                1   //����ͨ��ʱ�Ƿ���Ҫ��ʾ��ȥ������ʱ��


/*****************************************************************************
 * Module    :Ƶ����ʾ����
 *****************************************************************************/
#define AUX_FREQ_SHOW                   0
#define MUSIC_FREQ_SHOW                 0
#define USBAUDIO_FREQ_SHOW              0
#define BT_FREQ_SHOW                    0
#define FM_FREQ_SHOW                    0


/*****************************************************************************
 * Module    : ϵͳϸ������
 *****************************************************************************/
#define USE_P35_USBPOW_CTL              0   //P35�ŵ�ʹ��ѡ������MUTE��ʱΪ0����������USB����ʱѡ1�������ô�ʱΪ2
#define LED_GREEN_BLINK                 1   //�Ƿ�ʹ���̵���˸
#define IIC_REUSE_SD                    1   //IIC�ӿ��Ƿ���Ҫ��SD����P20 P27
#define SDCMD_REUSE_EARDET              1   //SD CMD���ö������
#define HARD_IR_EN                      1   //����ң��(NECӲ��)
#define IR_HARD_POWERDOWN_EN            1   //����POWER��Ӳ�ػ�ʹ��
#define RECORD_EN                       1   //¼�������ܿ��ء�Ϊ0ʱ�ر����е�¼������
#define POWER_OFF_VPG33                 0   //�ػ�ʱ�Ƿ���Ҫ�ر�VPG33��USER_2227ʹ��VPG33��SPI�������ȹ��磬�رպ�ɴﵽ��͹��ģ������ܻ���
#define K_HSF_CHANGE_MODE               1   //�Ƿ�ʹ�� HSF ���л�ģʽ


/*****************************************************************************
 * Module    :AUXͨ������
 *****************************************************************************/
//����MIC��ͨ��
#define MIC_CHANNEL_SEL                 MCHANNEL_3

//����FM��ͨ��
#define FM_CHANNEL_SEL                  CHANNEL_00

//ѡ��LINEIN��ͨ��
#define AUX_CHANNEL_SEL                 CHANNEL_72


/*****************************************************************************
 * Module    : ����������IO����
 *****************************************************************************/
//ADKey��ʹ�ã�0Ϊʹ��IOKey��1Ϊ1��ADKey��2Ϊ2��ADKey
#define USE_ADKEY                       1

//sd_portֵ:0: P20 P21 P27,    1: P20 P21 P27, clk���ü��,    2: P30 P31 P32,    3: P30 P31 P32, clk���ü��
#define SD_PORT_SEL                     1

//�Ƿ�ʹ��P01�����ڴ�ӡ����
#define DEBUG_UART_SEL                  1

//ADKEY��ֵ��
#define KEY1_NUM                        5
#define KEY1_ADC                        0x3F, 0x8C, 0xAC, 0xCA, 0xEA, 0xFF
#define KEY1_VAL                        T_KEY_HSF, T_KEY_PREV, T_KEY_NEXT, T_KEY_VOL_DOWN, T_KEY_VOL_UP, NO_KEY

//�Ƿ�Ϊ��������������ֹ����
#define IS_HSF_KEY(k)                   (k == KU_HSF)

//���ù��ܼ�
#define T_KEY_TIME_DOWN                 T_KEY_NEXT
#define T_KEY_TIME_UP                   T_KEY_PREV


/*****************************************************************************
 * Module    : IIC���������
 *****************************************************************************/
#define IIC_IO_SET
#define IIC_DATA                        7       //P27
#define IIC_CLK                         0       //P20
#define IIC_DATA_CLK_OUT()              P2PU1 &= ~BIT(6); P2PU0 &= ~BIT(0);  P2DIR &= ~(BIT(IIC_DATA) | BIT(IIC_CLK)); delay(10)
#define IIC_DATA_IN()                   P2DIR |= BIT(IIC_DATA); P2PU1 |= BIT(6)
#define IIC_DATA_OUT()                  P2PU1 |= BIT(6); P2DIR &= ~BIT(IIC_DATA)
#define IIC_DATA_CLK_HIGH()             P2 |= BIT(IIC_DATA) | BIT(IIC_CLK)
#define IIC_DATA_HIGH()                 P2 |= BIT(IIC_DATA)
#define IIC_DATA_LOW()                  P2 &= ~BIT(IIC_DATA)
#define IIC_CLK_HIGH()                  P2 |= BIT(IIC_CLK)
#define IIC_CLK_LOW()                   P2 &= ~BIT(IIC_CLK)
#define IIC_DATA_IS_HIGH()              (P2 & BIT(IIC_DATA))


/*****************************************************************************
 * Module    :LED IO����
 *****************************************************************************/
//��ɫLED
#define LED_GREEN                       5
#define LED_GREEN_INIT()                P1DIR &= ~BIT(LED_GREEN); P1 &= ~BIT(LED_GREEN)
#define LED_GREEN_ON()                  P1 |= BIT(LED_GREEN)
#define LED_GREEN_OFF()                 P1 &= ~BIT(LED_GREEN)
#define LED_GREEN_TOG()                 P1 ^= BIT(LED_GREEN)
#define LED_GREEN_IS_ON()               (P1 & BIT(LED_GREEN))


/*****************************************************************************
 * Module    : ��������IO�������
 *****************************************************************************/
//MUTE���ſ���
#define MUSIC_MUTE()                    P3DIR &= ~BIT(5); P3 |= BIT(5)
#define MUSIC_UNMUTE()                  P3DIR &= ~BIT(5); P3 &= ~BIT(5)


/*****************************************************************************
 * Module    : ���߼�����IO�������
 *****************************************************************************/
/*****************************************************************************
 * IO_SLEEP()��IO_WAKEUP()�������ǹػ�ʱ��������״̬
 * 1������ʱ������������Ϊ�������������³��⣺
 *    P35ΪMUTE�ţ���MUTE����������ߣ�
 *    P34Ϊ���ѽţ�����Ϊ���룻
 *    P06ΪADKEY���ⲿ����������Ϊ���룻
 *    P07Ϊ�������ţ������������������ܺ��⻽�ѣ�
 *    P14/P16/P17Ϊ����RST/RX/TX������������Դʱ����ԭ״̬��������Ϊ��������
 * 2������ʱ��������Ҫ������IO�ָ�Ϊ�ػ�ǰ״̬������ֻ���ȴ�SPI��
 *    P33ΪSPI��CS���ָ�Ϊ�����
 *    P04/P05ΪSPI��DAT/CLK���ָ�Ϊ�����
 *    ����IO��Ҫ��user_io_init�ָ���
*****************************************************************************/
#if POWER_OFF_VPG33
#define IO_SLEEP_P1()                   {P1DIR = 0xff;              /* �ر�������Դʱ��RST/RX/TX��Ϊ��������������IO©�� */\
                                        P1PD0 = 0xff;}
#else
#define IO_SLEEP_P1()                   {P1DIR |= 0x2f;             /* ����P14/P16/P17 */\
                                        P1PD0 = 0x2f;}
#endif
#define IO_SLEEP_P023()                 {PIE0 = 0xff;               /* ����IO��Ϊ�������� */\
                                        PIE1 = 0xff;\
                                        P0DIR = 0xff;\
                                        P2DIR = 0xff;\
                                        P3DIR = ~BIT(5);            /* ����P35 */\
                                        P1DRV1 = 0;                 /* �ر�P15���� */\
                                        P0PU0 = 0x80;               /* ����P07 */\
                                        P1PU0 = 0;\
                                        P2PU0 = 0;\
                                        P2PU1 = 0;\
                                        P3PU0 = 0;\
                                        P0PD0 = 0x3f;               /* ����P06/P07 */\
                                        P2PD0 = 0x55;\
                                        P2PD1 = 0x55;\
                                        P3PD0 = 0xc5;               /* ����P35��P34 */\
                                        P3PUD1 = 0x07;}

#define IO_SLEEP()                      IO_SLEEP_P023();IO_SLEEP_P1()

#define IO_WAKEUP()                     {P3DIR &= ~BIT(3);          /* �ָ�SPI��CS */\
                                        P0DIR &= ~(BIT(4)|BIT(5));  /* �ָ�SPI��DAT/CLK */\
                                        P0PD0 = 0x00;               /* �ص��������� */\
                                        P1PD0 = 0x00;\
                                        P2PD0 = 0x00;\
                                        P2PD1 = 0x00;\
                                        P3PD0 = 0x00;\
                                        P3PUD1 = 0x00;}

#define IS_KEY_WAKEUP()                 !(P0 & BIT(6))

#endif
