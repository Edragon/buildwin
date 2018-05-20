/*****************************************************************************
* Module    : IO
* File      : io_key.c
* Author    : Hanny
* Function  : 用户自定义按键定义
*****************************************************************************/
#ifndef CONFIG_C

#define KEY1_NUM 9
#define KEY2_NUM 10

#define TBL_KEY1_SIZE       (2 * (KEY1_NUM+1))
#define TBL_KEY2_SIZE       (2 * (KEY2_NUM+1))
/*
//NO.15-152 Demo板
//pu=10.000K, total 9 pd
//pd=0.000K vol=0.000V adc=0.0_0x0 cmp=0x15      //PLAY
//pd=2.000K vol=0.550V adc=42.5_0x2A cmp=0x34    //PREV
//pd=3.300K vol=0.819V adc=63.3_0x3F cmp=0x4A    //MODE
//pd=5.100K vol=1.115V adc=86.1_0x56 cmp=0x61    //VOL+
//pd=7.500K vol=1.414V adc=109.3_0x6D cmp=0x76   //VOL-
//pd=10.000K vol=1.650V adc=127.5_0x7F cmp=0x8C  //HSF
//pd=15.000K vol=1.980V adc=153.0_0x99 cmp=0xA1  //NNEXT/VOL+
//pd=20.000K vol=2.200V adc=170.0_0xAA cmp=0xB6  //PREV/VOL-
//pd=33.000K vol=2.533V adc=195.7_0xC3 cmp=0xE1  //NEXT
*/
IAR_CONST u8 tbl_key1[TBL_KEY1_SIZE] = {
    0x15, T_KEY_PLAY,
    0x34, T_KEY_PREV,
    0x4A, T_KEY_MODE,
    0x61, T_KEY_VOL_UP,
    0x76, T_KEY_VOL_DOWN,
    0x8C, T_KEY_HSF,
    0xA1, T_KEY_NEXT_VOL_UP,
    0xB6, T_KEY_PREV_VOL_DOWN,
    0xE1, T_KEY_NEXT,
    0xFF, NO_KEY,
};

IAR_CONST u8 tbl_key2[TBL_KEY2_SIZE] = {
    0xff, NO_KEY,
};

#if USE_IOKEY

#pragma inline=forced
void io_key_init(void)
{
    K_PLAY_INIT();
    K_PREV_INIT();
    K_NEXT_INIT();
}

#pragma inline=forced
__near_func u8 get_io_key(void)
{
    u8 key_val;
    //printf("x");
    if(IS_K_PLAY()) {
        key_val = T_KEY_PLAY;
    } else if (IS_K_PREV()) {
        key_val = T_KEY_PREV;
    } else if(IS_K_NEXT()) {
        key_val = T_KEY_NEXT;
    } else {
        key_val = NO_KEY;
    }
    return key_val;
}
#endif

#pragma inline=forced
__near_func void other_adc_check(void)
{
    u8 val = get_scan_adc_val();
    if(val < 0x71) {
        aux_online_flag = true;
        ear_online_flag = true;
    } else if(val < 0x91) {
        aux_online_flag = false;
        ear_online_flag = true;
    } else if(val < 0xF3) {
        aux_online_flag = true;
        ear_online_flag = false;
    } else {
        aux_online_flag = false;
        ear_online_flag = false;
    }
}

#endif
