/*****************************************************************************
 * Module    : API
 * File      : api.h
 * Author    : Hanny
 * Function  : API��׼�����б�
 *****************************************************************************/
#ifndef API_H
#define API_H

//ϵͳ����
#include "api_system.h"

//���Խӿ�
#include "api_debug.h"

//�ļ�ϵͳAPI�ӿ�
#include "api_filesystem.h"

//DAC��ADC����ؽӿ�
#include "api_dac.h"

//���ֲ���API�ӿ�
#include "api_music.h"

//�������API�ӿ�
#include "api_bt.h"

//SPIFlash API�ӿ�
#include "api_spi.h"

//�豸�������Ӧ�ӿ�
#include "api_device.h"

//������Ϣ
#include "api_param.h"

//�ַ������
//#include "user_string.h"
#include "uart_drv.h"

//�ַ������
#include "Menu.h"

void iis_init(void);
__near_func void iis_set_rate(u8 sample_rate);

void timer1_init(void);
void timer1_hook_enable(void);
/*****************************************************************************
 * Module    : ��ʱ���������Ҫɾ��
 *****************************************************************************/
extern u8 ocx_show_cnt;
#endif
