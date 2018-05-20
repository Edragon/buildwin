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
    P2DIR |= 0xfc;
    P3DIR |= BIT(4);
    P2PD &= ~0xFC;
    P3PD &= ~BIT(4);
    //com�ڴ���
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


