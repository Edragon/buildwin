/*****************************************************************************
* Module    : Task
* File      : task.c
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : 任务控制器
*****************************************************************************/
#include "include.h"
#include "task.h"
#include "btapi.h"
#include "task_music.h"

type_task_ctl task_ctl;
void BtMainLoop(void);
void btMMI_Phone();

void test_bt(void);
bool sbc_load_code(u16 addr, u8 segnum);
void btmode_enter(void);
void btmode_exit(void);


//清除、还原默认的设置
void task_clear(void)
{
    WATCHDOG_CLR();
    user_set_volex(3);                  //默认需要3DB音量补偿
#if LCD_THEME
    set_lcd_buf(LCD_MINI_MODE);         //设置显存，使用256Byte小缓存
#endif
    ocx_init(NULL, NULL);               //清空默认的控件事件及消息
    ocx_clear();
#if TASK_MUSIC_EN
    u_msc.music_voice_flag = 0;         //默认播音乐
#endif
    sys_flag.need_mute = 0;             //切任务后要再动态检测是否需要MUTE
    sys_ctl.sleep_cnt = 0;
    sys_ctl.disp_bat_val = 0;
#if VOL_MAXMIN_MUSIC
    sys_ctl.vol_maxmin_music_flag = 0;
#endif
#if BT_DYMANIC_SUPPRESSION || FM_SOUND_DETECT_EN || AUX_SOUND_DETECT_EN
    sound_detect_disable();
#endif
#if KEY_VOICE_EN
    key_voice_init();
#endif
}

//执行任务
void run_task(void)
{
    //bt_pincode_init();                //移到bt_init，详见注释

#if !TASK_BT_ONLY                       //只有BT任务时，不需要检测其他设备
    delay_5ms(40);                      //增加延时，以进行设备检测
    if(!MP3BT_DECT_EN || task_ctl.work_sta != TASK_BT) {
        comm_event();                   //无MP3BT检测或者非蓝牙时扫描设备
    }

    if(task_ctl.work_sta != TASK_BT){   //不进入蓝牙模式时对蓝牙初始化并让其进入低功耗模式以降低功耗
        BTLDO_EN();
        load_comm(COMM_BT);
        sbc_load_code(DCODE_BT_SBC, 3);
        btmode_enter();
        test_bt();
        BT_STATE = StateNone;
        hcmd_w_scan_enable(0x00);
        btmode_exit();
        BTLDO_DIS();
        load_comm(COMM_NORMAL);

        #if BT_SOFT_POWEROFF_AUTO_PLAY
            sys_flag.bt_auto_play_flag = 0;
            param_write8(PARAM_BT_AUTO_PLAY, sys_flag.bt_auto_play_flag);
        #endif
    }

#endif  //TASK_BT_ONLY

    while (1) {
        task_clear();
        switch (task_ctl.work_sta) {
#if TASK_MUSIC_EN
        case TASK_MUSIC:
            task_ctl.last_work = TASK_MUSIC;
            task_music();
            break;
#endif

#if TASK_MIC_EN
        case TASK_MIC:
            task_ctl.last_work = TASK_MIC;
            task_mic();
            break;
#endif

#if TASK_VOICE_EN
        case TASK_VOICE:
            task_ctl.last_work = TASK_VOICE;
            task_voice();
            break;
#endif

#if TASK_FM_EN
        case TASK_FM:
            task_ctl.last_work = TASK_FM;
            task_fm();
            break;
#endif

#if TASK_AUX_EN
        case TASK_AUX:
            task_ctl.last_work = TASK_AUX;
            task_aux();
            break;
#endif

#if TASK_LOUDSPEAKER_EN
        case TASK_LOUD_SPEAKER:
            task_ctl.last_work = TASK_LOUD_SPEAKER;
            task_loud_speaker();
            break;
#endif

#if TASK_USBDEV_EN
        case TASK_USBDEV:
            task_ctl.last_work = TASK_USBDEV;
            task_usbdev();
            break;
#endif

#if TASK_BT_EN
        case TASK_BT:
            task_ctl.last_work = TASK_BT;		
            task_bt();
            break;
#endif

#if TASK_RTC_EN
        case TASK_CLOCK:
            task_ctl.last_work = TASK_CLOCK;
            task_clock();
            break;
#endif

#if ALARM_EN
        case TASK_ALARM:
            task_alarm();
            break;
#endif

#if TASK_POWEROFF_EN
        case TASK_POWEROFF:
            task_poweroff();
            break;
#endif

#if TASK_IDLE_EN
        case TASK_IDLE:
            task_idle();
            break;
#endif

        case TASK_EXIT:
            task_ctl.work_sta = TASK_IDLE;
            //task_exit();
            break;
        default:
            task_exit();            //退出无效的菜单
            break;
        }
    }
}

