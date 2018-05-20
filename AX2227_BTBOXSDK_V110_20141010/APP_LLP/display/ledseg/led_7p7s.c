/*****************************************************************************
 * Module    : ledseg
 * File      : led_7p7s.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : led_7p7s数码管显示屏驱动
 *****************************************************************************/
#include "include.h"
#include "led_seg.h"

#if THEME_SELECT == LEDSEG_THEME_DEFAULT && (LEDSEG_TYPE_SELECT == LEDSEG_7P7S_FM || LEDSEG_TYPE_SELECT == LEDSEG_7P7S_MHZ)

#include "led_7p7s.h"
extern __no_init u8 disp_buf[5];
__no_init u8 pin_disp_buf[7];
IAR_XDATA_A u8 COM_7P7S;

//led_7p7s初始化函数
#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_init(void)
{
    P2DIR |= 0x78;  //所有用到的引脚都设为输入
    P3DIR |= 0x07;

    COM_7P7S = 0;
    my_memset((u8*)disp_buf,0x00,5);
    my_memset((u8 *)pin_disp_buf, 0x00, 7);
}

//根据7脚LED的真值表进行映射(每段输出高的IO记为COM端 7*7)COM:0~6  SEG:0~6
//#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_value_set(void)
{
    u8 ie0, ie1;
    bool ie_ea = IE_EA; //2014-3-5, 修正小米2S及部分手机播歌停顿、无声问题
    IE_EA = 0;          //关总中断，保护好IE0/IE1后再打开
    ie0 = IE0;          //保存IE0
    ie1 = IE1;          //保存IE1
    IE0 = 0;            //关掉IE0
    IE1 &= BIT(6);      //关掉IE1，除了UART中断(因为这段处理较长，不能一直关IE_EA，否则会阻塞蓝牙UART中断)
    IE_EA = ie_ea;      //恢复总中断

    my_memset((u8 *)pin_disp_buf, 0x00, 7);

/*********** SEG ****************** COM ******** SEG(out)  *******/
#if LEDSEG_TYPE_SELECT == LEDSEG_7P7S_FM

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[1] |= BIT(0); //设为1，扫描时对应此SEG位设为输出(FM)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[2] |= BIT(0);
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= BIT(0);
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= BIT(0);
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[5] |= BIT(0);
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[6] |= BIT(0);
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= BIT(1);

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[0] |= BIT(1);
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[0] |= BIT(2);
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[0] |= BIT(3);
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[0] |= BIT(4);
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[0] |= BIT(5);
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[0] |= BIT(6);
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[1] |= BIT(2);

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[3] |= BIT(2);
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[4] |= BIT(2);
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[5] |= BIT(2);
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[6] |= BIT(2);
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[4] |= BIT(3);
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[5] |= BIT(3);
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[6] |= BIT(3);

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[2] |= BIT(3);
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[2] |= BIT(4);
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[2] |= BIT(5);
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[2] |= BIT(6);
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= BIT(4);
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[3] |= BIT(5);
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[3] |= BIT(6);

    if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[3] |= BIT(1);
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[4] |= BIT(1);
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= BIT(1);
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[6] |= BIT(1);
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[1] |= BIT(3);
    if(disp_buf[4] & ICON_FM)       pin_disp_buf[1] |= BIT(5);
    if(disp_buf[4] & ICON_MP3)      pin_disp_buf[1] |= BIT(6);

#elif LEDSEG_TYPE_SELECT == LEDSEG_7P7S_MHZ

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[0] |= BIT(1); //设为1，扫描时对应此SEG位设为输出(MHZ)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[0] |= BIT(2);
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= BIT(0);
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= BIT(0);
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[0] |= BIT(3);
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[1] |= BIT(0);
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= BIT(0);

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[1] |= BIT(2);
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[1] |= BIT(3);
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[4] |= BIT(1);
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[1] |= BIT(5);
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[1] |= BIT(4);
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[2] |= BIT(1);
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[3] |= BIT(1);

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[4] |= BIT(3);
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[2] |= BIT(4);
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[3] |= BIT(4);
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[5] |= BIT(0);
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[5] |= BIT(2);
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[3] |= BIT(2);
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[4] |= BIT(2);

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[6] |= BIT(5);
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[5] |= BIT(6);
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[4] |= BIT(5);
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[5] |= BIT(3);
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= BIT(5);
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[5] |= BIT(4);
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[4] |= BIT(6);

    if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[0] |= BIT(5);
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[2] |= BIT(5);
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= BIT(1);
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[0] |= BIT(4);
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[2] |= BIT(3);
    if(disp_buf[4] & ICON_FM) {
        pin_disp_buf[2] |= BIT(6);
        pin_disp_buf[6] |= BIT(4);
    }
    if(disp_buf[4] & ICON_MP3)    pin_disp_buf[6] |= BIT(2);
#endif

    IE0 = ie0;          //先恢复IE0，此时会关IE_EA
    IE1 = ie1;          //再恢复IE1
    IE_EA = ie_ea;      //恢复总中断
}

