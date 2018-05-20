/*****************************************************************************
 * Module    : User
 * File      : sound_detect.c
 * Author    : jingfa
 * Email     :
 * Function  : 声音能量检测
 *****************************************************************************/
#include "include.h"

__near_func void dac_fade_in_2(bool digital);
__near_func void dac_fade_out_2(void);
__near_func u8 sound_detect(u32 v_pow);
__near_func void sound_set_flag(u8 flag, u8 cnt);

type_sound_detect_ctl sound_detect_ctl;

void sound_detect_init(void *ptr, u16 len)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    sound_detect_ctl.ptr = ptr;
    sound_detect_ctl.samples = len;
    sound_set_flag(0, 0);
    IE_EA = ie_ea;
}

void sound_detect_enable(bool fade_en)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    sound_detect_ctl.delay_cnt = 0;
    sound_detect_ctl.fade_en |= fade_en;
    sound_set_flag(0, 0);
    IE_EA = ie_ea;
}

void sound_detect_disable(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    sound_detect_ctl.delay_cnt = 0xff;
    sound_detect_ctl.fade_en &= ~BIT(0);
    sound_set_flag(0, 0);
    IE_EA = ie_ea;
}

//能量检测接口
__near_func void sound_detect_process(void)
{
    if(sound_detect_ctl.delay_cnt == 0) {
        sound_detect_ctl.delay_cnt = 4;     //5ms检测1次
        u16 v_pow = voice_maxpow(sound_detect_ctl.ptr, sound_detect_ctl.samples);
        //printf("%04x ",v_pow);
        sound_detect(v_pow);
        if(sound_detect_ctl.fade_en == 0x81){
            if (sound_flag()) {
                //uart_putchar('1');
                if(!is_sys_mute()) {
                    dac_fade_in_2(1);
                }
            } else {
                //uart_putchar('0');
                dac_fade_out_2();
            }
        }
    } else if(sound_detect_ctl.delay_cnt < 5) {
        sound_detect_ctl.delay_cnt--;
    }
}

void sound_detect_ctl_hook(bool enable)
{
#if BT_DYMANIC_SUPPRESSION || FM_SOUND_DETECT_EN || AUX_SOUND_DETECT_EN
    bool ie_ea = IE_EA;
    IE_EA = 0;
    if(enable) {
        sound_detect_ctl.fade_en |= 0x80;
    } else {
        sound_detect_ctl.fade_en &= ~0x80;
    }
    IE_EA = ie_ea;
#endif
}

//sbc能量检测接口
#pragma location="BT_MODE_API"
__near_func void sbc_sound_detect_hook(void)
{
#if BT_DYMANIC_SUPPRESSION
    sound_detect_ctl.delay_cnt = 0;
    sound_detect_process();
#endif
}

//MP3能量检测接口
#pragma location="MP3_DEC_COM"
__near_func void mp3_sound_detect_hook(void)
{
}
