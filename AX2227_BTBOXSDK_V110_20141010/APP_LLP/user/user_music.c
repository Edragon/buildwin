/*****************************************************************************
 * Module    : User
 * File      : user_music.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �û����ֲ��ſ���
 *****************************************************************************/
#include "include.h"
#include "user_music.h"
#include "task.h"
#include "task_music.h"


#pragma location="MP3_SEG"
void mp3_res_play_wait(u8 music_name)
{
    sys_flag.voice_play_flag = 1;
    if(sys_flag.mute_flag || sys_ctl.volume==0) {
        WATCHDOG_CLR();
        delay_5ms(OPA_UNMUTE_DELAY); //ĳЩ���Ž�MUTE��ʱ�ϳ����ɸ���ʵ���������
    }

#if CONSTANT_WARNING_VOLUME_EN
    user_change_volume(WARNING_VOLUME);
#endif

    mp3_res_play(music_name);
    while (music_get_status() > STATUS_PLAY_STOPPING) {
        WATCHDOG_CLR();
    }
    music_stop();

    sys_flag.voice_play_flag = 0;
    user_change_volume(sys_ctl.volume);
}

//�豸�����ļ�ϵͳ
#pragma location="TASK_MUSIC_SEG"
bool device_mount(void)
{
    //printf("mount, %d\n", fs_cur_dev());
    if (fs_need_mounted()) {
        fs_init();
    }
    if (!fs_is_mounted()) {
        //device_set_aborted(fs_cur_dev());   //�����豸�޷�����
        return false;
    }
    if (!device_is_actived(fs_cur_dev())) {
        //device_set_aborted(fs_cur_dev());   //�����豸�޷�����
        return false;
    }

    return true;
}

//����Ƿ��������豸
//#pragma location="TASK_MUSIC_SEG"
bool is_other_device(void)
{
    if(fs_cur_dev() == DEVICE_SDMMC) {
        return device_is_online(DEVICE_UDISK);
    } else {
        return device_is_online(DEVICE_SDMMC);
    }
}

//ɨ���豸�ļ�����ȡ�����ļ��ĸ���
#pragma location="TASK_MUSIC_SEG"
bool music_scan_file(void)
{
    led_busy();

#if USB_SD_WARNING_MUSIC
    if(!u_msc.music_voice_flag) {
        if(fs_cur_dev() == DEVICE_SDMMC){
            mp3_res_play_wait(RES_MP3_SD);
        }
        else if(fs_cur_dev() == DEVICE_UDISK){
            mp3_res_play_wait(RES_MP3_USB);
        }
        flush_msg();                        //��������ʱ�䳤������̫�ఴ����Ϣδ����
    }
#endif

    //��ȡ����������
    t_msc.music_total = music_total();
    //t_msc.dir_total = t_fat.dir_count;

    led_idle();

    if(t_msc.music_total == 0) {
        //device_set_aborted(fs_cur_dev());   //�����豸�޷�����
        return false;
    }
    return true;
}

//ѡ����/��һ�������豸
//#pragma location="TASK_MUSIC_SEG"
bool music_next_device(void)
{
    u8 cur_dev;
    if(fs_cur_dev() == DEVICE_SDMMC) {
        cur_dev = DEVICE_UDISK;
    } else {
        cur_dev = DEVICE_SDMMC;
    }
    if (device_is_online(cur_dev)) {                 //����п����豸����ѡ�����豸
        music_stop();                               //ֹͣ��ǰ����
        fs_sel(cur_dev);
        if(device_mount() && music_scan_file()) {   //�����ļ�ϵͳ
            t_msc.music_num = 1;                    //�򿪵��ļ����
            t_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
            return true;
        }
    }
    return false;
}

//����һ�������豸
//#pragma location="TASK_MUSIC_SEG"
bool music_mount_device(void)
{
    device_change();

    if(device_mount() && music_scan_file()) {       //�����ļ�ϵͳ
        return true;
    }

    if(music_next_device()) {
        return true;
    }

    return false;
}

//���¹����ļ�ϵͳ
//#pragma location="TASK_MUSIC_SEG"
bool music_remount_device(void)
{
    music_stop();                                   //ֹͣ��ǰ����

    if(!device_mount() || !music_scan_file()) {
        return false;
    }

    return true;
}

//ѡ����/��һ��¼���豸
//#pragma location="TASK_MUSIC_SEG"
bool record_next_device(void)
{
    u8 cur_dev;
    if(fs_cur_dev() == DEVICE_SDMMC) {
        cur_dev = DEVICE_UDISK;
    } else {
        cur_dev = DEVICE_SDMMC;
    }
    if (device_is_online(cur_dev)) {                //����п����豸����ѡ�����豸
        fs_sel(cur_dev);
        if(device_mount()) {                        //�����ļ�ϵͳ
            return true;
        }
    }
    return false;
}

//����һ��¼���豸
//#pragma location="TASK_MUSIC_SEG"
bool record_mount_device(void)
{
    device_change();

    if(device_mount()) {                            //�����ļ�ϵͳ
        return true;
    }

    if(record_next_device()) {
        return true;
    }

    return false;
}

#if MUSIC_ID3_EN && MUSIC_LRC_EN
    #error "��֧��ͬʱ��ID3�͸����ʾ"
#endif

#if MUSIC_ID3_EN


extern u8 id3_title[31];
extern u8 id3_artist[31];
extern u8 id3_album[41];
extern u16 ConvertGBKtoUTF16(u16 gb);

