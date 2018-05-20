/*****************************************************************************
 * Module    : Config
 * File      : config_setting.h
 * Author    : Hanny
 * Function  : 用户配置文件
 *****************************************************************************/
#ifndef USER_CONFIG_H
#define USER_CONFIG_H


/*****************************************************************************
 * Module    : 系统配置
 *****************************************************************************/

#define SYS_CRY                         CRY_32K         //系统时钟源选择
#define BUCK_MODE_EN                    0               //电源模式选择

#define DAC_INIT_MODE                   1               //DAC初始化模式。0为快速初始化，1为去啪声初始化
#define MAKECODE_ID                     0               //升级BOOT程序选择
//SPIFlash是否支持4K Sector擦除。支持4K Sector支持，可以减少参数区大小，节约SPIFlash空间。
//      缺点是影响SPIFlash的擦写均衡性，同时也会影响SPIFlash的兼容性。
#define SPI_4K_SECTOR_SUPPORT           0               //SPIFlash是否支持4K Sector擦除

#define DAC_MIX_OUT_EN                  0               //0:双声道方案. 1:单声道方案_左声道混合右声道输出

/*****************************************************************************
 * Module    : 主菜单及功能相关配置
 *****************************************************************************/

#define TASK_MENU_EN                    0   //是否打开主菜单功能。一般应用于LCD点阵屏
#define TASK_MUSIC_EN                   1   //是否打开MUSIC功能
#define TASK_AUX_EN                     1   //是否打开AUX功能
#define TASK_AUX_EQ_EN                  0   //是否打开AUX EQ功能
#define TASK_FM_EN                      1   //是否打开FM功能
#define TASK_USBDEV_EN                  1   //是否打开USB设备功能
#define TASK_POWEROFF_EN                1   //是否打开关机功能
#define TASK_BT_EN                      1   //是否打开蓝牙功能
#define TASK_RTC_EN                     1   //是否打开时钟功能
#define TASK_SPEAKER_EN                 1   //是否打开扩音器模块
#define TASK_MIC_EN                     1   //是否打开MIC录音模块
#define TASK_VOICE_EN                   1   //是否打开录音播放模块
#define TASK_TOMCAT_EN                  0   //是否打开汤姆猫功能 (需要SPIFlash支持4K Sector擦除)
#define TASK_HOLD_EN                    1   //是否打开空闲时进入HOLD状态
#define TASK_UPDATE_FW_EN               1   //是否打开离线升级模式（U盘或者TF卡均可）
#define TASK_BTHID_EN                   1   //是否打开独立自拍器模式

/*****************************************************************************
 * Module    : 主题选择
 *****************************************************************************/

//整体主题，必须要求有下列的全套才能选。否则请用下面的局部主题来定制
#define THEME_SELECT                    THEME_LEDSEG_5C7S

/*****************************************************************************
 * Module    : 低功耗相关配置
 *****************************************************************************/

//TASK_HOLD_EN 为低功耗的总控制开关。关闭TASK_HOLD_EN则关闭所有低功耗模式
#define STANDBY_TIME                	100 //进入休眠待机(HOLD模式)时间(单位为秒)
#define HOLD_POWER_TYPE                 0   //HOLD MODE下功耗选择 0:正常电压，1:低电压
#define TASK_HOLD_DAC_CLOSE             1   //是否需要彻底关掉dac，若彻底关掉，唤醒时有啪声，需要先mute住功放
#define BT_DISCONNECT_POWEROFF_TIME     0   //蓝牙HOLD模式下，蓝牙未连接时进入关机的时间(单位为秒)，0表示不启用此功能

#define MUSIC_AUTO_STANDBY_EN           1   //音乐播放任务下，是否需要自动进入休眠待机
#define FM_AUTO_STANDBY_EN              1   //FM收音任务下，是否需要自动进入休眠待机
#define BT_AUTO_STANDBY_EN              1   //BT任务下，是否需要自动进入休眠待机
#define AUX_AUTO_STANDBY_EN             1   //AUX/AUX EQ任务下，是否需要自动进入休眠待机

