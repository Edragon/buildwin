/*****************************************************************************
 * Module    : amux
 * File      : amux.c
 * Author    : Hanny
 * Function  : ģ��ͨ������
 *****************************************************************************/
#include "include.h"
#include "task.h"

//MIC�Ĺ��緽ʽѡ��0ΪVDDHP, 1ΪMICBIAS
bool mic_power_sel(void)
{
    return MICBIAS_EN;
}
