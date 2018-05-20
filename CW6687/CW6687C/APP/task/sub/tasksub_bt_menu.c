#if IS_LCD_DISPLAY

#ifdef TASK_SUB
#include "user_bt_pbap.h"

u8 *bt_tmp = (u8 *)filename;    // 注意：用filename是为了节省ram

#define BT_PHONEBOOK_ITEM_NUM    5
#define CONTACTS_LEN            (40/3*2+24*2+2+2+1)
#define MUSIC_EQ_ITEM_NUM           (MUSIC_EQ_NUM + 1)
IAR_XDATA_A u8 phonebook_disp_buf[3][CONTACTS_LEN] @"BT_PB_DISP_BUF";
extern string IAR_CONST tbl_music_eq[];
#pragma constseg = BT_MENU_DISP_PARAM
string IAR_CONST tbl_bt_menu_const[] = {
    STR_MAIN_BT,
    STR_BT_RECONNECT,
#if BT_PHONE_BOOK_EN
    STR_BT_PHONEBOOK,
#endif
    //STR_BT_LIST_CONNECTED,
    STR_MENU_EQ,
    STR_BT_DEL_PAIRED,
    STR_BT_INFO,
    STR_BACK_MENU,
    STR_MENU_EXIT,
    STR_BT_DISCONNECT,
};

#pragma constseg = "LIST_CONTENT"
string IAR_CONST tbl_bt_phonebook_const[] = {
    STR_BT_PHONEBOOK,
    STR_BT_CONTACTS,
    STR_BT_SYNC_CONTACTS,
    STR_BT_DELETE_ALL_CONTACTS,
    STR_MENU_EXIT,
};
#pragma constseg=default

enum{
    ITEM_PHONEBOOK_CONTACTS,
    ITEM_PHONEBOOK_SYNC_CONTACTS,
    ITEM_PHONEBOOK_DELETE_ALL_CONTACTS,
    ITEM_PHONEBOOK_EXIT,
};

__no_init __root string tbl_bt_menu[BT_MENU_ITEM_NUM] @ "BtPaired";
void task_bt_deal_msg_10s(void);

//蓝牙地址转字符串
#pragma location="BT_MENU_DISP"
void addr_to_string(u8 *dst, u8 const *src)
{
    u8 addr_len;
    for(addr_len=6; addr_len>0; addr_len--) {
        *dst++ = hex_to_char(*(src+addr_len-1)>>4);
        *dst++ = hex_to_char(*(src+addr_len-1)&0x0f);
        //*dst++ = ':';
    }
    *dst = '\0';
}

//显示设备信息
#pragma location="BT_MENU_DISP"
void task_bt_display_device(u8 show_time)
{
    draw_clear();
    disp_menu(STR_BT_INFO,0,0, 0,ME_CENTER);
    line_update(0);
    memset(bt_tmp, 0, sizeof(bt_tmp));
    //准备蓝牙名字
    bt_tmp[0]='N';
    bt_tmp[1]='a';
    bt_tmp[2]='m';
    bt_tmp[3]='e';
    bt_tmp[4]=':';
    btinfo_get_name(bt_tmp+5);
    bt_name_to_unicode(bt_tmp);
    move_ctl.name_wide=bt_name_len(bt_name_unicode);
    if(move_ctl.name_wide>96) {
        move_ctl.move_flag=1;
    }else {
        move_ctl.move_flag=0;
    }
    move_ctl.name_x=0;
    //准备蓝牙地址
    bt_tmp[0] = 'A';
    bt_tmp[1] = 'd';
    bt_tmp[2] = 'r';
    bt_tmp[3] = ':';
    btinfo_get_addr(bt_tmp+21);
    addr_to_string(bt_tmp+4, bt_tmp+21);
    line_clear();
    task_bt_disp_name(3);
    disp_text((char *)bt_tmp, 0, 0, 1);
    line_update(5);
    ocx_show_cnt = 0;
    while(1)
    {
        if(move_ctl.move_flag) {
            task_bt_disp_name(3);
        }
        u8 msg = get_msg();
        if(msg == KU_OK || msg == KU_BACK || msg == KU_PREV || msg == KU_NEXT || msg == KU_PLAY_POWER)
            break;
        if(show_time && ocx_show_cnt >= show_time)
            break;
        deal_msg(msg);
        if(msg == KL_PLAY_POWER) {
            task_ctl.work_sta = TASK_EXIT;
            return;
        }
    }
}