//检查，并加载设备
__near_func bool device_change(void)
{
    if (!task_ctl.dev_change) {
        return false;                //设备正常
    }
    task_ctl.dev_change = 0;
    return device_change_check();
}

//定时关机函数
#pragma inline=forced
__near_func void task_sleep_check(void)
{
    if (sys_ctl.sleep_cnt > 1) {
        sys_ctl.sleep_cnt--;
    }
}

#pragma inline=forced
__near_func void task_poweroff_check(void)
{
    if (sys_flag.lowpower_flag) {
        task_ctl.work_sta = TASK_POWEROFF;
    }
}

//退出任务
void task_exit(void)
{
    //如果没有主菜单，则切换到下一个任务
    switch (task_ctl.last_work) {
    case TASK_MUSIC:
#if TASK_VOICE_EN
        task_ctl.work_sta = TASK_VOICE;         //切换到播放录音
        break;

    case TASK_VOICE:
#endif

#if TASK_MIC_EN
        task_ctl.work_sta = TASK_MIC;           //切换到MIC
        break;

    case TASK_MIC:
#endif

#if TASK_FM_EN
        task_ctl.work_sta = TASK_FM;            //切换到FM
        break;

    case TASK_FM:
#endif

#if TASK_AUX_EN
        task_ctl.work_sta = TASK_AUX;
        break;

    case TASK_AUX:
#endif

#if TASK_LOUDSPEAKER_EN
        task_ctl.work_sta = TASK_LOUD_SPEAKER;
        break;

    case TASK_LOUD_SPEAKER:
#endif

#if TASK_RTC_EN
        task_ctl.work_sta = TASK_CLOCK;
        break;

    case TASK_CLOCK:
#endif

#if TASK_USBDEV_EN
        task_ctl.work_sta = TASK_USBDEV;
        break;

    case TASK_USBDEV:
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if TASK_BT_EN && (!MP3BT_DECT_EN)              //2227需要蓝牙与其他任务之间的互相切换以方便开发调试，S10不需要
        task_ctl.work_sta = TASK_BT;            //由FM切换到LINEIN
        break;

    case TASK_BT:
#endif

    default:
        task_ctl.work_sta = TASK_MUSIC;         //切回音乐播放任务
        break;
    }
}


