/*****************************************************************************
 * Module    : Task
 * File      : task_bt.c
 * Author    : dingping
 * Email     :
 * Function  : 蓝牙任务流程
 *****************************************************************************/
#include "include.h"
#if (TASK_BT_EN && BT_TYPE == BT6639)
#include "task.h"
#include "user_bt.h"
#include "btapi.h"

#define DEBUG_TASK_BT        0

void test_bt(void);
void btmode_enter(void);
void btmode_exit(void);
void bt_test_key(void);
void task_bt_display(void);
void spp_tx(u8 *ptr, u8 len);
void hid_key_camera(void);
bool hcmd_disconnect_sco(void);
u8 hsf_send_atcmd(u8 *str, u16 len);
bool sbc_load_code(u16 addr, u8 segnum);
HciCon * SearchHciBDAddrLinkType(u8* BDAddr, u8 linktype);
void task_bt_event(void);
u8 work_sta_interrupt;          //电话中断标志
extern u8 try_dis_sco;
#if BT_DISP_NUM_TIME
u8 disp_phone_num_cnt = 0;  //来去电号码滚动显示计数控制
u16 on_the_phone_time = 0;  //通话时间
#endif
#if BT_NAME_DYMANIC_CHANGE
#pragma constseg="BT_NAME"
IAR_CONST u8 bt_name[32] = {0};
#pragma constseg=default
#endif

#if BT_SOFT_POWEROFF_AUTO_PLAY || BT_CHANGE_MODE_AUTO_PLAY
u8 temp_remote_dev_addr[6];
#endif

#if NUMBER_KEY_TO_CALL
IAR_XDATA_A u8 call_number_key[24] @ "BT_XDATA";
#endif


void task_bt_state(void)
{
    u8 get_phone_call_number[]="AT+CLCC";  //发送此数组后会在蓝牙库里面自动调用hshf_rx_atcmd(u8 *cmd)去获取手机端拨出的号码
    if(BT_STATE_pre != BT_STATE)
    {
        BT_STATE_pre = BT_STATE;
        bt_dev_bat = 0xff;  //更新电量显示
#if LCD_THEME
        bt_disp_flag = 0xff;        //WARNING_MSG("State: %02x\n", BT_STATE);        switch (BT_STATE)
#endif
        switch (BT_STATE)
        {
        case StateNone:
            led_bt_none();
            break;
        case StateIdle:
            led_bt_idle();
            //SWUart_TX("\nNC\n");	
            break;
        case StateConnected:
            led_bt_connect();
            //SWUart_TX("\nCO\n");
            break;
        case StatePlay:
            led_bt_play();
            //SWUart_TX("\nPL\n");	
            break;
        case OutGoingCall:			
            led_bt_play();
            if(!bt_atcmd_clcc_flag){
                bt_atcmd_clcc_flag = 1;
                hsf_send_atcmd(get_phone_call_number, sizeof(get_phone_call_number)); //获取手机端拨出的号码
            }
            //SWUart_TX("\nOT\n");
            break;
        case OnThePhone:
            led_bt_play();
            //SWUart_TX("\nIN\n");
            break;
        }

#if (THEME_SELECT == LEDSEG_THEME_DEFAULT || THEME_SELECT == LCD_THEME) && BT_DISP_NUM_TIME
        if((BT_STATE != InComingCall && BT_STATE != OutGoingCall)){
            if(bt_atcmd_clcc_flag) {
                bt_atcmd_clcc_flag = 0;
            }
            if(bt_phonenum_flag) {
                bt_phonenum_flag = 0;
            }
        }
        if(BT_STATE != OnThePhone){
            on_the_phone_time = 0;
        }
#endif
    }
    else{   //测试盒测试时灯闪烁控制
        if(Test_Mode_Flag == 1){                                //播放1K正弦波中
            led_bt_play();
        }
        else if(Test_Mode_Flag == 2 || Test_Mode_Flag == 3){    //3：MIC测试； 2：按键测试
            led_bt_connect();
        }
    }
}

