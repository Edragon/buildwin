/*****************************************************************************
 * Module    : Config
 * File      : config_setting.h
 * Author    : Hanny
 * Function  : �û������ļ�
 *****************************************************************************/
#ifndef USER_CONFIG_H
#define USER_CONFIG_H


/*****************************************************************************
 * Module    : ϵͳ����
 *****************************************************************************/

#define SYS_CRY                         CRY_32K         //ϵͳʱ��Դѡ��
#define BUCK_MODE_EN                    0               //��Դģʽѡ��

#define DAC_INIT_MODE                   1               //DAC��ʼ��ģʽ��0Ϊ���ٳ�ʼ����1Ϊȥž����ʼ��
#define MAKECODE_ID                     0               //����BOOT����ѡ��
//SPIFlash�Ƿ�֧��4K Sector������֧��4K Sector֧�֣����Լ��ٲ�������С����ԼSPIFlash�ռ䡣
//      ȱ����Ӱ��SPIFlash�Ĳ�д�����ԣ�ͬʱҲ��Ӱ��SPIFlash�ļ����ԡ�
#define SPI_4K_SECTOR_SUPPORT           0               //SPIFlash�Ƿ�֧��4K Sector����

#define DAC_MIX_OUT_EN                  0               //0:˫��������. 1:����������_������������������

/*****************************************************************************
 * Module    : ���˵��������������
 *****************************************************************************/

#define TASK_MENU_EN                    0   //�Ƿ�����˵����ܡ�һ��Ӧ����LCD������
#define TASK_MUSIC_EN                   1   //�Ƿ��MUSIC����
#define TASK_AUX_EN                     1   //�Ƿ��AUX����
#define TASK_AUX_EQ_EN                  0   //�Ƿ��AUX EQ����
#define TASK_FM_EN                      1   //�Ƿ��FM����
#define TASK_USBDEV_EN                  1   //�Ƿ��USB�豸����
#define TASK_POWEROFF_EN                1   //�Ƿ�򿪹ػ�����
#define TASK_BT_EN                      1   //�Ƿ����������
#define TASK_RTC_EN                     1   //�Ƿ��ʱ�ӹ���
#define TASK_SPEAKER_EN                 1   //�Ƿ��������ģ��
#define TASK_MIC_EN                     1   //�Ƿ��MIC¼��ģ��
#define TASK_VOICE_EN                   1   //�Ƿ��¼������ģ��
#define TASK_TOMCAT_EN                  0   //�Ƿ����ķè���� (��ҪSPIFlash֧��4K Sector����)
#define TASK_HOLD_EN                    1   //�Ƿ�򿪿���ʱ����HOLD״̬
#define TASK_UPDATE_FW_EN               1   //�Ƿ����������ģʽ��U�̻���TF�����ɣ�
#define TASK_BTHID_EN                   1   //�Ƿ�򿪶���������ģʽ

/*****************************************************************************
 * Module    : ����ѡ��
 *****************************************************************************/

//�������⣬����Ҫ�������е�ȫ�ײ���ѡ��������������ľֲ�����������
#define THEME_SELECT                    THEME_LEDSEG_5C7S

/*****************************************************************************
 * Module    : �͹����������
 *****************************************************************************/

//TASK_HOLD_EN Ϊ�͹��ĵ��ܿ��ƿ��ء��ر�TASK_HOLD_EN��ر����е͹���ģʽ
#define STANDBY_TIME                	100 //�������ߴ���(HOLDģʽ)ʱ��(��λΪ��)
#define HOLD_POWER_TYPE                 0   //HOLD MODE�¹���ѡ�� 0:������ѹ��1:�͵�ѹ
#define TASK_HOLD_DAC_CLOSE             1   //�Ƿ���Ҫ���׹ص�dac�������׹ص�������ʱ��ž������Ҫ��muteס����
#define BT_DISCONNECT_POWEROFF_TIME     0   //����HOLDģʽ�£�����δ����ʱ����ػ���ʱ��(��λΪ��)��0��ʾ�����ô˹���

