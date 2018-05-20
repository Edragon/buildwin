/*****************************************************************************
* Module    : IO
* File      : io_key.c
* Author    : Hanny
* Function  : 用户自定义按键定义
*****************************************************************************/
#ifndef CONFIG_C

#define KEY1_NUM 4
#define KEY2_NUM 10

#define TBL_KEY1_SIZE       (2 * (KEY1_NUM+1))
#define TBL_KEY2_SIZE       (2 * (KEY2_NUM+1))
/*
//NO.15-103 Demo板
//pu=10.000K, total 4 pd
//pd=0.000K vol=0.000V adc=0.0_0x0 cmp=0x15      //PLAY
//pd=2.000K vol=0.550V adc=42.5_0x2A cmp=0x40    //NEXT
//pd=5.100K vol=1.115V adc=86.1_0x56 cmp=0x6A    //PREV
//pd=10.000K vol=1.650V adc=127.5_0x7F cmp=0xA1  //VOL/DEL
//pd=33.775K vol=2.546V adc=196.7_0xC4 cmp=0xCC  //FOR EAR & AUX
//pd=51.000K vol=2.759V adc=213.2_0xD5 cmp=0xDE  //FOR EAR DET
//pd=100.000K vol=3.000V adc=231.8_0xE7 cmp=0xF3 //FOR AUX DET
*/
IAR_CONST u8 tbl_key1[TBL_KEY1_SIZE] = {
    0x15, T_KEY_PLAY,
    0x40, T_KEY_NEXT,
    0x6A, T_KEY_PREV,
    0xA1, T_KEY_VOL_DOWN,
    0xFF, NO_KEY,
};

IAR_CONST u8 tbl_key2[TBL_KEY2_SIZE] = {
    0x0b, T_KEY_NUM_0,
    0x20, T_KEY_NUM_1,
    0x34, T_KEY_NUM_2,
    0x4a, T_KEY_NUM_3,
    0x61, T_KEY_NUM_4,
    0x76, T_KEY_NUM_5,
    0x8c, T_KEY_NUM_6,
    0xa1, T_KEY_NUM_7,
    0xb6, T_KEY_NUM_8,
    0xcd, T_KEY_NUM_9,
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
    u8 adc_val = get_scan_adc_val();

    if(adc_val < 0xCC){
        ear_online_flag = true;
        aux_online_flag = true;
    } else if (adc_val < 0xDE) {
        ear_online_flag = true;
        aux_online_flag = false;
    } else if (adc_val < 0xF3) {
        ear_online_flag = false;
        aux_online_flag = true;
    } else {
        ear_online_flag = false;
        aux_online_flag = false;
    }
    //printf("aux=%02x ear=%02x\n", aux_online_flag, ear_online_flag);
}

#endif
