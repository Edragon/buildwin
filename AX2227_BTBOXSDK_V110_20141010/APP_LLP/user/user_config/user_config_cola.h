/*****************************************************************************
 * Module    : User
 * File      : user_config_cola.h
 * Author    : jingfa
 * Email     :
 * Function  : �û� USER_COLA �Զ����������
 *****************************************************************************/
#ifndef USER_CONFIG_COLA_H
#define USER_CONFIG_COLA_H


/*****************************************************************************
 * Module    : ϵͳ����
 *****************************************************************************/
#define SYS_CRY                         CRY_12M  //ϵͳʱ��Դѡ��


/*****************************************************************************
 * Module    : ���˵��������������
 *****************************************************************************/
#define TASK_MUSIC_EN                   1   //�Ƿ��MUSIC����
#define TASK_AUX_EN                     1   //�Ƿ��LINEIN����
#define TASK_FM_EN                      1   //�Ƿ��FM����
#define TASK_USBDEV_EN                  1   //�Ƿ��USB�豸����
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
#define BT_RING_NUMBER                  1   //�Ƿ���Ҫ���籨��
#define KU_PLAY_ANSWER_AND_HANGUP       1   //�Ƿ�ʹ�ö̰����ż��������͹Ҷϵ绰(����PALY �� PLAY_S10)
#define KL_PLAY_HANDUP                  1   //�Ƿ�ʹ�ó���PLAY���Ҷϵ绰
#define BT_NAME_WITH_ADDR               1   //���������Ƿ���ʾ3λ��ַ(�����ã����磺BW-BTBOX-***)


/*****************************************************************************
 * Module    : ϵͳϸ������
 *****************************************************************************/
#define USE_P35_USBPOW_CTL              0   //P35�ŵ�ʹ��ѡ������MUTE��ʱΪ0����������USB����ʱѡ1�������ô�ʱΪ2
#define IIC_REUSE_SD                    1   //IIC�ӿ��Ƿ���Ҫ��SD����P20 P27
#define EAR_DETECT_EN                   1   //�Ƿ�ʹ�ö������
#define LED_BLUE_BLINK                  1   //�Ƿ�ʹ��������˸
#define LED_GREEN_BLINK                 0   //�Ƿ�ʹ���̵���˸


/*****************************************************************************
 * Module    : ����������IO����
 *****************************************************************************/
//ADKey��ʹ�ã�0Ϊʹ��IOKey��1Ϊ1��ADKey��2Ϊ2��ADKey
#define USE_ADKEY                       0

//�Ƿ�ʹ����������IO��������MODE��PLAY��
#define USE_MODE_PLAY_KEY               1

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
//�������
#define EAR_DETECT_INIT()               {P2DIR |= BIT(4);P2PU1 |= BIT(0);P2PU1 &= ~BIT(1);}
#define IS_EAR_IN()                     (P2 & BIT(4))

//MUTE���ſ���
#define MUSIC_MUTE()                    P3DIR &= ~BIT(5); P3 |= BIT(5)
#define MUSIC_UNMUTE()                  P3DIR &= ~BIT(5); P3 &= ~BIT(5)

//AB/D�๦�ſ���
#define OPA_CTL_INIT()                  {P2DIR &= ~BIT(3);P2 |= BIT(3);}
#define OPA_CTL_HIGH()                  P2 |= BIT(3)
#define OPA_CTL_LOW()                   P2 &= ~BIT(3)

//SD��������(���� ADKEY �� SD_CLK ���ʱ����Ҫ����SD_DETECT_INIT)
#define SD_DETECT_INIT()                P2DIR |= BIT(2);P2PU0 |= BIT(4);P2PU0 &= ~BIT(5);
#define IS_SD_IN()                      !(P2 & BIT(2))


/*****************************************************************************
 * Module    : ���߼�����IO�������
 *****************************************************************************/
#define IO_SLEEP()                      {P0 = 0x00;P1 = 0xb0;P2 = 0x00;\
                                        P0DIR = 0xfd;P1DIR = 0xbf;P2DIR = 0xdf;P3DIR = 0xbf;}
#define IO_WAKEUP()                     {P0 = 0x7e;P1 = 0xd7;P2 = 0xcb;P3 = 0xef;\
                                        P0DIR = 0xd1;P1DIR = 0xbf;P2DIR = 0x5e;P3DIR = 0x97;}
#define IS_KEY_WAKEUP()                 !(P3 & BIT(2)) || !(P3 & BIT(0)) || !(P0 & BIT(6)) || !(P1 & BIT(2)) || !(P1 & BIT(3))


#endif
