/*****************************************************************************
 * Module    : User
 * File      : user_key_voice.c
 * Author    : junwen
 * Function  : 用户按键音配置
 *****************************************************************************/
#include "include.h"
#include "user_key_voice.h"

#if KEY_VOICE_EN

__root const u8 keyvoice_44100[] = {
    0x00,0x12,0x23,0x35,0x45,0x53,0x60,0x6b,0x74,0x7a,0x7e,0x7f,0x7e,0x7a,0x74,0x6c,0x61,0x54,0x45,
    0x35,0x24,0x13,0x00,0xee,0xdc,0xcb,0xbb,0xad,0xa0,0x94,0x8c,0x85,0x81,0x80,0x81,0x84,0x8a,0x93,
    0x9e,0xaa,0xb9,0xc9,0xda,0xec,0xfe
};

#pragma location="INIT_SEG"
void key_voice_init(void)
{
   PCON1 &= ~BIT(7);        //DAC clock enable ; 配置寄存器前，必须先打开时钟
   at_write(DACCFG, (at_read(DACCFG)&0xfe) | 0x01);  //DAC digital filter/delta-sigma modulator enable
   ATCON0 |= BIT(3);        //DAC analog quantizer enable

   u16 keyv_tbl = (u16)keyvoice_44100;

   KVCADR = 0x80 | (keyv_tbl >> 8);
   KVCADR = keyv_tbl;       //key voice DMA address
   KVCADR = keyv_tbl + sizeof(keyvoice_44100);

   KVCON1 |= BIT(1);        //enable MP3 play when play key voice
   KVCON2 = 0x2d;           //0x00:0db; 0x01:0db/2; 0x02 0db/4; 0x03:0db/8; 0x04:0db/16; 0x05:0db/32; 0x06:0db/64; 0x07:0db/128;
}

#endif
