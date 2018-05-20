/*****************************************************************************
 * Module    : API
 * File      : api_filesystem.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �ļ�ϵͳAPI�ӿ�
 *****************************************************************************/
#ifndef _API_FILESYSTEM_H_
#define _API_FILESYSTEM_H_

/******************************��ϸ�ĺ���˵������ġ�AX2228-APM080���ο����ĵ�.pdf��************************/

#define  DIR_NAME_LEN	7
#define LONGNAME DIR_NAME_LEN+14

typedef enum {
    F_SEEK_ORG = 0,
    F_SEEK_CUR
}SEEK_MODE;

bool fs_sel(u8 dev);                                        //ѡ���豸
__near_func u8 fs_cur_dev(void);                            //���ص�ǰ�豸
void f_scan_set(u8 scan_mode, bool (*file_filter)(void), bool (*dir_filter)(void));//����ɨ���ļ�������

__near_func bool fs_is_mounted(void);                       //�ж��ļ�ϵͳ�Ƿ��Ѿ���ʼ����������Ч��
__near_func bool fs_need_mounted(void);                     //�ж��ļ�ϵͳ�Ƿ���Ҫ��ʼ�������δ��ʼ����������г�ʼ����
__near_func bool fs_writable(void);                         //�ж��ļ�ϵͳ�Ƿ��д
bool fs_init(void);                                         //�ļ�ϵͳ�ĳ�ʼ��
void fs_unmount(void);                                      //ж���ļ�ϵͳ

u16 f_gettotal(void);                                       //��ȡ�洢�����ϵ��ļ�������
u16 f_get_indexbefore(void);                                //��ȡ��ǰĿ¼֮ǰ�ж��ٸ��ļ�
u16 f_get_dirtotal(void);                                   //��ȡ��ǰĿ¼�ж��ٸ��ļ�
bool f_mkdir(char *path);                                   //����һ��Ŀ¼
bool f_open(u16 file_num);                                  //�����ļ���ţ���һ���ļ���
bool f_openfile(char *path);                                //�����ļ���ַ����һ���ļ���
bool f_delete(u16 file_num);                                //�����ļ���ţ�ɾ��һ���ļ���
u8 f_create(char *file_name);                               //�����ļ�������һ���ļ������ļ�����
u8 f_create_long_name(char *fname, u8 lfn_buf);             //�����ļ�������һ���ļ������ļ�����
//u8 f_create_long_name(char *file_name);

bool f_lseek(SEEK_MODE type, u32 ofs);                       //�ļ���дָ�����ת
bool f_read(void *buf);                                      //�ļ��Ķ�ȡ��ÿ�ζ�ȡ�Ĵ�СΪ512Byte��
u8 f_write(void *buf);                                       //�ļ���д��
bool f_sync(void);                                           //�ļ�дͬ��
bool f_close(void);                                          //�ر��ļ�

u16 fat_folder_select(u16 dir_num);

//�������read seek ����
bool f_open_lrcfile(void);                                   //�򿪸���ļ�
bool f_open_opt(u16 file_num);                               //���ٰ汾��f_open����Ҫ���������ļ��Ĳ��ŵ��Ż������û�����Ӧ�õ��У�����ʹ��f_open
__near_func bool f_read_opt(void *buf);                      //���ٰ汾��f_read����Ҫ���������ļ��Ĳ��ŵ��Ż������û�����Ӧ�õ��У�����ʹ��f_read��
__near_func bool f_seek_opt(u8 mode, u32 offset);            //���ٰ汾��f_lseek����Ҫ���������ļ��Ĳ��ŵ��Ż������û�����Ӧ�õ��У�����ʹ��f_lseek


void dir_path_save_hook(void);
u8 get_scan_dept(void);                                      //��ȡ��ǰ�����ļ��еļ�����0��ʾ��ǰ����λ��Ϊ��Ŀ¼
u16 f_get_filelen(void);
void f_get_filename(void);                                   //��ȡ�ļ���(���ļ���)
bool f_attr_is_dir(void);                                    //�ж�Ŀ¼���Ƿ�Ϊ�ļ���
bool f_attr_is_file(void);                                   //�ж�Ŀ¼���Ƿ�Ϊ�ļ�


#endif
