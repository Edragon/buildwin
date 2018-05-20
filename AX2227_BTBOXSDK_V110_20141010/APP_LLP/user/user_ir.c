/*****************************************************************************
 * Module    : User
 * File      : user_ir.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户的IO设置
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_ir.h"
#include "user_key.h"


#if (HARD_IR_EN || SOFT_IR_EN)

#pragma constseg="IR_TABLE"
#define IR_KEY_NUM     21
IAR_CONST u8 tbl_ir_dat[IR_KEY_NUM] = {0x07,            0x09,           0x15,           0x16,               0x19,
                                       0x40,            0x43,           0x44,           0x45,               0x46,
                                       0x47,            0x0d,           0x0c,           0x18,               0x5e,
                                       0x08,            0x1c,           0x5a,           0x42,               0x52,
                                       0x4a};
IAR_CONST u8 tbl_ir_val[IR_KEY_NUM] = {T_KEY_PREV,      T_KEY_PLAY,     T_KEY_NEXT,     T_KEY_VOL_DOWN,     T_KEY_VOL_UP,
                                       T_KEY_LOOP,      T_KEY_EQ,       T_KEY_MODE,     T_KEY_POWER,        T_KEY_STOP,
                                       T_KEY_MUTE,      T_KEY_NUM_0,    T_KEY_NUM_1,    T_KEY_NUM_2,        T_KEY_NUM_3,
                                       T_KEY_NUM_4,     T_KEY_NUM_5,    T_KEY_NUM_6,    T_KEY_NUM_7,        T_KEY_NUM_8,
                                       T_KEY_NUM_9};
#pragma constseg=default


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

enum{
    IR_CAPTURE,
    IR_STA0,
    IR_STA1,
};
u8 ir_sta;
u8 ir_bit_val;
u8 ir_count;
u16 ir_data1;

/*****************timer1初始化,红外遥控软解码使用************************/
void ir_timer1_init(void)
{
    IE_TM1 = 0;
    P0DIR |= BIT(7);
    P0PU1 |= BIT(7);//开上拉
    P0PD0 &= ~BIT(7);
    TMR1CON0 = 0x74;    //Falling Edge trigger// Capture Mode //P07 //SystemClk
    TMR1CON1 = 0x37;   //OverflowInt,CaptureInt enalbe   //devide 128
    TMR1PRH = 0xA1;    //The overflow period of the timer is: Tinc-source * T1PSR * (T1PR + 1).
    TMR1PRL = 0x22;    //(1/48000000)*128*(0xA122+1) = 110 ms
    TMR1CNTL = 0;
    TMR1CNTH = 0;
    IPH0  |=  BIT(2);
    ir_sta = IR_CAPTURE;
    ir_count = 0;
    ir_data1 = 0;
    IE_TM1 = 1;
}

