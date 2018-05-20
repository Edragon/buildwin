/*****************************************************************************
 * Module    : ledseg
 * File      : led_5c7s.c
 * Author    : Hanny
 * Function  : led_5c7s�������ʾ������
*****************************************************************************/
#include "include.h"
#include "led_seg.h"

#if IS_LEDSEG_5C7S_DISPLAY
#include "led_5c7s.h"

__no_init volatile u8 scan_buf[5];

#include SET_USER_PATH(CFG_IO_KEY_DIR, /io/io_led_seg.c)

#pragma location="INIT_SEG"
void led_5c7s_init(void)
{
    LED_COM_INIT();
    COM_ALL_OFF();

    SEG_PULL_DOWN();
    SEG_ALL_OFF();

    memset((u8*)disp_buf, 0x00, 5);
    memset((u8*)scan_buf, 0x00, 5);
}

//������ʾbuf
#pragma location="DISP_LED_5C7S_SEG"
void led_5c7s_value_set(void)
{
    bool ie_ea = IE_EA;                   //2014-3-5, ����С��2S�������ֻ�����ͣ�١���������
    IE_EA = 0;                            //2014-3-5, ��ʱ�䴦��ֱ�ӹ�IE_EA
    smemcpy((u8*)scan_buf, disp_buf,5);
    IE_EA = ie_ea;
}

//ɨ����亯�������ɨ��buf
#pragma inline=forced
#pragma location="DISP_LEDSEG_SCAN"
__near_func  void led_5c7s_set_seg(u8 seg_val)
{
    if(seg_val & LED_BIT_A)     SEG6_ON();
    if(seg_val & LED_BIT_B)     SEG5_ON();
    if(seg_val & LED_BIT_C)     SEG4_ON();
    if(seg_val & LED_BIT_D)     SEG3_ON();
    if(seg_val & LED_BIT_E)     SEG2_ON();
    if(seg_val & LED_BIT_F)     SEG1_ON();
    if(seg_val & LED_BIT_G)     SEG0_ON();
}

//������ɨ�躯��,��Ҫÿ��һ��ʱ��(1~3ms)����һ��, ����timer�ж���Ӧ�����е���
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_5c7s_scan(void)
{
    static u8 com_idx = 0;
    com_idx++;
    if (com_idx > 5)
    {
        com_idx = 0;
    }
    COM_ALL_OFF();        //ȫ��
    SEG_ALL_OFF();
    if (com_idx < 5) {
        led_5c7s_set_seg(scan_buf[com_idx]); //��������ÿһ��com����Ҫ��ʾ��ֵ.
        if (0 == com_idx) {
            COM0_ON();
        } else if (1 == com_idx) {
            COM1_ON();
        } else if (2 == com_idx) {
            COM2_ON();
        } else if (3 == com_idx) {
            COM3_ON();
        } else if (4 == com_idx) {
            COM4_ON();
        }
    } else {
#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
        LINEIN_SEG_ENTER_DETECT();
        delay(20);
        linein_detect();
        LINEIN_SEG_EXIT_DETECT();
#endif
    }
}

#endif
