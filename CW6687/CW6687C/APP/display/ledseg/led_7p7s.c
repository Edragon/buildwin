/*****************************************************************************
 * Module    : ledseg
 * File      : led_7p7s.c
 * Author    : Hanny
 * Function  : led_7p7s数码管显示屏驱动
 *****************************************************************************/
#include "include.h"
#include "led_seg.h"

#if IS_LEDSEG_7PIN_DISPLAY
#include "led_7p7s.h"

__no_init u8 pin_disp_buf[7];
u8 led_seg_valid[7];
IAR_XDATA_A u8 COM_7P7S;

//有效位置1
IAR_CONST u8 tbl_ledseg_valid[7] = {
#if (THEME_SELECT == THEME_LEDSEG_7PIN)
    PIN1 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN2 | PIN3 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN3 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN3,
    PIN0 | PIN1 | PIN2 | PIN3,
    PIN0 | PIN1 | PIN2 | PIN3,
#else
    PIN1 | PIN2 | PIN3 | PIN4 | PIN5,
    PIN0 | PIN2 | PIN3 | PIN4 | PIN5,
    PIN0 | PIN1 | PIN3 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN4 | PIN5,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN6,
    PIN2 | PIN4 | PIN5,
#endif
};

#include SET_USER_PATH(CFG_IO_KEY_DIR, /io/io_led_seg.c)

//led_7p7s初始化函数
#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_init(void)
{
    led_7p7s_io_init();

    COM_7P7S = 0;

    my_memset((u8*)disp_buf,0x00,5);
    my_memset((u8 *)pin_disp_buf, 0x00, 7);
}

//根据7脚LED的真值表进行映射（每段输出高的IO记为COM端 7*7）COM:0~6  SEG:0~6
//#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_value_set(void)
{
    u8 ie0, ie1;
    bool ie_ea = IE_EA;
    IE_EA = 0;          //关总中断，保护好IE0/IE1后再打开
    ie0 = IE0;          //保存IE0
    ie1 = IE1;          //保存IE1
    IE0 = 0;            //关掉IE0
    IE1 &= BIT(6);      //关掉IE1，除了UART中断(因为这段处理较长，不能一直关IE_EA，否则会阻塞蓝牙UART中断)
    IE_EA = ie_ea;      //恢复总中断

    my_memset((u8 *)pin_disp_buf, 0x00, 7);

/*********** SEG ****************** COM ******** SEG(out)  *******/
#if (THEME_SELECT == THEME_LEDSEG_7PIN)

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[1] |= PIN0; //设为1，扫描时对应此SEG位设为输出(FM)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[2] |= PIN0;
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= PIN0;
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= PIN0;
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[5] |= PIN0;
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[6] |= PIN0;
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= PIN1;

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[0] |= PIN1;
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[0] |= PIN2;
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[0] |= PIN3;
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[0] |= PIN4;
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[0] |= PIN5;
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[0] |= PIN6;
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[1] |= PIN2;

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[3] |= PIN2;
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[4] |= PIN2;
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[5] |= PIN2;
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[6] |= PIN2;
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[4] |= PIN3;
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[5] |= PIN3;
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[6] |= PIN3;

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[2] |= PIN3;
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[2] |= PIN4;
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[2] |= PIN5;
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[2] |= PIN6;
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= PIN4;
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[3] |= PIN5;
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[3] |= PIN6;
    if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[3] |= PIN1;
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[4] |= PIN1;
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= PIN1;
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[6] |= PIN1;
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[1] |= PIN3;
    if(disp_buf[4] & ICON_FM)       pin_disp_buf[1] |= PIN5;
    if(disp_buf[4] & ICON_MP3)      pin_disp_buf[1] |= PIN6;

#elif (THEME_SELECT == THEME_LEDSEG_7PIN_MHZ)

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[0] |= PIN1; //设为1，扫描时对应此SEG位设为输出(MHZ)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[0] |= PIN2;
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= PIN0;
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= PIN0;
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[0] |= PIN3;
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[1] |= PIN0;
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= PIN0;

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[1] |= PIN2;
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[1] |= PIN3;
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[4] |= PIN1;
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[1] |= PIN5;
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[1] |= PIN4;
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[2] |= PIN1;
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[3] |= PIN1;

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[4] |= PIN3;
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[2] |= PIN4;
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[3] |= PIN4;
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[5] |= PIN0;
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[5] |= PIN2;
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[3] |= PIN2;
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[4] |= PIN2;

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[6] |= PIN5;
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[5] |= PIN6;
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[4] |= PIN5;
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[5] |= PIN3;
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= PIN5;
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[5] |= PIN4;
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[4] |= PIN6;

    if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[0] |= PIN5;
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[2] |= PIN5;
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= PIN1;
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[0] |= PIN4;
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[2] |= PIN3;
    if(disp_buf[4] & ICON_FM) {
        pin_disp_buf[2] |= PIN6;
        pin_disp_buf[6] |= PIN4;
    }
    if(disp_buf[4] & ICON_MP3)    pin_disp_buf[6] |= PIN2;
#endif

    IE0 = ie0;          //先恢复IE0，此时会关IE_EA
    IE1 = ie1;          //再恢复IE1
    IE_EA = ie_ea;      //恢复总中断
}

#pragma location="DISP_LEDSEG_SCAN"
__near_func bool led_7p7s_reuse(void)
{
    bool res = false;

#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
    LINEIN_SEG_ENTER_DETECT();
#endif
#if (EAR_DETECT_EN && EAR_SEG_REUSE)
    EAR_SEG_ENTER_DETECT();
#endif			
    delay(20);  //SYSCLK_24M时,20约为20 uS
#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
    linein_detect();
#endif
#if (EAR_DETECT_EN && EAR_SEG_REUSE)
    earphone_detect();
#endif
#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
    LINEIN_SEG_EXIT_DETECT();
#endif
#if (EAR_DETECT_EN && EAR_SEG_REUSE)
    EAR_SEG_EXIT_DETECT();
#endif		
#if ((LINEIN_DETECT_EN && LINEIN_SEG_REUSE) || (EAR_DETECT_EN && EAR_SEG_REUSE))
    res = true;
#endif	

    return res;
}

#endif
