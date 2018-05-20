/*****************************************************************************
 * Module    : User
 * File      : file_filter.h
 * Author    : Hanny
 * Function  : 文件扫描过滤器
 *****************************************************************************/
#ifndef FILE_FILTER_H
#define FILE_FILTER_H

#define SCAN_SUB_FOLDER     0x01        //是否需要扫描子目录
#define SCAN_SPEED          0x20        //是否使用搜文件加速

#define AM_RDO              0x01        /* Read only */
#define AM_HID              0x02        /* Hidden */
#define AM_SYS              0x04        /* System */
#define AM_VOL              0x08        /* Volume label */
#define AM_LFN              0x0F        /* LFN entry */
#define AM_DIR              0x10        /* Directory */
#define AM_ARC              0x20        /* Archive */
#define AM_NFC              0x80        /* NoFatChain */

#define FS_FAT12            1
#define FS_FAT16            2
#define FS_FAT32            3
#define FS_EXFAT            4

bool file_filter_all(void);
bool dir_filter_all(void);
bool file_filter_music(void);
bool dir_filter_music(void);
bool file_filter_voice(void);
bool dir_filter_voice(void);
bool dir_filter_txt(void);
bool file_filter_txt(void);
#if MUSIC_ENCRYPT_EN
void music_check_encrypt(void);
#endif
u16 GetFileLength(void);

/* File system object structure */
typedef struct _FATFS {
//  u16     id;             /* File system mount ID */
    u16     n_rootdir;      /* Number of root directory entries */
    u16     s_size;         /* sector size*/
    u32     winsect;        /* Current sector appearing in the win[] */
    u32     fatbase;        /* FAT start sector */
    u32     dirbase;        /* Root directory start sector */
    u32     database;       /* Data start sector */
    u32     sects_fat;      /* Sectors per fat */
    u32     max_clust;      /* Maximum cluster# + 1 */

    u32     last_clust;     /* Last allocated cluster */
    u32     free_clust;     /* Number of free clusters */
    u16     limit_clust;    /* 预分配簇大小限制 (number of clust) */

    u8      fs_type;        /* FAT sub type */
//  u32     csize;          /* Number of sectors per cluster,int for exfat */
    u8      csize_shift;    /* 记录csize的移位 */
    u8      n_fats;         /* Number of FAT copies */
    u8      winflag;        /* win[] dirty flag (1:must be written back) */
//  u8      n512per_sect;
    u8      n512sec_shift;  /* 记录n512per_sect的移位 */

    bool    (__near_func *disk_read)(void *,u32);
    bool    (__near_func *disk_write)(void *,u32);
//  u8      (__near_func *check_fs)(u32 sect);

    u8*     win;            /* Disk access window for Directory/FAT/File */
    u8*     win_back;

    BYTE    fattrib_save;   /* save file attrib*/
    DWORD   fsize_clust;    /* File size, for delete*/
    u32     fat_sector;     /* save temp for fat sector, decrease invoke times*/
    u8*     fat_temp;       /* save fat sector buffer*/

    u32     free_fatbase;   //空簇扫描加速，记录可用的空簇
} FATFS;

//将fat_fs部份放入pdata
typedef struct {
    u32     clust_mask;         //这个必须放在第一个，汇编时需要调用
    u8      n512clust_shift;    //记录csize * n512per_sect的移位和

    u8      mp3_input;          //控制Input Ouput的大小,1-3
} FATFS_HOOK;

/* Directory object structure */
typedef struct _DIR {
    /* 将DIR与FIL相同的字段放在同一个位置，以方便get_cluster操作 */
    FATFS*  fs;                 /* Pointer to the owner file system object */
//  u8      fname[8+3+1];       /* Name (8+3 format) */
    u32     sclust;             /* Start cluster */
    u32     clust;              /* Current cluster */
    u32     sect;               /* Current sector */
    u32     cindex;             /* Current index */
//  u16     fileTotal;          /* file counte in dir */
//  u8      dirDeep;            /* dir deepth */
} DIR;

/* DIR status structure */
typedef struct _FILINFO {
    u32     fsize;          /* Size */
//  u16     fdate;          /* Date */
//  u16     ftime;          /* Time */
    u32     sclust;         /* File start cluster */
//  u32     curr_sect;      /* Current sector */
//  u16     cindex;         /* Current dir Item index */
    u16     exFatNameHash;
    DIR     dir;            /*DIR*    dir;*/
    u8      fattrib;        /* Attribute */
    char    fname[8+3+1];   /* Name (8+3 format) */
} FILINFO;

/* File object structure */
typedef struct _FIL {
    u8      flag;               // File status flags                             //1
//  u16     id;                 /* Owner file system mount ID */
    FATFS*  fs;                 /* Pointer to the owner file system object */   // 2
    /* 将DIR与FIL相同的字段放在同一个位置，以方便get_cluster操作 */
    u8      fattrib;            // attributes and NoFatChain                 // 4
    u32     org_clust;          // File start cluster                         // 5
    u32     cur_clust;          // Current cluster                            //9
    u32     cur_sect;           // Current sector                            // 13
    u8      ser_clust;          // 连续簇的大小                              // 17
    u32     fptr;               // File R/W pointer                          //18
    u32     fsize;              // File size                                // 22
    u32     sclust;             /* File start cluster */                    //  26
    s32     seek;               // seek byte length                         //  30
//  u32     csect;              /* Sector address in the cluster */
    char    fname[8+3+1];       // Name (8+3 format)                       //  34
//  u8      dirName[8+3+1];     // 文件所在的目录的目录名

    DIR     dir;                //文件所在目录的信息                //  46
    u8      fileDeep;           //当前文件所在的路径深度             // 64
//  u8*     buf;

    u16     remain_clust;       /* 预分配簇剩余个数 */               //65
    u32     end_clust;          /* 预分配时文件结束簇 */            // 67
    u32     total_clust;        /* 文件总簇数 */                    // 71
    u32     dir_sect;           /* Sector containing the directory entry */   //75
    u8*     dir_ptr;            /* Ponter to the directory entry in the window */// 79

    void    (__near_func *load_clust)(void);    // 81
    void    (__near_func *save_clust)(void);  //83
} FIL;

/* File system API info */
typedef struct _FSAPIINFO {
    char *file_name;			// file name ptr
    u16  file_number;			// 当前文件序号
    u16  file_count;			// 搜索文件过程中文件数累加
    u16 dir_count;                      //文件夹计数
    u16 dir_number;                     //目标文件夹编号
    /*u32  file_dir_index;        // 当前文件在文件夹中的序号       //暂时未用到
    u16  file_total_dir;        // 当前目录一共有多少个有效文件
    u16  file_total_boundary;   // 前一个目录中最后一个文件的序号
    u8   dir_deep;			    // dir deepth for search*/
} FSAPIINFO;

extern FATFS fat_fs;
extern FATFS *pfs;
extern FATFS_HOOK fs_hook;
extern DIR file_dir;                    //当前目录项
extern FIL fat_file, fat_file_opt;      //内部的两个文件句柄，一般fat_file_opt用于mp3播放
extern FSAPIINFO fs_apiinfo;            //应用接口信息

#endif