/*****************************************************************************
 * Module    : 开关机相关配置
 *****************************************************************************/

#define SOFT_POWER_ON_OFF               1   //是否使用软开关机

//如果使用软关机，3uA软关机 和 20 uA软关机 必须且只能打开一个。
#define SOFT_POWER_OFF_20UA             1   //20uA软关机
#define SOFT_POWER_OFF_3UA              0   //3uA软关机 （CW6687E与CW6687G不支持）

/*****************************************************************************
 * Module    : 提示音选择
 *****************************************************************************/

#define WARNING_MUSIC	                1   //是否使用提示音。为0则完全关闭提示音功能
#define WARNING_VOLUME                  10  //提示音的音量

#define WARNING_POWER_ON                1   //开机提示音
#define WARNING_TASK_BT                 1   //蓝牙任务提示音
#define WARNING_TASK_CLOCK              1   //时钟任务提示音
#define WARNING_TASK_AUX                1   //AUX/AUX EQ任务提示音
#define WARNING_TASK_FM                 1   //FM任务提示音
#define WARNING_TASK_VOICE              1   //录音播放任务提示音
#define WARNING_TASK_SPEAKER            1   //扩音器任务提示音
#define WARNING_TASK_MIC                1   //MIC录音任务提示音
#define WARNING_TASK_USBDEV             1   //PC任务提示音
#define WARNING_TASK_TOMCAT             0   //汤姆猫任务提示音
#define WARNING_BT_CONNECT              1   //蓝牙连接提示音
#define WARNING_BT_DISCONNECT           1   //蓝牙断开连接提示音
#define WARNING_LOWPOWER                1   //低电提示（不关机）
#define WARNING_POWER_OFF               1   //关机或低电关机提示音
#define WARNING_VOL_MAXMIN              1   //是否需要音量调节到最大或最小时提示音
#define WARNING_USB_SD                  1   //播放U盘或SD卡提示音

/*****************************************************************************
 * Module    : 音乐任务相关配置
 *****************************************************************************/

#define MUSIC_ENCRYPT_EN                0   //是否需要MP3文件加密接口
#define MUSIC_LRC_EN                    0   //是否需要MP3歌词显示
#define AUTO_CHANGE_DEVICE              1   //双设备时自动切换设备
#define K_MODE_CHANGE_DEVICE            1   //双设备时使用模式键切换设备
#define MUSIC_MODE_AUTO                 1   //插入设备，自动切换到音乐播放
#define BREAK_POINT_EN                  1   //断点记忆
#define AB_REPEAT_EN                    0   //AB复读
#define PLAY_DEVICE_SAVE_EN             0   //记忆播放设备
#define FOLDER_SELECT                   0   //文件夹选择
#define MUSIC_DRC_EN                    1   //是否需要DRC功能

//MUSIC_ID3_EN为ID3功能的总开关，关闭MUSIC_ID3_EN则关闭所有格式的ID3功能
#define MUSIC_ID3_EN                    0   //是否打开ID3总开关
#define MP3_ID3_EN                      0   //是否打开mp3的ID3
#define WAV_ID3_EN                      0   //是否打开wav的ID3
#define WMA_ID3_EN                      0   //是否打开wma的ID3

/*****************************************************************************
 * Module    : FM模块相关配置
 *****************************************************************************/
//FM类型选择
#define FM_TYPE                         FM_RTC6218
#define SEEK_DISPLAY_CHANNEL_NUM        1               //搜台过程中显示搜到的频道
#define FM_VOUT_THROUGH_MCU             0               //FM VOUT是否通过主控
#define FM_CLK_SEL                      SHARE_CRYSTAL_32K //FM时钟选则
#define FM_TIME_EN                      0               //定时收听电台
#define FM_SET_VOL_CHANGE_BANK          1               //FM设置音量是否切bank
#define FM_RECORD_EN                    0               //是否打开FM录音功能

/*****************************************************************************
 * Module    : USB设备模块相关配置
 *****************************************************************************/
