/*****************************************************************************
* Module    : IO
* File      : io_key.c
* Author    : Hanny
* Function  : 用户自定义按键定义
*****************************************************************************/
#ifndef CONFIG_C

#define KEY1_NUM 7
#define KEY2_NUM 10

#define TBL_KEY1_SIZE       (2 * (KEY1_NUM+1))
#define TBL_KEY2_SIZE       (2 * (KEY2_NUM+1))

IAR_CONST u8 tbl_key1[TBL_KEY1_SIZE] = {
    0x0b, T_KEY_PREV,
    0x20, T_KEY_PLAY,
    0x34, T_KEY_NEXT,
    0x4a, T_KEY_VOL_DOWN,
    0x61, T_KEY_VOL_UP,
    0x76, T_KEY_MODE,
    0x8c, T_KEY_HSF,
    0xff, NO_KEY,
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

#endif