void task_bt_reuse_init(void)
{
#if BTRST_REUSE_SDCMD
    sd_stop(1);
    SDCON0 |= BIT(1);
    delay_5ms(2);
    SDCON0 &= ~(BIT(1) | BIT(0));
#endif
}

void task_bt_reuse_exit(void)
{
#if BTRST_REUSE_SDCMD
    sd_reactive();
#endif
}

void task_bt_enter(void)
{
    set_sys_clk(SYS_48M);

#if BT_DYMANIC_SUPPRESSION
    sound_ctl_init(10, 0x08, 100, 0x03);     //连续10次超过0x08认为开始有声；连续100次0x03以下认为静音
    sound_detect_init((void *)0x8000, 144);
    sound_detect_enable(1);
#endif
    draw_clear();
    draw_wait();
    BT_STATE_pre = 0xff;
    led_bt_none();
    music_unmute();
    dac_enable();

#if BT_MODE_MUSIC
    led_bt_voice();
    mp3_res_play_wait(RES_MP3_BT_MODE);
#endif

#if BT_VOLUME_MAX
    user_set_volume(VOLUME_MAX);
#endif

    task_bt_reuse_init();
    load_comm(COMM_BT);
    sbc_load_code(DCODE_BT_SBC, 3);
    btmode_enter();
    music_unmute();

#if BT_NAME_DYMANIC_CHANGE
    param_read((u8 *)&bt_name, PARAM_BT_NAME, 32);
    //printf("enter-read_bt_name[0]= 0x%02x\n",bt_name[0]);
#endif

#if BT_ALL_TASK
    if(flag_bt_first){
        BTLDO_EN();
        flag_bt_first = 0;
        test_bt();
    } else {
        sbc_init();
    }
#else
    BTLDO_EN();
    test_bt();
#endif
#if BT_FREQ_SHOW
    music_freq_init(FREQ_DAC);
#if LCD_THEME
    memset(disp_freq_show_buf, 0, sizeof(disp_freq_show_buf));
#endif
#endif
    dac_fade_in(1);
    show_update();
    ocx_init(task_bt_event, NULL);       //设置OCX需要响应的消息及事件
#if LCD_THEME
    bt_disp_flag = 0xff;
#endif
}

//退出蓝牙任务
#pragma location="TASK_BT_SEG_1"
void task_bt_exit(void)
{
#if BT_CHANGE_MODE_AUTO_PLAY
    if(BT_STATE == StatePlay){
        param_write8(PARAM_BT_AUTO_PLAY, 1);
        param_write(Link_BdAddr, PARAM_REMOTE_DEV_ADDR, 6);
    }
    else{
        param_write8(PARAM_BT_AUTO_PLAY, 0);
    }
    sys_flag.bt_auto_play_flag = 0;
#endif

#if BT_ALL_TASK
    dac_fade_out(1);
    btmode_exit();
    bt_voice_state = BT_VMS_IDLE;
#else
    BT_Disconnect();
#if BT_MODE_EXIT_MUSIC
    bt_voice_put(RES_WAV_BT_EXIT);
#endif
    while (bt_connect_flag || bt_voice_state != BT_VMS_IDLE || bt_voice_outptr != bt_voice_inptr) {
        WATCHDOG_CLR();
        BtMainLoop();
    }
    dac_fade_out(1);
    bool temp_sint = IE_SINT;
    IE_SINT = 0;
    hcmd_w_scan_enable(0x00);
    btmode_exit();
    bt_voice_state = BT_VMS_IDLE;
    load_comm(COMM_NORMAL);
    BT_STATE = StateNone;
    IE_SINT = temp_sint;
    sys_flag.voice_play_flag = 0;
    BTLDO_DIS();
#endif
    dac_anl_fade_wait();
    dac_disable();
    task_bt_reuse_exit();
}