//USB设备枚举类型
#define USBDEV_ENUM_TYPE               (USB_DEV_MASS | USB_DEV_SPEAKER | USB_DEV_MIC | USB_DEV_HID)

/*****************************************************************************
 * Module    : AUX 相关配置
 *****************************************************************************/

#define MIC_CHANNEL_SEL                 MCHANNEL_1          //定制MIC的通道
#define FM_CHANNEL_SEL                  CHANNEL_02_03       //定制FM的通道
#define AUX_CHANNEL_SEL                 CHANNEL_00_01       //选择LINEIN的通道
#define LINEIN_DETECT_EN                1                   //是否使用LINEIN检测
#define LINEIN_SEG_REUSE                0                   //LINEIN检测复用LED SEG
#define LINEIN_DETECT_REUSE_IICCLK      0                   //LINEIN检测复用IIC CLK
#define SDCMD_REUSE_LINEIN_DET          0                   //LINEIN检测复用SD CMD
#define AUX_RECORD_EN                   1                   //是否打开AUX录音功能

/*****************************************************************************
 * Module    : 录音相关配置
 *****************************************************************************/

#define RECORD_FILE_NAME                SHORT_NAME          //是否使用长文件名录音
#define REC_TYPE                        REC_MP3             //录音类型选择MP3
#define MIC_DIRECT_OUT_EN               0                   //各个模式添加mic直通

/*****************************************************************************
 * Module    : 蓝牙相关配置
 *****************************************************************************/

#define BT_ALL_TASK                     0     //是否全程使用蓝牙（蓝牙后台）
#define BT_CALL_PRIVATE                 0     //是否强制使用私密接听（手机端接听）
#define BT_RING_NUMBER                  1     //是否需要来电报号
#define BT_NAME_WITH_ADDR               0     //蓝牙名称是否显示3位地址（调试用，例如：BW-BTBOX-***）
#define BT_SAMPLERATE_16K               1     //蓝牙通话是否使用16k采样，0为8k采样
#define BT_HSHF_EN                      1     //是否打开通话HSHF服务
#define BT_A2DP_EN                      1     //是否打开音频A2DP服务
#define BT_SPP_EN                       1     //是否打开蓝牙串口SPP服务
#define BT_SPP_MSC_EN                   1     //是否使用SPP获取MUSIC曲目及播放列表（需同时打开BT_ALL_TASK及BT_SPP_EN宏定义）
#define BT_SPP_EQ_EN                    0     //是否打开蓝牙在线调试EQ功能（需同时打开BT_ALL_TASK及BT_SPP_EN宏定义）
#define BT_HID_EN                       1     //是否打开蓝牙HID（自拍功能）服务
#define BT_HID_MANU                     0     //蓝牙HID是否需要手动连接（即蓝牙默认不连接HID）
#define BT_POWER_ON_CONN_TIMES          3     //上电重连次数
#define BT_TIME_OUT_CONN_TIMES          24    //远距离断线回连次数
#define BT_NOISE_VALUE                  1536  //噪声阈值（环境噪声的大小,低于阈值认为是噪音）
#define BT_FAR_DELAY                    12    //远端延迟（远端声音到近端MIC的延迟,一般为8~16）
#define BT_ECHO_ATTEN                   5     //回音衰减（消除回音的级别，级别越高衰减越明显，双边通话效果变差）
#define BT_ANA_GAIN                     6     //MIC模拟增益,范围为-1~9
#define BT_DIGIT_GAIN                   96    //MIC数字增益,范围为0~127
#define BT_VOLUME_MAX                   0     //蓝牙模式下是否要默认音量最大
#define BT_AUTO_PLAY                    0     //蓝牙播放状态下软关机后开机自动回连成功后是否需要自动播放(默认不需要)
#define BT_DYNAMIC_NOISE                DYNAMIC_DAC_SPK     //蓝牙播放是否使用动态降噪
#define BT_ANTI_LOST                    1     //是否打开蓝牙防丢功能
#define PLAY_DETECT_TO_BT               0     //非蓝牙模式下检测到手机蓝牙播放音乐，则切换到蓝牙
#define BT_3WCALL_EN                    1     //是否支持三方通话
#define BT_PHONE_BOOK_EN                0     //是否支持电话本功能

