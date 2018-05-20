/*****************************************************************************
 * Module    : User
 * File      : user_config_6680e_s10_simple.h
 * Author    : junwen
 * Email     : NC
 * Function  : �û� USER_6680E_S10_SIMPLE �Զ����������
 *****************************************************************************/
#ifndef USER_CONFIG_6680E_S10_SIMPLE_H
#define USER_CONFIG_6680E_S10_SIMPLE_H


/*****************************************************************************
 * Module    : ϵͳ����
 *****************************************************************************/
#define SYS_PKG                         PKG_LQFP48_6680E//���ط�װѡ��
#define SYS_CRY                         CRY_RC          //ϵͳʱ��Դѡ��
#define MAKECODE_ID                     1               //����ʱʹ��P13
#define SD_UPDATE_ID                    2               //SD����ʹ��P3��
#define MP3RES_ID                       0x01            //Ĭ����Ӣ��������Դ


/*****************************************************************************
 * Module    : ���˵��������������
 *****************************************************************************/
#define TASK_MUSIC_EN                   1   //�Ƿ��MUSIC����
#define TASK_AUX_EN                     1   //�Ƿ��LINEIN����
#define TASK_FM_EN                      1   //�Ƿ��FM����
#define TASK_USBDEV_EN                  0   //�Ƿ��USB�豸����
#define TASK_POWEROFF_EN                1   //�Ƿ�򿪹ػ�����
#define TASK_BT_EN                      1   //�Ƿ����������
#define TASK_LOUDSPEAKER_EN             1   //�Ƿ��������ģ��
#define TASK_MIC_EN                     1   //�Ƿ��MIC¼��ģ��
#define TASK_VOICE_EN                   1   //�Ƿ��¼������ģ��


/*****************************************************************************
 * Module    : ����ѡ��
 *****************************************************************************/
//�������⣬����Ҫ��ȫ�׵��������ѡ��������ֱ��ƾֲ�����
#define THEME_SELECT                    NO_THEME


/*****************************************************************************
 * Module    : ϵͳ��ʾ���������
 *****************************************************************************/
#define POWER_ON_VOICE                  0   //�Ƿ���Ҫ������ʾ��
#define BT_RING_NUMBER                  1   //�Ƿ���Ҫ���籨��
#define VOICE_MODE_WARNING_MUSIC        1   //�Ƿ���Ҫ¼������ģʽ��ʾ��
#define MIC_RECORD_WARNING_MUSIC        1   //�Ƿ���Ҫ¼��ģʽ��ʾ��
#define LOUDSPEAKER_WARNING_MUSIC       1   //�Ƿ���Ҫ������ģʽ��ʾ��
#define USB_DEVICE_WARNING_MUSIC        0   //�Ƿ���ҪUSB�豸ģʽ��ʾ��


/*****************************************************************************
 * Module    : Musicģ���������
 *****************************************************************************/
#define AUTO_CHANGE_DEVICE              0   //˫�豸ʱ�Զ��л��豸
#define K_MODE_CHANGE_DEVICE            0   //˫�豸ʱʹ��ģʽ���л��豸
#define MUSIC_MODE_AUTO                 1   //�����豸���Զ��л������ֲ���
#define BREAK_POINT_EN                  1   //�ϵ����


/*****************************************************************************
 * Module    : FMģ���������
 *****************************************************************************/
#define FM_TYPE                         FM_RTC6218  //FM����ѡ��
#define FMOSC_FREQ_SEL                  FMOSC_32K   //����SDCLK��ΪFMʱ��ԴʱFMʱ��Ƶ��ѡ��
#define KU_PLAY_TO_SEEKCHANNEL          1           //�Ƿ�ʹ�ö̰����ż�����̨( ����PALY �� PLAY_S10)


/*****************************************************************************
 * Module    : AUX¼�������������
 *****************************************************************************/
#define AUX_RECORD_EN                   1   //AUX�Ƿ���Ҫ¼��
#define KU_PLAY_LINEIN_MUTE_EN          0   //LINEINģʽ���Ƿ�ʹ�ö̰����ż�MUTE����(����PALY �� PLAY_S10)


/*****************************************************************************
 * Module    : �����������
 *****************************************************************************/
