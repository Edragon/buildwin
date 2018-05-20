/*****************************************************************************
 * Module    : User
 * File      : user_setting.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 系统参数设置
 *****************************************************************************/
#ifndef USER_SETTING_H
#define USER_SETTING_H

//记忆信息偏移地址
#define PARAM_LANG_ID           0       //语言          1byte
#define PARAM_CONTRAST          1       //对比度        1byte
#define PARAM_MODE_PLAY         2       //播放模式      1byte
#define PARAM_MODE_RECORD       3       //录音模式      1byte
#define PARAM_VOLUME            4       //系统音量      1byte
#define PARAM_BACKLIGHT         5       //背光延时      1byte
#define PARAM_POWEROFF          6       //自动关机      1byte
#define PARAM_ALARM_HOUR        7       //闹钟时间      2byte
#define PARAM_ALARM_MIN         8
#define PARAM_ALARM_MODE        9       //闹钟模式      1byte
#define PARAM_ALARM_SWITCH      10      //闹钟开关      1byte
#define PARAM_MUSIC_NUM_SD      11      //当前曲目      2byte
#define PARAM_EQ_NUM            13      //EQ选择        1byte
#define PARAM_EBOOK_NUM         14      //当前电子书    2byte
#define PARAM_EBOOK_MODE        16      //阅读模式      1byte
#define PARAM_EBOOK_SPEED       17      //阅读速度      1byte
#define PARAM_VOICE_NUM         18      //VIOCE曲目     2byte
#define PARAM_MUSIC_NUM_USB     20      //U盘曲目       2byte

#define PARAM_FM_CHSEL          22      //当前台号  1byte
#define PARAM_FM_FREQ           23      //当前台的频率 2byte
#define PARAM_MIX_STEP          25      //当前的混响级数 1byte
#define PARAM_FM_TIME           26      //定时fm标志
#define PARAM_FM_HOUR           27
#define PARAM_FM_MIN            28
#define PARAM_FM_TIME_FREQ      29      //2byte
#define PARAM_EBOOK_FPTR        31      //电子书断点地址记忆 4byte

#define PARAM_HSHF_VOLUME       35      //通话音量      1byte
#define PARAM_BT_AUTO_PLAY      36      //蓝牙播放状态下软关机后开机自动回连成功后是否需要自动播放
#define PARAM_REMOTE_DEV_ADDR   37      //连接设备的蓝牙地址    6byte

#define PARAM_BT_NAME           43      //程序动态修改后的蓝牙名字  32byte
#define PARAM_BT_PINCODE_SUM    75      //蓝牙配对密码校验码  1byte
#define PARAM_BT_PINCODE        76      //蓝牙配对密码  4byte

//#define PARAM_BT_SAVE_NUM       61      //配对蓝牙设备数量  1byte
//#define PARAM_BT_PAIRED_INDEX   62      //配对信息索引  2byte，总共有MAX_PAIRED_DEVICE_NUM*2 byte

#define PARAM_FM_CH             80      //第一个是频道数量2byte，后跟所有频道。共160byte

#define VOLUME_DEFAULT      10          //音量默认值
#define HSHFVOL_DEFAULT     12          //通话音量默认值
#define VOLUME_MIN          5           //开机检查最小音量

//一些参数的最大值
#define CONTRAST_MAX        15          //对比度的最大值
#define MUSIC_EQ_NUM        7           //EQ的个数

#ifndef MODE_PLAY_NUM
#define MODE_PLAY_NUM       4           //播放模式的项数
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

#define MIX_STEP_MAX        30          //混响级数最大值

typedef struct {
#if LCD_THEME
    u8 lang_id;                 //语言选项
    u8 lang_num;                //语言种数
    u8 contrast;                //对比度
    u8 backlight_sta;           //当前状态
#endif
    u8 eq_num;                  //EQ选择
    u8 mode_play;               //播放模式
    u8 mode_record;             //录音模式
    u8 volume;                  //音量
    u8 hshf_vol;                //通话音量
    s8 vol_ex;                  //音量补偿

#if ALARM_EN
    u8 alarm_hour;              //闹钟: 时
    u8 alarm_min;               //闹钟: 分
    u8 alarm_switch;            //闹钟开关
    u8 ring_times;              //再响次数
    u32 next_alarm;             //下一闹钟时间
#endif

    u8 bat_val;                 //实际电池电量
    u8 disp_bat_val;            //当前显示的电池电量
    u16 sleep_cnt;              //休眠延时
    u8 lowpower_cnt;            //低电延时

    u8 dig_vol;                 //数字音量
    u8 mic_volume;              //麦克风的音量

#if IR_HARD_POWERDOWN_EN || STANDBY_WAKEUP_EN
    u8 wakeup_delay;            //唤醒后延时
#endif
#if VOL_MAXMIN_MUSIC
    u8 vol_maxmin_music_flag;   //音量调到最大最小值时播放提示音后松开音量调节按键标志
#endif
#if LOWPOWER_WARNING
    u8 lowpower_warning_flag;   //低电提醒标志，播放完低电提醒提示音后恢复正常播放
#endif
} type_sys_ctl;

typedef union
{
    struct {
        u8 mute_flag :              1;      //系统mute标志
        u8 need_mute :              1;      //主要用于动态检测是否需要MUTE功放，如非播放状态
        u8 voice_play_flag :        1;      //播放语音标志
        u8 ir_power_flag :          1;      //红外POWER键关机标志(进入hold_mode)
        u8 lowpower_flag :          1;      //低电标志(进入task_poweroff)
#if BT_SOFT_POWEROFF_AUTO_PLAY || BT_CHANGE_MODE_AUTO_PLAY
        u8 bt_auto_play_flag :      1;      //蓝牙自动播放控制标志
#endif
    };
    u8 all;                                 //最多8个bit
} type_sys_flag;

extern IAR_BDATA type_sys_flag sys_flag;
extern IAR_XDATA type_sys_ctl sys_ctl;

void digital_vol_set(u16 dig_vol);
void agc_anl_set(u8 vol);

bool user_set_eq(u8 eq_num);
void user_set_mode_play(u8 mode_play);
bool user_set_volume(u8 volume);
bool user_set_hshfvol(u8 volume);
bool user_change_volume(u8 volume);
bool change_digital_volume(u8 volume);
bool change_mic_volume(u8 volume);
u8 user_volume_inc(u8 volume);
u8 user_volume_dec(u8 volume);
u8 user_mixstep_inc(u8 step);
u8 user_mixstep_dec(u8 step);
u8 user_eq_adjust(u8 eq_num);
u8 user_mode_play_adjust(u8 mode_play);


void user_set_volex(s8 volex);
void user_set_backlight(u8 backlight);
void user_set_alarm_time(u8 hour, u8 min);
void user_set_alarm_switch(u8 alarm_switch);
__near_func void set_backlight(void);
__near_func void reset_backlight(void);

#endif
