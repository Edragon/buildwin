/*****************************************************************************
 * Module    : IO
 * File      : io_hold.c
* Author     : Hanny
 * Function  : ����task_hold���˳�task_hold��io�������
 ******************************************************************************/
#ifndef CONFIG_C
#pragma location="TASK_HOLD_SEG"
void task_hold_io_sleep(void)
{
    //seg�ڴ���
    P2DIR |= 0xef;
    P2PD &= ~0xef;
    //com�ڴ���
    P0DIR |= BIT(6)|BIT(7);
    P1DIR |= BIT(2)|BIT(4);
    P3DIR |= BIT(6);
}

#pragma location="TASK_HOLD_SEG"
void task_hold_io_reset(void)
{
    P0DIR &= ~(BIT(6)|BIT(7)); //COM��
    P1DIR &= ~(BIT(2)|BIT(4));
    P3DIR &= ~BIT(6);
    P0 &= ~(BIT(6)|BIT(7));
    P1 &= ~(BIT(2)|BIT(4));
    P3 &= ~BIT(6);
    P2PD |= 0xef;
}

#endif


