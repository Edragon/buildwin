/*****************************************************************************
 * Module    : User
 * File      : user_fat.h
 * Author    : Hanny
 * Function  : �ļ����ӿڵ�һЩ����
 *****************************************************************************/
#ifndef USER_FAT_H
#define USER_FAT_H

bool f_search_fname(u16 file_num, char *lfn, u8 len);
u16 str_len2(u8 *str, u8 fb_gbk);

#endif
