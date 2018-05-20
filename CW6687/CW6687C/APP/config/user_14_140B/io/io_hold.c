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
    P2 |= 0xfe;
    P2DIR &= ~0xfe;
    P0DIR = 0;
    P1DIR = 0x08;
  //  P2DIR = 0;
    P3DIR = 0;
}

#pragma location="TASK_HOLD_SEG"
void task_hold_io_reset(void)
{
    //user_io_init();
    //ledseg_init();
}

#endif


