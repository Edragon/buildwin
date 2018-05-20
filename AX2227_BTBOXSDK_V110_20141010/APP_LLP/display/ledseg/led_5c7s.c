/*****************************************************************************
 * Module    : ledseg
 * File      : led_5c7s.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : led_5c7s数码管显示屏驱动
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

    P2PD0 = (P2PD0 & 0x0F) | 0xA0;    //P22、P23开内部500R下拉
    P2PD1 = (P2PD1 & 0xC0) | 0x2A;    //P24、P25、P26开内部500R下拉
    P3PD0 = (P3PD0 & 0xF0) | 0x0A;    //P30、P31开内部500R下拉
    SEG_ALL_OFF();

    my_memset((u8*)disp_buf, 0x00, 5);
    my_memset((u8*)scan_buf, 0x00, 5);
}

/*----------------------------------------------------------------------------*/
/**@brief 更新显示buf
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LED_5C7S_SEG"
void led_5c7s_value_set(void)
{
    bool ie_ea = IE_EA;                   //2014-3-5, 修正小米2S及部分手机播歌停顿、无声问题
    IE_EA = 0;                            //2014-3-5, 短时间处理，直接关IE_EA
    my_memcpy((u8*)scan_buf, disp_buf,5);
    IE_EA = ie_ea;
}

/*----------------------------------------------------------------------------*/
/***@brief 扫描填充函数，填充扫描buf
    @details
    @param  [in] com_idex,表未当前要填充的com口
    @return 无
    @note 无
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
/***@brief 段码屏扫描函数
    @details
    @param  无
    @return 无
    @note 需要每隔一段时间(1~3ms)调用一次, 如在timer中断响应函数中调用
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
    COM_ALL_OFF();        //全灭
    SEG_ALL_OFF();
    if (com_idx < 5) {
        led_5c7s_set_seg(scan_buf[com_idx]); //依次设置每一个com口中要显示的值.
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