#pragma location="TASK_BT_SEG"
void task_bt_event_poweroff(void)
{
    BT_IS_SLEEP_INIT();
    if(BT_IS_SLEEP() || sys_flag.ir_power_flag){
        BT_IS_SLEEP_END();
#if IR_HARD_POWERDOWN_EN
        if(sys_flag.ir_power_flag && BT_STATE == StatePlay){    //红外遥控关机并且处于播放状态时先暂停再进入hold_mode
            btMMI_PlayPause();
            while(BT_STATE == StatePlay){   //等待蓝牙协议处理完，蓝牙状态切换到暂停(StateConnected)后再进入hold_mode
                WATCHDOG_CLR();
                BtMainLoop();
            }
        }
#endif

#if !STANDBY_WAKEUP_EN
        BT_Disconnect();                        //不需要唤醒时，断开连接并关掉LDO
        while (bt_connect_flag || bt_voice_state != BT_VMS_IDLE) {
            WATCHDOG_CLR();
            BtMainLoop();
        }
        hcmd_w_scan_enable(0x00);
        BTLDO_DIS();
#endif
#if MP3BT_DECT_EN && !BTRST_REUSE_SDCMD
        if(device_need_activate(DEVICE_SDMMC)){ //进入低功耗模式前若卡在线先激活卡，这样可以减少因SD卡在线而引起的电流消耗
            device_activate(DEVICE_SDMMC);
        }
#endif
        music_mute();
        hold_mode_enter();
        music_unmute();
        BT_STATE_pre = 0xff;
        show_update();
    } else {
        BT_IS_SLEEP_END();
        sys_ctl.sleep_cnt = 2;                  //若此时BT_SLEEP()仍为低(蓝牙模块未处于空闲状态)，则此后1秒判断一次BT_SLEEP()
    }
}

#if BT_SOFT_POWEROFF_AUTO_PLAY || BT_CHANGE_MODE_AUTO_PLAY
#pragma location="TASK_BT_SEG"
void bt_auto_play(void)
{
    sys_flag.bt_auto_play_flag = param_read8(PARAM_BT_AUTO_PLAY);//printf("auto: %d\n",sys_flag.bt_auto_play_flag);
    param_read(temp_remote_dev_addr, PARAM_REMOTE_DEV_ADDR, 6);
    if(sys_flag.bt_auto_play_flag == 1 && 0 == memcmp(temp_remote_dev_addr, Link_BdAddr, 6)){
        need_send_ppkey = 1;
    }
    param_write8(PARAM_BT_AUTO_PLAY, 0);
    sys_flag.bt_auto_play_flag = 1;
}
#endif

//蓝牙事件
#pragma location="TASK_BTEVENT_SEG"
void task_bt_event(void)
{
    comm_event();                               //BtMainLoop放到comm_event统一调用，详见BT_ALL_TASK

    if(BT_STATE_pre != BT_STATE || Test_Mode_Flag) {    //防止一直切BANK
        task_bt_state();
        show_update();
    }

#if BT_CALL_PRIVATE     //强制私密接听
    if(HSFCallActive) {
        if (scoflag && try_dis_sco == 0) {
    	//if(SearchHciBDAddrLinkType(Link_BdAddr, HCI_SCO)) {
            try_dis_sco = 1;
            hcmd_disconnect_sco();
        }
    }
#else
    #if BT_ALL_TASK
        if (work_sta_interrupt) {
            if (!HSFRingActive && !HSFCallActive && !scoflag) {
                task_ctl.work_sta = work_sta_interrupt;
                //printf("ret %d\n", work_sta_interrupt);
            }
        }
    #endif
#endif

#if AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((sys_ctl.sleep_cnt == 1 || sys_flag.ir_power_flag) && !sys_flag.lowpower_flag){
        task_bt_event_poweroff();
    }
#endif

#if BT_DISP_NUM_TIME
    //号码及通话时间显示已移到task_bt_state()
#endif

#if BT_SOFT_POWEROFF_AUTO_PLAY || BT_CHANGE_MODE_AUTO_PLAY
    if(BT_STATE == StateConnected && !sys_flag.bt_auto_play_flag){
        bt_auto_play();
    }
#endif

#if BT_FREQ_SHOW
    if (music_freq_get()) {//频谱计算完成
       /* for(u8 i=0;i<16;i++)
        {
            printf("%04x ",freq_buf[i]);
        }
        printf("\n");*/
        music_freq_kick();//kick下一次频谱计算
    }
#endif

}

