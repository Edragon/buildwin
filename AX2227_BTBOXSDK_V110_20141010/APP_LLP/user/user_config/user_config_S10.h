/*****************************************************************************
 * Module    : User
 * File      : user_config_s10.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �û� USER_S10 �Զ����������
 *****************************************************************************/
#ifndef USER_CONFIG_S10_H
#define USER_CONFIG_S10_H


/*****************************************************************************
 * Module    : ���˵��������������
 *****************************************************************************/
#define TASK_MUSIC_EN                   1   //�Ƿ��MUSIC����
#define TASK_AUX_EN                     1   //�Ƿ��LINEIN����
#define TASK_FM_EN                      1   //�Ƿ��FM����
#define TASK_USBDEV_EN                  0   //�Ƿ��USB�豸����
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
#define MUSIC_MODE_AUTO                 1   //�����豸���Զ��л������ֲ���
#define BREAK_POINT_EN                  1   //�ϵ����


/*****************************************************************************
 * Module    : FMģ���������
 *****************************************************************************/
#define FM_TYPE                         FM_QN8065   //FM����ѡ��
#define KU_PLAY_TO_SEEKCHANNEL          1           //�Ƿ�ʹ�ö̰����ż�����̨( ����PALY �� PLAY_S10)


/*****************************************************************************
 * Module    : USB�豸ģ���������
 *****************************************************************************/
//USB�豸ö������
#define USBDEV_ENUM_TYPE                (USB_DEV_MASS | USB_DEV_SPEAKER | USB_DEV_MIC | USB_DEV_HID)


/*****************************************************************************
 * Module    : �����������
 *****************************************************************************/
#define BT_TYPE                         BT6639
#define BT_CALL_PRIVATE                 0   //ǿ��˽�ܽ���
#define BT_MODE_MUSIC                   1   //�Ƿ���Ҫ����ģʽ��ʾ��
#define BT_RING_NUMBER                  1   //�Ƿ���Ҫ���籨��
#define KU_PLAY_ANSWER_AND_HANGUP       1   //�Ƿ�ʹ�ö̰����ż��������͹Ҷϵ绰(����PALY �� PLAY_S10)
#define BT_VOLUME_MAX                   1   //����ģʽ���Ƿ�ҪĬ���������
#define BT_PHONE_SWITCH		            0   //ͨ��ʱ�����ڲ����绰ʱ�Ƿ���Ҫ�ֻ������������л�
#define BT_NAME_WITH_ADDR               1   //���������Ƿ���ʾ3λ��ַ(�����ã����磺BW-BTBOX-***)


/*****************************************************************************
 * Module    : ϵͳϸ������
 *****************************************************************************/
#define MP3BT_DECT_EN                   1   //�Ƿ�ʹ�ò������ؿ������MP3��BT�Ĺ���
#define USE_P35_USBPOW_CTL              0   //P35�ŵ�ʹ��ѡ������MUTE��ʱΪ0����������USB����ʱѡ1�������ô�ʱΪ2
#define IIC_REUSE_SD                    1   //IIC�ӿ��Ƿ���Ҫ��SD����P20 P27
#define LED_BLUE_BLINK                  1   //�Ƿ�ʹ��������˸
#define LED_GREEN_BLINK                 1   //�Ƿ�ʹ���̵���˸
#define LOWPOWER_MUSIC		            1   //�Ƿ���Ҫ�͵���ʾ��


/*****************************************************************************
 * Module    : �豸�������
 *****************************************************************************/
//ADKey��ʹ�ã�0Ϊʹ��IOKey��1Ϊ1��ADKey��2Ϊ2��ADKey
#define USE_ADKEY                       0

//sd_portֵ:0: P20 P21 P27,    1: P20 P21 P27, clk���ü��,    2: P30 P31 P32,    3: P30 P31 P32, clk���ü��
#define SD_PORT_SEL                     0

//�Ƿ�ʹ��P01�����ڴ�ӡ����
#define DEBUG_UART_SEL                  1


/*****************************************************************************
 * Module    :IO��������
 *****************************************************************************/
