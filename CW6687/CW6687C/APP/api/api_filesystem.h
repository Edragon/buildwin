/*****************************************************************************
 * Module    : API
 * File      : api_filesystem.h
 * Author    : Hanny
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


__near_func bool fs_is_mounted(void);                       //�ж��ļ�ϵͳ�Ƿ��Ѿ���ʼ����������Ч��
__near_func bool fs_need_mounted(void);                     //�ж��ļ�ϵͳ�Ƿ���Ҫ��ʼ�������δ��ʼ����������г�ʼ����
__near_func bool fs_writable(void);
bool fs_init(void);                                         //�ļ�ϵͳ�ĳ�ʼ��
bool f_mkdir(char *path);                                   //����һ��Ŀ¼
u16 f_gettotal(void);                                       //��ȡ�洢�����ϵ��ļ�������
u16 f_get_indexbefore(void);                                //��ȡ��ǰĿ¼֮ǰ�ж��ٸ��ļ�
u16 f_get_dirtotal(void);                                   //��ȡ��ǰĿ¼�ж��ٸ��ļ�
bool f_open(u16 file_num);                                  //�����ļ���ţ���һ���ļ���
bool f_delete(u16 file_num);                                //�����ļ���ţ�ɾ��һ���ļ���
bool f_openfile(char *path);                                //�����ļ���ַ����һ���ļ���
u8 f_create(char *file_name);                               //�����ļ�������һ���ļ�
u8 f_create_long_name(char *file_name,u8 long_name_ptr);    //�������ļ����ļ�

bool f_close(void);                                          //�ر��ļ�
void f_stop(void);
bool f_read(void *buf);                                      //�ļ��Ķ�ȡ��ÿ�ζ�ȡ�Ĵ�СΪ512Byte��
u8 f_write(void *buf);                                       //�ļ���д��
bool f_sync(void);                                           //�ļ�ͬ��
bool f_lseek(SEEK_MODE type, u32 ofs);                       //�ļ���дָ�����ת

void file_path_save_hook(void);

//�������read seek ����
bool f_open_opt(u16 file_num);                               //���ٰ汾��f_open����Ҫ���������ļ��Ĳ��ŵ��Ż����Լ������ļ��͸���ļ���ͬʱ�򿪡����û�����Ӧ�õ��У�����ʹ��f_open

bool f_open_lrcfile(void);                                   //�򿪸���ļ�

bool f_open_tmpfile(void);                                   //�򿪸����õ���ʱ�ļ�
bool f_open_binfile(void);                                   //����������Code�ļ�
bool f_open_folder(u16 dir_num);
bool f_delete_tmpfile(void);                                 //ɾ�������õ���ʱ�ļ�

u8 get_scan_dept(void);                                      //��ȡ��ǰ�����ļ��еļ�����0��ʾ��ǰ����λ��Ϊ��Ŀ¼
void f_get_filename(char* file_name);                                  //��ȡ�ļ��������ļ�����
void f_get_opt_filename(char* file_name);                                  //�����ļ��Ļ�ȡ�ļ��������ļ�����
bool f_attr_is_dir(FIL *hFile);                                    //�ж�Ŀ¼���Ƿ�Ϊ�ļ���
bool f_attr_is_file(FIL *hFile);                                   //�ж�Ŀ¼���Ƿ�Ϊ�ļ�

//�ļ��������


bool fs_sel(u8 dev);                                //ѡ���豸
__near_func u8 fs_cur_dev(void);                    //���ص�ǰ�豸
void fs_unmount(void);                              //ж���ļ�ϵͳ
u16 fnav_init(u16 file_num);                        //���ݵ�ǰ�ļ���ţ���ʼ���ļ�����
void fnav_exit(void);                               //�˳��ļ�����
u16 fnav_dir_scan(void);                            //ɨ�裬��ȡ��ǰĿ¼�µ��ļ��м��ļ�����
void fnav_fill(void *buf, u16 start, u16 num);      //����ļ������б����ʾ����Buffer
bool fnav_delete(u16 index);                        //�ڵ�����ɾ���ļ�
bool fnav_cd(u16 dir_num);                          //�����ļ���
u16 fnav_cdup(void);                                //������һ��
u16 fnav_get_index(u16 index);                      //��ȡ�ļ����µ��ļ�������Ŀ¼�е��ļ�������
u16 fnav_get_dirtotal(void);                        //��ȡ��ǰ�ļ����µ����ļ��и���


void f_scan_set(u8 scan_mode, bool (*file_filter)(void), bool (*dir_filter)(void));//����ɨ���ļ�������
#endif
