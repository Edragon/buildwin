/*****************************************************************************
 * Module    : IO
 * File      : io_hold.c
* Author     : Hanny
 * Function  : 进入task_hold和退出task_hold的io相关配置
 ******************************************************************************/
#ifndef CONFIG_C
#pragma location="TASK_HOLD_SEG"
void task_hold_io_sleep(void)
{
    //seg口处理
    P2DIR |= 0xfc;
    P3DIR |= BIT(4);
    P2PD &= ~0xFC;
    P3PD &= ~BIT(4);
    //com口处理
    P1DIR |= 0x07;
    P2DIR |= 0x03;
}

#pragma location="TASK_HOLD_SEG"
void task_hold_io_reset(void)
{
    P1DIR &= ~0x07;
    P2DIR &= ~0x03;
    P1 &= ~0x07;   //ALL OFF
    P2 &= ~0x03;
    P2PD |= 0xFC;
    P3PD |= BIT(4);
}

#endif