#define K_PLAY_INIT()                   P0DIR |= BIT(6); P0PU0 |= BIT(6)
#define IS_K_PLAY()                     ((P0 & BIT(6)) == 0)
#define K_NEXT_INIT()                   {P3DIR |= (BIT(0)|BIT(2));\
                                        P3PU0 |= (BIT(0)|BIT(4));\
                                        P3PU0 &= ~(BIT(1)|BIT(5));}
#define IS_K_NEXT()                     ((P3 & BIT(0)) == 0)
#define K_PREV_INIT()
#define IS_K_PREV()                     ((P3 & BIT(2)) == 0)

//�Ƿ�Ϊ��������������ֹ����
#define IS_HSF_KEY(k)                   (k == KU_PLAY_S10)


/*****************************************************************************
 * Module    :AUXͨ������
 *****************************************************************************/
//����MIC��ͨ��
#define MIC_CHANNEL_SEL                 MCHANNEL_3

//����FM��ͨ��
#define FM_CHANNEL_SEL                  CHANNEL_00

//����AUX��ͨ��
#define AUX_CHANNEL_SEL                 CHANNEL_22


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
#define LED_BLUE                        5
#define LED_BLUE_INIT()                 P2DIR &= ~BIT(LED_BLUE); P2 &= ~BIT(LED_BLUE)
#define LED_BLUE_ON()                   P2 |= BIT(LED_BLUE)
#define LED_BLUE_OFF()                  P2 &= ~BIT(LED_BLUE)
#define LED_BLUE_TOG()                  P2 ^= BIT(LED_BLUE)
#define LED_BLUE_IS_ON()                (P2 & BIT(LED_BLUE))
//��ɫLED
#define LED_GREEN                       5
#define LED_GREEN_INIT()                P1DIR |= BIT(LED_GREEN); P1DRV1 &= ~BIT(7)
#define LED_GREEN_ON()                  P1DRV1 |= BIT(7)
#define LED_GREEN_OFF()                 P1DRV1 &= ~BIT(7)
#define LED_GREEN_TOG()                 P1DRV1 ^= BIT(7)
#define LED_GREEN_IS_ON()               (P1DRV1 & BIT(7))


/*****************************************************************************
 * Module    : �������IO����
 *****************************************************************************/
//������Դ����
#define BTLDO_POWIO_SET
#define BTLDO_DIR_INIT()                PWRCON1 &= ~BIT(4); P3DIR &= ~BIT(6); P3 &= ~BIT(6)
#define BTLDO_EN()                      P3 |= BIT(6)
#define BTLDO_DIS()                     P3 &= ~BIT(6)


/*****************************************************************************
 * Module    : ��������IO�������
 *****************************************************************************/
//�ϵ�ʱBT/MP3���
#define MP3BT_DECT_INIT()               {P2DIR |= BIT(4); P2PU1 |= BIT(0); P2PU1 &= ~BIT(1);}
                                        asm("nop");asm("nop");asm("nop");asm("nop");\
                                        asm("nop");asm("nop");asm("nop");asm("nop");
#define MP3BT_DECT_IS_BT()              (P2 & BIT(4))

//MUTE���ſ���
#define MUSIC_MUTE()                    P3DIR &= ~BIT(5); P3 |= BIT(5)
#define MUSIC_UNMUTE()                  P3DIR &= ~BIT(5); P3 &= ~BIT(5)

//SD��������(���� ADKEY �� SD_CLK ���ʱ����Ҫ����SD_DETECT_INIT)
#define SD_DETECT_INIT()                P2DIR |= BIT(2);P2PU0 |= BIT(4);P2PU0 &= ~BIT(5);
#define IS_SD_IN()                      !(P2 & BIT(2))

/*****************************************************************************
 * Module    : ���߼�����IO�������
 *****************************************************************************/
#define IO_SLEEP()                      {P0 = 0x00;P1 = 0xb0;P2 = 0x00;\
                                        P0DIR = 0xfd;P1DIR = 0xb7;P2DIR = 0xef;P3DIR = 0xbf;}
#define IO_WAKEUP()                     {P0 = 0x7e;P1 = 0xd7;P2 = 0xcb;P3 = 0xef;\
                                        P0DIR = 0xd1;P1DIR = 0xb7;P2DIR = 0x4e;P3DIR = 0x97;}
#define IS_KEY_WAKEUP()                 !(P3 & BIT(2)) || !(P3 & BIT(0))|| !(P0 & BIT(6))

#endif
