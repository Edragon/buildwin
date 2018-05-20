/*****************************************************************************
 * Module    : User
 * File      : file_filter.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 文件扫描过滤器
 *****************************************************************************/
#include "include.h"

extern FILINFO file_info;      //读取的目录项的信息
extern FSAPIINFO fs_apiinfo;   //应用接口信息
extern DIR file_dir;           //当前目录项
extern u8 music_ibuf[0xa08];
extern type_dir_save dir_save;


#pragma constseg="FILE_FILTER_CONST"
#if BT_ALL_TASK
    #define MUSIC_EXT_NUM 1     //一共搜索2个扩展名
    IAR_CONST char tbl_music_ext[MUSIC_EXT_NUM][4] = {"mp3"};
#else
    #define MUSIC_EXT_NUM 3     //一共搜索4个扩展名
    IAR_CONST char tbl_music_ext[MUSIC_EXT_NUM][4] = {"mp3", "wma", "wav"};
#endif
IAR_CONST char tbl_lrc_ext[4] = "lrc";
IAR_CONST char tbl_dir_record_name[] = "RECORD";    //要与str_mic_rec_file前几个字符一致
#pragma constseg=default


//判断文件是否为需要查找的文件
#pragma location="FILE_FILTER_SEG"
bool file_is_target_number(void)
{
    if (fs_apiinfo.file_number == fs_apiinfo.file_count) {
        return true;
    }
    else if (dir_save.flag == 1 && dir_save.dir_count == dir_save.dir_num)
    {
	    return true;
    }
    return false;
}

//所有文件的过滤器
#pragma location="FILE_FILTER_SEG"
bool file_filter_all(void)
{
    if (!f_attr_is_file()) {
        return false;
    }
    fs_apiinfo.file_count++;
    return file_is_target_number();
}

//转换为Unicode，并获取Unicode字串的长度
u8 ustr_get(void *des, void *src)
{
    char *des1 = des;
    char *src1 = src;
    u8 len = 0;
    if (src1[FILE_NAME_FLAG] & FB_GBK) {
        char ch;
        u8 src_len = 0;
        while ((ch = *src1) && src_len < (FILE_NAME_LEN-2)) {
            len += 2;
            *des1++ = *src1++;
            if (ch & 0x80) {
                *des1++ = *src1++;
                src_len += 2;
            } else {
                *des1++ = 0;
                src_len++;
            }
        }
    } else {
        while ((src1[0] || src1[1]) && len < (FILE_NAME_LEN-2)) {
            src1 += 2;
            len += 2;
        }
        memcpy(des, src, len);
    }
    return len;
}

//获取小写的扩展名
#pragma location="FILE_FILTER_SEG"
void get_ext_name(char *ext_name)
{
    memcpy(ext_name, &file_info.fname[8], 3);
    string_tolow(ext_name, 3);
}

#pragma location="FILE_FILTER_SEG"
bool file_is_music(char *ext_name)
{
    u8 i;
    for (i=0; i!=MUSIC_EXT_NUM; i++) {
        if (0 == memcmp(ext_name, (char *)(tbl_music_ext[i]), 3)) {
            return true;
        }
    }
    return false;
}


//获取图标类型
#pragma location="FILE_FILTER_SEG"
u8 dirfile_type(void)
{
    if (f_attr_is_dir()) {
        return FB_FOLDER;               //文件夹
    }
    char ext_name[3];
    get_ext_name(ext_name);
    if (file_is_music(ext_name)) {
        return FB_MUSIC;
    }
    return FB_FILE;
}

//音乐播放的文件过滤器
#pragma location="FILE_FILTER_SEG"
bool file_filter_music(void)
{
    if (!f_attr_is_file()) {
        return false;
    }

    char ext_name[3];
    get_ext_name(ext_name);
    //在EXFAT中，后缀名可能为小写
    if (file_is_music(ext_name)) {
        if(f_get_indexbefore() == fs_apiinfo.file_count)
        {
            dir_save.dir_count++;
        }

        fs_apiinfo.file_count++;
        //printf("F:%d %d\n", fs_apiinfo.file_number, fs_apiinfo.file_count);
        dir_path_save_hook();
        return file_is_target_number();
    }
    return false;
}

//音乐播放的目录过滤器
#pragma location="FILE_FILTER_SEG"
bool dir_filter_music(void)
{
    if (!f_attr_is_dir()) {
        return false;
    }
    //判断文件夹的深度为0（即文件夹在根目录），并且文件夹的名字为tbl_dir_record_name
#if TASK_VOICE_EN
    if (get_scan_dept() == 0 && 0 == memcmp(file_info.fname, (char *)tbl_dir_record_name, 6)) {
        return false;   //过滤掉根目录下的RECORD文件夹
    }
#endif
    return true;
}

#if TASK_VOICE_EN
//录音播放的文件过滤器
#pragma location="FILE_FILTER_SEG"
bool file_filter_voice(void)
{
    if (get_scan_dept() == 1) {
        return file_filter_music();
    }
    return false;
}

//录音播放的目录过滤器
#pragma location="FILE_FILTER_SEG_1"
bool dir_filter_voice(void)
{
    //下面的判断是!取反，所以要加这个判断
    if (!f_attr_is_dir()) {
        return false;
    }
    return !dir_filter_music();
}
#endif
#if MUSIC_LRC_EN
//匹配长文件名
#pragma location="FAT_LFN_SEG"
bool compare_lrc_longname(void)
{
    u8 mfile_len, lrcfile_len;
    //长文件名判断歌词的速度太慢
    mfile_len = ustr_get(&music_ibuf[0x800+100], &music_ibuf[0x800]);
    f_get_filename();                               //获取歌词文件的长文件名
    lrcfile_len = ustr_get(&music_ibuf[0x800+200], filename);
    if (mfile_len == lrcfile_len && mfile_len >= 6) {
        if (0 == memcmp(&music_ibuf[0x800+100], &music_ibuf[0x800+200], mfile_len - 6)) {
            return true;
        }
    }
    return false;
}

//LRC文件过滤器，歌词文件确认
#pragma location="FILE_FILTER_SEG"
bool file_filter_lrc(void)
{
    if (!f_attr_is_file()) {
        return false;
    }
    char ext_name[3];
    bool res = false;
    get_ext_name(ext_name);
    //在EXFAT中，后缀名可能为小写
    if (0 == memcmp(ext_name, (char *)tbl_lrc_ext, 3)) {
        res = true;
    }
    /*
    if (0 == memcmp(ext_name, (char *)tbl_lrx_ext, 3)) {
        res = true;
    }
    */

    if (!res) {
        return false;
    }
        //这是一个歌词文件
#if 1
    //先使用短文件名前2byte判断歌词
    if (0 != memcmp(&music_ibuf[0x800+500], file_info.fname, 2)) {
        return false;
    }

    //匹配长文件名
    if (!compare_lrc_longname()) {
        return false;
    }
#else
    //使用短文件名判断歌词
    if (0 == memcmp(&music_ibuf[0x800+500], file_info.fname, 8)) {
        return true;
    }
#endif
    return true;
}
#endif

