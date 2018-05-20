/*****************************************************************************
 * Module    : user
 * File      : uaer_lunar_algorithm.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : FM驱动的相关函数定义
 *****************************************************************************/
#ifndef USER_LUNAR_ALGORITHM
#define USER_LUNAR_ALGORITHM

bool get_moon_day(u8 month_p,u16 table_addr);
void Conversion_lunar(u8 year,u8 month,u8 day);

#endif
