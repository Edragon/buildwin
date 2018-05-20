/*****************************************************************************
 * Module    : User
 * File      : file_filter.c
 * Author    : Hanny
 * Function  : 文件扫描过滤器
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_music.h"

#if MUSIC_ENCRYPT_EN
extern u8 music_encrypt;
#endif

#pragma constseg="FILE_FILTER_CONST"
#define MUSIC_EXT_NUM 3     //一共搜索3个扩展名
IAR_CONST char tbl_music_ext[MUSIC_EXT_NUM][4] = {"mp3", "wav", "wma"};
IAR_CONST char tbl_txt_ext[4] = "txt";
//IAR_CONST char tbl_lrc_ext[4] = "lrc";
IAR_CONST char tbl_dir_record_name[] = "RECORD";
IAR_CONST char tbl_bin_name[] = "APP";
IAR_CONST char tbl_bin_ext[] = "BIN";
//IAR_CONST char tbl_lrx_ext[] = "lrx";       //加密测试扩展名
#pragma constseg=default

#pragma constseg="LRC_FILTER_CONST"
IAR_CONST char tbl_lrc_ext[4] = "lrc";
#pragma constseg=default

//固件升级Code文件过滤器
#pragma location="FILE_FILTER_SEG"
bool filter_binfile(void)
{
    //printf("fname:%s\n", file_info.fname);
    if(get_scan_dept() == 0) {
        if (0 == memcmp(fat_file_opt.fname, (char *)tbl_bin_name,3) &&
            0 == memcmp(fat_file_opt.fname+8, (char *)tbl_bin_ext, 3))
        {
            task_ctl.work_sta = TASK_UPDATE_FW;
            return true;
        }
    }
    return false;
}

//返回文件起始簇
#pragma location="FILE_FILTER_SEG"
u32 get_sclust_addr(void)
{
    return fat_file.org_clust;//fat_file_opt.sclust;//fat_file.sclust;
}

//判断文件是否为需要查找的文件
#pragma location="FILE_FILTER_SEG"
bool file_is_target_number(void)
{
    if (fs_apiinfo.file_number == fs_apiinfo.file_count) {
        //smemcpy(fat_file.fname, file_info.fname, 12);  //保存文件名
        //smemcpy(&fat_file.dir, &file_dir, sizeof(DIR));
        //printf("%d: %s\n", fs_apiinfo.file_number, fat_file_opt.fname);
        return true;
    }
    return false;
}

//所有文件的过滤器
#pragma location="FILE_FILTER_SEG"
bool file_filter_all(void)
{
    if (!f_attr_is_file(&fat_file_opt)) {
        return false;
    }
    fs_apiinfo.file_count++;
    return file_is_target_number();
}

//转换为Unicode，并获取Unicode字串的长度
#pragma location = "FAT_SEG"
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
        smemcpy(des, src, len);
    }
    return len;
}

//获取小写的扩展名
#pragma location="FILE_FILTER_SEG"
void get_ext_name(char *ext_name,FIL *hFile)
{
    smemcpy(ext_name, &hFile->fname[8], 3);
    string_tolower(ext_name, 3);
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
#pragma location="MINI_FILTER_SEG_1"
u8 dirfile_type(void)
{
    if (f_attr_is_dir(&fat_file_opt)) {
        return FB_FOLDER;               //文件夹
    }
    char ext_name[3];
    get_ext_name(ext_name,&fat_file_opt);
    if (file_is_music(ext_name)) {
        return FB_MUSIC;
    }
    return FB_FILE;
}

//音乐播放的文件过滤器
#pragma location="FILE_FILTER_SEG"
bool file_filter_music(void)
{
    if (!f_attr_is_file(&fat_file_opt)) {
        return false;
    }

    char ext_name[3];
    get_ext_name(ext_name,&fat_file_opt);
    //在EXFAT中，后缀名可能为小写
    if (file_is_music(ext_name)) {
        /* 此段统计文件夹编号，不用时可屏蔽 */
