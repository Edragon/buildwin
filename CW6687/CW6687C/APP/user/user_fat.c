/*****************************************************************************
 * Module    : User
 * File      : user_fat.c
 * Author    : Hanny
 * Function  : �ļ����ӿڣ��������Ӽ����㷨��
 *****************************************************************************/
#include "include.h"

extern type_dev_ctl dev_ctl;
extern IAR_DATA_A u8 music_critical;
extern u8 music_ibuf[0xa08];

#if MUSIC_ENCRYPT_EN
u8 music_encrypt;

u16 music_start_offset;     //Music�ļ�����ʼƫ�ƣ���λΪ512Byte
u16 lrc_start_offset;       //LRC�ļ�����ʼƫ�ƣ���λΪ512Byte
#endif

//���ص�ǰ��дָ��
#pragma location="FAT_COM_SEG"
__near_func u32 f_getpos(FIL *fp)
{
    return (fp->fptr + fp->seek * 512);
}


//�����ļ������ܽӿڣ��û��ɸ���ʵ���㷨�����޸ġ���������Ӻ����������__near_func
//�����û����ܵ��ļ����뵽512Byte���Ա㾫���ļ������жϵĴ���
#pragma location="FAT_COM_SEG"
__near_func void music_read_decrypt(u8 *buf)
{
    u16 len = 512;          //���ܳ���ʾ��
    while (len--) {
        *buf ^= 0x5a;
        buf++;
    }
}

//�����ļ����ӿڣ�ÿ�ζ�ȡһ��512Byte
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
            msc_ctl.status = STATUS_PLAY_STOPPING;  //��ֹ�����аγ��豸����β��������
        }
       // printf("read fail!\n");
        return false;                   //��ʧ��
    }
#if MUSIC_ENCRYPT_EN
    if (music_encrypt) {
        if (f_getpos(&fat_file_opt) >= ((u32)lrc_start_offset << 9)) {
            return false;               //�ж������ļ�����
        }
        music_read_decrypt(buf);
    }
#endif
    return true;
}

//��������ٵ�Seek�ӿ�
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
//���Դ򿪼��ܸ��
#pragma location="MUSIC_SEG"
bool lrc_open_encrypt(void)
{
    if (!music_encrypt) {
        return false;
    }
    memcpy(&fat_file, &fat_file_opt, sizeof(fat_file));     //ֱ�Ӵ�ͬһ���ļ�
    return true;
}
#endif

//�����ļ������ܽӿڣ��û��ɸ���ʵ���㷨�����޸ġ����õ��Ӻ�������Ҫ����__near_func
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

//����LRC�ļ�ͷ
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
//MP3�ļ�ͷ���ܴ������Ը�����չ�����������ܣ�Ҳ���������ﴦ��ͷ�ļ�
#pragma location="MUSIC_SEG"
void music_head_decrypt(void)
{
    music_start_offset = 0;
    lrc_start_offset = 0;
    if (music_encrypt) {
        f_read_opt(dec_ibuf);                                   //ע�⣬����Music�ļ��Ķ�ȡ����Ҫ��f_read_opt
        music_read_decrypt(dec_ibuf);                           //����ͷ
        music_start_offset = LOAD_U32(dec_ibuf) >> 9;           //����м��ܵĻ�����Ҫ������ƫ�Ƶ����ܵ��ļ�ͷ����λΪ512Byte
        lrc_start_offset = LOAD_U32(&dec_ibuf[4]) >> 9;         //Ҳ����ͨ���ڼ��ܵ�ͷ��Ϣ����LRC����ʼͷ����λΪ512Byte
        //printf("ms: %04x\n", music_start_offset);
        //printf("ls: %04x\n", lrc_start_offset);
    }
}
#endif

/*
//����ļ�ͷ���ܴ������Ը�����չ�����������ܣ�Ҳ���������ﴦ��ͷ�ļ�
void lrc_head_decrypt(void)
{
    if (music_encrypt) {
        lrc_read(lrc_ibuf);             //LRC��Ҫ��f_read����ȡ�����Ϣ
        lrc_start_offset += 0;          //���LRC��ʼ��λ�û���ͷ��Ҳ������������д���
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
