/*****************************************************************************
 * Module    : IO
 * File      : io_led_seg.c
 * Author    : Hanny
 * Function  : LED数码管显示屏的相关操作函数
 ******************************************************************************/
#ifndef CONFIG_C

#if IS_LEDSEG_7PIN_DISPLAY

#define LEDSEG_7PIN_SCAN_MODE   SCAN_MODE_COM   //七段数码管的扫描方式 IO全部带下拉选：SCAN_MODE_COM 其他选：SCAN_MODE_SEG

//LED 7脚屏IO初始化函数，所有用到的引脚都设为输入
#pragma location="INIT_SEG"
void led_7p7s_io_init(void)
{
    P2DIR |= 0xef;
#if (LEDSEG_7PIN_SCAN_MODE == SCAN_MODE_COM)
    P2PDS1 |= 0xef;
#endif
}

#if (LEDSEG_7PIN_SCAN_MODE == SCAN_MODE_COM)

//LED扫描函数(1COM 6 SEG 以COM为单位扫描)
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_7p7s_scan(void)
{
    u8 dis_seg = 0, dir_temp, out_temp;
    u8 dir_dp = 0;

    out_temp = P2 & 0x10;      //把所用到的脚全部置0
    dir_temp = P2DIR | 0xef;   //把所用到的脚全部设为输入

    if(COM_7P7S > 6) {
        COM_7P7S = 0;
        if (led_7p7s_reuse()) {
            return;
        }	
    }
    dis_seg = pin_disp_buf[COM_7P7S];

    dir_dp |= ((dis_seg & 0x0f) | ((dis_seg & 0x70)<<1));  //把要显示的段开下拉

    if(COM_7P7S < 3) {
        dir_temp &= ~BIT(7-COM_7P7S);
        out_temp |= BIT(7-COM_7P7S);
    } else {
        dir_temp &= ~BIT(6-COM_7P7S);
        out_temp |= BIT(6-COM_7P7S);
    }

    P2 = out_temp;
    P2DIR = dir_temp;
    P2PD = dir_dp;

    COM_7P7S++;     //扫描下一COM
}
#else

//LED扫描函数(1COM 1 SEG 以SEG为单位扫描)
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_7p7s_scan(void)
{
    static u8 count = 0;
    u8 dis_seg = 0, dir_temp, out_temp;

    out_temp = P2 & 0x10;      //把所用到的脚全部置0
    dir_temp = P2DIR | 0xef;   //把所用到的脚全部设为输入

    while (1) {
        if (count > 6) {
            count = 0;
            COM_7P7S++;
        }
        if(COM_7P7S > 6) {
            COM_7P7S = 0;
            if (led_7p7s_reuse()) {
                return;
            }
        }
        if (tbl_ledseg_valid[COM_7P7S] & BIT(count)) {         //节约CPU，只扫描有效的SEG段
            break;
        }
        count++;
    }
    dis_seg = pin_disp_buf[COM_7P7S] & BIT(count);

    dir_temp &= ~((dis_seg & 0x0f) | ((dis_seg & 0x70)<<1));  //把要显示的段设为输出 (前面已经把所有脚置0)

    if(COM_7P7S < 3) {
        dir_temp &= ~BIT(7-COM_7P7S);
        out_temp |= BIT(7-COM_7P7S);
    } else {
        dir_temp &= ~BIT(6-COM_7P7S);
        out_temp |= BIT(6-COM_7P7S);
    }

    P2 = out_temp;
    P2DIR = dir_temp;

    count++;        //扫描下一SEG
}
#endif
#endif

#endif


