/*****************************************************************************
 * Module    : User
 * File      : user_setting.h
 * Author    : Hanny
 * Function  : 系统参数设置
 *****************************************************************************/
#ifndef USER_SETTING_H
#define USER_SETTING_H

/*****************************************************************************
 * Module    : RTC存放信息偏移地址
 *****************************************************************************/
#define NEXT_ALARM          0       //下一次闹钟时间，4byte

//断点记忆
#define USB_BREAK_FRAME     10        //4BYTE
#define USB_BREAK_POS       14       //4BYTE
#define USB_NAME_CRC        18       //2BYTE

#define SD_BREAK_FRAME      20       //4BYTE
#define SD_BREAK_POS        24       //4BYTE
#define SD_NAME_CRC         28       //2BYTE

//闹铃音乐信息
#define ALM_MUSIC_TYPE      30       //1byte
#define ALM_FILE_NUM        31       //2byte
#define ALM_NAME_CRC        33       //2BYTE

#define FIRST_POWERON_FLAG  35       //1BYTE
/*****************************************************************************
 * Module    : Flash系统信息偏移地址
 *****************************************************************************/
#define PARAM_RANDOM_KEY        0       //随机字串，可用于蓝牙地址  4byte

#define PARAM_LANG_ID           4       //语言          1byte
#define PARAM_CONTRAST          5       //对比度        1byte
#define PARAM_MODE_PLAY         6       //播放模式      1byte
#define PARAM_MODE_RECORD       7       //录音模式      1byte
#define PARAM_VOLUME            8       //系统音量      1byte
#define PARAM_BACKLIGHT         9       //背光延时      1byte
#define PARAM_POWEROFF          10      //自动关机      1byte
#define PARAM_ALARM_HOUR        11      //闹钟时间      2byte
#define PARAM_ALARM_MIN         12
#define PARAM_ALARM_MODE        13      //闹钟模式      1byte
#define PARAM_ALARM_SWITCH      14      //闹钟开关      1byte
#define PARAM_MUSIC_NUM_SD      15      //当前曲目      2byte
#define PARAM_EQ_NUM            17      //EQ选择        1byte
#define PARAM_EBOOK_NUM         18      //当前电子书    2byte
#define PARAM_EBOOK_MODE        20      //阅读模式      1byte
#define PARAM_EBOOK_SPEED       21      //阅读速度      1byte
#define PARAM_VOICE_NUM         22      //VIOCE曲目     2byte
#define PARAM_MUSIC_NUM_USB     24      //U盘曲目       2byte

#define PARAM_FM_CHSEL          26      //当前台号  1byte
#define PARAM_FM_FREQ           27      //当前台的频率 2byte
#define PARAM_MIX_STEP          29      //当前的混响级数 1byte
#define PARAM_FM_TIME           30      //定时fm标志
#define PARAM_FM_HOUR           31
#define PARAM_FM_MIN            32
#define PARAM_FM_TIME_FREQ      33      //2byte
#define PARAM_EBOOK_FPTR        35      //电子书断点地址记忆 4byte


#define PARAM_HSHF_VOLUME       39      //通话音量          1byte
#define PARAM_PLAY_DEVICE       40      //播放设备记忆      1byte
#define PARAM_PHONEBOOK_NUM     41      //电话本联系人总数  2byte
#define PARAM_PHONEBOOK_BT_ADDR 43      //当前已获取电话本信息的远端设备蓝牙地址 6byte

//#define PARAM_BT_AUTO_PLAY            //蓝牙播放状态下软关机后开机自动回连成功后是否需要自动播放
//#define PARAM_REMOTE_DEV_ADDR         //连接设备的蓝牙地址    6byte

//#define PARAM_BT_NAME           43      //程序动态修改后的蓝牙名字  32byte
//#define PARAM_BT_PINCODE_EN     75      //是否使能蓝牙密码配对  1byte
//#define PARAM_BT_PINCODE        76      //蓝牙配对密码  4byte

//#define PARAM_BT_SAVE_NUM       61      //配对蓝牙设备数量  1byte
//#define PARAM_BT_PAIRED_INDEX   62      //配对信息索引  2byte，总共有MAX_PAIRED_DEVICE_NUM*2 byte

#define PARAM_FM_CH             80      //第一个是频道数量2byte，后跟所有频道。共160byte

#define VOLUME_DEFAULT      10          //音量默认值
#define HSHFVOL_DEFAULT     12          //通话音量默认值
#define VOLUME_MIN          5           //开机检查最小音量

//一些参数的最大值
#define CONTRAST_MAX        15          //对比度的最大值
#if MUSIC_DRC_EN
    #define MUSIC_EQ_NUM    7           //EQ的个数
#else
    #define MUSIC_EQ_NUM    6           //EQ的个数
#endif