__near_func void ir_timer1(void)
{
    u8 ir_bit_val2 = 0;
       // IE_TM1 = 0;

    switch (ir_sta){
    case IR_CAPTURE:
        if (TMR1CON1 & 0x40)	//CAPTURE
        {
            TMR1PRH = 0x02;    //The overflow period of the timer is: Tinc-source * T1PSR * (T1PR + 1).
            TMR1PRL = 0x2A;    //(1/48000000)*128*(0xE0+1) = 0.6 ms
            TMR1CON1 &= ~(BIT(6)|BIT(4));
            TMR1CON1 |= BIT(5);
            TMR1CON0 = 0x54;
            TMR1CNTL = 0;
            TMR1CNTH = 0;           //Clear CNT
            ir_sta = IR_STA0;
            ir_state = 0;
            ir_data = 0;
        }
        else	//overflow
        {
            TMR1CON1 &= ~BIT(7);
            ir_state = 13;
            ir_data = 0;
        }
        break;
    case IR_STA0:     //1/3

        ir_bit_val = P0 & BIT(7);
        TMR1PRH = 0x00;    //The overflow period of the timer is: Tinc-source * T1PSR * (T1PR + 1).
        TMR1PRL = 0xDC;    //(1/48000000)*128*(0xE0+1) = 0.6 ms
        TMR1CON1 &= ~BIT(7);
        ir_sta = IR_STA1;
        break;
    case IR_STA1:        //2/3
        ir_bit_val2 = P0 & BIT(7);
        if(ir_bit_val == ir_bit_val2)
        {
            TMR1PRH = 0xA1;    //The overflow period of the timer is: Tinc-source * T1PSR * (T1PR + 1).
            TMR1PRL = 0x22;    //(1/48000000)*128*(0xA122+1) = 110 ms
            TMR1CON0 = 0x74; //设置为capture模式
            TMR1CON1 &= ~(BIT(7)|BIT(6));
            TMR1CON1 |= BIT(4);
            ir_sta =  IR_CAPTURE;
            break;
        }
        else
        {
            ir_data <<= 1;
            if(!ir_bit_val)
            {
                ir_data |= BIT(0);
            }
            ir_state ++;
            TMR1CON1 &= ~BIT(7);
            TMR1PRH = 0x01;      //The overflow period of the timer is: Tinc-source * T1PSR * (T1PR + 1).
            TMR1PRL = 0xBB;      //(1/48000000)*128*(0xE0+1) = 0.6 ms
            ir_sta = IR_STA0;
        }
        break;
    }
     // IE_TM1 = 1;
}
__near_func u8 key_deal_ir(void)
{
    u8 key_temp = 0;
    u8 key_val = 0;
    if(ir_state == 13)
    {
        //printf("k:%04x\n",ir_data);
        if(!ir_count)
            ir_data1 = ir_data;
        if(!ir_data1)
        {
            ir_data = 0;
            ir_state = 0;
            ir_count = 0;
            return 0;
        }
        key_val = ir_key_get(ir_data1 & 0x3f);
        key_temp = key_val;
        key_val = NO_KEY;
        if(ir_data1 == ir_data)
        {
            ir_count++;
            if(ir_count >7)
            {
                key_val = key_temp | T_KEY_LONG;//长按
            }
        }
        else
        {
            if(ir_count < 4)
            {
                if(key_temp < 31)
                    key_val = key_temp | T_KEY_SHORT_UP;//短按
                else
                    key_val = key_temp;
            }
            else if(ir_count <7)
            {
                key_val = key_temp | T_KEY_LONG_UP;
            }
            ir_count = 0;
        }
        if(key_temp > 31 && key_val>= 0x80)
        {
            key_val = NO_KEY;
        }
        if(key_temp > 31 && key_val > 0)
        {
            key_val = key_temp;
        }
        ir_data = 0;
        ir_state = 0;

    }
    return key_val;
}

#pragma vector = VECTOR_OFFSET(2)
#pragma register_bank=3
__interrupt __near_func void timer1_isr(void)
{
    ISR_ENTER();
    ir_timer1();
    ISR_EXIT();
}

#endif //SOFT_IR_EN


#if HARD_IR_EN

#pragma location="INIT_SEG_1"
void ir_hardware_init(void)
{
#if SYS_CRY == CRY_32K
    CLKCON2  =  (CLKCON2 &0xF8)|0x06; //0x06, IR Select 32K OSC
    PCON0 &= ~BIT(3);  //IR clk_en
    P0DIR |= BIT(7);
    P0PU1 |= BIT(7);
    P0PD0 &= ~BIT(7);
    IRCON0 = 0x01;  // p07 recv pin, ir enable
    IRCON1 = 0x5e;
    IRCON1 = 0x28;
    IRCON1 = 0x02;
    IRCON1 = 0x04;
    IRCON1 = 0x09;
#else
    CLKCON2  =  (CLKCON2 &0xF8)|0x00;     //0x00 1M//0x04 32K  //IR Devide Select PLL 1M
    PCON0 &= ~BIT(3);  //IR clk_en
    P0DIR |= BIT(7);
    P0PU1 |= BIT(7);
    P0PD0 &= ~BIT(7);

    IRCON0 = 0x01; //0x01 // p07 recv pin  //ir enable
    IRCON1 = 0x9C;
    IRCON1 = 0x50;
    IRCON1 = 0x04;
    IRCON1 = 0x08;
    IRCON1 = 0x11;
#endif
}

#pragma location="IR_SEG"
__near_func u8 ir_hardware_scan(void)
{
    static u8 rep_cnt = 0;
    if (IRCON0 & BIT(2)) {
        rep_cnt = 0;
        IRCON0 &= ~BIT(2);
        ir_data = IRDAT2;   //IRDAT0和IRDAT1是用户码，开发人员可以根据自己需求设置判断条件
        //printf("ir_data:%d\n",ir_data);
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
