/*****************************************************************************
 * Module    : Display
 * File      : lcdseg_msgbox.c
 * Author    : Hanny
 * Function  : �б�ؼ������ڴ���UI���õ��ľ�̬�б���
 *****************************************************************************/
#include "include.h"
#if IS_LCDSEG_DISPLAY
#include "task.h"

//index: ��ʼ����;  total: ��������;  modeģʽѡ��;  param: ���ݲ���; show_time: �б����ʾʱ��
//����ѡ�е�ֵ
#pragma location="LIST_BOX_SEG"
u8 listbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time)
{
    return 0;
}

#endif