#pragma location="LIST_BOX_A_SEG"
u8 *phonebook_buf_get(u16 index, u8 cnt)
{
    u8 i,j;
    u8 count = (phonebook_total_num > cnt)? cnt : phonebook_total_num;
    memset(phonebook_disp_buf, 0, sizeof(phonebook_disp_buf));
    for(i=0; i<count; i++){
        spi_phonebook_read(phonebook_name_number, index+i);
        j = utf8_to_utf16(phonebook_disp_buf[i], phonebook_name_number, 40);
        phonebook_disp_buf[i][j] = ':';                     //name:telephone number
        phonebook_disp_buf[i][j+1] = 0x00;
        utf8_to_utf16(&phonebook_disp_buf[i][j+2], &phonebook_name_number[40], 24);
        phonebook_disp_buf[i][CONTACTS_LEN-3] = 0x00;       //字符串结束符'\0'
        phonebook_disp_buf[i][CONTACTS_LEN-2] = 0x00;
        phonebook_disp_buf[i][CONTACTS_LEN-1] = FB_UNICODE; //unicode encode flag
        //printf("Read_PB_Disp:%s:%s\n",phonebook_name_number,&phonebook_name_number[40]);
        //printf_array(phonebook_disp_buf[i], CONTACTS_LEN);
    }
    return phonebook_disp_buf[0];
}

