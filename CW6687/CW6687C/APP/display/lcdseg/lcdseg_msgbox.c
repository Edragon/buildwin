/*****************************************************************************
 * Module    : Display
 * File      : lcdseg_msgbox.c
 * Author    : Hanny
 * Function  : �Ի���ؼ������ڴ���UI���õ��ĶԻ�����
 *****************************************************************************/
#include "include.h"
#if IS_LCDSEG_DISPLAY
#include "task.h"
//modeģʽѡ��;  param: ���ݲ���
//����ȷ����ȡ��
#pragma location="MSG_BOX_SEG"
bool msgbox(u16 param, u8 mode, u8 show_time)
{
    return true;
}
#endif
