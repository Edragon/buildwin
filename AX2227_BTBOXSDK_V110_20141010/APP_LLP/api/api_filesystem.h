/*****************************************************************************
 * Module    : API
 * File      : api_filesystem.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 文件系统API接口
 *****************************************************************************/
#ifndef _API_FILESYSTEM_H_
#define _API_FILESYSTEM_H_

/******************************详细的函数说明请查阅《AX2228-APM080二次开发文档.pdf》************************/

#define  DIR_NAME_LEN	7
#define LONGNAME DIR_NAME_LEN+14

typedef enum {
    F_SEEK_ORG = 0,
    F_SEEK_CUR
}SEEK_MODE;

bool fs_sel(u8 dev);                                        //选择设备
__near_func u8 fs_cur_dev(void);                            //返回当前设备
void f_scan_set(u8 scan_mode, bool (*file_filter)(void), bool (*dir_filter)(void));//配置扫描文件过滤器

__near_func bool fs_is_mounted(void);                       //判断文件系统是否已经初始化，并且有效。
__near_func bool fs_need_mounted(void);                     //判断文件系统是否需要初始化。如果未初始化过，则进行初始化。
__near_func bool fs_writable(void);                         //判断文件系统是否可写
bool fs_init(void);                                         //文件系统的初始化
void fs_unmount(void);                                      //卸载文件系统

u16 f_gettotal(void);                                       //获取存储介质上的文件总数。
u16 f_get_indexbefore(void);                                //获取当前目录之前有多少个文件
u16 f_get_dirtotal(void);                                   //获取当前目录有多少个文件
bool f_mkdir(char *path);                                   //创建一个目录
bool f_open(u16 file_num);                                  //根据文件编号，打开一个文件。
bool f_openfile(char *path);                                //根据文件地址，打开一个文件。
bool f_delete(u16 file_num);                                //根据文件编号，删除一个文件。
u8 f_create(char *file_name);                               //根据文件名创建一个文件（短文件名）
u8 f_create_long_name(char *fname, u8 lfn_buf);             //根据文件名创建一个文件（长文件名）
//u8 f_create_long_name(char *file_name);

bool f_lseek(SEEK_MODE type, u32 ofs);                       //文件读写指针的跳转
bool f_read(void *buf);                                      //文件的读取。每次读取的大小为512Byte。
u8 f_write(void *buf);                                       //文件的写入
bool f_sync(void);                                           //文件写同步
bool f_close(void);                                          //关闭文件

u16 fat_folder_select(u16 dir_num);

//带缓冲的read seek 操作
bool f_open_lrcfile(void);                                   //打开歌词文件
bool f_open_opt(u16 file_num);                               //加速版本的f_open，主要用于音乐文件的播放的优化，在用户其它应用当中，建议使用f_open
__near_func bool f_read_opt(void *buf);                      //加速版本的f_read，主要用于音乐文件的播放的优化，在用户其它应用当中，建议使用f_read。
__near_func bool f_seek_opt(u8 mode, u32 offset);            //加速版本的f_lseek，主要用于音乐文件的播放的优化，在用户其它应用当中，建议使用f_lseek


void dir_path_save_hook(void);
u8 get_scan_dept(void);                                      //获取当前所在文件夹的级数。0表示当前所在位置为根目录
u16 f_get_filelen(void);
void f_get_filename(void);                                   //获取文件名(长文件名)
bool f_attr_is_dir(void);                                    //判断目录项是否为文件夹
bool f_attr_is_file(void);                                   //判断目录项是否为文件


#endif
