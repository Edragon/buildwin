/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Function  : 用户的IO设置
 *****************************************************************************/
#ifndef USER_IR_H
#define USER_IR_H

void ir_timer2_init(void);
void ir_hardware_init(void);
__near_func u8 ir_key_get(u8 ir_data);
__near_func u8 ir_hardware_scan(void);
typedef struct {
    u8  state;
    u16 data;
} type_ir;
extern volatile type_ir ir;
#endif
