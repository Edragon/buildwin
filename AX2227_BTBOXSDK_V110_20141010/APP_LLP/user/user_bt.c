/*****************************************************************************
 * Module    : Task
 * File      : user_bt.c
 * Author    : dingping
 * Email     :
 * Function  : 蓝牙用户流程
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_bt.h"
#pragma location="BT_API_CMD"
void sbc_uart_io_init(void)
{
    PMUXCON0 |= BIT(6);    //  UART1 select P16,P17
    P1 |= BIT(6)|BIT(7);          //FPGA_D5
    P1DIR &= ~BIT(6);      // P16  output
    P1DIR |= (BIT(7));       //P17 Input
    P1PU0 |= (BIT(7));
    SBC_CTS_L();
    SBC_CTS_DIR_OUT();
}

/*----------------------------------------------------------------------------*/
/**@brief 向蓝牙模块BT8202发送请求
   @param 无
   @return
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="BT_API_CMD"
void uart_bt_putc(u8 c)
{
    while ((UART1STA & 0x04)==0);
    UART1DATA = c;
}

#pragma location="BT_API_CMD"
void sbc_command_normal(u8 group, u8 request,u8 length, u8 *pdata)
{
    u8 i;
    UART1CON &= ~(BIT(0) | BIT(3));
    UART1CON |= BIT(4);

    uart_bt_putc(0xAC);
    uart_bt_putc(length);
    uart_bt_putc(group);

    for(i=0;i<(length-1);i++)
    {
      if(pdata == NULL)
          uart_bt_putc(request);
      uart_bt_putc(*pdata++);
    }
    delay_5ms(2);
    UART1CON |= BIT(0);
    UART1CON |= BIT(3);
}
#pragma location="BT_DEC_SEG"
__near_func void set_sbc_cts(void)
{
    SBC_CTS_H();
}
#pragma location="BT_DEC_SEG"
__near_func void clr_sbc_cts(void)
{
    SBC_CTS_L();
}
