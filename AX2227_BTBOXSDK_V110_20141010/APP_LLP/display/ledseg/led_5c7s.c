/*****************************************************************************
 * Module    : ledseg
 * File      : led_5c7s.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : led_5c7s�������ʾ������
*****************************************************************************/
#include "include.h"
#include "led_seg.h"

#if THEME_SELECT == LEDSEG_THEME_DEFAULT && LEDSEG_TYPE_SELECT == LEDSEG_5C7S_MHZ

#include "led_5c7s.h"
extern __no_init u8 disp_buf[5];
__no_init volatile u8 scan_buf[5];

void led_5c7s_init(void)
{
    LED_COM0_DIR &= ~((1<<LED_COM0_bit) | (1<<LED_COM1_bit) | (1<<LED_COM2_bit) | (1<<LED_COM3_bit));
    LED_COM4_DIR &= ~(1<<LED_COM4_bit);
    COM_ALL_OFF();

    P2PD0 = (P2PD0 & 0x0F) | 0xA0;    //P22��P23���ڲ�500R����
    P2PD1 = (P2PD1 & 0xC0) | 0x2A;    //P24��P25��P26���ڲ�500R����
    P3PD0 = (P3PD0 & 0xF0) | 0x0A;    //P30��P31���ڲ�500R����
    SEG_ALL_OFF();

    my_memset((u8*)disp_buf, 0x00, 5);
    my_memset((u8*)scan_buf, 0x00, 5);
}

/*----------------------------------------------------------------------------*/
/**@brief ������ʾbuf
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LED_5C7S_SEG"
void led_5c7s_value_set(void)
{
    bool ie_ea = IE_EA;                   //2014-3-5, ����С��2S�������ֻ�����ͣ�١���������
    IE_EA = 0;                            //2014-3-5, ��ʱ�䴦��ֱ�ӹ�IE_EA
    my_memcpy((u8*)scan_buf, disp_buf,5);
    IE_EA = ie_ea;
}

/*----------------------------------------------------------------------------*/
/***@brief ɨ����亯�������ɨ��buf
    @details
    @param  [in] com_idex,��δ��ǰҪ����com��
    @return ��
    @note ��
*/
/*----------------------------------------------------------------------------*/
#pragma inline=forced
#pragma location="DISP_LEDSEG_SCAN"
__near_func  void led_5c7s_set_seg(u8 seg_val)
{
    if(seg_val & LED_BIT_A)     SEG0_ON();
    if(seg_val & LED_BIT_B)     SEG1_ON();
    if(seg_val & LED_BIT_C)     SEG2_ON();
    if(seg_val & LED_BIT_D)     SEG3_ON();
    if(seg_val & LED_BIT_E)     SEG4_ON();
    if(seg_val & LED_BIT_F)     SEG5_ON();
    if(seg_val & LED_BIT_G)     SEG6_ON();
}

/*----------------------------------------------------------------------------*/
/***@brief ������ɨ�躯��
    @details
    @param  ��
    @return ��
    @note ��Ҫÿ��һ��ʱ��(1~3ms)����һ��, ����timer�ж���Ӧ�����е���
*/
/*----------------------------------------------------------------------------*/
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
            COM4_ON();
        } else if (1 == com_idx) {
            COM3_ON();
        } else if (2 == com_idx) {
            COM2_ON();
        } else if (3 == com_idx) {
            COM1_ON();
        } else if (4 == com_idx) {
            COM0_ON();
        }
    } else {
#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
        SEG_ENTER_DETECT();
        delay(20);
        linein_detect();
        SEG_EXIT_DETECT();
#endif
    }
}

#endif