#if FOLDER_SELECT
        if(f_get_indexbefore() == fs_apiinfo.file_count)
        {
            fs_apiinfo.dir_count++;
            if(fs_apiinfo.dir_number == fs_apiinfo.dir_count) {
                fs_apiinfo.file_number = f_get_indexbefore()+1;
            }
        }
#endif
        fs_apiinfo.file_count++;
      //  printf("F:%d %d\n", fs_apiinfo.file_number, fs_apiinfo.file_count);
        file_path_save_hook();
        if(sys_ctl.sclust == fat_file_opt.sclust)
        {
            sys_ctl.file_num = fs_apiinfo.file_count;
        }
        return file_is_target_number();
    }
#if TASK_UPDATE_FW_EN
    else if((task_ctl.work_sta == TASK_MUSIC) && (t_msc.music_total == 0)) {    //在获取音乐文件总数的时候顺便查找是否有升级文件
        return filter_binfile();
    }
#endif
    return false;
}

#if MUSIC_ENCRYPT_EN
//判断是否为加密mp3
#pragma location="MUSIC_SEG"
void music_check_encrypt(void)
{
    char ext_name[3];
    get_ext_name(ext_name,&fat_file_opt);
    //在EXFAT中，后缀名可能为小写
    if (ext_name[2] == 'x') {
        music_encrypt = 1;      //加密MP3
    } else {
        music_encrypt = 0;      //原始MP3
    }
}
#endif

//音乐播放的目录过滤器
#pragma location="FILE_FILTER_SEG"
bool dir_filter_music(void)
{
    if (!f_attr_is_dir(&fat_file_opt)) {
        return false;
    }
    //判断文件夹的深度为0（即文件夹在根目录），并且文件夹的名字为tbl_dir_record_name
#if TASK_VOICE_EN
    if (get_scan_dept() == 0 && 0 == memcmp(fat_file_opt.fname, (char *)tbl_dir_record_name, 6)) {
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
    //printf(":%d\n",get_scan_dept());
    if (get_scan_dept() == 1) {
        return file_filter_music();
    }
    return false;
}

//录音播放的目录过滤器
#pragma location="FILE_FILTER_SEG"
bool dir_filter_voice(void)
{
    //下面的判断是!取反，所以要加这个判断
    if (!f_attr_is_dir(&fat_file_opt)) {
        return false;
    }
    return !dir_filter_music();
}
#endif

#if MUSIC_LRC_EN
IAR_XDATA_A char lrc_filename[100] @ 0x10FD0;
IAR_XDATA_A char filename_temp[100] @ 0x11034;
IAR_XDATA_A char lrc_filename_temp[100] @ 0x11098;

//匹配长文件名
#pragma location = "FAT_SEG"
bool compare_lrc_longname(void)
{
    f_get_filename(lrc_filename);
    u8 mfile_len, lrcfile_len;

    mfile_len = ustr_get(filename_temp, filename);
    lrcfile_len = ustr_get(lrc_filename_temp, lrc_filename);
    if (mfile_len == lrcfile_len && mfile_len >= 6) {
        if (0 == memcmp(filename_temp, lrc_filename_temp, mfile_len - 6)) {
            return true;
        }
    }
    return false;
}

//LRC文件过滤器，歌词文件确认
#pragma location="LRC_FILTER_SEG"
bool file_filter_lrc(void)
{
    if (!f_attr_is_file(&fat_file)) {
        return false;
    }
    char ext_name[3];
    bool res = false;
    get_ext_name(ext_name,&fat_file);
    //在EXFAT中，后缀名可能为小写
    if (0 == memcmp(ext_name, (char *)tbl_lrc_ext, 3)) {
        res = true;
    }
    if (!res) {
        return false;
    }
        //这是一个歌词文件
    //先使用短文件名前2byte判断歌词
                //printf("%s\n", fat_file.fname);
                //printf("%s\n", filename);
    if (0 != memcmp(fat_file_opt.fname,fat_file.fname, 2)) {
        return false;
    }
    //匹配长文件名
    if (!compare_lrc_longname()) {
        return false;
    }
    return true;
}
#endif

u16 GetFileLength(void)
{
    return (u16)(fat_file_opt.fsize >> 8);
}
