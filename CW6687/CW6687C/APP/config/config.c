/*****************************************************************************
 * Module    : Config
 * File      : config.c
 * Author    : Hanny
 * Function  : 用户配置文件，点击左侧目录展开，可编辑相应的配置
 *****************************************************************************/
#include "config.h"
#define CONFIG_C
#include SET_USER_PATH(CFG_IO_KEY_DIR, /io/io_key.c)
#include SET_USER_PATH(CFG_IO_IR_DIR, /io/io_ir.c)
#include SET_USER_PATH(CFG_IO_LED_DIR, /io/io_led.c)
#include SET_USER_PATH(CFG_IO_HOLD_DIR, /io/io_hold.c)
#include SET_USER_PATH(CFG_IO_LED_SEG_DIR, /io/io_led_seg.c)

//任务UI，在这里include的目的是为了方便选取
#include SET_USER_PATH(CFG_UI_COMM_DIR, /ui/ui_comm.c)
#include SET_USER_PATH(CFG_UI_MUSIC_DIR, /ui/ui_music.c)
#include SET_USER_PATH(CFG_UI_RECORD_DIR, /ui/ui_record.c)
#include SET_USER_PATH(CFG_UI_FM_DIR, /ui/ui_fm.c)
#include SET_USER_PATH(CFG_UI_CLOCK_DIR, /ui/ui_clock.c)
#include SET_USER_PATH(CFG_UI_CLOCK_DIR, /ui/ui_alarm.c)
#include SET_USER_PATH(CFG_UI_BT_DIR, /ui/ui_bt.c)
#include SET_USER_PATH(CFG_UI_SPEAKER_DIR, /ui/ui_speaker.c)
#include SET_USER_PATH(CFG_UI_MENU_DIR, /ui/ui_menu.c)
#include SET_USER_PATH(CFG_UI_USBDEV_DIR, /ui/ui_usbdev.c)
#include SET_USER_PATH(CFG_UI_AUX_EQ_DIR, /ui/ui_aux_eq.c)
#include SET_USER_PATH(CFG_UI_UPDATEFW_DIR, /ui/ui_updatefw.c)
#include SET_USER_PATH(CFG_UI_TOMCAT_DIR, /ui/ui_tomcat.c)
