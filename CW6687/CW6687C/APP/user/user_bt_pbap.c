/*****************************************************************************
 * Module    : User
 * File      : user_bt_pbap.c
 * Author    : junwen
 * Function  : 蓝牙PBAP操作接口
 *****************************************************************************/
#include "include.h"
#include "user_bt_pbap.h"
u16 phonebook_total_num;

#define DBG_BT_PBAP_EN      1


IAR_XDATA_A u8 phonebook_name_number[40+24] @"BT_PBAP_XDATA";

#pragma constseg="BT_PBAP_API_CONST"
IAR_CONST char str_show_pb_get_finish[] = "phonebook_get_finish: Index-%d , total_item-%ld\n";
#pragma constseg="BT_PBAP_CONST_SEG"
IAR_CONST char str_show_pb_get_stop[] = "phonebook_get_stop: Index-%d , current_item-%ld\n";
#pragma constseg=default


/******************************************************************************************************
函数功能：电话本信息获取一个联系人信息完成;
          电话本名字与电话本号码为字符型UTF-8编码，字符串结束为'\0';
		  电话本名字长度最大为40字节，电话本号码长度最大为24字节，名字与号码的最后一个字节一直为'\0';
		  每条记录占用字节为64字节;
入口参数：(1)、contacts_buf：电话本信息缓冲区指针;
          (2)、itemCnt：该次电话本信息记录数量，最大值为8;
               一个联系人可能有多个名字及多个号码信息，itemCnt即为其个数，最多为8个；
          (3)、phone_sim_index：电话本类型，0为手机电话本，1为SIM卡电话本;
******************************************************************************************************/
#pragma location="BT_PBAP_BANK_SEG"
void phonebook_get_one_contacts_finish(PHONEBOOK_ITEM *contacts_buf, u8 itemCnt, u8 phone_sim_index)
{
    /*if(!phonebook_total_num){   //开始获取电话本时先擦除SPI相应电话本信息参数区
        spi_phonebook_erase();
#if DBG_BT_PBAP_EN
        printf("spi_pb_erase\n");
#endif
    }*/

#if DBG_BT_PBAP_EN
    for(u8 i=0; i<itemCnt; i++) {
        printf("PB:%s:%s\n",contacts_buf[i].pbName,contacts_buf[i].pbNum);
        //printf_array(contacts_buf[i].pbName,40);
        //printf_array(contacts_buf[i].pbNum,24);
    }
#endif

    for(u8 i=0; i<itemCnt; i++){
        if((phonebook_total_num+i) >= 1024) break;  //最多保存1024条电话本信息
        spi_phonebook_write((u8 *)&contacts_buf[i], phonebook_total_num+i);
#if 0//DBG_BT_PBAP_EN
        spi_phonebook_read(phonebook_name_number, phonebook_total_num+i);
        printf("Read_PB:%s:%s\n",phonebook_name_number,&phonebook_name_number[40]);
#endif
    }
    phonebook_total_num += itemCnt;
}

/******************************************************************************
函数功能：电话本信息获取完成；
入口参数：(1)、phone_sim_index：电话本类型，0为手机电话本，1为SIM卡电话本;
          (2)、total_item：本次电话本信息获取总条目
******************************************************************************/
#pragma location="BT_PBAP_API"
void phonebook_get_finish(u8 phone_sim_index, u32 total_item)
{
    sys_param_write16(PARAM_PHONEBOOK_NUM, phonebook_total_num);
#if DBG_BT_PBAP_EN
    printf(str_show_pb_get_finish, phone_sim_index, total_item);
#endif
}

/******************************************************************************
函数功能：电话本信息获取中止；
入口参数：(1)、phone_sim_index：电话本类型，0为手机电话本，1为SIM卡电话本;
          (2)、current_item：当前已获取条目
******************************************************************************/
#pragma location="BT_PBAP_SEG"
void phonebook_get_stop(u8 phone_sim_index, u32 current_item)
{
    sys_param_write16(PARAM_PHONEBOOK_NUM, phonebook_total_num);
#if DBG_BT_PBAP_EN
    printf(str_show_pb_get_stop, phone_sim_index, current_item);
#endif
}

#pragma location="BT_PBAP_BANK_SEG"
u8 string_len(void *ptr, u8 maxlen)
{
    char *cptr = (char *)ptr;
    u8 i;
    for (i=0; i!=maxlen; i++) {
        if (0 == *cptr++) {
            break;
        }
    }
    return i;
}

#pragma location="BT_PBAP_BANK_SEG"
u16 check_contacts(u8 *phone_num, u8 len)
{
    u8 phone_num_len;
    memset(phonebook_name_number, 0, sizeof(phonebook_name_number));
    for(u8 i=0; i<phonebook_total_num; i++){
        spi_phonebook_phonenum_read(phonebook_name_number, i);
        phone_num_len = string_len(phonebook_name_number, 24);
        if(phone_num_len >= len && !memcmp(&phonebook_name_number[phone_num_len-len], phone_num, len)){
            return i;
        }
    }
    return 0xffff;
}