//蓝牙菜单显示:param,0,蓝牙未连接时调用，1，蓝牙连接时调用
#pragma location="BT_MENU_DISP"
void task_bt_menu(void)
{
    u8 index,param = 0;
#if BT_PHONE_BOOK_EN
    u8 i;
    u8 *phonebook_last_bt_addr = bt_tmp;        //借用 bt_tmp
    u8 *phonebook_current_bt_addr = bt_tmp+6;
#endif

    memcpy(tbl_bt_menu,(u8 *)tbl_bt_menu_const,BT_MENU_ITEM_NUM*2);
    if(sys_ctl.bt_sta >= BT_STA_CONNECTED) {   //蓝牙已连接
        tbl_bt_menu[1] = tbl_bt_menu_const[BT_MENU_ITEM_NUM];
        param = 1;
    }
    index = listbox(0, BT_MENU_ITEM_NUM, NULL, (u16)tbl_bt_menu, LIST_TIME);
    switch (index) {
    case ITEM_BT_CON:   //  最近设备重连或断开蓝牙
        if(param == 0) {
            bt_connect();       // 重新连接
        } else {
            bt_disconnect();    // 断开连接
        }
        break;
#if BT_PHONE_BOOK_EN
    case ITEM_BT_PHONEBOOK:
        if(sys_ctl.bt_sta < BT_STA_CONNECTED){
            msgbox(STR_BT_NOT_LINK, NULL, MSGBOX_TIME*2);
            break;
        }
        sys_param_read(phonebook_last_bt_addr, PARAM_PHONEBOOK_BT_ADDR, 6);
        bt_get_linkaddr(phonebook_current_bt_addr);

        index = listbox(0, BT_PHONEBOOK_ITEM_NUM, NULL, (u16)tbl_bt_phonebook_const, LIST_TIME);
        if(index == ITEM_PHONEBOOK_CONTACTS){
            if(!phonebook_total_num || memcmp(phonebook_last_bt_addr, phonebook_current_bt_addr, 6)){
                msgbox(STR_BT_NO_CONTACTS, NULL, MSGBOX_TIME*4);
            } else {
                u16 contacts_index = listboxa(0, phonebook_total_num+1, NULL, CONTACTS_LEN, (u32)&phonebook_buf_get);
                if(contacts_index < phonebook_total_num){
                    spi_phonebook_read(phonebook_name_number, contacts_index);
                    memset(phonebook_name_number, 0, 40);
                    phonebook_name_number[0] = 'A';
                    phonebook_name_number[1] = 'T';
                    phonebook_name_number[2] = 'D';
                    for(i=0; i<24; i++){
                        if(phonebook_name_number[40+i] == 0){
                            break;
                        }
                    }
                    memcpy(&phonebook_name_number[3], &phonebook_name_number[40], i);
                    phonebook_name_number[3+i] = ';';
                    phonebook_name_number[3+i+1] = '\r';
                    bt_send_atcmd((char *)phonebook_name_number);
                    //u8 hsf_send_atcmd(u8 *str, u16 len);
                    //hsf_send_atcmd((u8 *)phonebook_name_number,3+i+2);
                    //printf("call:%s\n",&phonebook_name_number[3]);
                    for(u8 j=0; j<i; j++){  //借用 t_bt_ring.num_buf 保存去电号码用于显示
                        t_bt_ring.num_buf[j] = phonebook_name_number[3+j]-0x30;
                    }
                    set_num_len(i);
                }
            }
        } else if (index == ITEM_PHONEBOOK_SYNC_CONTACTS) {
            spi_phonebook_erase();
            phonebook_total_num = 0;
            sys_param_write16(PARAM_PHONEBOOK_NUM, phonebook_total_num);
            if(phonebook_get_start(0)){
                bool sync_contacts_stop_flag = 0;
                sync_contacts_wait();
                while(!IsPBAPInIdleMode()){
                    WATCHDOG_CLR();
                    comm_event();
                    u8 msg = get_msg();
                    if(msg == KL_PLAY){     //长按PLAY键中止电话本联系人获取
                        PhonebookStop();
                        sync_contacts_stop_flag = 1;
                        break;
                    } else if(msg == QSYSTEM_1S) {
                        deal_msg(msg);
                    } else if(msg == QSYSTEM_10S) {
                        task_bt_deal_msg_10s();
                    } else if(msg >= 0x6d && msg <= 0x7e) { //重要的系统消息保留下来等待退出后处理
                        put_msg(msg);
                    }
                }
                if(!sync_contacts_stop_flag){
                    if(phonebook_total_num){
                        msgbox(STR_BT_SYNC_SUCCESS, NULL, MSGBOX_TIME*2);
                    } else {
                        msgbox(STR_BT_SYNC_FAIL, NULL, MSGBOX_TIME*2);
                    }
                } else {
                    msgbox(STR_BT_SYNC_STOP, NULL, MSGBOX_TIME*2);
                }
                if(phonebook_total_num){
                    if(memcmp(phonebook_last_bt_addr, phonebook_current_bt_addr, 6)){
                        sys_param_write(phonebook_current_bt_addr, PARAM_PHONEBOOK_BT_ADDR, 6);
                        //printf("save_pb_addr\n");
                    }
                }
            } else {
                msgbox(STR_BT_SYNC_FAIL, NULL, MSGBOX_TIME*2);
            }
        } else if (index == ITEM_PHONEBOOK_DELETE_ALL_CONTACTS) {
            if(msgbox(STR_BT_CONFIRM_DELETE_CONTACTS, MB_YESNO, 0)){
                phonebook_total_num = 0;
                sys_param_write16(PARAM_PHONEBOOK_NUM, phonebook_total_num);
                msgbox(STR_BT_DELETE_CONTACTS_SUCCESS, NULL, MSGBOX_TIME*2);
            }
        }
        break;
#endif
 /* case ITEM_BT_LIST:  //连接设备历史列表

        break;*/
    case ITEM_BT_EQ:    //EQ调节
        index = listbox(sys_ctl.eq_num, MUSIC_EQ_ITEM_NUM, LB_EQ, (u16)tbl_music_eq, LIST_TIME);
        user_set_eq(index);
        music_eq(sys_ctl.eq_num);
        break;
    case ITEM_BT_DEL_PAIRED:    //删除配对信息
        bt_get_linkaddr(bt_tmp);
        bt_delete_link(bt_tmp);
        break;
    case ITEM_BT_INFO:          //设备信息
        task_bt_display_device(LIST_TIME);
        break;
    case ITEM_BTMENU_BACK:
        task_ctl.work_sta = TASK_EXIT;      //退出
        break;
    }
    t_bt.updata_flag = 1;
}

#pragma location="BT_MENU_DISP"
void tasksub_bt_menu_enter(void)
{

}

#pragma location="BT_MENU_DISP"
void tasksub_bt_menu_exit(void)
{

}

#pragma location="BT_MENU_DISP"
void tasksub_bt_menu_event(void)
{

}

#pragma location="BT_MENU_DISP"
void task_bt_menu_deal_msg(u8 msg)
{

}

#pragma location="BT_MENU_DISP"
void tasksub_bt_menu_loop(void)
{
    task_bt_menu();
}

#pragma location="BT_MENU_DISP"
void tasksub_bt_menu(void)
{
   // printf("tasksub_bt_menu\n");
    tasksub_bt_menu_enter();
    tasksub_bt_menu_loop();
    tasksub_bt_menu_exit();
    //printf("tasksub_bt_menu exit\n");
}

#endif
#endif
