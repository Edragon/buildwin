/*****************************************************************************
 * Module    : Display
 * File      : ledseg_msgbox.c
 * Author    : Hanny
 * Function  : �Ի���ؼ������ڴ���UI���õ��ĶԻ�����
 *****************************************************************************/
#include "include.h"
#if IS_LEDSEG_DISPLAY
#include "task.h"
//modeģʽѡ��;  param: ���ݲ���
//����ȷ����ȡ��
#pragma location="MSG_BOX_SEG"
bool msgbox(u16 param, u8 mode, u8 show_time)
{
    return true;
}
#endif