#define MUSIC_AUTO_STANDBY_EN           1   //���ֲ��������£��Ƿ���Ҫ�Զ��������ߴ���
#define FM_AUTO_STANDBY_EN              1   //FM���������£��Ƿ���Ҫ�Զ��������ߴ���
#define BT_AUTO_STANDBY_EN              1   //BT�����£��Ƿ���Ҫ�Զ��������ߴ���
#define AUX_AUTO_STANDBY_EN             1   //AUX/AUX EQ�����£��Ƿ���Ҫ�Զ��������ߴ���

/*****************************************************************************
 * Module    : ���ػ��������
 *****************************************************************************/

#define SOFT_POWER_ON_OFF               1   //�Ƿ�ʹ�����ػ�

//���ʹ����ػ���3uA��ػ� �� 20 uA��ػ� ������ֻ�ܴ�һ����
#define SOFT_POWER_OFF_20UA             1   //20uA��ػ�
#define SOFT_POWER_OFF_3UA              0   //3uA��ػ� ��CW6687E��CW6687G��֧�֣�

/*****************************************************************************
 * Module    : ��ʾ��ѡ��
 *****************************************************************************/

#define WARNING_MUSIC	                1   //�Ƿ�ʹ����ʾ����Ϊ0����ȫ�ر���ʾ������
#define WARNING_VOLUME                  10  //��ʾ��������

#define WARNING_POWER_ON                1   //������ʾ��
#define WARNING_TASK_BT                 1   //����������ʾ��
#define WARNING_TASK_CLOCK              1   //ʱ��������ʾ��
#define WARNING_TASK_AUX                1   //AUX/AUX EQ������ʾ��
#define WARNING_TASK_FM                 1   //FM������ʾ��
#define WARNING_TASK_VOICE              1   //¼������������ʾ��
#define WARNING_TASK_SPEAKER            1   //������������ʾ��
#define WARNING_TASK_MIC                1   //MIC¼��������ʾ��
#define WARNING_TASK_USBDEV             1   //PC������ʾ��
#define WARNING_TASK_TOMCAT             0   //��ķè������ʾ��
#define WARNING_BT_CONNECT              1   //����������ʾ��
#define WARNING_BT_DISCONNECT           1   //�����Ͽ�������ʾ��
#define WARNING_LOWPOWER                1   //�͵���ʾ�����ػ���
#define WARNING_POWER_OFF               1   //�ػ���͵�ػ���ʾ��
#define WARNING_VOL_MAXMIN              1   //�Ƿ���Ҫ�������ڵ�������Сʱ��ʾ��
#define WARNING_USB_SD                  1   //����U�̻�SD����ʾ��

/*****************************************************************************
 * Module    : ���������������
 *****************************************************************************/

#define MUSIC_ENCRYPT_EN                0   //�Ƿ���ҪMP3�ļ����ܽӿ�
#define MUSIC_LRC_EN                    0   //�Ƿ���ҪMP3�����ʾ
#define AUTO_CHANGE_DEVICE              1   //˫�豸ʱ�Զ��л��豸
#define K_MODE_CHANGE_DEVICE            1   //˫�豸ʱʹ��ģʽ���л��豸
#define MUSIC_MODE_AUTO                 1   //�����豸���Զ��л������ֲ���
#define BREAK_POINT_EN                  1   //�ϵ����
#define AB_REPEAT_EN                    0   //AB����
#define PLAY_DEVICE_SAVE_EN             0   //���䲥���豸
#define FOLDER_SELECT                   0   //�ļ���ѡ��
#define MUSIC_DRC_EN                    1   //�Ƿ���ҪDRC����

//MUSIC_ID3_ENΪID3���ܵ��ܿ��أ��ر�MUSIC_ID3_EN��ر����и�ʽ��ID3����
#define MUSIC_ID3_EN                    0   //�Ƿ��ID3�ܿ���
#define MP3_ID3_EN                      0   //�Ƿ��mp3��ID3
#define WAV_ID3_EN                      0   //�Ƿ��wav��ID3
#define WMA_ID3_EN                      0   //�Ƿ��wma��ID3