//所有任务的公共消息处理函数
__near_func void deal_msg(u8 msg)
{
    switch (msg) {
        //发起蓝牙连接请求，切换到蓝牙模式
#if BT_ALL_TASK
    case BT_CONN_REQ:
    #if TASK_BT_EN
        task_ctl.work_sta = TASK_BT;
    #endif
        break;
#endif
        //长按菜单键，退出到主菜单
    case KU_MODE:
#if KL_PLAY_S10_CHANGE_MODE
    case KL_PLAY_S10:   //S10在非蓝牙模式下长按播放键切换任务
#endif
#if K_HSF_CHANGE_MODE
  #if BT_CALL_PRIVATE
    case KU_HSF:
  #else
    case KL_HSF:
  #endif
#endif
        if ((BT_STATE != StateNone) && (HSFRingActive || HSFCallActive || scoflag)) {
            break;      //通话过程中不切模式
        } else {
#if MODE_CHANGE_BLUE_ON
            led_mode_on();
#endif
            task_ctl.work_sta = TASK_EXIT;
            break;
        }

#if(SOFT_POWER_ON_OFF) //使用软开关机功能
    case KL_PLAY:
    case KL_POWER:
    #if BT_SOFT_POWEROFF_AUTO_PLAY
        if(BT_STATE == StatePlay){
            sys_flag.bt_auto_play_flag = 1;
            param_write8(PARAM_BT_AUTO_PLAY, sys_flag.bt_auto_play_flag);
            param_write(Link_BdAddr, PARAM_REMOTE_DEV_ADDR, 6);
        }
        else{
            sys_flag.bt_auto_play_flag = 0;
            param_write8(PARAM_BT_AUTO_PLAY, sys_flag.bt_auto_play_flag);
        }
    #endif
        task_ctl.work_sta = TASK_POWEROFF;
        break;
#endif

#if IR_HARD_POWERDOWN_EN
    case KU_POWER:
        sys_flag.ir_power_flag = 1;
        //printf("IR_POWER_DOWN\n");
        break;
#endif

    //1秒消息
    case QSYSTEM_1S:
#if ALARM_EN
        task_alarm_check();
#endif
        task_sleep_check();
        task_poweroff_check();
        break;

#if (HARD_IR_EN || SOFT_IR_EN)
    case T_KEY_MUTE:
        if(sys_flag.mute_flag){
            dac_fade_in(0);
            music_unmute();
        }
        else{
            dac_fade_out(0);
            dac_anl_fade_wait();
            music_mute();
        }
        break;
#endif
    }
}

//所有任务的公共事件处理
__near_func bool comm_event(void)
{
    bool ret = true;
    WATCHDOG_CLR();

#if BT_ALL_TASK
    if (flag_btmode) {
        BtMainLoop();
    } else {
        BtMainLoopTimes(1);
    }
    if (HSFRingActive || HSFCallActive || scoflag) {
        return true;                                        //通话过程不做任何切换
    }
#else
    if (flag_btmode) {
        BtMainLoop();
    #if MP3BT_DECT_EN
        return true;                                        //开机有MP3BT检测，不需要自动切模式
    #else
        if (HSFRingActive || HSFCallActive || scoflag) {
            return true;                                    //通话过程不做任何切换
        }
    #endif
    }
#endif

#if LINEIN_DETECT_EN
    if (device_need_activate(DEVICE_LINEIN)) {
        if (device_activate(DEVICE_LINEIN)) {
            task_ctl.work_sta = TASK_AUX;                   //任意状态下，插入LINEIN
            ret = false;
        }
    }
#endif

#if MUSIC_MODE_AUTO
    if (device_need_activate(DEVICE_SDMMC) || device_need_activate(DEVICE_UDISK)) {
        if(!u_msc.music_voice_flag) {
            task_ctl.work_sta = TASK_MUSIC;                 //任意状态下，插入设备
        }
        ret = false;
    }
#endif

#if TASK_USBDEV_EN
    if (device_need_activate(DEVICE_PC)) {
        if (device_activate(DEVICE_PC)) {
            task_ctl.work_sta = TASK_USBDEV;
            ret = false;
        }
    }
#endif
    return ret;
}

//判断utf8有多少个字节
#pragma location="LRC_SEG"
u8 utf8_char_size(char code)
{
    if (code <= 0x7f)//说明最高位为'0'，这意味着utf8编码只有1个字节！
    {
        return 1;
    }
    else if ((code & 0xe0)== 0xc0)//只保留最高三位，看最高三位是不是110，如果是则意味着utf8编码有2个字节！
    {
        return 2;
    }
    else if ((code & (0xf0))== 0xe0)//只保留最高四位，看最高三位是不是1110，如果是则意味着utf8编码有3个字节！
    {
        return 3;
    }
    else
    {
        return 0;
    }
}