#define BT_TYPE                         BT6639
#define BT_UART_IO                      0   //��������IOѡ��0=P16 P17��1=P36 P37
#define BT_PWK_SEL                      1   //����ʹ��P07����
#define BT_CALL_PRIVATE                 0   //ǿ��˽�ܽ���
#define KU_PLAY_ANSWER_AND_HANGUP       1   //�Ƿ�ʹ�ö̰����ż��������͹Ҷϵ绰(����PALY �� PLAY_S10)
#define BT_PHONE_SWITCH		            0   //ͨ��ʱ�����ڲ����绰ʱ�Ƿ���Ҫ�ֻ������������л�
#define BT_NAME_WITH_ADDR               1   //���������Ƿ���ʾ3λ��ַ(�����ã����磺BW-BTBOX-***)


/*****************************************************************************
 * Module    : ϵͳϸ������
 *****************************************************************************/
#define MP3BT_DECT_EN                   1   //�Ƿ�ʹ�ò������ؿ������MP3��BT�Ĺ���
#define MP3BT_DECT_REUSE_EN             1   //���ؿ������MP3��BT��IO�Ƿ�������IO
#define USE_P35_USBPOW_CTL              2   //�Ƿ�ʹ��P35����USB����
#define LED_BLUE_BLINK                  1   //�Ƿ�ʹ��������˸
#define KL_PLAY_S10_CHANGE_MODE         1   //�Ƿ�ʹ�ó��� PLAY_S10 ���л�ģʽ(������ PLAY ��)
#define POWERON_TO_FM                   0   //�Ƿ���Ҫ����������FM
#define FMOSC_REUSE_SDCLK               1   //�Ƿ���SDCLK��ΪFMʱ��Դ(��Ҫ�����޾���)
#define IICCLK_REUSE_USB                1   //IICCLK�Ƿ���USB����(��Ҫ�����޾���SDCLK������12Mʱ��ʱ)
#define IICDAT_REUSE_SDDAT              1   //IICDAT�Ƿ���SDDAT����(��Ҫ�����޾���SDCLK������12Mʱ��ʱ)
#define FM_SET_VOL_CHANGE_BANK          0   //FMģʽ�µ�������ʱ�Ƿ���Ҫ��BANK
#define AUTO_STANDBY_EN                 0   //��ʱ���޲���(ʱ���ɺ� STANDBY_TIME ����)�Ƿ���Ҫ�Զ��������ߴ���
#define FM_VOUT_THROUGH_MCU             0   //FM_VOUT�Ƿ񾭹�����
#define SD_DECT_REUSE_LED               1   //�Ƿ���LED���SD��(��Ҫ����2226 IO �ڲ�����ʱ)
#define MUTE_USE_USBIO                  1   //�Ƿ�ʹ��USBDP/USBDM��ΪMUTE����
#define RECORD_EN                       1   //¼�������ܿ��ء�Ϊ0ʱ�ر����е�¼������


/*****************************************************************************
 * Module    : ����������IO����
 *****************************************************************************/
//ADKey��ʹ�ã�0Ϊʹ��IOKey��1Ϊ1��ADKey��2Ϊ2��ADKey
#define USE_ADKEY                       1

//ѡ���1��ADKEY��IO
#define ADKEY_PORT                      ADKEY_P13

//sd_portֵ:0: P20 P21 P27,    1: P20 P21 P27, clk���ü��,    2: P30 P31 P32,    3: P30 P31 P32, clk���ü��
#define SD_PORT_SEL                     2

//�Ƿ�ʹ��P01�����ڴ�ӡ����
#define DEBUG_UART_SEL                  0

//ADKEY��ֵ��
#define KEY1_NUM                        3
#define KEY1_ADC                        0x2e, 0x6e, 0xbf, 0xFF
#define KEY1_VAL                        T_KEY_PLAY_S10, T_KEY_PREV_S10, NO_KEY, T_KEY_NEXT_S10,

//�Ƿ�Ϊ��������������ֹ����
#define IS_HSF_KEY(k)                   (k == KU_PLAY_S10)


/*****************************************************************************
 * Module    :AUXͨ������
 *****************************************************************************/
//����MIC��ͨ��
#define MIC_CHANNEL_SEL                 MCHANNEL_3

//����FM��ͨ��
#define FM_CHANNEL_SEL                  CHANNEL_22

//����AUX��ͨ��
#define AUX_CHANNEL_SEL                 CHANNEL_77


/*****************************************************************************
 * Module    :IIC IO����
 *****************************************************************************/
