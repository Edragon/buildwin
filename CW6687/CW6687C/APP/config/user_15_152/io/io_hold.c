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
    P2DIR |= 0xef;
    P2PD &= ~0xef;
    //com口处理
    P0DIR |= BIT(6)|BIT(7);
    P1DIR |= BIT(2)|BIT(4);
    P3DIR |= BIT(6);
}

#pragma location="TASK_HOLD_SEG"
void task_hold_io_reset(void)
{
    P0DIR &= ~(BIT(6)|BIT(7)); //COM口
    P1DIR &= ~(BIT(2)|BIT(4));
    P3DIR &= ~BIT(6);
    P0 &= ~(BIT(6)|BIT(7));
    P1 &= ~(BIT(2)|BIT(4));
    P3 &= ~BIT(6);
    P2PD |= 0xef;
}

#endif