/*****************************************************************************
 * Module    : FMģ���������
 *****************************************************************************/
//FM����ѡ��
#define FM_TYPE                         FM_RTC6218
#define SEEK_DISPLAY_CHANNEL_NUM        1               //��̨��������ʾ�ѵ���Ƶ��
#define FM_VOUT_THROUGH_MCU             0               //FM VOUT�Ƿ�ͨ������
#define FM_CLK_SEL                      SHARE_CRYSTAL_32K //FMʱ��ѡ��
#define FM_TIME_EN                      0               //��ʱ������̨
#define FM_SET_VOL_CHANGE_BANK          1               //FM���������Ƿ���bank
#define FM_RECORD_EN                    0               //�Ƿ��FM¼������

/*****************************************************************************
 * Module    : USB�豸ģ���������
 *****************************************************************************/
//USB�豸ö������
#define USBDEV_ENUM_TYPE               (USB_DEV_MASS | USB_DEV_SPEAKER | USB_DEV_MIC | USB_DEV_HID)

/*****************************************************************************
 * Module    : AUX �������
 *****************************************************************************/

#define MIC_CHANNEL_SEL                 MCHANNEL_1          //����MIC��ͨ��
#define FM_CHANNEL_SEL                  CHANNEL_02_03       //����FM��ͨ��
#define AUX_CHANNEL_SEL                 CHANNEL_00_01       //ѡ��LINEIN��ͨ��
#define LINEIN_DETECT_EN                1                   //�Ƿ�ʹ��LINEIN���
#define LINEIN_SEG_REUSE                0                   //LINEIN��⸴��LED SEG
#define LINEIN_DETECT_REUSE_IICCLK      0                   //LINEIN��⸴��IIC CLK
#define SDCMD_REUSE_LINEIN_DET          0                   //LINEIN��⸴��SD CMD
#define AUX_RECORD_EN                   1                   //�Ƿ��AUX¼������

/*****************************************************************************
 * Module    : ¼���������
 *****************************************************************************/

#define RECORD_FILE_NAME                SHORT_NAME          //�Ƿ�ʹ�ó��ļ���¼��
#define REC_TYPE                        REC_MP3             //¼������ѡ��MP3
#define MIC_DIRECT_OUT_EN               0                   //����ģʽ���micֱͨ

/*****************************************************************************
 * Module    : �����������
 *****************************************************************************/

#define BT_ALL_TASK                     0     //�Ƿ�ȫ��ʹ��������������̨��
#define BT_CALL_PRIVATE                 0     //�Ƿ�ǿ��ʹ��˽�ܽ������ֻ��˽�����
#define BT_RING_NUMBER                  1     //�Ƿ���Ҫ���籨��
#define BT_NAME_WITH_ADDR               0     //���������Ƿ���ʾ3λ��ַ�������ã����磺BW-BTBOX-***��
#define BT_SAMPLERATE_16K               1     //����ͨ���Ƿ�ʹ��16k������0Ϊ8k����
#define BT_HSHF_EN                      1     //�Ƿ��ͨ��HSHF����
#define BT_A2DP_EN                      1     //�Ƿ����ƵA2DP����
#define BT_SPP_EN                       1     //�Ƿ����������SPP����
#define BT_SPP_MSC_EN                   1     //�Ƿ�ʹ��SPP��ȡMUSIC��Ŀ�������б���ͬʱ��BT_ALL_TASK��BT_SPP_EN�궨�壩
#define BT_SPP_EQ_EN                    0     //�Ƿ���������ߵ���EQ���ܣ���ͬʱ��BT_ALL_TASK��BT_SPP_EN�궨�壩
#define BT_HID_EN                       1     //�Ƿ������HID�����Ĺ��ܣ�����
#define BT_HID_MANU                     0     //����HID�Ƿ���Ҫ�ֶ����ӣ�������Ĭ�ϲ�����HID��
#define BT_POWER_ON_CONN_TIMES          3     //�ϵ���������
#define BT_TIME_OUT_CONN_TIMES          24    //Զ������߻�������
#define BT_NOISE_VALUE                  1536  //������ֵ�����������Ĵ�С,������ֵ��Ϊ��������
#define BT_FAR_DELAY                    12    //Զ���ӳ٣�Զ������������MIC���ӳ�,һ��Ϊ8~16��
#define BT_ECHO_ATTEN                   5     //����˥�������������ļ��𣬼���Խ��˥��Խ���ԣ�˫��ͨ��Ч����
#define BT_ANA_GAIN                     6     //MICģ������,��ΧΪ-1~9
#define BT_DIGIT_GAIN                   96    //MIC��������,��ΧΪ0~127
#define BT_VOLUME_MAX                   0     //����ģʽ���Ƿ�ҪĬ���������
#define BT_AUTO_PLAY                    0     //��������״̬����ػ��󿪻��Զ������ɹ����Ƿ���Ҫ�Զ�����(Ĭ�ϲ���Ҫ)
#define BT_DYNAMIC_NOISE                DYNAMIC_DAC_SPK     //���������Ƿ�ʹ�ö�̬����
#define BT_ANTI_LOST                    1     //�Ƿ��������������
#define PLAY_DETECT_TO_BT               0     //������ģʽ�¼�⵽�ֻ������������֣����л�������
#define BT_3WCALL_EN                    1     //�Ƿ�֧������ͨ��
#define BT_PHONE_BOOK_EN                0     //�Ƿ�֧�ֵ绰������

