/*****************************************************************************
* Module    : Main
* File      : makecfg.c
* Author    : Hanny
* Function  : COD�ļ���������
*****************************************************************************/
#include "include.h"

#pragma constseg="MAKECODE_CONST"
IAR_ROOT IAR_CONST u8 makecode[32] = {
//====== makecode_xx.bin ======
//00: Ĭ�ϵ���������
    MAKECODE_ID,
//====== update_xx.bin ======
    0x00,
//====== table.bin ======
//00: ��ʹ���ֿ�ת����
    CHARACTER_CONVERT,
//====== font_res.bin ======
//00: ��ʹ���ֿ�
    CHARACTER,
//====== res.bin ======
//00: ��ʹ��ͼƬ��Դ
    PICTURE,
//====== menu.bin ======
//00: ��ʹ�ò˵�ͼƬ��Դ
    MENU_PICTURE,
//====== mp3res.bin ======
//00: ��ʹ��������ʾ����
    0x01,
//====== btinfo.bin ======
//00: ���������Ϣ
    0x01,
//====== ���� ======
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00,

//====== LOADER DEBUG ======
//�Ƿ���Ҫ����LOADER��DEBUG��Ϣ
    DEBUG_LOADER_DIS,        //27

//====== Ԥ���ռ� ======
//0300: Ԥ��128K+64K(�е绰������ʱ)���ڱ���ϵͳ�������������绰����Ϣ
    0x00, 0x02+BT_PHONE_BOOK_EN,    //28 - 29
    0x00, 0x00,                     //30 - 31
};

#pragma constseg="MAKEBAT_CONST"
__root IAR_CONST char tbl_make_bat[] =
//����MP3RES��Դ�ļ�
 "copy /y \"..\\..\\config\\"
 SET_USER_PATH(CFG_DIR, \\mp3res\\mp3res.bin)
 "\" \"bin\\mp3res.bin\"\r\n"


 ;
