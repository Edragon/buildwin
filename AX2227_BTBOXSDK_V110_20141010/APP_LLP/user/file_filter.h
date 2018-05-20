/*****************************************************************************
 * Module    : User
 * File      : file_filter.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �ļ�ɨ�������
 *****************************************************************************/
#ifndef FILE_FILTER_H
#define FILE_FILTER_H

#define SCAN_SUB_FOLDER     0x01        //�Ƿ���Ҫɨ����Ŀ¼

#define AM_RDO  0x01    /* Read only */
#define AM_HID  0x02    /* Hidden */
#define AM_SYS  0x04    /* System */
#define AM_VOL  0x08    /* Volume label */
#define AM_LFN  0x0F    /* LFN entry */
#define AM_DIR  0x10    /* Directory */
#define AM_ARC  0x20    /* Archive */
#define AM_NFC  0x80    /* NoFatChain */

bool file_filter_all(void);
bool dir_filter_all(void);
bool file_filter_music(void);
bool dir_filter_music(void);
bool file_filter_voice(void);
bool dir_filter_voice(void);
u16 GetFileLength(void);

/* File system object structure */
typedef struct _FATFS {
//  u16     id;                /* File system mount ID */
    u16     n_rootdir;      /* Number of root directory entries */
    u16     s_size;         /* sector size*/
    u32     winsect;        /* Current sector appearing in the win[] */
    u32     fatbase;        /* FAT start sector */
    u32     dirbase;        /* Root directory start sector */
    u32     database;       /* Data start sector */
    u32     sects_fat;      /* Sectors per fat */
    u32     max_clust;      /* Maximum cluster# + 1 */

    u32 last_clust;        /* Last allocated cluster */
    u32 free_clust;        /* Number of free clusters */

    u8      fs_type;        /* FAT sub type */
    u32     csize;          /* Number of sectors per cluster,int for exfat */
    u8      n_fats;         /* Number of FAT copies */
    u8      winflag;        /* win[] dirty flag (1:must be written back) */
    u8      n512per_sect;

    bool    (__near_func *disk_read)(void *,u32,u32);
    bool    (__near_func *disk_write)(void *,u32,u32);
    //u8    (__near_func *check_fs)(FATFS *fs, u32 sect);

    u8*     win;                /* Disk access window for Directory/FAT/File */
    u8*     win_back;

    BYTE    fattrib_save;       /* save file attrib*/
    DWORD   fsize_clust;        /* File size, for delete*/
    u32     fat_sector;          /* save temp for fat sector, decrease invoke times*/
    u8*     fat_temp;           /* save fat sector buffer*/
} FATFS;

/* Directory object structure */
typedef struct _DIR
{
    /* ��DIR��FIL��ͬ���ֶη���ͬһ��λ�ã��Է���get_cluster���� */
    u8      fattrib;            /* Attribute */
    FATFS*  fs;                 /* Pointer to the owner file system object */
//    u8    fname[8+3+1];       /* Name (8+3 format) */
    u32     sclust;             /* Start cluster */
    u32     clust;              /* Current cluster */
    u32     sect;               /* Current sector */
    u32     cindex;             /* Current index */
    //u16     fileTotal;            /* file counte in dir */
    //u8      dirDeep;          /* dir deepth */
    u32     last_sect;      /* more than one dentry & long file name reserve*/

} DIR;

/* DIR status structure */
typedef struct _FILINFO
{
    u32     fsize;          /* Size */
//    u16   fdate;          /* Date */
//    u16   ftime;          /* Time */
    u32     sclust;         /* File start cluster */
//    u32   curr_sect;      /* Current sector */
//    u16     cindex;           /* Current dir Item index */
    u16     exFatNameHash;
    DIR    dir;/*DIR*    dir;*/
    u8      fattrib;        /* Attribute */
    char    fname[8+3+1];       /* Name (8+3 format) */
} FILINFO;

/* File object structure */
typedef struct _FIL
{
    u8      flag;           // File status flags
//    u16   id;                 /* Owner file system mount ID */
    FATFS*  fs;                 /* Pointer to the owner file system object */
    /* ��DIR��FIL��ͬ���ֶη���ͬһ��λ�ã��Է���get_cluster���� */
    u8      fattrib;        // attributes and NoFatChain
    u32     org_clust;          // File start cluster
    u32     curr_clust;         // Current cluster
    u32     curr_sect;          // Current sector
    u8      ser_clust;          // �����صĴ�С
    u32     fptr;               // File R/W pointer
    u32     fsize;              // File size
    u32     sclust;             /* File start cluster */
    s32     seek;               // seek byte length
    u32      csect;                     /* Sector address in the cluster */
    char    fname[8+3+1];           // Name (8+3 format)
    u8      dirName[8+3+1];         // �ļ����ڵ�Ŀ¼��Ŀ¼��
    //void    (*load_clust)(void);
    //void    (*save_clust)(void);
    DIR    dir;               //�ļ�����Ŀ¼����Ϣ
    u8      fileDeep;               //��ǰ�ļ����ڵ�·�����
    //u8*     buf;

    u32      dir_sect;          /* Sector containing the directory entry */
    u8*      dir_ptr;           /* Ponter to the directory entry in the window */

    void    (__near_func *load_clust)(void);
    void    (__near_func *save_clust)(void);
} FIL;

/* File system API info */
typedef struct _FSAPIINFO
{
    char *file_name;			// file name ptr
    u16  file_number;			// ��ǰ�ļ����
    u16  file_total;			// �ļ�����
    u16  file_count;			// �����ļ��������ļ����ۼ�
    u32  file_dir_index;        // ��ǰ�ļ����ļ����е����       //��ʱδ�õ�
    u16  file_total_dir;        // ��ǰĿ¼һ���ж��ٸ���Ч�ļ�
    u16  file_total_boundary;   // ǰһ��Ŀ¼�����һ���ļ������
    u8   dir_deep;			// dir deepth for search
} FSAPIINFO;

typedef struct{
    u16 dir_count;//�ļ���Ŀ¼ͳ��
    u16 dir_num;
    u16 dir_total;
    u8 flag;
}type_dir_save;

#endif
