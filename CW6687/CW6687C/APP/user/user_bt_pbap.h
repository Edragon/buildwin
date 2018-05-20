/*****************************************************************************
 * Module    : User
 * File      : user_bt_pbap.h
 * Author    : junwen
 * Function  : À¶ÑÀPBAP²Ù×÷½Ó¿Ú
 *****************************************************************************/
#ifndef USER_BT_PABP_H
#define USER_BT_PABP_H


typedef struct{
	u8 pbName[40];
	u8 pbNum[24];
}PHONEBOOK_ITEM;


extern u16 phonebook_total_num;
extern IAR_XDATA_A u8 phonebook_name_number[40+24];

u8 utf8_to_utf16(u8 *utf16_ptr, u8 *utf8_ptr, u32 len);

u16 check_contacts(u8 *phone_num, u8 len);


#endif
