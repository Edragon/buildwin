#ifndef __USER_MP3RES_H_
#define __USER_MP3RES_H_

#if MP3RES_ID == 0x00
    #include "mp3res_00.h"
#elif MP3RES_ID == 0x01     //DEFAULT_CN
    #include "mp3res_01.h"
#elif MP3RES_ID == 0x02     //DEFAULT_EN
    #include "mp3res_02.h"
#elif MP3RES_ID == 0x80     //ALL_CN
    #include "mp3res_80.h"
#elif MP3RES_ID == 0x81     //ALL_EN
    #include "mp3res_81.h"
#elif MP3RES_ID == 0x82     //YL_CN
    #include "mp3res_82.h"
#elif MP3RES_ID == 0x83     //YL_EN
    #include "mp3res_83.h"
#elif MP3RES_ID == 0x84     //BT_AUX_CN
    #include "mp3res_84.h"
#elif MP3RES_ID == 0x85     //BT_AUX_EN
    #include "mp3res_85.h"
#elif MP3RES_ID == 0x86     //TX1336_2_CN
    #include "mp3res_86.h"
#elif MP3RES_ID == 0x87     //TX1336_2_EN
    #include "mp3res_87.h"
#elif MP3RES_ID == 0x89     //WS_188-887_EN
    #include "mp3res_89.h"
#elif MP3RES_ID == 0x8A     //HZNF_BT-08_CN
    #include "mp3res_8A.h"
#elif MP3RES_ID == 0x90
    #include "mp3res_90.h"
#elif MP3RES_ID == 0x92
    #include "mp3res_92.h"
#endif


#endif