IAR_XDATA u8 id3_disp_buf[FILE_NAME_LEN + 31*2 + 31*2 + 41*2] @ "ID3_BUFF";

#pragma constseg="ID3_CONST"
u8 const  unkown[14] = {0x55,0,0x6e,0,0x6b,0,0x6f,0,0x77,0,0x6e,0,0x20,0};//Unkown
u8 const  artist[12] = {0x41,0,0x72,0,0x74,0,0x69,0,0x73,0,0x74,0};//Aritst
u8 const  album[10] = {0x41,0,0x6c,0,0x62,0,0x75,0,0x6d,0};//Album
u8 const  title[10] = {'T',0,'i',0,'t',0,'l',0,'e',0};//Title
#pragma constseg=default

#pragma location="FONT_SEG"
u8 gbk_to_unicode(u8 *rptr,u8 *ptr,u8 max_len)
{
    u8 lcnt = 0;
    u8 wcnt = 0;
    u16 code;
    u8 ch_len;
    while(*rptr && (lcnt<max_len))
    {
        if(*rptr & 0x80)
        {
            code = rptr[1] | (rptr[0] << 8);  //��λ��ǰ
            ch_len = 2;
            if((lcnt+1) == max_len)
                break;
        }
        else
        {
            code = *rptr;
            ch_len = 1;
        }
        if(code >= 256)
            code = ConvertGBKtoUTF16(code);
        ptr[wcnt] = (u8)code;
        ptr[wcnt+1] = (u8)(code>>8);
        rptr += ch_len;
        lcnt += ch_len;
        wcnt += 2;
    }
    return wcnt;
}

#pragma location="ID3_SEG"
u8 unicode_cpy(u8 *rptr,u8 *ptr,u8 max_len)
{
    u8 len = 0;
    while(len<max_len)
    {
        if(rptr[len] || rptr[len+1])
            len += 2;
        else
            break;
    }
    if(len)
        memcpy(ptr,rptr,len);
    return len;
}

#pragma location="ID3_SEG"
void id3_transmit(void)
{
    u8 len;
    u8 *rptr;
    u8 * ptr;

    /*
    printf_array(filename,99);
    printf("\n");
    printf_array(id3_artist,31);
    printf("\n");
    printf_array(id3_album,41);
    printf("\n");
    printf_array(id3_title,31);
    printf("\n");
    */

    memset(id3_disp_buf,0,sizeof(id3_disp_buf));
    ptr = id3_disp_buf;

    rptr = (u8 *)filename;
    if(filename[FILE_NAME_FLAG] & FB_GBK) {         //filename
        len = gbk_to_unicode(rptr, ptr, (FILE_NAME_LEN-1)/2);
    } else {
        len = unicode_cpy(rptr, ptr, FILE_NAME_LEN-2);
    }
    ptr += len;
    *ptr++ = ' ';
    *ptr++ = 0x00;

    rptr = id3_artist;
    if(id3_artist[30]) {                            //artist
        len = unicode_cpy(rptr,ptr,30);
    } else {
        len = gbk_to_unicode(rptr,ptr,30);
    }
    if(len == 0) {
        memcpy(ptr,(u8*)unkown,14);
        ptr += 14;
        memcpy(ptr,(u8*)artist,12);
        len = 12;
    }
    ptr += len;
    *ptr++ = ' ';
    *ptr++ = 0x00;

    rptr = id3_album;                               //album
    if(id3_album[40]) {
        len = unicode_cpy(rptr,ptr,40);
    } else {
        len = gbk_to_unicode(rptr,ptr,40);
    }
    if(len == 0) {
        memcpy(ptr,(u8*)unkown,14);
        ptr += 14;
        memcpy(ptr,(u8*)album,10);
        len = 10;
    }
    ptr += len;
    *ptr++ = ' ';
    *ptr++ = 0x00;

    rptr = id3_title;                               //title
    if(id3_title[30]) {
        len = unicode_cpy(rptr,ptr,30);
    } else {
        len = gbk_to_unicode(rptr,ptr,30);
    }
    if(len == 0) {
        memcpy(ptr,(u8*)unkown,14);
        ptr += 14;
        memcpy(ptr,(u8*)title,10);
        len = 10;
    }
    ptr += len;
    *ptr++ = '\0';
    *ptr++ = 0x00;
    /*printf_array(id3_disp_buf, 300);
    printf("\n");*/

}
#endif

void utf16_to_ansi(void *dst, void *src, u8 len)
{
    u8 *pdst, *psrc;
    pdst = (u8 *)dst;
    psrc = (u8 *)src;

    while(len--) {
        if(*(psrc+1) != 0x00) {
            *pdst++ = '*';      //������'*'����
        } else {
            *pdst++ = *psrc;
        }
        psrc += 2;
    }
}

void gbk_to_ansi(void *dst, void *src, u8 len)
{
    u8 *pdst, *psrc;
    pdst = (u8 *)dst;
    psrc = (u8 *)src;

    while(len--) {
        if(*psrc > 0x80) {
            *pdst++ = '*';      //������'*'����
            psrc += 2;
        } else {
            *pdst++ = *psrc++;
        }
    }
}

void make_lfn_to_ansi(void)
{
    if((filename[FILE_NAME_FLAG] & FB_GBK) != 0) {
        gbk_to_ansi(filename, filename, FILE_NAME_LEN/2);
    } else {
        utf16_to_ansi(filename, filename, FILE_NAME_LEN/2);
    }
}