#define IIC_IO_SET
#define IIC_DATA                        2       //P32
#define IIC_DATA_CLK_OUT()              USB_DM_OUTPUT(); P3DIR |= BIT(IIC_DATA); P3PU0 |= BIT(IIC_DATA*2+1); P3PU0 &= ~BIT(IIC_DATA*2)
#define IIC_DATA_IN()                   P3DIR |= BIT(IIC_DATA); P3PD0 &= ~(BIT(IIC_DATA*2+1)|BIT(IIC_DATA*2)); P3PU0 &= ~BIT(IIC_DATA*2+1); P3PU0 |= BIT(IIC_DATA*2)
#define IIC_DATA_OUT()
#define IIC_DATA_HIGH()                 P3DIR |= BIT(IIC_DATA); P3PD0 &= ~(BIT(IIC_DATA*2+1)|BIT(IIC_DATA*2)); P3PU0 |= BIT(IIC_DATA*2+1); P3PU0 &= ~BIT(IIC_DATA*2)
#define IIC_DATA_LOW()                  P3DIR |= BIT(IIC_DATA); P3PU0 &= ~(BIT(IIC_DATA*2+1)|BIT(IIC_DATA*2)); P3PD0 |= BIT(IIC_DATA*2+1); P3PD0 &= ~BIT(IIC_DATA*2)
#define IIC_DATA_CLK_HIGH()             USB_DM_HIHG(); IIC_DATA_HIGH()
#define IIC_CLK_HIGH()                  USB_DM_HIHG()
#define IIC_CLK_LOW()                   USB_DM_LOW()
#define IIC_DATA_IS_HIGH()              (P3 & BIT(IIC_DATA))


/*****************************************************************************
 * Module    :LED IO����
 *****************************************************************************/
//��ɫLED
#define LED_BLUE                        1
#define LED_BLUE_INIT()                 P0DIR &= ~BIT(LED_BLUE);P0 &= ~BIT(LED_BLUE)
#define LED_BLUE_ON()                   P0 |= BIT(LED_BLUE)
#define LED_BLUE_OFF()                  P0 &= ~BIT(LED_BLUE)
#define LED_BLUE_TOG()                  P0 ^= BIT(LED_BLUE)
#define LED_BLUE_IS_ON()                (P0 & BIT(LED_BLUE))


/*****************************************************************************
 * Module    : �������IO����
 *****************************************************************************/
//#define BT_SLEEP_IO_SET
//#define BT_IS_SLEEP()                   (P0 & BIT(7))
//#define BT_IS_SLEEP_INIT()              IE_EA = 0; P0DIR |= BIT(7);\
                                        asm("nop");asm("nop");asm("nop");asm("nop");\
                                        asm("nop");asm("nop");asm("nop");asm("nop");
//#define BT_IS_SLEEP_END()               P0DIR &= ~BIT(7); IE_EA = 1;

//����ģ�鸴λ IO
#define BT_RESET_IO_SET
#define BT_RST_INIT()                   P1DIR &= ~BIT(2);
#define BT_RST_LOW()                    P1 &= ~BIT(2);
#define BT_RST_HIGH()                   P1 |= BIT(2);


/*****************************************************************************
 * Module    : ��������IO�������
 *****************************************************************************/
//�ϵ�ʱBT/MP3���
#define MP3BT_DECT_INIT()               P0DIR |= BIT(5);P0PU0 |= BIT(5);\
                                        asm("nop");asm("nop");asm("nop");asm("nop");\
                                        asm("nop");asm("nop");asm("nop");asm("nop");
#define MP3BT_DECT_IS_BT()              (P0 & BIT(5))
#define MP3BT_DECT_END()                P0PU0 &= ~BIT(5);P0DIR &= ~BIT(5)

//SD��������(���� ADKEY �� SD_CLK ���ʱ����Ҫ����SD_DETECT_INIT)
#define SD_DETECT_INIT()                P0DIR |= BIT(1);P0PU0 |= BIT(1);\
                                        asm("nop");asm("nop");asm("nop");asm("nop");\
                                        asm("nop");asm("nop");asm("nop");asm("nop");
#define IS_SD_IN()                      !(P0 & BIT(1))
#define SD_DETECT_END()                 P0DIR &= ~BIT(1);P0PU0 &= ~BIT(0);

//MUTE���ſ���
#define MUSIC_MUTE()                    USB_DP_HIHG();
#define MUSIC_UNMUTE()                  USB_DP_LOW();


/*****************************************************************************
 * Module    : ���߼�����IO�������
 *****************************************************************************/
#define IO_SLEEP()                      {P1DIR &= ~BIT(3); P1 &= ~BIT(3);LED_BLUE_OFF();}
#define IO_WAKEUP()                     {P1DIR |= BIT(3);}
#define IS_KEY_WAKEUP()                 !(P1 & BIT(3))


#endif
