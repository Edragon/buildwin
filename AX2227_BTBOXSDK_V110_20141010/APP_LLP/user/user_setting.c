/*****************************************************************************
 * Module    : User
 * File      : user_setting.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 系统参数设置
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_music.h"
//系统参数
IAR_XDATA type_sys_ctl sys_ctl;
IAR_BDATA type_sys_flag sys_flag @ 0x2b;

#define MIN_RREQ 8750

#if FM_SET_VOL_CHANGE_BANK
#pragma constseg="USER_SETTING_CONST"
#else
#pragma constseg=default
#endif
u8 const tbl_volume[VOLUME_MAX+1] =
{
    60-60, 60-43, 60-32, 60-26, 60-22, 60-20, 60-17, 60-14,
    60-12, 60-11, 60-10, 60-8, 60-7, 60-5, 60-3, 60,
};
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
#if 0
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
    bool ie_ea = IE_EA;
    IE_EA = 0;
    sys_ctl.backlightcnt = tbl_backlight[sys_ctl.backlight];
    set_backlight();
    IE_EA = ie_ea;
}
#endif

//恢复出厂设置，这里只对变量进行初始化。禁止有写SPIFlash的操作。
#pragma location="USER_SETTING_SEG"
void user_setting_factory(u8 *setting_buf)
{
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

#if ALARM_EN
    sys_ctl.alarm_switch = 0;                   //alarm off
    setting_buf[PARAM_ALARM_SWITCH] = 0;

    sys_ctl.alarm_hour = 0;                     //alarm time 00:00
    sys_ctl.alarm_min = 0;
    setting_buf[PARAM_ALARM_HOUR] = 0;
    setting_buf[PARAM_ALARM_MIN] = 0;
#endif


    setting_buf[PARAM_BT_AUTO_PLAY] = 0;
    setting_buf[PARAM_FM_CHSEL] = 0;
    setting_buf[PARAM_FM_CH] = 0;
    U16_ACCESS(&setting_buf[PARAM_FM_FREQ]) = MIN_RREQ;
    //param_write8(PARAM_FM_CHSEL, 0);              //保存当前台号
   //param_write16(PARAM_FM_FREQ, MIN_RREQ);                //保存起始频率
   // param_write8(PARAM_FM_CH, 0);
}

//上电加载用户参数
#pragma location="USER_SETTING_SEG"
bool user_setting_load(u8 *setting_buf)
{
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
#endif

    user_change_volume(sys_ctl.volume);

    return true;
}

//保存EQ值
#pragma location="USER_SETTING_SEG"
bool user_set_eq(u8 eq_num)
{
    if (eq_num == sys_ctl.eq_num) {
        return false;                       //没有更改
    }
    if (eq_num < MUSIC_EQ_NUM) {
        sys_ctl.eq_num = eq_num;
        param_write8(PARAM_EQ_NUM, eq_num);
        return true;
    }
    return false;
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
        param_write8(PARAM_MODE_PLAY, mode_play);
    }
}

#if ALARM_EN
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
        param_write8(PARAM_ALARM_HOUR, hour);
        param_write8(PARAM_ALARM_MIN, min);
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
    param_write8(PARAM_ALARM_SWITCH, alarm_switch);
}
#endif

//保存设置音量
#pragma location="USER_SETTING_SEG"
bool user_set_volume(u8 volume)
{
    if(sys_flag.voice_play_flag == 0) {         //播放语音过程中不调整音量
        user_change_volume(volume);
    }

    if (volume == sys_ctl.volume) {
        return false;                             //没有更改
    }
    if (volume <= VOLUME_MAX) {
        sys_ctl.volume = volume;
        param_write8(PARAM_VOLUME, volume);
    }
    return true;
}

//保存设置音量
#pragma location="USER_SETTING_SEG"
bool user_set_hshfvol(u8 volume)
{
    user_change_volume(volume);
    if (volume == sys_ctl.hshf_vol) {
        return false;                             //没有更改
    }
    if (volume <= VOLUME_MAX) {
        sys_ctl.hshf_vol = volume;
        param_write8(PARAM_HSHF_VOLUME, volume);
    }
    return true;
}


//保存设置音量
#pragma location="USER_SETTING_SEG"
bool user_change_volume(u8 volume)
{
    if (volume <= VOLUME_MAX) {
        u8 dac_vol = tbl_volume[volume] + sys_ctl.vol_ex;
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

//mic音量，mic的音量绝对不能超过15
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
u8 user_mixstep_inc(u8 step)
{
    step++;
    if(step > MIX_STEP_MAX) {
        step = 0;
    }
    return step;
}

#pragma location="USER_SETTING_SEG"
u8 user_mixstep_dec(u8 step)
{
    if(step == 0) {
        step = MIX_STEP_MAX;
    } else {
        step--;
    }
    return step;
}

#pragma location="USER_SETTING_SEG"
u8 user_eq_adjust(u8 eq_num)
{
    eq_num++;
    if(eq_num > EQ_BASS) {
        eq_num = EQ_NORMAL;
    }
    return eq_num;
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

//这个函数实际没有使用，只是为了避免IAR编译时报找不到sys_flag的错误
void user_set_mute(void)
{
    sys_flag.mute_flag = 1;
}
