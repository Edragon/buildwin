/*****************************************************************************
 * Module    : Config
 * File      : config.h
 * Author    : Hanny
 * Function  : 配置文件，本文件只做用户选择，其它请勿改动。
 *****************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

/*****************************************************************************
 * Module    : 用户目录定义，详细用户配置请修改config_setting.h
   user_14_140B		    COB_大红板测试	
   user_14_164B         CW6687E(LQFP48) DEMO
   user_15_122          CW6687G(LQFP64) DEMO
   user_15_146          CW6687B(QFN32)  DEMO
   user_14_164B_6687C   CW6687C(LQFP48) DEMO
   user_15_119B         CW6633B(SSOP28) DEMO
   user_15_152          CW6633E(LQFP48) DEMO
   user_14_164B_6631E   CW6631E(LQFP48) DEMO
   user_15_103          CW6687E 128x64 LCD点阵屏 DEMO
 *****************************************************************************/
#define CFG_DIR         user_14_164B

/*****************************************************************************
 * Module    : 宏配置选择列表，以下宏定义值请勿随意修改，
               详细用户配置请修改config_setting.h
 *****************************************************************************/
#include "config_define.h"
#endif