//蓝牙模块WAKEUP IO
#define BT_IS_SLEEP                 	((0 == bt_sleep_cnt) && (BTCON2 & BIT(5)))
#define BTLDO_POWCTRL_SET
#define BTLDO_DIR_INIT()
#define BTLDO_EN()
#define BTLDO_DIS()

/*****************************************************************************
 * Module : 唤醒及ALARM 相关配置
 *****************************************************************************/

#define ALARM_EN                        1       //闹钟
#define ALARM_WKUP_POWERDOWN            1       //power down关机后，闹钟可以唤醒开机
#define WKO_WKUP_POWERDOWN              1       //关机是否使用WKO唤醒
#define IRTCC_WKUP_EN                   0       //IRRTC WAKE_UP 脚使用
#define ALM_TYPE                        0       //选择闹钟周期的配置，0为复选框配置，1为直接用列表配置

/*****************************************************************************
 * Module    : 系统细节配置
 *****************************************************************************/

#define MP3BT_DECT_EN                   0       //是否使用拨动开关开机检测MP3和BT的功能。
#define USE_P35_USBPOW_CTL              0       //P35脚的使用选择。用做MUTE脚时为0，用做控制USB供电时选1，其他用处时为2
#define LED_GREEN_BLINK                 0       //是否使用绿灯闪烁
#define LED_BLUE_BLINK                  0       //是否使用蓝灯闪烁（双LED时，蓝灯主要用于蓝牙模式）
#define EAR_DETECT_EN                   1       //是否使用耳机检测（耳机插入时MUTE功放）
#define EAR_SEG_REUSE                   0       //耳机检测复用LED SEG
#define EAR_ADKEY_REUSE                 1       //耳机检测复用ADKEY
#define EAR_DETECT_REUSE_IICCLK         0       //是否复用IICCLK检测耳机插入
#define SDCMD_REUSE_EARDET              0       //SD CMD复用耳机检测
#define IR_HARD_POWERDOWN_EN            0       //红外POWER键硬关机使能(默认不使能)

#define IIC_REUSE_SD                    1       //IIC接口是否需要与SD复用
#define IICCLK_REUSE_USB                0       //IICCLK是否与USB复用（主要用于无晶振，SDCLK复用推12M时钟时）
#define IICDAT_REUSE_SDDAT              0       //IICDAT是否与SDDAT复用（主要用于无晶振，SDCLK复用推12M时钟时）

#define LOWPOWER_WARNING_CYCLE          100     //低电提醒预报周期  CYCLE*100ms
#define LOWPOWER_WARNING_VOLTAGE        0x7B    //低电提醒电压控制，0x76-3.1V  0x79-3.2V, 0x7B-3.3V
#define LOWPOWER_VOLTAGE                0x79    //低电关机电压控制，0x76-3.1V  0x79-3.2V, 0x7B-3.3V
#define LOWER_POWER_DISPLAY_OFF         1       //低电时是否需要显示一下OFF并且在3秒后关机
#define LED_STATUS_BLINK                0       //是否使用电池电量状态灯（低电时闪烁）
#define MODE_CHANGE_BLUE_ON             0       //是否需要模式切换时蓝灯闪烁一下
#define POWERON_TO_FM                   0       //是否需要开机检测进入FM
#define SD_DECT_REUSE_ADKEY             0       //是否使用ADKEY检测SD卡
#define AUX_DECT_REUSE_ADKEY            1       //是否使用ADKEY检测AUX
#define LED_REUSE_ADKEY                 0       //LED是否复用ADKEY（主要用于IO不够时）
#define FIRST_PLAY_UDISK                0       //是否需要优先播放U盘
#define EQ_DISPLAY_NUM                  0       //是否使用EQ显示Eq0-Eqn(默认显示EQ模式名称的前三个字母)
#define DEBUG_LOADER_DIS                0       //是否屏蔽LOADER调试信息. 1关闭P01打印，0开P01打印
#define VCMBUF_EN                       0       //0不使用VCMBUF,1 使用VCMBUF直推耳机 (VCMBUF与P01绑在一起，使用VCMBUF时需要关闭P01打印的两个宏DEBUG_LOADER_DIS->1, DEBUG_UART_SEL->0)
#define DEBUG_UART_SEL                  1       //串口(使用P01串口打印)
#define SDCLK_REUSE_ADKEY               0       //是否复用SD_CLK作ADKEY检测(使用该宏需要把宏ADKEY_PORT选成SD_CLK对应的脚)
#define FFT_LED_EN                      0       //是否使用LED频谱灯
#define MICBIAS_EN                      0       //是否使用MICBIAS给MIC供电，0：为使用VDDHP给MIC供电

