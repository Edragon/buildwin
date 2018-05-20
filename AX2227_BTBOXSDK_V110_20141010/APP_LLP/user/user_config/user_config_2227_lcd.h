/*****************************************************************************
 * Module    : User
 * File      : user_config_2227_lcd.h
 * Author    : caiyue
 * Email     : NC
 * Function  : �û� USER_2227 LCD �Զ����������
 *****************************************************************************/
#ifndef USER_CONFIG_2227_LCD_H
#define USER_CONFIG_2227_LCD_H


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
#define THEME_SELECT                    LCD_THEME_DEFAULT


/*****************************************************************************
 * Module    : ϵͳ��ʾ���������
 *****************************************************************************/
#define POWER_ON_VOICE                  1   //�Ƿ���Ҫ������ʾ��
#define VOICE_MODE_WARNING_MUSIC        1   //�Ƿ���Ҫ¼������ģʽ��ʾ��
#define MIC_RECORD_WARNING_MUSIC        1   //�Ƿ���Ҫ¼��ģʽ��ʾ��
#define LOUDSPEAKER_WARNING_MUSIC       1   //�Ƿ���Ҫ������ģʽ��ʾ��
#define CLOCK_MODE_WARNING_MUSIC        1   //�Ƿ���Ҫʱ��ģʽ��ʾ��


/*****************************************************************************
 * Module    : Musicģ���������
 *****************************************************************************/
#define AUTO_CHANGE_DEVICE              1   //˫�豸ʱ�Զ��л��豸
#define K_MODE_CHANGE_DEVICE            1   //˫�豸ʱʹ��ģʽ���л��豸
#define MUSIC_MODE_AUTO                 1   //�����豸���Զ��л������ֲ���
#define BREAK_POINT_EN                  1   //�ϵ����
#define MUSIC_LRC_EN                    0   //�����ʾ������ͬʱ��ID3��ʾ
#define MUSIC_ID3_EN                    1   //ID3��ʾ������ͬʱ�򿪸����ʾ


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


/*****************************************************************************
 * Module    :Ƶ����ʾ����
 *****************************************************************************/
#define MUSIC_FREQ_SHOW                 1   //����ģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)
#define FM_FREQ_SHOW                    1   //FMģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)
#define AUX_FREQ_SHOW                   1   //AUXģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)
#define USBAUDIO_FREQ_SHOW              1   //USB�豸ģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)
#define BT_FREQ_SHOW                    0   //����ģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)


/*****************************************************************************
 * Module    : LCD���������
 *****************************************************************************/
#define LCD_X_FIX                       0   //�Ƿ���Ҫ���͵�ַ����
#define LCD_SOFT_RESET                  1   //�Ƿ���Ҫ�����λLCD��
#define LCD_INVERT_DISPLAY              0   //�Ƿ���Ҫ����LCD��invert�Ĵ���
#define LCD_CONTRAST_DEFAULT            10  //Ĭ�ϵ�LCD���Աȶ�(ȡֵ��Χ: 0~15)

//LCD IO ����
#define LCD_IO_SET
#define LCD_DATA                        5   //P25
#define LCD_CLK                         6   //P26
#define LCD_A0                          0   //P30
#define LCD_RES                         1   //p31
#define LCD_CS                          2   //P32
#define LCD_BACKLIGHT                   5   //p15

//LCD�˿�����
#define LCD_PORT_SET
#define LCD_RESET()                     P3DIR &= ~BIT(LCD_RES); P3 &= ~BIT(LCD_RES)
#define LCD_PORT_INIT()                 /*LCD_BACKLIGHT_ON();*/\
                                        PMUXCON0 |= BIT(4); \
                                        P2 |= BIT(LCD_CLK);\
                                        P3 |= BIT(LCD_CS);\
                                        P3 |= BIT(LCD_A0);\
                                        P2DIR &= ~(BIT(LCD_CLK) | BIT(LCD_DATA));\
                                        P3DIR &= ~BIT(LCD_CS);\
                                        P3DIR &= ~BIT(LCD_A0);\
                                        P3DIR &= ~BIT(LCD_RES)

#define SET_LCD_A0                      P3 |= BIT(LCD_A0);
#define CLR_LCD_A0                      P3 &= ~BIT(LCD_A0);
#define SET_LCD_CS                      P3 |= BIT(LCD_CS);
#define CLR_LCD_CS                      P3 &= ~BIT(LCD_CS);
#define SET_LCD_RES                     P3 |= BIT(LCD_RES);
#define CLR_LCD_RES                     P3 &= ~BIT(LCD_RES);