#if 0   //0：7脚屏以SEG为单位进行扫描(需在0.5ms中断里面扫描)；  1：7脚屏以COM为单位扫描
/*----------------------------------------------------------------------------*/
/***@brief 7脚屏扫描函数
    @details
    @param  无
    @return 无
    @note：对应段点亮条件：COM脚及要点亮的SEG脚设为输出，且COM脚输出高电平，
           SEG脚输出低电平，其余5个SEG脚设为输入
*/
/*----------------------------------------------------------------------------*/
//LED扫描函数(1COM 6 SEG 以COM为单位扫描)
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_7p7s_scan(void)
{
    u8 dis_seg = 0,dir_temp2,out_temp2,dir_temp3,out_temp3;

    if(COM_7P7S > 6) {
        COM_7P7S = 0;
    }
    dis_seg = pin_disp_buf[COM_7P7S];

    out_temp2 = P2 & 0x87;      //把所用到的脚全部置0 (并保存没用到的P2、P3脚原来的电平)
    out_temp3 = P3 & 0xf8;
    dir_temp2 = P2DIR | 0x78;   //把所用到的脚全部设为输入 (并保存没用到的P2、P3脚原来的方向)
    dir_temp3 = P3DIR | 0x07;

    dir_temp2 &= ~((dis_seg & 0x0f) << 3);  //把要显示的段设为输出 (前面已经把所有脚置0)
    dir_temp3 &= ~((dis_seg & 0x70) >> 4);

    if(COM_7P7S < 4) {          //把COM脚设为输出，并且输出高电平
        dir_temp2 &= ~BIT(COM_7P7S + 3);
        out_temp2 |= BIT(COM_7P7S + 3);
    } else {
        dir_temp3 &= ~BIT(COM_7P7S - 4);
        out_temp3 |= BIT(COM_7P7S - 4);
    }

    P3 = out_temp3;
    P2 = out_temp2;
    P3DIR = dir_temp3;
    P2DIR = dir_temp2;

    COM_7P7S++;     //扫描下一COM
}

#else

//LED扫描函数(1COM 1 SEG 以SEG为单位扫描)
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_7p7s_scan(void)
{
    static u8 count = 0;
    u8 dis_seg = 0,dir_temp2,out_temp2,dir_temp3,out_temp3;

    if (count > 6) {
        count = 0;
        COM_7P7S++;
    }
    if(COM_7P7S > 6) {
        COM_7P7S = 0;
    }
    dis_seg = pin_disp_buf[COM_7P7S];
    if(count == COM_7P7S) {     //SEG及COM同一IO情况不存在,扫描下一段
        count += 1;
    }
    dis_seg = dis_seg & BIT(count);

    out_temp2 = P2 & 0x87;      //把所用到的脚全部置0 (并保存没用到的P2、P3脚原来的电平)
    out_temp3 = P3 & 0xf8;
    dir_temp2 = P2DIR | 0x78;   //把所用到的脚全部设为输入 (并保存没用到的P2、P3脚原来的方向)
    dir_temp3 = P3DIR | 0x07;

    if(COM_7P7S < 4) {          //把COM脚设为输出，并且输出高电平
        dir_temp2 &= ~BIT(COM_7P7S + 3);
        out_temp2 |= BIT(COM_7P7S + 3);
    } else {
        dir_temp3 &= ~BIT(COM_7P7S - 4);
        out_temp3 |= BIT(COM_7P7S - 4);
    }

    if(count < 4) {             //把要点亮的SEG脚设为输出 (前面已经把所有脚置0)
        dir_temp2 &= ~(dis_seg << 3);
    } else {
        dir_temp3 &= ~(dis_seg >> 4);
    }

    P3 = out_temp3;
    P2 = out_temp2;
    P3DIR = dir_temp3;
    P2DIR = dir_temp2;

    count++;        //扫描下一SEG
}
#endif

#endif
