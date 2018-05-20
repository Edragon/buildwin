/*****************************************************************************
* Module    : IO
* File      : io_key.c
* Author    : Hanny
* Function  : 用户自定义按键定义
*****************************************************************************/
#ifndef CONFIG_C
#define IR_KEY_NUM     21


IAR_CONST u8 tbl_ir_dat[IR_KEY_NUM] = {0x07,            0x09,           0x15,           0x16,               0x19,
                                       0x40,            0x43,           0x44,           0x45,               0x46,
                                       0x47,            0x0d,           0x0c,           0x18,               0x5e,
                                       0x08,            0x1c,           0x5a,           0x42,               0x52,
                                       0x4a};
IAR_CONST u8 tbl_ir_val[IR_KEY_NUM] = {T_KEY_PREV,      T_KEY_PLAY,     T_KEY_NEXT,     T_KEY_VOL_DOWN,     T_KEY_VOL_UP,
                                       T_KEY_LOOP,      T_KEY_EQ,       T_KEY_MODE,     T_KEY_POWER,        T_KEY_STOP,
                                       T_KEY_MUTE,      T_KEY_NUM_0,    T_KEY_NUM_1,    T_KEY_NUM_2,        T_KEY_NUM_3,
                                       T_KEY_NUM_4,     T_KEY_NUM_5,    T_KEY_NUM_6,    T_KEY_NUM_7,        T_KEY_NUM_8,
                                       T_KEY_NUM_9};


#endif