//LCD����IO�������
#define LCD_BACKLIGHT_SET
#define LCD_BACKLIGHT_ON()              if (!sys_ctl.backlight_sta) {sys_ctl.backlight_sta = 1; P1DRV1 |= BIT(7); P1DIR |= BIT(LCD_BACKLIGHT);}
#define LCD_BACKLIGHT_OFF()             if (sys_ctl.backlight_sta)  {sys_ctl.backlight_sta = 0; P1DRV1 &= ~BIT(7);}


/*****************************************************************************
 * Module    : �ؼ������ͣ��ʱ�����
 *****************************************************************************/
//������progressbox��ͣ��ʱ��
#define PROGRESS_TIME                   30  //3��

//��Ϣ��ͣ��ʱ��
#define MSGBOX_TIME                     5   //1.5��

//��ֻ����ʾ������£��Ƿ�ʹ�ü򵥵���Ϣ��
#define MSGBOX_SIMPLE                   1


/*****************************************************************************
 * Module    : ϵͳϸ������
 *****************************************************************************/
#define USE_P35_USBPOW_CTL              0   //P35�ŵ�ʹ��ѡ������MUTE��ʱΪ0����������USB����ʱѡ1�������ô�ʱΪ2
#define LED_GREEN_BLINK                 0   //�Ƿ�ʹ���̵���˸
#define HARD_IR_EN                      1   //����ң��(NECӲ��)
#define IR_HARD_POWERDOWN_EN            1   //����POWER��Ӳ�ػ�ʹ��
#define K_HSF_CHANGE_MODE               1   //�Ƿ�ʹ�� HSF ���л�ģʽ
#define RECORD_EN                       1   //¼�������ܿ��ء�Ϊ0ʱ�ر����е�¼������
#define SOFT_POWER_ON_OFF               0   //�Ƿ�ʹ�����ػ�


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
#define IIC_DATA                        3   //P13
#define IIC_CLK                         2   //P12

#define IIC_DATA_CLK_OUT()              P1PU0 &= ~(BIT(IIC_DATA) | BIT(IIC_CLK));  P1DIR &= ~(BIT(IIC_DATA) | BIT(IIC_CLK)); delay(10)
#define IIC_DATA_IN()                   P1DIR |= BIT(IIC_DATA); P1PU0 |= BIT(IIC_DATA)
#define IIC_DATA_OUT()                  P1PU0 |= BIT(IIC_DATA); P1DIR &= ~BIT(IIC_DATA)
#define IIC_DATA_CLK_HIGH()             P1 |= BIT(IIC_DATA) | BIT(IIC_CLK)
#define IIC_DATA_HIGH()                 P1 |= BIT(IIC_DATA)
#define IIC_DATA_LOW()                  P1 &= ~BIT(IIC_DATA)
#define IIC_CLK_HIGH()                  P1 |= BIT(IIC_CLK)
#define IIC_CLK_LOW()                   P1 &= ~BIT(IIC_CLK)
#define IIC_DATA_IS_HIGH()              (P1 & BIT(IIC_DATA))


/*****************************************************************************
 * Module    :LED IO����
 *****************************************************************************/
//��ɫLED
#define LED_GREEN                       4
#define LED_GREEN_INIT()                P2DIR &= ~BIT(LED_GREEN); P2 &= ~BIT(LED_GREEN)
#define LED_GREEN_ON()                  P2 |= BIT(LED_GREEN)
#define LED_GREEN_OFF()                 P2 &= ~BIT(LED_GREEN)
#define LED_GREEN_TOG()                 P2 ^= BIT(LED_GREEN)
#define LED_GREEN_IS_ON()               (P2 & BIT(LED_GREEN))


/*****************************************************************************
 * Module    : ��������IO�������
 *****************************************************************************/
//�������
#define EAR_DETECT_INIT()               P1DIR |= BIT(1);P1PU0 |= BIT(1)
#define IS_EAR_IN()                     (P1 & BIT(1))

//MUTE���ſ���
#define MUSIC_MUTE()                    P3DIR &= ~BIT(5); P3 |= BIT(5)
#define MUSIC_UNMUTE()                  P3DIR &= ~BIT(5); P3 &= ~BIT(5)

//���ػ�IO�������
#define SOFT_POWER_CHK_INIT()           P2DIR |= BIT(2)
#define SOFT_POWER_CHK_IS_ON()          (P2 & BIT(2))

//���ػ�IO��������
#define SOFT_POWER_CTL_INIT()
#define SOFT_POWER_CTL_ON()             irtcc_write_cfg(RTC_CFG_WCMD,irtcc_read_cfg(RTC_CFG_RCMD) | BIT(4));    //IRTWKO output high
#define SOFT_POWER_CTL_OFF()            irtcc_write_cfg(RTC_CFG_WCMD,irtcc_read_cfg(RTC_CFG_RCMD) & ~BIT(4));   //IRTWKO output controlled by IRTCC alarm function (low)


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
