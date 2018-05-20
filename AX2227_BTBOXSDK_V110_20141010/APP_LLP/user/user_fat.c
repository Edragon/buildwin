/*****************************************************************************
 * Module    : User
 * File      : user_fat.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 文件读接口，可以增加加密算法等
 *****************************************************************************/
#include "include.h"


extern u8 music_ibuf[0xa08];


//返回当前读写指针
#pragma location="FAT_COM_SEG"
__near_func u32 f_getpos(FIL *fp)
{
    return (fp->fptr + fp->seek * 512);
}

//音乐文件读接口，每次读取一个512Byte
#pragma location="FAT_COM_SEG"
__near_func bool music_read(void *buf)
{
    if (!f_read_opt(buf)) {
        return false;                   //读失败
    }
    return true;
}

//带缓存加速的Seek接口
#pragma location="FAT_COM_SEG"
__near_func bool music_seek(u8 mode, u32 offset)
{
    return f_seek_opt(mode, offset);
}
#pragma location = "LRC_SEG"
bool lrc_read(void *buf)
{
    if (!f_read(buf)) {
        return false;
    }
    return true;
}

//返回LRC文件头
#pragma location = "LRC_SEG"
bool lrc_seek_org(void)
{
    if (!f_lseek(F_SEEK_ORG, 0)) {
        return false;
    }
    return true;
}
#pragma location = "FAT_API_SEG"
void load_code_fat(void)
{
    load_code(DCODE_FAT,1);
}