//����ģ��WAKEUP IO
#define BT_IS_SLEEP                 	((0 == bt_sleep_cnt) && (BTCON2 & BIT(5)))
#define BTLDO_POWCTRL_SET
#define BTLDO_DIR_INIT()
#define BTLDO_EN()
#define BTLDO_DIS()

/*****************************************************************************
 * Module : ���Ѽ�ALARM �������
 *****************************************************************************/

#define ALARM_EN                        1       //����
#define ALARM_WKUP_POWERDOWN            1       //power down�ػ������ӿ��Ի��ѿ���
#define WKO_WKUP_POWERDOWN              1       //�ػ��Ƿ�ʹ��WKO����
#define IRTCC_WKUP_EN                   0       //IRRTC WAKE_UP ��ʹ��
#define ALM_TYPE                        0       //ѡ���������ڵ����ã�0Ϊ��ѡ�����ã�1Ϊֱ�����б�����

/*****************************************************************************
 * Module    : ϵͳϸ������
 *****************************************************************************/

#define MP3BT_DECT_EN                   0       //�Ƿ�ʹ�ò������ؿ������MP3��BT�Ĺ��ܡ�
#define USE_P35_USBPOW_CTL              0       //P35�ŵ�ʹ��ѡ������MUTE��ʱΪ0����������USB����ʱѡ1�������ô�ʱΪ2
#define LED_GREEN_BLINK                 0       //�Ƿ�ʹ���̵���˸
#define LED_BLUE_BLINK                  0       //�Ƿ�ʹ��������˸��˫LEDʱ��������Ҫ��������ģʽ��
#define EAR_DETECT_EN                   1       //�Ƿ�ʹ�ö�����⣨��������ʱMUTE���ţ�
#define EAR_SEG_REUSE                   0       //������⸴��LED SEG
#define EAR_ADKEY_REUSE                 1       //������⸴��ADKEY
#define EAR_DETECT_REUSE_IICCLK         0       //�Ƿ���IICCLK����������
#define SDCMD_REUSE_EARDET              0       //SD CMD���ö������
#define IR_HARD_POWERDOWN_EN            0       //����POWER��Ӳ�ػ�ʹ��(Ĭ�ϲ�ʹ��)

#define IIC_REUSE_SD                    1       //IIC�ӿ��Ƿ���Ҫ��SD����
#define IICCLK_REUSE_USB                0       //IICCLK�Ƿ���USB���ã���Ҫ�����޾���SDCLK������12Mʱ��ʱ��
#define IICDAT_REUSE_SDDAT              0       //IICDAT�Ƿ���SDDAT���ã���Ҫ�����޾���SDCLK������12Mʱ��ʱ��

#define LOWPOWER_WARNING_CYCLE          100     //�͵�����Ԥ������  CYCLE*100ms
#define LOWPOWER_WARNING_VOLTAGE        0x7B    //�͵����ѵ�ѹ���ƣ�0x76-3.1V  0x79-3.2V, 0x7B-3.3V
#define LOWPOWER_VOLTAGE                0x79    //�͵�ػ���ѹ���ƣ�0x76-3.1V  0x79-3.2V, 0x7B-3.3V
#define LOWER_POWER_DISPLAY_OFF         1       //�͵�ʱ�Ƿ���Ҫ��ʾһ��OFF������3���ػ�
#define LED_STATUS_BLINK                0       //�Ƿ�ʹ�õ�ص���״̬�ƣ��͵�ʱ��˸��
#define MODE_CHANGE_BLUE_ON             0       //�Ƿ���Ҫģʽ�л�ʱ������˸һ��
#define POWERON_TO_FM                   0       //�Ƿ���Ҫ����������FM
#define SD_DECT_REUSE_ADKEY             0       //�Ƿ�ʹ��ADKEY���SD��
#define AUX_DECT_REUSE_ADKEY            1       //�Ƿ�ʹ��ADKEY���AUX
#define LED_REUSE_ADKEY                 0       //LED�Ƿ���ADKEY����Ҫ����IO����ʱ��
#define FIRST_PLAY_UDISK                0       //�Ƿ���Ҫ���Ȳ���U��
#define EQ_DISPLAY_NUM                  0       //�Ƿ�ʹ��EQ��ʾEq0-Eqn(Ĭ����ʾEQģʽ���Ƶ�ǰ������ĸ)
#define DEBUG_LOADER_DIS                0       //�Ƿ�����LOADER������Ϣ. 1�ر�P01��ӡ��0��P01��ӡ
#define VCMBUF_EN                       0       //0��ʹ��VCMBUF,1 ʹ��VCMBUFֱ�ƶ��� (VCMBUF��P01����һ��ʹ��VCMBUFʱ��Ҫ�ر�P01��ӡ��������DEBUG_LOADER_DIS->1, DEBUG_UART_SEL->0)
#define DEBUG_UART_SEL                  1       //����(ʹ��P01���ڴ�ӡ)
#define SDCLK_REUSE_ADKEY               0       //�Ƿ���SD_CLK��ADKEY���(ʹ�øú���Ҫ�Ѻ�ADKEY_PORTѡ��SD_CLK��Ӧ�Ľ�)
#define FFT_LED_EN                      0       //�Ƿ�ʹ��LEDƵ�׵�
#define MICBIAS_EN                      0       //�Ƿ�ʹ��MICBIAS��MIC���磬0��Ϊʹ��VDDHP��MIC����

#define SMALL_VOLUME_EN                 0       //�Ƿ�ʹ��С������(1.96v), 0Ϊ��������(2.8v)
#define INTERNAL_CHARGE_EN              0       //�Ƿ�ʹ���ڲ���繦�ܣ�CW6687E��CW6687G��֧�֣�
#define LED_POWER_BLINK                 0       //�Ƿ�ʹ�ó��ָʾ��

//LDO MODE�µ�PMU����
#define USER_OTHER_INIT()               lqfp48_pmu_init();P3PD |= BIT(6) //P36�ϵ�ᷭת����COM

/*****************************************************************************
 * Module    : ����������IO����
 *****************************************************************************/

#define USE_ADKEY                       2           //ADKey��ʹ�ã�0Ϊ��ʹ�ã�1Ϊ1��ADKey��2Ϊ2��ADKey
#define USE_IOKEY                       0           //IOKey��ʹ�ã�0Ϊ��ʹ�ã�1Ϊʹ��IOKey
#define USE_WKOKEY                      1           //1Ϊ���WKO PIN�ϵİ�����0���ô˹���
#define KEY_DOUBLE_CLICK_EN             0           //ʹ��ĳһ��������˫������,�����ں�DOUBLE_CLICK_KEY������
#define DOUBLE_CLICK_KEY                KU_PLAY_POWER   //֧��˫���İ���
#define LONG_PRESS_POWERON              1           //����ĳ�����ܿ�����
#define WKO_OUTPUT_EN                   0           //WKO�����������һ��IO��ʹ�á������/�ͷֱ���ͨ������10K������60K��ʵ�ֵġ�

//sd_portֵ:0: P20 P21 P27,    1: P20 P21 P27, clk���ü��,    2: P30 P31 P32,    3: P30 P31 P32, clk���ü��
#define SD_PORT_SEL                     3
#define SD_CMD_REUSE_SD_DETECT          0
#define HARD_IR_EN                      0           //����ң�أ�Ӳ�⣬ֻ���ⲿ��32K���񣬲���ʹ��Ӳ�⣬������ʹ����⣩
#define SOFT_IR_EN                      0           //����ң�أ�NEC��⣩

#define ADKEY_PORT                      ADKEY_P13   //ѡ���1��ADKEY��IO
#define ADKEY2_PORT                     ADKEY_P33   //ѡ���2��ADKEY��IO
#define ADKEY_INT_PU10K                 0           //��1��ADKEY�Ƿ�ʹ���ڲ�10K������ֻ��3������ʱ����ʹ�ã�
#define ADKEY2_INT_PU10K                0           //��2��ADKEY�Ƿ�ʹ���ڲ�10K������ֻ��3������ʱ����ʹ�ã�

#define K_PLAY_INIT()
#define IS_K_PLAY()                     (0)
#define K_NEXT_INIT()
#define IS_K_NEXT()                     (0)
#define K_PREV_INIT()
#define IS_K_PREV()                     (0)

#if ((SD_PORT_SEL == 0) || (SD_PORT_SEL == 2) && !SD_CMD_REUSE_SD_DETECT) //����IO���
#define SD_DETECT_INIT()                P2DIR |= BIT(6); P2PU |= BIT(6);
#define IS_SD_IN()                      (!(P2 & BIT(6)))
#endif

#define OTHER_ADC_CHECK()                           		//����ADKE1�ļ�⣬��AUX/SD/������⸴��ADKEY1
#define OTHER_ADC2_CHECK()              other_adc_check()	//����ADKE2�ļ�⣬��AUX/SD/������⸴��ADKEY2

/*****************************************************************************
 * Module    : IIC���������
 *****************************************************************************/
#define IIC_IO_SET
#define IIC_DATA                        2       //P32
#define IIC_CLK                         1       //P31
#define IIC_DATA_CLK_OUT()              P3PU &= ~(BIT(IIC_DATA) | BIT(IIC_CLK)); P3DIR &= ~(BIT(IIC_DATA) | BIT(IIC_CLK)); delay(10)
#define IIC_DATA_IN()                   P3DIR |= BIT(IIC_DATA); P3PU |= BIT(IIC_DATA)
#define IIC_DATA_OUT()                  P3PU |= BIT(IIC_DATA); P3DIR &= ~BIT(IIC_DATA)
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
#define LED_GREEN                       //5
#define LED_GREEN_INIT()                //P1DIR &= ~BIT(LED_GREEN); P1 &= ~BIT(LED_GREEN)
#define LED_GREEN_ON()                  //P1 |= BIT(LED_GREEN)
#define LED_GREEN_OFF()                 //P1 &= ~BIT(LED_GREEN)
#define LED_GREEN_TOG()                 //P1 ^= BIT(LED_GREEN)
#define LED_GREEN_IS_ON()               //(P1 & BIT(LED_GREEN))

//��ɫLED
#define LED_BLUE                        //5
#define LED_BLUE_INIT()                 //P1DIR |= BIT(LED_BLUE); P1PU &= ~BIT(LED_BLUE); P1PUS1 &= ~BIT(LED_BLUE);P1PUS0 |= BIT(LED_BLUE);
#define LED_BLUE_ON()                   //P1PU |= BIT(LED_BLUE)
#define LED_BLUE_OFF()                  //P1PU &= ~BIT(LED_BLUE)
#define LED_BLUE_TOG()                  //P1PU ^= BIT(LED_BLUE)
#define LED_BLUE_IS_ON()                //(P1PU & BIT(LED_BLUE))