#pragma constseg="BT_RFC_TX_CONST"
IAR_CONST u8 at_spp0_ptr[] = "PREV"; //only the pointer form is workable to send vol into RfcommSendATCommand(), (why?) arthur
IAR_CONST u8 at_spp1_ptr[] = "NEXT";
#pragma constseg=default

#if VOL_MAXMIN_MUSIC
#pragma location="TASK_BT_SEG"
void bt_play_vol_music(void)
{
#if !VOL_MUSIC_ONCE
    if(!sys_ctl.vol_maxmin_music_flag){
        sys_ctl.vol_maxmin_music_flag = 2;
    }
    bt_voice_put(RES_WAV_VOL_MAXMIN);
    flush_msg();
#else
    if(!sys_ctl.vol_maxmin_music_flag){
        sys_ctl.vol_maxmin_music_flag = 2;
        bt_voice_put(RES_WAV_VOL_MAXMIN);
    }
#endif
}
#endif

#if BT_NAME_DYMANIC_CHANGE
/*#pragma location="TASK_BT_SEG"
void task_bt_number_box(u8 num)
{
    *((u8 *)&bt_name[0]) = num + '0';
    numbox(num, NULL);
}*/

/*----------------------------------------------------------------------------*/
/*@brief: 修改蓝牙名字函数接口：
//@param: name为所要修改蓝牙名字字符串，字符串的长度最大为32个;
//@return:无
//@note:  调用此函数修改蓝牙名称后下一次进入蓝牙模式方可有效;
*/
/*----------------------------------------------------------------------------*/
#pragma location="TASK_BT_SEG"
void change_bt_name(char __code* name)
{
    //printf("change_bt_name\n");
    //memset((u8 *)&bt_name, 0, sizeof(bt_name));
    //task_bt_number_box(msg - T_KEY_NUM_0);
    memcpy((u8 *)&bt_name, (u8 *)name, sizeof(bt_name));
    param_write((u8 *)&bt_name, PARAM_BT_NAME, 32);
}
#endif

#pragma location="TASK_BT_SEG"
void task_bt_1s_msg(void)
{
    if(IS_APPLE_BT_DEVICE) {                //苹果设备显示电量
        u8 send_bat_to_iphone(u8 batval);
        if(sys_ctl.bat_val != bt_dev_bat) {
            if(!send_bat_to_iphone(sys_ctl.bat_val ? (2*sys_ctl.bat_val-1) : 0)) {
                bt_dev_bat = sys_ctl.bat_val;
            }
        }
    }

    if(BT_STATE <= StateConnected && !Test_Mode_Flag) {
#if AUTO_STANDBY_EN
        BT_IS_SLEEP_INIT();
        if(BT_IS_SLEEP()) {                 //BT_IS_SLEEP()无需查询太频繁
            BT_IS_SLEEP_END();
            if(sys_ctl.sleep_cnt == 0) {
                sys_ctl.sleep_cnt = STANDBY_TIME;
            }
        } else {
            BT_IS_SLEEP_END();
            sys_ctl.sleep_cnt = 0;
        }
#endif
        sys_flag.need_mute = 1;             //非播放非测试状态，需要MUTE功放
    } else {
        sys_ctl.sleep_cnt = 0;
        sys_flag.need_mute = 0;             //播放或测试状态，不需要MUTE功放
    }

#if THEME_SELECT == LEDSEG_THEME_DEFAULT && BT_DISP_NUM_TIME
    if(BT_STATE == OutGoingCall || BT_STATE == InComingCall){
        if(disp_phone_num_cnt < 2 || phone_call_number[disp_phone_num_cnt - 2]){
            disp_phone_num_cnt ++;
        }
        else{
            disp_phone_num_cnt = 0;
        }
        show_update();
    }
    else {
        disp_phone_num_cnt = 0;
    }
#endif

#if LOWPOWER_WARNING
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        //bt_voice_put(RES_WAV_PLEASE_CHARGE);
        bt_voice_put(RES_WAV_VOL_MAXMIN);
        if(HSFRingActive){
            bt_voice_put(BT_VOICE_RING);
        }
    }
#endif
}