#define SMALL_VOLUME_EN                 0       //是否使用小音量表(1.96v), 0为大音量表(2.8v)
#define INTERNAL_CHARGE_EN              0       //是否使用内部充电功能（CW6687E与CW6687G不支持）
#define LED_POWER_BLINK                 0       //是否使用充电指示灯

//LDO MODE下的PMU配置
#define USER_OTHER_INIT()               lqfp48_pmu_init();P3PD |= BIT(6) //P36上电会翻转，做COM

/*****************************************************************************
 * Module    : 按键及外设IO配置
 *****************************************************************************/

#define USE_ADKEY                       2           //ADKey的使用，0为不使用，1为1组ADKey，2为2组ADKey
#define USE_IOKEY                       0           //IOKey的使用，0为不使用，1为使用IOKey
#define USE_WKOKEY                      1           //1为检测WKO PIN上的按键，0不用此功能
#define KEY_DOUBLE_CLICK_EN             0           //使能某一个按键的双击功能,按键在宏DOUBLE_CLICK_KEY中配置
#define DOUBLE_CLICK_KEY                KU_PLAY_POWER   //支持双击的按键
#define LONG_PRESS_POWERON              1           //长按某键才能开机。
#define WKO_OUTPUT_EN                   0           //WKO用于输出。作一般IO口使用。输出高/低分别是通过上拉10K和下拉60K来实现的。

//sd_port值:0: P20 P21 P27,    1: P20 P21 P27, clk复用检测,    2: P30 P31 P32,    3: P30 P31 P32, clk复用检测
#define SD_PORT_SEL                     3
#define SD_CMD_REUSE_SD_DETECT          0
#define HARD_IR_EN                      0           //红外遥控（硬解，只有外部接32K晶振，才能使用硬解，其它请使用软解）
#define SOFT_IR_EN                      0           //红外遥控（NEC软解）

#define ADKEY_PORT                      ADKEY_P13   //选择第1组ADKEY的IO
#define ADKEY2_PORT                     ADKEY_P33   //选择第2组ADKEY的IO
#define ADKEY_INT_PU10K                 0           //第1组ADKEY是否使用内部10K上拉（只有3个按键时可以使用）
#define ADKEY2_INT_PU10K                0           //第2组ADKEY是否使用内部10K上拉（只有3个按键时可以使用）

#define K_PLAY_INIT()
#define IS_K_PLAY()                     (0)
#define K_NEXT_INIT()
#define IS_K_NEXT()                     (0)
#define K_PREV_INIT()
#define IS_K_PREV()                     (0)

#if ((SD_PORT_SEL == 0) || (SD_PORT_SEL == 2) && !SD_CMD_REUSE_SD_DETECT) //单独IO检测
#define SD_DETECT_INIT()                P2DIR |= BIT(6); P2PU |= BIT(6);
#define IS_SD_IN()                      (!(P2 & BIT(6)))
#endif

#define OTHER_ADC_CHECK()                           		//其它ADKE1的检测，如AUX/SD/耳机检测复用ADKEY1
#define OTHER_ADC2_CHECK()              other_adc_check()	//其它ADKE2的检测，如AUX/SD/耳机检测复用ADKEY2

/*****************************************************************************
 * Module    : IIC的相关配置
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
 * Module    :LED IO配置
 *****************************************************************************/