//����ɫLED
#define LED_POWER                       //1
#define LED_POWER_INIT()                //P3PU &= ~BIT(LED_POWER); P3DIR |= BIT(LED_POWER);  P3PUS0 |= BIT(LED_POWER);P3PUS1 &= ~BIT(LED_POWER)
#define LED_POWER_ON()                  //P3PU |= BIT(LED_POWER)
#define LED_POWER_OFF()                 //P3PU &= ~BIT(LED_POWER)
#define LED_POWER_TOG()                 //P3PU ^= BIT(LED_POWER)
#define LED_POWER_IS_ON()               0//(P3PU & BIT(LED_POWER))

/*****************************************************************************
 * Module    : ��������IO�������
 *****************************************************************************/
//MUTE���ſ���
#define SPEAKER_MUTE()                  P3DIR &= ~BIT(5); P3 |= BIT(5)
#define SPEAKER_UNMUTE()                P3DIR &= ~BIT(5); P3 &= ~BIT(5)

//��Դ���ؿ���
#define SOFT_POWER_CTL_INIT()
#define SOFT_POWER_CTL_ON()
#define SOFT_POWER_CTL_OFF()
#define SOFT_POWER_CHK_INIT()
#define SOFT_POWER_CHK_IS_ON()          (1)

//SEGx�����⸴��ģʽ��IO��Ϊ���룬�ر���������200K����,SEG��ȫ������Ϊ����
#define LINEIN_SEG_ENTER_DETECT()       P2PUS0 |= BIT(6); P2PUS1 &= ~BIT(6);/*//��500ŷ��������*/\
                                        P2PU |= BIT(6); P2DIR |= 0x7f; P1DIR |= 0x40; P2PD &= ~BIT(6); /*��������������Ϊ����*/\
                                        P2PUS0 |= BIT(6); P2PUS1 |= BIT(6)  //���������200K����
//SEGx�˳���⸴��ģʽ���ر�����
#define LINEIN_SEG_EXIT_DETECT()        P2PU |= BIT(6)

#define EAR_SEG_ENTER_DETECT()          P2PUS0 |= BIT(5); P2PUS1 &= ~BIT(5);/*//��500ŷ��������*/\
                                        P2PU |= BIT(5); P2DIR |= 0x6f; P1DIR |= 0x40; P2PD &= ~BIT(5); /*��������������Ϊ����*/\
                                        P2PUS0 |= BIT(5); P2PUS1 |= BIT(5)  //���������200K����
//SEGx�˳���⸴��ģʽ���ر�����
#define EAR_SEG_EXIT_DETECT()           P2PU |= BIT(5)

#define DCIN_INIT()                     P4DIR |= BIT(0) //�ⲿ������,����Ͳ����ڲ�������
#define IS_DCIN()                       (P4 & BIT(0))

#define MP3BT_DECT_INIT()                       //�ϵ�ʱBT/MP3���
#define MP3BT_DECT_IS_BT()              0
#define MP3BT_DECT_END()                        //����ʱҪ�Զ���

//����SDCMD����ADKEY���ʱ����Ҫ����EAR_DETECT_INIT��IS_EAR_IN
#if !SDCMD_REUSE_EARDET && !EAR_ADKEY_REUSE
#ifndef EAR_DETECT_INIT
#define EAR_DETECT_INIT()               P3DIR |= BIT(7); P3PU |= BIT(7);  //�������
#endif
#ifndef EAR_IS_IN
#define EAR_IS_IN()                     (!(P3 & BIT(7)))
#endif
#endif

//����SDCMD����ADKEY���ʱ����Ҫ����LINEIN_DETECT_INIT��IS_LINEIN_IN
#if !SDCMD_REUSE_LINEIN_DET && !AUX_DECT_REUSE_ADKEY
#ifndef LINEIN_DETECT_INIT
#define LINEIN_DETECT_INIT()    		P4DIR |= BIT(0); P4PU |= BIT(0)
#endif
#ifndef IS_LINEIN_IN
#define IS_LINEIN_IN()                  (!(P4 & BIT(0)))
#endif
#endif

