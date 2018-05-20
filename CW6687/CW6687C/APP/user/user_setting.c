/*****************************************************************************
 * Module    : User
 * File      : user_setting.c
 * Author    : Hanny
 * Function  : 系统参数设置
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_music.h"
#include "user_fm.h"
//系统参数
type_sys_ctl sys_ctl;
#define MIN_RREQ 8750


//音量设置
#pragma constseg="PROGRESS_CONTENT"
extern IAR_CONST type_progress_param tbl_pg_volume = {
    STR_VOLUME_SET, &user_change_volume, &user_set_volume
};

#pragma constseg="PROGRESS_CONTENT"
extern IAR_CONST type_progress_param tbl_digital_volume = {
    STR_ACCOMPANY_VOLUME, &change_digital_volume, NULL
};

#pragma constseg="PROGRESS_CONTENT"
extern IAR_CONST type_progress_param tbl_mic_volume = {
    STR_MIC_VOLUME, &change_mic_volume, NULL
};

#if FM_SET_VOL_CHANGE_BANK
#pragma constseg="USER_SETTING_CONST"
#else
#pragma constseg=default
#endif
#if !SMALL_VOLUME_EN
u8 const tbl_volume[VOLUME_MAX+1] =
{
    60-60, 60-43, 60-32, 60-26, 60-22, 60-20, 60-17, 60-14,
    60-12, 60-11, 60-10, 60-8, 60-7, 60-5, 60-3, 60,
};
#else
u8 const tbl_volume[VOLUME_MAX+1] =
{
    60-60, 60-46, 60-35, 60-29, 60-25, 60-23, 60-20, 60-17,
    60-15, 60-14, 60-13, 60-11, 60-10, 60-8, 60-6, 60-3,
};
#endif
#pragma constseg="USER_SETTING_CONST"
u16 const digital_volume[16] = {0x0000,0x0700, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x7000, 0x9000, 0xa000, 0xb000, 0xc000, 0xd000, 0xe000, 0xef00, 0xff00};

#define WIN_OFFSET 288
#pragma constseg="MUSIC_SEG_CONST"
IAR_CONST u16 tbl_speed[] =
{
    1.0 * WIN_OFFSET,           //正常
    0.8 * WIN_OFFSET,           //慢1
    0.5 * WIN_OFFSET,           //慢2
    1.5 * WIN_OFFSET,           //快1
    2.0 * WIN_OFFSET,           //快2
};
#pragma constseg=default

const u8 tbl_backlight[BACKLIGHT_NUM] = {0xff, 5, 10, 15, 20, 0};
const u16 tbl_poweroff[POWEROFF_NUM] = {0xffff, 900, 1800, 2700, 3600};

//#if 1
#if IS_LCD_DISPLAY
__near_func void set_backlight(void)
{
    if (sys_ctl.backlightcnt) {
        LCD_BACKLIGHT_ON();
    } else {
        LCD_BACKLIGHT_OFF();
    }
}

__near_func void reset_backlight(void)
{
    bool tmp_ietm1 = IE_TM1;
    IE_TM1 = 0;
    sys_ctl.backlightcnt = tbl_backlight[sys_ctl.backlight];
    set_backlight();
    IE_TM1 = tmp_ietm1;
}

__near_func void set_poweroff(void)
{
    bool tmp_ietm1 = IE_TM1;
    IE_TM1 = 0;
    sys_ctl.poweroffcnt = tbl_poweroff[sys_ctl.poweroff];
    IE_TM1 = tmp_ietm1;
}
#endif

//恢复出厂设置，这里只对变量进行初始化。禁止有写SPIFlash的操作。
#pragma location="USER_SETTING_SEG"
void user_setting_factory_sys(u8 *setting_buf)
{
    sys_ctl.lang_id = 0;                        //语言
    setting_buf[PARAM_LANG_ID] = 0;

    sys_ctl.contrast = LCD_CONTRAST_DEFAULT;    //对比度
    setting_buf[PARAM_CONTRAST] = LCD_CONTRAST_DEFAULT;

    sys_ctl.eq_num = 0;                         //EQ选择
    setting_buf[PARAM_EQ_NUM] = 0;

    sys_ctl.mode_play = 0;                      //播放模式
    setting_buf[PARAM_MODE_PLAY] = 0;

    sys_ctl.mode_record = 1;                    //录音模式
    setting_buf[PARAM_MODE_RECORD] = 1;

    sys_ctl.volume = VOLUME_DEFAULT;            //音量
    setting_buf[PARAM_VOLUME] = VOLUME_DEFAULT;

    sys_ctl.hshf_vol = HSHFVOL_DEFAULT;         //通话音量
    setting_buf[PARAM_HSHF_VOLUME] = HSHFVOL_DEFAULT;

    sys_ctl.backlight = 0;                      //背光延时
    setting_buf[PARAM_BACKLIGHT] = 0;
    //reset_backlight();

    sys_ctl.poweroff = 0;                       //自动关机
    setting_buf[PARAM_POWEROFF] = 0;
    //set_poweroff();

#if ALARM_EN
    sys_ctl.alarm_mode = 0;                     //alarm mode : every day
    setting_buf[PARAM_ALARM_MODE] = 0;

    sys_ctl.alarm_switch = 0;                   //alarm off
    setting_buf[PARAM_ALARM_SWITCH] = 0;

    sys_ctl.alarm_hour = 0;                     //alarm time 00:00
    sys_ctl.alarm_min = 0;
    setting_buf[PARAM_ALARM_HOUR] = 0;
    setting_buf[PARAM_ALARM_MIN] = 0;
#endif


    setting_buf[PARAM_FM_CHSEL] = 0;
    setting_buf[PARAM_FM_CH] = 0;
    U16_ACCESS(&setting_buf[PARAM_FM_FREQ]) = MIN_RREQ;
    //sys_param_write8(PARAM_FM_CHSEL, 0);              //保存当前台号
   //param_write16(PARAM_FM_FREQ, MIN_RREQ);                //保存起始频率
   // sys_param_write8(PARAM_FM_CH, 0);
#if FM_TIME_EN
    sys_ctl.time_fm_on = 0;
    setting_buf[PARAM_FM_TIME] = 0;
    sys_ctl.fm_hour = 0;
    sys_ctl.fm_min = 0;
    setting_buf[PARAM_FM_HOUR] = sys_ctl.fm_hour;
    setting_buf[PARAM_FM_MIN] = sys_ctl.fm_min;
#endif

    U32_ACCESS(&setting_buf[PARAM_RANDOM_KEY]) = get_rand_key();

#if BT_AUTO_PLAY
    sys_ctl.bt_auto_play_flag = 0;
    setting_buf[PARAM_BT_AUTO_PLAY] = 0;
#endif
}

//恢复出厂设置
#pragma location="USER_SETTING_SEG"
void user_setting_factory(u8 page, u8 *setting_buf)
{
    switch (page) {
    case PARAM_SYS:
        user_setting_factory_sys(setting_buf);
        break;

    case PARAM_BT:
        break;
    }
}

//上电加载用户参数
#pragma location="USER_SETTING_SEG"
bool user_setting_load_sys(u8 *setting_buf)
{
    sys_ctl.lang_id = setting_buf[PARAM_LANG_ID];           //语言
    if (sys_ctl.lang_id > sys_ctl.lang_num ) {
        return false;                                       //语言参数非法
    }

    sys_ctl.contrast = setting_buf[PARAM_CONTRAST];         //对比度
    if (sys_ctl.contrast > CONTRAST_MAX) {
        return false;                                       //对比度参数非法
    }

    sys_ctl.eq_num = setting_buf[PARAM_EQ_NUM];             //EQ选择
    if (sys_ctl.eq_num >= MUSIC_EQ_NUM) {
        return false;                                       //EQ值非法
    }

    sys_ctl.mode_play = setting_buf[PARAM_MODE_PLAY];       //播放
    if (sys_ctl.mode_play >= MODE_PLAY_NUM) {
        return false;                                       //播放模式非法
    }

    sys_ctl.mode_record = setting_buf[PARAM_MODE_RECORD];   //录音
    if (sys_ctl.mode_record > MODE_RECORD_MAX) {
        return false;                                       //录音模式非法
    }

    sys_ctl.volume = setting_buf[PARAM_VOLUME];             //音量
    //printf("sys_ctl.volume: %d\n",sys_ctl.volume);
    if (sys_ctl.volume > VOLUME_MAX) {
        return false;                                       //音量非法
    }
    if (sys_ctl.volume < VOLUME_MIN) {
        sys_ctl.volume = VOLUME_MIN;                        //设置开机最小音量
    }

    sys_ctl.hshf_vol = setting_buf[PARAM_HSHF_VOLUME];      //通话音量
    if (sys_ctl.hshf_vol > VOLUME_MAX) {
        return false;                                       //音量非法
    }
    if (sys_ctl.hshf_vol < VOLUME_MIN) {
        sys_ctl.hshf_vol = VOLUME_MIN;                      //设置开机最小音量
    }

    sys_ctl.backlight = setting_buf[PARAM_BACKLIGHT];       //背光延时
    if (sys_ctl.backlight >= BACKLIGHT_NUM) {
        return false;
    }
#if ALARM_EN
    sys_ctl.alarm_switch = setting_buf[PARAM_ALARM_SWITCH]; //闹钟开关
    if (sys_ctl.alarm_switch > 1) {
        sys_ctl.alarm_switch = 0;
    }

    sys_ctl.alarm_hour = setting_buf[PARAM_ALARM_HOUR];
    if (sys_ctl.alarm_hour > MAX_ALARM_HOUR) {
        sys_ctl.alarm_hour = 0;
        sys_ctl.alarm_switch = 0;
    }
    sys_ctl.alarm_min = setting_buf[PARAM_ALARM_MIN];
    if (sys_ctl.alarm_min > MAX_ALARM_MIN) {
        sys_ctl.alarm_min = 0;
        sys_ctl.alarm_switch = 0;
    }

    sys_ctl.alarm_mode = setting_buf[PARAM_ALARM_MODE];
    if (sys_ctl.alarm_mode > MAX_ALARM_MODE) {
        sys_ctl.alarm_mode = 0;
    }
#endif
    sys_ctl.poweroff = setting_buf[PARAM_POWEROFF];         //自动关机延时
    if (sys_ctl.poweroff >= POWEROFF_NUM) {
        return false;
    }
#if FM_TIME_EN
    sys_ctl.time_fm_on = setting_buf[PARAM_FM_TIME];
    if(sys_ctl.time_fm_on > 1)
    {
        return false;
    }
    sys_ctl.fm_hour = setting_buf[PARAM_FM_HOUR];
    if(sys_ctl.fm_hour >=24)
      return false;
    sys_ctl.fm_min = setting_buf[PARAM_FM_MIN];
    if(sys_ctl.fm_min >= 60)
      return false;
#endif
    user_change_volume(sys_ctl.volume);

    if (LOAD_U32(&setting_buf[PARAM_RANDOM_KEY]) == 0) {
        return false;                                       //随机字串，不能全为0
    }

#if BT_AUTO_PLAY
    sys_ctl.bt_auto_play_flag = setting_buf[PARAM_BT_AUTO_PLAY];
    if(sys_ctl.bt_auto_play_flag > 1){
        return false;
    }
#endif

    //reset_backlight();
    //set_poweroff();
    return true;
}

//上电加载参数
#pragma location="USER_SETTING_SEG"
bool user_setting_load(u8 page, u8 *setting_buf)
{
    switch (page) {
    case PARAM_SYS:
        return user_setting_load_sys(setting_buf);

    case PARAM_BT:
        return true;
    }
    return false;
}

//保存设置语言
#pragma location="USER_SETTING_SEG"
void user_set_lang(u8 lang_id)
{
    if (lang_id == sys_ctl.lang_id) {
        return;                             //没有更改
    }
    if (lang_id < sys_ctl.lang_num+1) {
        sys_ctl.lang_id = lang_id;
        sys_param_write8(PARAM_LANG_ID, lang_id);
    }
}

//保存设置对比度
#pragma location="USER_SETTING_SEG"
void user_set_contrast(u8 contrast)
{
    if (contrast == sys_ctl.contrast) {
        return;                             //没有更改
    }
    if (contrast <= CONTRAST_MAX) {
        sys_ctl.contrast = contrast;
        sys_param_write8(PARAM_CONTRAST, contrast);
    }
}

//保存背光延时时间
#pragma location="USER_SETTING_SEG"
void user_set_backlight(u8 backlight)
{
    if (backlight == sys_ctl.backlight) {
        return;                             //没有更改
    }
    if (backlight < BACKLIGHT_NUM) {
        sys_ctl.backlight = backlight;
        sys_param_write8(PARAM_BACKLIGHT, backlight);
#if IS_LCD_DISPLAY
        reset_backlight();
#endif
    }
}

//保存自动关机时间
#pragma location="USER_SETTING_SEG"
void user_set_poweroff(u8 poweroff)
{
    if (poweroff == sys_ctl.poweroff) {
#if IS_LCD_DISPLAY
        set_poweroff();
#endif
        return;                             //没有更改
    }
    if (poweroff < POWEROFF_NUM) {
        sys_ctl.poweroff = poweroff;
        sys_param_write8(PARAM_POWEROFF, poweroff);
        set_poweroff();
    }
}

//保存设置播放模式
#pragma location="USER_SETTING_SEG"
void user_set_mode_play(u8 mode_play)
{
    if (mode_play == sys_ctl.mode_play) {
        return;                             //没有更改
    }
    if (mode_play < MODE_PLAY_NUM) {
        sys_ctl.mode_play = mode_play;
        sys_param_write8(PARAM_MODE_PLAY, mode_play);
    }
}
//保存闹钟时间
#pragma location="USER_SETTING_SEG"
void user_set_alarm_time(u8 hour, u8 min)
{
    //printf("set alarm time: %x -> %x\n",sys_ctl.alarm_time,alarm_time);
    if (hour == sys_ctl.alarm_hour && min == sys_ctl.alarm_min) {
        return;                             //没有更改
    }
    if (hour <= MAX_ALARM_HOUR && min <= MAX_ALARM_MIN) {
        sys_ctl.alarm_hour = hour;
        sys_ctl.alarm_min = min;
        sys_param_write8(PARAM_ALARM_HOUR, hour);
        sys_param_write8(PARAM_ALARM_MIN, min);
    }
}

//保存闹钟模式
#pragma location="USER_SETTING_SEG"
void user_set_alram_mode(u8 alarm_mode)
{
    if (alarm_mode == sys_ctl.alarm_mode) {
        return;                             //没有更改
    }
    if (alarm_mode <= MAX_ALARM_MODE) {
        sys_ctl.alarm_mode = alarm_mode;
        sys_param_write8(PARAM_ALARM_MODE, alarm_mode);
    }
}

//保存闹钟开关
#pragma location="USER_SETTING_SEG"
void user_set_alarm_switch(u8 alarm_switch)
{
    if (alarm_switch == sys_ctl.alarm_switch) {
        return;                             //没有更改
    }
    sys_ctl.alarm_switch = alarm_switch;
    sys_param_write8(PARAM_ALARM_SWITCH, alarm_switch);
}

//保存设置录音模式
#pragma location="USER_SETTING_SEG"
void user_set_mode_record(u8 mode_record)
{
    if (mode_record == sys_ctl.mode_record) {
        return;                             //没有更改
    }
    if (mode_record <= MODE_RECORD_MAX) {
        sys_ctl.mode_record = mode_record;
        sys_param_write8(PARAM_MODE_RECORD, mode_record);
    }
}

//保存设置音量
#pragma location="USER_SETTING_SEG"
bool user_set_volume(u8 volume)
{
    if(sys_ctl.voice_play_flag == 0) {          //播放语音过程中不调整音量
        user_change_volume(volume);
    }

    if (volume == sys_ctl.volume) {
        return false;                             //没有更改
    }
    if (volume <= VOLUME_MAX) {
        sys_ctl.volume = volume;
        sys_param_write8(PARAM_VOLUME, volume);
    }
    return true;
}

//保存设置通话音量，通话音量不能设为0
#pragma location="USER_SETTING_SEG"
bool user_set_hshfvol(u8 volume)
{
    if (bt_in_call() && volume) {
        user_change_volume(volume);                 //只有进入通话时，才需要修改这个音量值
    }
    if ((volume == sys_ctl.hshf_vol) || !volume) {
        return false;                               //没有更改
    }
    if (volume <= VOLUME_MAX) {
        sys_ctl.hshf_vol = volume;
        sys_param_write8(PARAM_HSHF_VOLUME, volume);
    }
    return true;
}

//保存设置音量
#pragma location="USER_SETTING_SEG"
bool user_change_volume(u8 volume)
{
    u8 dac_vol;
    if (volume <= VOLUME_MAX) {
        dac_vol = tbl_volume[volume] + sys_ctl.vol_ex;
        if (volume == 0) {
            dac_vol = 0;
        }
        if (dac_vol >= 128) {
            dac_vol = 0;
        } else if (dac_vol > 65) {
            dac_vol = 65;
        }
        dac_set_volume(dac_vol);
    }
    return true;
}

//数字音量
#pragma location="USER_SETTING_SEG"
bool change_digital_volume(u8 volume)
{
    if (volume <= VOLUME_MAX) {
        u16 dac_vol = digital_volume[volume];
        digital_vol_set(dac_vol);
        sys_ctl.dig_vol = volume;
    }
    return true;
}

//mic音量
#pragma location="USER_SETTING_SEG"
bool change_mic_volume(u8 volume)
{
    if (volume <= VOLUME_MAX) {
        agc_anl_set(volume);
        sys_ctl.mic_volume = volume;
    }
    return true;
}

//设置DAC音量模式。数字模式下需要补偿3DB
void user_set_volex(s8 volex)
{
    sys_ctl.vol_ex = volex;
    user_change_volume(sys_ctl.volume);
}

#pragma location="USER_SETTING_SEG"
u8 user_volume_inc(u8 volume)
{
    volume++;
    if(volume > VOLUME_MAX)
        volume = VOLUME_MAX;
    return volume;
}

#pragma location="USER_SETTING_SEG"
u8 user_volume_dec(u8 volume)
{
    if(volume > 0)
        volume--;
    return volume;
}

#pragma location="USER_SETTING_SEG"
u8 user_eq_adjust(u8 eq_num)
{
    if (eq_num >= MUSIC_EQ_NUM) {
        eq_num = EQ_NORMAL;
        eq_disable();
        return eq_num;
    }
    music_eq(eq_num);
    return eq_num;
}

#pragma location="USER_SETTING_SEG"
void user_def_eq(void)
{
    PCON3 &= ~BIT(5);                     //eq clk en
    at_write(EQCON1, 0x00);               //disable EQ
    at_write(EQCON2, 0x00);               //rest
    at_write(EQCON2, 0x01);               //release rest
    at_write(EQCON2, 0x07);               //release reset, eq buf & cof address clear
    while (!(at_read(EQCON2) & BIT(7)));  //eq buf initial done flag

    u16 eqpower = 0x7fff;
    at_write(EQVOLIN, eqpower >> 8);      //input volume
    at_write(EQVOLIN, eqpower);
    for (u8 i=0; i<123; i++) {
        at_write(EQCOF, bteq_buf[i]);
    }
    at_write(EQCON1, 0x47);           //eq en & set eq bank

}

//保存EQ值
#pragma location="USER_SETTING_SEG"
bool user_set_eq(u8 eq_num)
{
    if (eq_num == sys_ctl.eq_num) {
        return false;                       //没有更改
    }
    u8 num_eq = user_eq_adjust(eq_num);
    if (num_eq < MUSIC_EQ_NUM) {
        sys_ctl.eq_num = num_eq;
        sys_param_write8(PARAM_EQ_NUM, eq_num);
        return true;
    }
    return false;
}

//保存设置播放模式
#pragma location="USER_SETTING_SEG"
u8 user_mode_play_adjust(u8 mode_play)
{
    mode_play++;
    if(mode_play > ITEM_LOOP_RAMDOM) {
        mode_play = ITEM_LOOP_ALL;
    }
    return mode_play;
}

