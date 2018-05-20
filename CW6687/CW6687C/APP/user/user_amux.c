/*****************************************************************************
 * Module    : amux
 * File      : amux.c
 * Author    : Hanny
 * Function  : 模拟通道设置
 *****************************************************************************/
#include "include.h"
#include "task.h"

//MIC的供电方式选择：0为VDDHP, 1为MICBIAS
bool mic_power_sel(void)
{
    return MICBIAS_EN;
}
