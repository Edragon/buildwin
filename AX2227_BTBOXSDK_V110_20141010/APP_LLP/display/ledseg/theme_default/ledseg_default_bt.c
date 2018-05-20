/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_menu.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD��������Ĭ������ - ���ֲ���
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_BT == LEDSEG_THEME_DEFAULT)
#include "task.h"
#include "user_bt.h"
#include "btapi.h"

//��ʾ�豸��Ϣ
#pragma location="BT_API2"
void task_bt_display_device(u8 show_time)
{

}

//����������ʾ����
#pragma location="TASK_BT_SEG"
void task_bt_display(void)
{
    if (!ledseg_ocx_event()) {      //û�и�����ʾʱ����һֱˢ��
        return;
    }

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
#if BT_DISP_NUM_TIME
        if((BT_STATE == OutGoingCall || BT_STATE == InComingCall) && bt_phonenum_flag){ //��Щ�ֻ���֧�ֺ�����ʾʱ����ʾbt
            ledseg_disp(MENU_PHONENUMBER);
        } else if(BT_STATE == OnThePhone){
            ledseg_disp(MENU_ON_THE_PHONE_TIME);
        } else
#endif
        {
            ledseg_disp(MENU_BT);
        }
        break;

    default:
        ledseg_ocx_display();
        break;
    }
}
#endif
