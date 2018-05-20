/*****************************************************************************
 * Module    : User
 * File      : file_filter.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �ļ�ɨ�������
 *****************************************************************************/
#include "include.h"

extern FILINFO file_info;      //��ȡ��Ŀ¼�����Ϣ
extern FSAPIINFO fs_apiinfo;   //Ӧ�ýӿ���Ϣ
extern DIR file_dir;           //��ǰĿ¼��
extern u8 music_ibuf[0xa08];
extern type_dir_save dir_save;


#pragma constseg="FILE_FILTER_CONST"
#if BT_ALL_TASK
    #define MUSIC_EXT_NUM 1     //һ������2����չ��
    IAR_CONST char tbl_music_ext[MUSIC_EXT_NUM][4] = {"mp3"};
#else
    #define MUSIC_EXT_NUM 3     //һ������4����չ��
    IAR_CONST char tbl_music_ext[MUSIC_EXT_NUM][4] = {"mp3", "wma", "wav"};
#endif
IAR_CONST char tbl_lrc_ext[4] = "lrc";
IAR_CONST char tbl_dir_record_name[] = "RECORD";    //Ҫ��str_mic_rec_fileǰ�����ַ�һ��
#pragma constseg=default


//�ж��ļ��Ƿ�Ϊ��Ҫ���ҵ��ļ�
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

//�����ļ��Ĺ�����
#pragma location="FILE_FILTER_SEG"
bool file_filter_all(void)
{
    if (!f_attr_is_file()) {
        return false;
    }
    fs_apiinfo.file_count++;
    return file_is_target_number();
}

//ת��ΪUnicode������ȡUnicode�ִ��ĳ���
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

//��ȡСд����չ��
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


//��ȡͼ������
#pragma location="FILE_FILTER_SEG"
u8 dirfile_type(void)
{
    if (f_attr_is_dir()) {
        return FB_FOLDER;               //�ļ���
    }
    char ext_name[3];
    get_ext_name(ext_name);
    if (file_is_music(ext_name)) {
        return FB_MUSIC;
    }
    return FB_FILE;
}

//���ֲ��ŵ��ļ�������
#pragma location="FILE_FILTER_SEG"
bool file_filter_music(void)
{
    if (!f_attr_is_file()) {
        return false;
    }

    char ext_name[3];
    get_ext_name(ext_name);
    //��EXFAT�У���׺������ΪСд
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

//���ֲ��ŵ�Ŀ¼������
#pragma location="FILE_FILTER_SEG"
bool dir_filter_music(void)
{
    if (!f_attr_is_dir()) {
        return false;
    }
    //�ж��ļ��е����Ϊ0�����ļ����ڸ�Ŀ¼���������ļ��е�����Ϊtbl_dir_record_name
#if TASK_VOICE_EN
    if (get_scan_dept() == 0 && 0 == memcmp(file_info.fname, (char *)tbl_dir_record_name, 6)) {
        return false;   //���˵���Ŀ¼�µ�RECORD�ļ���
    }
#endif
    return true;
}

#if TASK_VOICE_EN
//¼�����ŵ��ļ�������
#pragma location="FILE_FILTER_SEG"
bool file_filter_voice(void)
{
    if (get_scan_dept() == 1) {
        return file_filter_music();
    }
    return false;
}

//¼�����ŵ�Ŀ¼������
#pragma location="FILE_FILTER_SEG_1"
bool dir_filter_voice(void)
{
    //������ж���!ȡ��������Ҫ������ж�
    if (!f_attr_is_dir()) {
        return false;
    }
    return !dir_filter_music();
}
#endif
#if MUSIC_LRC_EN
//ƥ�䳤�ļ���
#pragma location="FAT_LFN_SEG"
bool compare_lrc_longname(void)
{
    u8 mfile_len, lrcfile_len;
    //���ļ����жϸ�ʵ��ٶ�̫��
    mfile_len = ustr_get(&music_ibuf[0x800+100], &music_ibuf[0x800]);
    f_get_filename();                               //��ȡ����ļ��ĳ��ļ���
    lrcfile_len = ustr_get(&music_ibuf[0x800+200], filename);
    if (mfile_len == lrcfile_len && mfile_len >= 6) {
        if (0 == memcmp(&music_ibuf[0x800+100], &music_ibuf[0x800+200], mfile_len - 6)) {
            return true;
        }
    }
    return false;
}

//LRC�ļ�������������ļ�ȷ��
#pragma location="FILE_FILTER_SEG"
bool file_filter_lrc(void)
{
    if (!f_attr_is_file()) {
        return false;
    }
    char ext_name[3];
    bool res = false;
    get_ext_name(ext_name);
    //��EXFAT�У���׺������ΪСд
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
        //����һ������ļ�
#if 1
    //��ʹ�ö��ļ���ǰ2byte�жϸ��
    if (0 != memcmp(&music_ibuf[0x800+500], file_info.fname, 2)) {
        return false;
    }

    //ƥ�䳤�ļ���
    if (!compare_lrc_longname()) {
        return false;
    }
#else
    //ʹ�ö��ļ����жϸ��
    if (0 == memcmp(&music_ibuf[0x800+500], file_info.fname, 8)) {
        return true;
    }
#endif
    return true;
}
#endif