//void set_pincode(char __code* pincode);
//void bt_connect_appointed_dev(u8 device_num);
//bool DB_ClearDBData(void);
//void printf_remote_dev_bt_info();
//u8 call_fj[]="AT+VTS=1";
//u8 hsf_send_atcmd(u8 *str, u16 len);
//hsf_send_atcmd(call_number_key,  sizeof(call_number_key));

//蓝牙消息处理
#pragma location="TASK_BT_SEG"
void task_bt_deal_msg(u8 msg)
{
#if VOL_MAXMIN_MUSIC
    if(sys_ctl.vol_maxmin_music_flag == 1){
        sys_ctl.vol_maxmin_music_flag = 0;
        if(!HSFCallActive && !HSFRingActive && !scoflag){
            user_set_volume(sys_ctl.volume);    //修复在系统音量为0或者最大情况下调节通话音量时会恢复系统音量
        }
        flush_msg();
        return;
    }
#endif

    if (msg == NULL) {
        return;
    }

    //static u8 appointed_dev_num = 0;
#if NUMBER_KEY_TO_CALL
    u8 call_fj[8]="AT+VTS=1";
#endif
    u8 flag_key = 1;
    switch (msg) {

#if KL_PLAY_DISCONNECT
    case KL_PLAY:   //2227长按播放键断开蓝牙
        BT_Disconnect();
        //hsf_send_atcmd((u8 *)&call_number_key,  sizeof(call_number_key));
        //change_bt_name("change_bt_name");
        //bt_connect_appointed_dev(rand(4));
        //printf_remote_dev_bt_info();
        //set_pincode("1234");
        break;
#endif

#if NUMBER_KEY_TO_CALL
    case T_KEY_NUM_0:
    case T_KEY_NUM_1:
    case T_KEY_NUM_2:
    case T_KEY_NUM_3:
    case T_KEY_NUM_4:
    case T_KEY_NUM_5:
    case T_KEY_NUM_6:
    case T_KEY_NUM_7:
    case T_KEY_NUM_8:
    case T_KEY_NUM_9:
        if(BT_STATE == StateConnected || BT_STATE == StatePlay){
            memset((u8 *)&call_number_key, 0, sizeof(call_number_key));
            *((u8 *)&call_number_key[0]) = 'A';
            *((u8 *)&call_number_key[1]) = 'T';
            *((u8 *)&call_number_key[2]) = 'D';
            *((u8 *)&call_number_key[3]) = msg - T_KEY_NUM_0 + '0';
            numbox(msg - T_KEY_NUM_0, NULL);

            //printf("call_number_key: %s\n",(u8 *)&call_number_key + 3);
            hsf_send_atcmd((u8 *)&call_number_key,  sizeof(call_number_key));
        }
        else if(BT_STATE == OnThePhone){
            call_fj[7] = msg - T_KEY_NUM_0 + '0';
            hsf_send_atcmd(call_fj, sizeof(call_fj));
        }

        //bt_connect_appointed_dev(msg - T_KEY_NUM_0);
        //set_pincode("0000");
        break;
#endif

    /*case KLU_PLAY:
        //hcmd_remote_name_request(Link_BdAddr);
        //printf("enter:\n");
        //printf_remote_dev_bt_info();
        DB_ClearDBData();
        //printf("\nexit:\n");
        //printf_remote_dev_bt_info();
        break;*/


#if !BT_CALL_PRIVATE
    /* 非强制私密接听时PLAY/PLAY_S10/HSF键定义 */

        //播放、暂停、回连
    case KU_PLAY:
    case KU_PLAY_S10:
        if(HSFRingActive || HSFCallActive){
        #if KU_PLAY_ANSWER_AND_HANGUP
            btMMI_Phone();  //S10短按播放键接听及挂断电话
        #endif
        } else{
            btMMI_PlayPause();
        }
        break;

        //接听、挂断、回拨
    case KU_HSF:
        btMMI_Phone();
        break;

        //响铃时挂断电话
#if KL_PLAY_HANDUP
    case KL_PLAY:
#endif
    case KL_PLAY_S10:
    case KL_HSF:
        if (HSFRingActive) {        //远端打来电话正在响铃
#if !KU_PLAY_S10_TWICE_REJECT       //使用长按PLAY键挂断电话
            btMMI_HandupDuringRing();
#endif
        } else if (HSFCallActive) { //正在拨出电话或者正在通话
#if BT_PHONE_SWITCH
            btMMI_CallTransfer();   //切换手机端与蓝牙端接听
#endif
        } else if (scoflag) {
            ;
        } else {
#if KL_PLAY_S10_DISCONNECT
            BT_Disconnect();        //长按PLAY_S10键断开蓝牙
#elif !MP3BT_DECT_EN                  //没有MP3BT检测并且没有模式键时长按切换模式
    #if(BT_AUX_MODE_ONLY)
        #if LINEIN_DETECT_EN
            if(device_is_actived(DEVICE_LINEIN))
        #endif
                task_ctl.work_sta = TASK_AUX;   //只有蓝牙和AUX模式
    #else
            if(msg == KL_HSF) {
            #if K_HSF_CHANGE_MODE
                task_ctl.work_sta = TASK_EXIT;  //2227在没有响铃的情况下长按HSF键切换任务
            #endif

            } else if (msg == KL_PLAY_S10){
        #if KL_PLAY_S10_CHANGE_MODE
                task_ctl.work_sta = TASK_EXIT;  //在没有响铃的情况下长按PLAY键切换任务
        #else
            #if(!KU_PLAY_S10_TWICE_CALLBACK)
                btMMI_Phone();                  //不使用长按PLAY_S10键切换模式时则用来长按回拨
            #endif
        #endif
            }
    #endif
#else                               //有MP3BT检测时长按可用作其他功能，如回拨或者断开蓝牙接连
        #if(!KU_PLAY_S10_TWICE_CALLBACK)
            btMMI_Phone();          //默认长按PLAY_S10键回拨
        #endif
#endif
        }
        break;


#if KU_PLAY_S10_TWICE_REJECT || KU_PLAY_S10_TWICE_CALLBACK
    case T_KEY_PLAY_TWICE_S10:
      #if KU_PLAY_S10_TWICE_REJECT         //双击挂断电话
        btMMI_HandupDuringRing();
      #elif(KU_PLAY_S10_TWICE_CALLBACK)    //双击回拨功能
        btMMI_Phone();
      #endif
      break;
#endif


#else   //!BT_CALL_PRIVATE
    /* 强制私密接听时PLAY/PLAY_S10/HSF键定义 */

        //播放、暂停、回连
    case KU_PLAY:
    case KU_PLAY_S10:
        btMMI_PlayPause();
        break;

#if KL_PLAY_S10_DISCONNECT
    case KL_PLAY_S10:
        BT_Disconnect();        //长按PLAY_S10键断开蓝牙
        break;
#elif MP3BT_DECT_EN
    case KL_PLAY_S10:
        break;
#endif

#endif  //!BT_CALL_PRIVATE


#if MP3BT_DECT_EN
    case KU_MODE:
        break;
#endif

        //上下一曲
    case KU_PREV:
    case KU_PREV_S10:
        if(!HSFCallActive && !HSFRingActive && !scoflag){   //修复部分手机在通话过程中按上下一曲时会调节通话音量
            btMMI_Backward();
        }
        break;
    case KU_NEXT:
    case KU_NEXT_S10:
        if(!HSFCallActive && !HSFRingActive && !scoflag){   //修复部分手机在通话过程中按上下一曲时会调节通话音量
            btMMI_Forward();
        }
        break;

    case KL_PREV:
        spp_tx((void*)at_spp0_ptr, 4);
        break;
    case KL_NEXT:
        hid_key_camera();       //拍照键，暂时用长按NEXT
        spp_tx((void*)at_spp1_ptr, 4);
        break;
#if LCD_THEME
        //音量加减
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_S10:
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_S10:
    case BT_PHONE_VOL_DOWN:
    case BT_PHONE_VOL_UP:
        lcd_volume_set();
        bt_disp_flag = 0xff;
        break;
#else
        //音量加减
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_S10:
#if !BT_CALL_PRIVATE
        if((HSFCallActive)||(HSFRingActive)|| scoflag){
            user_set_hshfvol(user_volume_dec(sys_ctl.hshf_vol));
            send_hshfvol_hook();
            show_volume();
            break;
        }
#endif
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
		
        if(IS_APPLE_VOL_CTRL) {
		    AVRCPChangeVolume();
        }
		
#if HARD_IR_EN || SOFT_IR_EN
        if(sys_flag.mute_flag) {
            if(sys_ctl.volume) {
                music_unmute();
                dac_fade_in(1);
                sys_ctl.sleep_cnt = 0;
            }
        }
#endif

#if VOL_MAXMIN_MUSIC
        if(!sys_ctl.volume){
            bt_play_vol_music();
        }
#endif
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_S10:
#if !BT_CALL_PRIVATE
        if((HSFCallActive)||(HSFRingActive)|| scoflag){
            user_set_hshfvol(user_volume_inc(sys_ctl.hshf_vol));
            send_hshfvol_hook();
            show_volume();
            break;
        }
#endif
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
		
        if(IS_APPLE_VOL_CTRL) {
		    AVRCPChangeVolume();
        }

#if HARD_IR_EN || SOFT_IR_EN
        if(sys_flag.mute_flag) {
            if(sys_ctl.volume) {
                music_unmute();
                dac_fade_in(1);
                sys_ctl.sleep_cnt = 0;
            }
        }
#endif

#if VOL_MAXMIN_MUSIC
        if(sys_ctl.volume == VOLUME_MAX){
            bt_play_vol_music();
        }
#endif
        break;
        //手机端调系统音量
    case BT_PHONE_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
        break;
    case BT_PHONE_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        break;
#endif
#if (HARD_IR_EN || SOFT_IR_EN) && THEME_SELECT != NO_THEME
    case T_KEY_EQ:
        if(sys_ctl.eq_num < (MUSIC_EQ_NUM - 1)){
            sys_ctl.eq_num ++;
        }
        else{
            sys_ctl.eq_num = 0;
        }

        param_write8(PARAM_EQ_NUM, sys_ctl.eq_num);
        music_eq(sys_ctl.eq_num);
        show_eq();
        //printf("sys_ctl.eq_num: %d\n",sys_ctl.eq_num);
        break;
#endif



#if !BT_CALL_PRIVATE
        //手机端调通话音量
    case BT_PHONE_CHANGE_HSHFVOL:
        show_volume();
        break;
#endif

        //蓝牙断开
    case BT_DIS_CONN:
        //task_ctl.work_sta = TASK_EXIT;
        break;

    case QSYSTEM_1S:
        flag_key = 0;           //2014-3-13, QSYSTEM_1S消息不需要按键测试音
        task_bt_1s_msg();
        deal_msg(msg);
        break;

    default:
        if(!Test_Mode_Flag){
            flag_key = 0;
        }
        deal_msg(msg);
        break;
    }
    if (flag_key) {
        bt_test_key();
    }
}

#pragma constseg="TASK_BT_SEG_CONST"
IAR_CONST char str_show_taskbt[] = "task_bt\n";
IAR_CONST char str_show_taskbtexit[] = "task_bt_exit\n";
#pragma constseg=default

#pragma location="TASK_BTEVENT_SEG"
void task_bt_loop()
{
    u8 msg;
    while (task_ctl.work_sta == TASK_BT || bt_voice_state != BT_VMS_IDLE || HSFRingActive || HSFCallActive || scoflag) {
        msg = get_msg();
        task_bt_event();
        if (msg) {
            task_bt_deal_msg(msg);
        }
#if (THEME_SELECT_BT != NO_THEME)
        task_bt_display();
#endif
        if(sys_flag.lowpower_flag) {   //低电时退出蓝牙
            task_ctl.work_sta = TASK_POWEROFF;
            break;
        }
    }
}

//蓝牙任务
#pragma location="TASK_BT_SEG"
void task_bt(void)
{
    printf(str_show_taskbt);
    task_bt_enter();
    task_bt_loop();
    task_bt_exit();
    printf(str_show_taskbtexit);
}
#endif