//绿色LED
#define LED_GREEN                       //5
#define LED_GREEN_INIT()                //P1DIR &= ~BIT(LED_GREEN); P1 &= ~BIT(LED_GREEN)
#define LED_GREEN_ON()                  //P1 |= BIT(LED_GREEN)
#define LED_GREEN_OFF()                 //P1 &= ~BIT(LED_GREEN)
#define LED_GREEN_TOG()                 //P1 ^= BIT(LED_GREEN)
#define LED_GREEN_IS_ON()               //(P1 & BIT(LED_GREEN))

//蓝色LED
#define LED_BLUE                        //5
#define LED_BLUE_INIT()                 //P1DIR |= BIT(LED_BLUE); P1PU &= ~BIT(LED_BLUE); P1PUS1 &= ~BIT(LED_BLUE);P1PUS0 |= BIT(LED_BLUE);
#define LED_BLUE_ON()                   //P1PU |= BIT(LED_BLUE)
#define LED_BLUE_OFF()                  //P1PU &= ~BIT(LED_BLUE)
#define LED_BLUE_TOG()                  //P1PU ^= BIT(LED_BLUE)
#define LED_BLUE_IS_ON()                //(P1PU & BIT(LED_BLUE))

//充电红色LED
#define LED_POWER                       //1
#define LED_POWER_INIT()                //P3PU &= ~BIT(LED_POWER); P3DIR |= BIT(LED_POWER);  P3PUS0 |= BIT(LED_POWER);P3PUS1 &= ~BIT(LED_POWER)
#define LED_POWER_ON()                  //P3PU |= BIT(LED_POWER)
#define LED_POWER_OFF()                 //P3PU &= ~BIT(LED_POWER)
#define LED_POWER_TOG()                 //P3PU ^= BIT(LED_POWER)
#define LED_POWER_IS_ON()               0//(P3PU & BIT(LED_POWER))

/*****************************************************************************
 * Module    : 其它杂项IO相关配置
 *****************************************************************************/
//MUTE功放控制
#define SPEAKER_MUTE()                  P3DIR &= ~BIT(5); P3 |= BIT(5)
#define SPEAKER_UNMUTE()                P3DIR &= ~BIT(5); P3 &= ~BIT(5)

//电源软开关控制
#define SOFT_POWER_CTL_INIT()
#define SOFT_POWER_CTL_ON()
#define SOFT_POWER_CTL_OFF()
#define SOFT_POWER_CHK_INIT()
#define SOFT_POWER_CHK_IS_ON()          (1)

//SEGx进入检测复用模式，IO设为输入，关闭下拉，打开200K上拉,SEG口全部设置为输入
#define LINEIN_SEG_ENTER_DETECT()       P2PUS0 |= BIT(6); P2PUS1 &= ~BIT(6);/*//开500欧加速上拉*/\
                                        P2PU |= BIT(6); P2DIR |= 0x7f; P1DIR |= 0x40; P2PD &= ~BIT(6); /*所有推屏口设置为输入*/\
                                        P2PUS0 |= BIT(6); P2PUS1 |= BIT(6)  //开正常检测200K上拉
//SEGx退出检测复用模式，关闭上拉
#define LINEIN_SEG_EXIT_DETECT()        P2PU |= BIT(6)

#define EAR_SEG_ENTER_DETECT()          P2PUS0 |= BIT(5); P2PUS1 &= ~BIT(5);/*//开500欧加速上拉*/\
                                        P2PU |= BIT(5); P2DIR |= 0x6f; P1DIR |= 0x40; P2PD &= ~BIT(5); /*所有推屏口设置为输入*/\
                                        P2PUS0 |= BIT(5); P2PUS1 |= BIT(5)  //开正常检测200K上拉
//SEGx退出检测复用模式，关闭上拉
#define EAR_SEG_EXIT_DETECT()           P2PU |= BIT(5)

#define DCIN_INIT()                     P4DIR |= BIT(0) //外部有下拉,这里就不开内部下拉了
#define IS_DCIN()                       (P4 & BIT(0))