#if !LINEIN_DETECT_REUSE_IICCLK
#ifndef LINEIN_DETECT_END
#define LINEIN_DETECT_END()
#endif
#endif

//SD��������(���� ADKEY �� SD_CLK ���ʱ����Ҫ����SD_DETECT_INIT)
#if !SD_DECT_REUSE_ADKEY
#ifndef SD_DETECT_INIT
#define SD_DETECT_INIT()
#endif
#endif

/*****************************************************************************
 * Module    : ���߼�����IO�������
 *****************************************************************************/
#define IO_SLEEP()                      {P0 = 0x00;P1 = 0xb0;P2 = 0x00;\
                                        P0DIR = 0xfd;P1DIR = 0xb7;P2DIR = 0xef;P3DIR = 0xbf;}
#define IO_WAKEUP()                     {P0 = 0x7e;P1 = 0xd7;P2 = 0xcb;P3 = 0xef;\
                                        P0DIR = 0xd1;P1DIR = 0xb7;P2DIR = 0x4e;P3DIR = 0x97;}
#define IS_KEY_WAKEUP                   (!(P1 & BIT(3)))  //ADkey ����

/*****************************************************************************
 * Module    : δ����
 *****************************************************************************/
#define TASK_SHOWTIME_EN                0
#define TASK_IDLE_EN                    0
#define FILE_NAV_EN                     0
#define SLIDER_EN                       0
#define USE_VOL_KEY                     NULL

#define RETURN_SHOW_TIME_SEC            10          //���������˵������£��޲���һ��֮��󷵻���ʾʱ�������
#define LIST_TIME                       80          //�˵��б�listbox��ͣ��ʱ��
#define PROGRESS_TIME                   30          //������progressbox��ͣ��ʱ��
#define MSGBOX_TIME                     5           //��Ϣ��ͣ��ʱ��
#define MSGBOX_SIMPLE                   1           //��ֻ����ʾ������£��Ƿ�ʹ�ü򵥵���Ϣ��
#define FONT_SEL                        BIG_FONT    //����ѡ��

#define SYSTEM_TIME_SET                 0           //���Ӻ�ʱ�����ã�1�����Ӻ�ʱ��������ϵͳ������ 0�����Ӻ�ʱ������������ģʽ��
#define USE_LUNAR                       0           //�Ƿ�ʹ��ũ��

#define LCD_SOFT_RESET                  1           //�Ƿ���Ҫ�����λLCD��
#define LCD_INVERT_DISPLAY              0           //LCD������invert�Ĵ���
#define LCD_X_FIX                       0           //�Ƿ���Ҫ���͵�ַ����
#define LCD_CONTRAST_DEFAULT            10          //Ĭ�ϵ�LCD���Աȶ�

#if !HARD_IR_EN && !SOFT_IR_EN
#undef  IR_HARD_POWERDOWN_EN
#define IR_HARD_POWERDOWN_EN            0
#endif

#if IR_HARD_POWERDOWN_EN
#ifndef IR_PWR_DAT
#define IR_PWR_DAT                      0x45        //�͹��ģ�����ң�ػ��Ѱ���ֵ
#endif
#endif

/*****************************************************************************
 * Module    : aux�������
 *****************************************************************************/

#if (!TASK_AUX_EN && !TASK_AUX_EQ_EN)
#undef  LINEIN_DETECT_EN
#undef  LINEIN_DETECT_INIT
#undef  LINEIN_DETECT
#define LINEIN_DETECT_EN                0
#define LINEIN_DETECT_INIT()
#define LINEIN_DETECT()                 0
#endif

/*****************************************************************************
 * Module    : IIC�����������
 *****************************************************************************/
#if IIC_REUSE_SD || IICCLK_REUSE_USB || IICDAT_REUSE_SDDAT || EAR_DETECT_REUSE_IICCLK || LINEIN_DETECT_REUSE_IICCLK
    #define ICC_REUSE_EN                1
#else
    #define ICC_REUSE_EN                0
#endif
#endif
