/*****************************************************************************
* Module    : Main
* File      : makecfg.c
* Author    : Hanny
* Function  : COD文件生成配置
*****************************************************************************/
#include "include.h"

#pragma constseg="MAKECODE_CONST"
IAR_ROOT IAR_CONST u8 makecode[32] = {
//====== makecode_xx.bin ======
//00: 默认的升级程序
    MAKECODE_ID,
//====== update_xx.bin ======
    0x00,
//====== table.bin ======
//00: 不使用字库转换表
    CHARACTER_CONVERT,
//====== font_res.bin ======
//00: 不使用字库
    CHARACTER,
//====== res.bin ======
//00: 不使用图片资源
    PICTURE,
//====== menu.bin ======
//00: 不使用菜单图片资源
    MENU_PICTURE,
//====== mp3res.bin ======
//00: 不使用语音提示功能
    0x01,
//====== btinfo.bin ======
//00: 蓝牙相关信息
    0x01,
//====== 保留 ======
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00,

//====== LOADER DEBUG ======
//是否需要屏蔽LOADER的DEBUG信息
    DEBUG_LOADER_DIS,        //27

//====== 预留空间 ======
//0300: 预留128K+64K(有电话本功能时)用于保存系统和蓝牙参数、电话本信息
    0x00, 0x02+BT_PHONE_BOOK_EN,    //28 - 29
    0x00, 0x00,                     //30 - 31
};

#pragma constseg="MAKEBAT_CONST"
__root IAR_CONST char tbl_make_bat[] =
//复制MP3RES资源文件
 "copy /y \"..\\..\\config\\"
 SET_USER_PATH(CFG_DIR, \\mp3res\\mp3res.bin)
 "\" \"bin\\mp3res.bin\"\r\n"


 ;
