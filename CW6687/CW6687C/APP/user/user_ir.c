/*****************************************************************************
 * Module    : User
 * File      : user_ir.c
 * Author    : Hanny
 * Function  : 用户的IO设置
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_ir.h"
#include "user_key.h"


#if (HARD_IR_EN || SOFT_IR_EN)

#include SET_USER_PATH(CFG_IO_IR_DIR, /io/io_ir.c)

volatile  u16 ir_data;
volatile  u8 ir_state;

__near_func u8 ir_key_get(u8 ir_data)
{
    u8 knum = IR_KEY_NUM;
    while (knum--)
    {
        if(ir_data == tbl_ir_dat[knum])
        {
            return tbl_ir_val[knum];
        }
    }
    return NO_KEY;
}


#if SOFT_IR_EN

/*****************timer2初始化,红外遥控软解码使用************************/
volatile type_ir ir;
#pragma vector = VECTOR_OFFSET(TIMER2_INT)
#pragma register_bank=2   //0优先级(最低优先级)必须用bank1 //2优先级必须用bank2 //3优先级必须用bank3
__interrupt __near_func void timer2_isr(void)
{
    u8 ir_cnt;
    ISR_ENTER();
    if(TMR2CON1 & BIT(6)){
        //printf("cp!");
        ir_cnt = TMR2CNTH;
        //ir_cnt <<= 8;
        //ir_cnt +=  TMR1CNTL;  //1/48000*128*(0x1A6+1) =1.128   //1/48000*128*(0x353+1)=2.272 与理论符合.
        TMR2CNTH = 0;
        TMR2CNTL = 0;
        TMR2CON1 &= ~BIT(6);
        //uart_putchar(ir_cnt);

    }else if(TMR2CON1 & BIT(7)){
        //printf("op>");
        TMR2CON1 &= ~BIT(7);
        ir_cnt = 0;   //溢出中断 -->ir_state=0;
    }
    //2C 13 01 01 01 01 01 01 01 01 03 03 03 03 03 03 03 03 03 01 03 01 01 01 03 01 01 03
    //01 03 03 03 01 03 3B 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10
    //(9+2.25)/(1/48000*128)/256 = 0x10.7A
    //(110-11.25)/(1/48000*128)/256 = 0x90.A74 (0x8E)这里有点误差,实测0x8E
    //第一个重复脉冲串实测0x13
    if (((ir_cnt > 0x0A) && (ir_cnt < 0x15)) || ((ir_cnt > 0x33) && (ir_cnt < 0x46)) || ((ir_cnt > 0x87) && (ir_cnt < 0x97)))
    {
       //irState = 32,即按键已得到，当前键码有效使得irState=32，使irData值也不变，随时供KeyScan提取
       goto EXIT_TM2;
    }

    ir.data >>= 1;               //irData低8位存码值
    ir.state++;
    //uart_putchar(ir.state);
    //if(32 == ir.state){uart_putchar(ir.data>>8);uart_putchar(ir.data);}
    if (ir_cnt == 3)
    {
        ir.data |= 0x8000;       //irData为32位  // 这里是关键，它是半道插入1，相当于irData为16位的情形，通过它，刚好把数据位提出来放到最后一个字节
    }
    else if (ir_cnt == 1)
    {
        goto EXIT_TM2;
    }
    else
    {
        ir.state = 0;               //可见 只有counter = 01 02 0B 29 62会认为是有效值，其它都会将irState清0，预示着一下解析IRd ID的开始
        ir.data = NO_KEY;
    }
EXIT_TM2:
    ISR_EXIT();
}


void ir_timer2_init(void)
{
    IE_TM2 = 0;
    P1DIR |= BIT(6);
    P1PUS0 &= ~BIT(6);
    P1PUS1 &= ~BIT(6);
    P1PU  |= BIT(6);//开上拉
    TMR2CON0 = 0x74;    //Falling Edge trigger// Capture Mode //P07 //SystemClk
    TMR2CON1 = 0x37;   //OverflowInt,CaptureInt enalbe   //devide 128
    TMR2PRH = 0xA8;    //The overflow period of the timer is: Tinc-source * T1PSR * (T1PR + 1).
    TMR2PRL = 0x74;    //(1/48000000)*128*(0xA122+1) = 110 ms   //125/(1/48000*128)-1 = 0xA874
    TMR2CNTL = 0;
    TMR2CNTH = 0;
    IP0 &= ~BIT(3);    //优先级2
    IPH0 |= BIT(3);    //Timer2使用优先级：2
    IE_TM2 = 1;
}

#endif //SOFT_IR_EN


#if HARD_IR_EN

#pragma location="INIT_SEG_1"
void ir_hardware_init(void)
{
#if SYS_CRY == CRY_32K
    CLKCON2  =  (CLKCON2 &0xF8)|0x06; //0x06, IR Select 32K OSC
    PCON0 &= ~BIT(3);  //IR clk_en
    P3DIR |= BIT(3);
    P3PU |= BIT(3);
    P3PD &=~ BIT(3);
    IRCON0 = 0x21;  //0x21:P33 0x01:P07 0x11:P16  0x31:P40  recv pin  //ir enable
    IRCON1 = 0x5e;
    IRCON1 = 0x28;
    IRCON1 = 0x02;
    IRCON1 = 0x04;
    IRCON1 = 0x09;
#else
    #error "NO CRY_32K must sel SOFT_IR_EN"
    /*
    CLKCON2  =  (CLKCON2 &0xF8)|0x00;     //0x00 1M//0x04 32K  //IR CLK Select PLL 1M
    PCON0 &= ~BIT(3);  //IR clk_en
    P3DIR |= BIT(3);
    P3PU |= BIT(3);
    P3PD &=~ BIT(3);
    IRCON0 = 0x21; //0x21:P33 0x01:P07 0x11:P16  0x31:P40  recv pin  //ir enable
    IRCON1 = 0x9C;
    IRCON1 = 0x50;
    IRCON1 = 0x04;
    IRCON1 = 0x08;
    IRCON1 = 0x11;
    */
#endif
}

//#pragma location="IR_SEG"
__near_func u8 ir_hardware_scan(void)
{
    static u8 rep_cnt = 0;
    if (IRCON0 & BIT(2)) {
        rep_cnt = 0;
        IRCON0 &= ~BIT(2);
        ir_data = IRDAT2;   //IRDAT0和IRDAT1是用户码，开发人员可以根据自己需求设置判断条件
        //printf("%02x\n",ir_data);
    }
    if (rep_cnt < 23) {
        rep_cnt++;
    } else {                //超过110 ms没有红外按键
        ir_data = 0xff;        //PLZ make sure ir_table[irData] is NO_KEY
        rep_cnt = 30;       //time out
    }

    return ir_key_get(ir_data & 0xff);
}

#endif //HARD_IR_EN

#endif //(HARD_IR_EN || SOFT_IR_EN)
