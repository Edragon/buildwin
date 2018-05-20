/*****************************************************************************
* Module    : Main
* File      : makecfg.c
* Author    : Hanny
* Email     : coldney@163.com
* Function  : COD文件生成配置
*****************************************************************************/
#include "include.h"

#pragma constseg="MAKECODE_CONST"
IAR_ROOT IAR_CONST u8 makecode[32] = {
//====== makecode_xx.bin ======
//00: 使用P06做升级Key
//01: 使用P13做升级Key
    MAKECODE_ID,
//====== update_xx.bin ======
//00: 不需要SD卡升级
//01: 需要SD卡升级
    SD_UPDATE_ID,
#if LCD_THEME
//====== table_xx.bin ======
//00: 不使用字库转换表
    0x01,
//====== font_res_xx.bin ======
//00: 不使用字库
    0x01,
//====== res_xx.bin ======
//00: 不使用图片资源
    0x01,
//====== menu_xx.bin ======
//00: 不使用菜单图片资源
    0x01,
#else
//====== table_xx.bin ======
//00: 不使用字库转换表
    0x00,
//====== font_res_xx.bin ======
//00: 不使用字库
    0x00,
//====== res_xx.bin ======
//00: 不使用图片资源
    0x00,
//====== menu_xx.bin ======
//00: 不使用菜单图片资源
    0x00,
#endif
//====== mp3res_xx.bin ======
//00: 不使用语音提示功能
//01: 使用中文提示音
//02: 使用英文提示音
    MP3RES_ID,
//====== btinfo_xx.bin ======
//00: 蓝牙相关信息
    0x00,
//====== 保留 ======
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,

//====== 预留空间 ======
//0200: 预留128K用于保存系统和蓝牙参数
    0x00, 0x02,

//====== 对齐Flash空间 ======
//对齐为256K/512K/1M/2M/4M/8M/16M/32M byte
//对齐时蓝牙参数分配到最后3个sector，不会再分配预留空间
    0x00, ALIGN_COD,
};
