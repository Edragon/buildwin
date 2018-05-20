/*****************************************************************************
 * Module    : User
 * File      : user_fat.c
 * Author    : Hanny
 * Function  : 文件读接口，可以增加加密算法等
 *****************************************************************************/
#include "include.h"

extern type_dev_ctl dev_ctl;
extern IAR_DATA_A u8 music_critical;
extern u8 music_ibuf[0xa08];

#if MUSIC_ENCRYPT_EN
u8 music_encrypt;

u16 music_start_offset;     //Music文件的起始偏移，单位为512Byte
u16 lrc_start_offset;       //LRC文件的起始偏移，单位为512Byte
#endif

//返回当前读写指针
#pragma location="FAT_COM_SEG"
__near_func u32 f_getpos(FIL *fp)
{
    return (fp->fptr + fp->seek * 512);
}


//音乐文件读解密接口，用户可根据实际算法进行修改。如需调用子函数必须加上__near_func
//建议用户加密的文件对齐到512Byte，以便精简文件结束判断的处理
#pragma location="FAT_COM_SEG"
__near_func void music_read_decrypt(u8 *buf)
{
    u16 len = 512;          //解密程序示例
    while (len--) {
        *buf ^= 0x5a;
        buf++;
    }
}

//音乐文件读接口，每次读取一个512Byte
#pragma location="FAT_COM_SEG"
__near_func bool music_read(void *buf)
{
    if (!decoder_read(buf)) {
        if (!device_is_actived(fs_cur_dev())) {
            dac_fade_out();
            delay_5ms(1);
            IE_MP3 = 0;
            SPMODE &= ~BIT(7);
            music_critical = 1;
            msc_ctl.status = STATUS_PLAY_STOPPING;  //防止播放中拔出设备有拖尾音的问题
        }
       // printf("read fail!\n");
        return false;                   //读失败
    }
#if MUSIC_ENCRYPT_EN
    if (music_encrypt) {
        if (f_getpos(&fat_file_opt) >= ((u32)lrc_start_offset << 9)) {
            return false;               //判断音乐文件结束
        }
        music_read_decrypt(buf);
    }
#endif
    return true;
}

//带缓存加速的Seek接口
#pragma location="FAT_COM_SEG"
__near_func bool music_seek(u8 mode, u32 offset)
{
#if MUSIC_ENCRYPT_EN
    if (mode == F_SEEK_ORG) {
        offset += music_start_offset;
    }
#endif
    return decoder_seek(mode, offset);
}

#if MUSIC_ENCRYPT_EN
//尝试打开加密歌词
#pragma location="MUSIC_SEG"
bool lrc_open_encrypt(void)
{
    if (!music_encrypt) {
        return false;
    }
    memcpy(&fat_file, &fat_file_opt, sizeof(fat_file));     //直接打开同一个文件
    return true;
}
#endif

//音乐文件读解密接口，用户可根据实际算法进行修改。调用的子函数不需要加上__near_func
#pragma location = "LRC_SEG"
bool lrc_read(void *buf)
{
    if (!f_read(buf)) {
        return false;
    }
#if MUSIC_ENCRYPT_EN
    if (music_encrypt) {
        music_read_decrypt(buf);
    }
#endif
    return true;
}

//返回LRC文件头
#pragma location = "LRC_SEG"
bool lrc_seek_org(void)
{
#if MUSIC_ENCRYPT_EN
    if (!f_lseek(F_SEEK_ORG, lrc_start_offset)) {
#else
    if (!f_lseek(F_SEEK_ORG, 0)) {
#endif
        return false;
    }
    return true;
}

#if MUSIC_ENCRYPT_EN
//MP3文件头解密处理，可以根据扩展名来解析加密，也可以在这里处理头文件
#pragma location="MUSIC_SEG"
void music_head_decrypt(void)
{
    music_start_offset = 0;
    lrc_start_offset = 0;
    if (music_encrypt) {
        f_read_opt(dec_ibuf);                                   //注意，这里Music文件的读取，需要用f_read_opt
        music_read_decrypt(dec_ibuf);                           //解密头
        music_start_offset = LOAD_U32(dec_ibuf) >> 9;           //如果有加密的话，需要在这里偏移掉加密的文件头，单位为512Byte
        lrc_start_offset = LOAD_U32(&dec_ibuf[4]) >> 9;         //也可以通过在加密的头信息设置LRC的起始头，单位为512Byte
        //printf("ms: %04x\n", music_start_offset);
        //printf("ls: %04x\n", lrc_start_offset);
    }
}
#endif

/*
//歌词文件头解密处理，可以根据扩展名来解析加密，也可以在这里处理头文件
void lrc_head_decrypt(void)
{
    if (music_encrypt) {
        lrc_read(lrc_ibuf);             //LRC需要用f_read来获取相关信息
        lrc_start_offset += 0;          //如果LRC起始的位置还有头，也可以在这里进行处理。
    }
}
*/

#pragma location="MUSIC_SEG"
u16 str_len2(u8 *str, u8 fb_gbk)
{
    u8 *s=str;

    if (fb_gbk) {
        while (1) {
            if (*str == 0) {
                str++;
                break;
            }
            if (*str & 0x80) {
                str++;
            }
            str++;
        }
    } else {
        while (1) {
            if (*str == 0 && *(str+1) == 0) {
                str += 2;
                break;
            }
            str += 2;
        }
    }
    return (u16)str - (u16)s;
}