#define MP3BT_DECT_INIT()                       //上电时BT/MP3检测
#define MP3BT_DECT_IS_BT()              0
#define MP3BT_DECT_END()                        //复用时要自定义

//复用SDCMD或者ADKEY检测时不需要定义EAR_DETECT_INIT及IS_EAR_IN
#if !SDCMD_REUSE_EARDET && !EAR_ADKEY_REUSE
#ifndef EAR_DETECT_INIT
#define EAR_DETECT_INIT()               P3DIR |= BIT(7); P3PU |= BIT(7);  //耳机检测
#endif
#ifndef EAR_IS_IN
#define EAR_IS_IN()                     (!(P3 & BIT(7)))
#endif
#endif

//复用SDCMD或者ADKEY检测时不需要定义LINEIN_DETECT_INIT及IS_LINEIN_IN
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

//SD检测脚配置(复用 ADKEY 或 SD_CLK 检测时不需要定义SD_DETECT_INIT)
#if !SD_DECT_REUSE_ADKEY
#ifndef SD_DETECT_INIT
#define SD_DETECT_INIT()
#endif
#endif

/*****************************************************************************
 * Module    : 休眠及唤醒IO相关配置
 *****************************************************************************/
#define IO_SLEEP()                      {P0 = 0x00;P1 = 0xb0;P2 = 0x00;\
                                        P0DIR = 0xfd;P1DIR = 0xb7;P2DIR = 0xef;P3DIR = 0xbf;}
#define IO_WAKEUP()                     {P0 = 0x7e;P1 = 0xd7;P2 = 0xcb;P3 = 0xef;\
                                        P0DIR = 0xd1;P1DIR = 0xb7;P2DIR = 0x4e;P3DIR = 0x97;}
#define IS_KEY_WAKEUP                   (!(P1 & BIT(3)))  //ADkey 按下

/*****************************************************************************
 * Module    : 未整理
 *****************************************************************************/
#define TASK_SHOWTIME_EN                0
#define TASK_IDLE_EN                    0
#define FILE_NAV_EN                     0
#define SLIDER_EN                       0
#define USE_VOL_KEY                     NULL

#define RETURN_SHOW_TIME_SEC            10          //在日历跟菜单任务下，无操作一定之间后返回显示时间的任务
#define LIST_TIME                       80          //菜单列表listbox的停留时间
#define PROGRESS_TIME                   30          //进度条progressbox的停留时间
#define MSGBOX_TIME                     5           //消息框停留时间
#define MSGBOX_SIMPLE                   1           //在只有提示的情况下，是否使用简单的消息框
#define FONT_SEL                        BIG_FONT    //字体选择

#define SYSTEM_TIME_SET                 0           //闹钟和时间设置，1：闹钟和时间设置在系统设置下 0：闹钟和时间设置在日历模式下
#define USE_LUNAR                       0           //是否使用农历

#define LCD_SOFT_RESET                  1           //是否需要软件复位LCD屏
#define LCD_INVERT_DISPLAY              0           //LCD屏设置invert寄存器
#define LCD_X_FIX                       0           //是否需要发送地址修正
#define LCD_CONTRAST_DEFAULT            10          //默认的LCD屏对比度

#if !HARD_IR_EN && !SOFT_IR_EN
#undef  IR_HARD_POWERDOWN_EN
#define IR_HARD_POWERDOWN_EN            0
#endif

#if IR_HARD_POWERDOWN_EN
#ifndef IR_PWR_DAT
#define IR_PWR_DAT                      0x45        //低功耗，红外遥控唤醒按键值
#endif
#endif

/*****************************************************************************
 * Module    : aux相关配置
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
 * Module    : IIC复用相关配置
 *****************************************************************************/
#if IIC_REUSE_SD || IICCLK_REUSE_USB || IICDAT_REUSE_SDDAT || EAR_DETECT_REUSE_IICCLK || LINEIN_DETECT_REUSE_IICCLK
    #define ICC_REUSE_EN                1
#else
    #define ICC_REUSE_EN                0
#endif
#endif
