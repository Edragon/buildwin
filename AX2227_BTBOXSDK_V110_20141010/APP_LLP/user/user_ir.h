/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户的IO设置
 *****************************************************************************/
#ifndef USER_IR_H
#define USER_IR_H

void ir_timer1_init(void);
__near_func void ir_timer1(void);
__near_func u8 key_deal_ir(void);

void ir_hardware_init(void);
__near_func u8 ir_hardware_scan(void);

#endif