#ifndef MODE_PLAY_NUM
#define MODE_PLAY_NUM       6           //播放模式的项数
#endif

#define MODE_RECORD_MAX     2           //录音模式的最大值
#define VOLUME_MAX          15          //最大音量
#define BACKLIGHT_NUM       6           //背光设置项数
#define POWEROFF_NUM        5           //自动关机设置项数
#define MAX_ALARM_HOUR      23          //闹钟时间最大值
#define MAX_ALARM_MIN       59
#if ALM_TYPE
#define MAX_ALARM_MODE      1          //闹钟模式最大值
#else
#define MAX_ALARM_MODE      0x7f       //闹钟模式最大值
#endif

typedef struct {
    u8 lang_id;                 //语言选项
    u8 lang_num;                //语言种数
    u8 contrast;                //对比度
    u8 eq_num;                  //EQ选择
    u8 mode_play;               //播放模式
    u8 mode_record;             //录音模式
    u8 volume;                  //音量
    u8 hshf_vol;                //通话音量
    s8 vol_ex;                  //音量补偿
    u8 backlight;               //背光时间选择
    u8 poweroff;                //自动关机时间

    u8 backlightcnt;            //背光时间
    u8 backlight_sta;           //当前状态
    u16 poweroffcnt;            //定时关机时间计数器

    u8 alarm_hour;              //闹钟: 时
    u8 alarm_min;               //闹钟: 分
    u8 alarm_mode;              //闹钟模式
    u8 alarm_switch;            //闹钟开关
    u8 bat_val;                 //实际电池电量
    u8 disp_bat_val;            //当前显示的电池电量
    u16 sleep_cnt;              //暂停或休眠计数
    u8 lowpower_cnt;            //低电延时

    u8 bt_sta;                  //蓝牙状态
    u8 connected;               //蓝牙是否已连接

    u8 voice_play_flag;         //播放语音标志
    u8 mute_flag;               //系统mute标志
    u8 spk_mute;                //功放是否MUTE住标志
    u8 dig_vol;                 //数字音量
    u8 mic_volume;              //麦克风的音量
    u8 time_fm_on;              //定时fm开关标志
    u8 fm_min;
    u8 fm_hour;
    u8 bt_online_flag;          //蓝牙连线标志
    u8 bt_online_change;        //蓝牙连线状态改变
    u32 sclust;                 //录音文件起始簇
    u16 file_num;
    u8 vol_maxmin_music_flag;   //音量调到最大最小值时播放提示音后松开音量调节按键标志
    u8 vol_maxmin_music_play;   //音量调到最大最小值时播放提示音标志
    bool sleep_flag;            //关机标志( 进入hold_mode)
    bool lowpower_flag;         //低电标志
    u8 lowpower_warning_flag;   //低电提醒标志
    u8 lowpower_warning_cycle;  //低电提醒循环延迟，如果低电时
    bool led_status_blink_flag;
    bool soft_poweroff_flag;    //软关机标志 0: 默认状态；1：处于软关机状态
#if BT_AUTO_PLAY
    bool bt_auto_play_flag;     //蓝牙播放状态下软关机后开机自动回连成功后是否需要自动播放
#endif
    bool ir_power_flag;         //红外POWER键硬关机标志
    bool alarm_wakeup_flag;
} type_sys_ctl;

#if ALARM_EN
extern u32 next_alarm;          //下一闹钟时间
#endif

extern IAR_BIT_A bool mute_flag;


extern type_sys_ctl sys_ctl;
extern IAR_CONST type_progress_param tbl_pg_volume;
extern IAR_CONST type_progress_param tbl_digital_volume;
extern IAR_CONST type_progress_param tbl_mic_volume;
extern IAR_XDATA_A u8 bteq_buf[123];

void digital_vol_set(u16 dig_vol);
void agc_anl_set(u8 vol);

void user_set_lang(u8 lang_id);
void user_set_contrast(u8 contrast);
void user_def_eq(void);
bool user_set_eq(u8 eq_num);
void user_set_mode_play(u8 mode_play);
void user_set_mode_record(u8 mode_record);
bool user_set_volume(u8 volume);
bool user_set_hshfvol(u8 volume);
bool user_change_volume(u8 volume);
bool change_digital_volume(u8 volume);
bool change_mic_volume(u8 volume);
u8 user_volume_inc(u8 volume);
u8 user_volume_dec(u8 volume);
u8 user_mode_play_adjust(u8 mode_play);


void user_set_volex(s8 volex);
void user_set_backlight(u8 backlight);
void user_set_poweroff(u8 poweroff);
void user_set_alarm_time(u8 hour, u8 min);
void user_set_alram_mode(u8 alarm_mode);
void user_set_alarm_switch(u8 alarm_switch);
__near_func void set_backlight(void);
__near_func void reset_backlight(void);
__near_func void set_poweroff(void);
#endif
