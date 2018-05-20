/*****************************************************************************
 * Module    : API
 * File      : api.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : API标准函数列表
 *****************************************************************************/
#ifndef API_H
#define API_H

//系统函数
#include "api_system.h"

//调试接口
#include "api_debug.h"

//文件系统API接口
#include "api_filesystem.h"

//音乐播放API接口
#include "api_music.h"

//SPIFlash API接口
#include "api_spi.h"

//设备管理的相应接口
#include "api_device.h"

//记忆信息
#include "api_param.h"

//字符串相关
//#include "user_string.h"
#include "uart_drv.h"

//字符串相关
#include "Menu.h"

void iis_init(void);
__near_func void iis_set_rate(u8 sample_rate);

void timer2_init(void);
void timer2_hook_enable(void);
/*****************************************************************************
 * Module    : 临时区，最后需要删除
 *****************************************************************************/
extern IAR_DATA u8 ocx_show_cnt;
#endif
