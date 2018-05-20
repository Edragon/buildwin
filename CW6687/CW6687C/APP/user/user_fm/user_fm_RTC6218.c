#include "include.h"
#include "RTC6218.h"
#include "user_fm.h"
/*
RTC6218收音效果
a、五十k步进
RTC62XX_CKHopping  数组倒数第5byte，  越大台越多(可能假台也多了,假台多可以调小它)
RTC62XX_power_up   数组倒数第1byte，  越大台越多(可能假台也多了,假台多可以调小它)
b、一百k步进
RTC62XX_power_up   数组倒数第1、2 byte，  越大台越多(可能假台也多了,假多少可以调小它)
*/

#if (FM_TYPE == FM_RTC6218)

#define RTC62xxDelay
//#define INTERNAL_XTAL_6218  //增强晶振驱动能力
#define SINGLE_SETP_SEEK

#define CRYSTAL_32KHz       0
#define CRYSTAL_12MHz       1

#if (FM_CLK_SEL == P33_OUT_32K)
    #define FM_CLOCK        CRYSTAL_32KHz
#elif ((FM_CLK_SEL == P33_OUT_12M) || (FM_CLK_SEL == FMOSC_REUSE_12MSDCLK))
    #define FM_CLOCK        CRYSTAL_12MHz
#else
    #define FM_CLOCK        CRYSTAL_32KHz
#endif

#if (FM_CLK_SEL != P33_OUT_12M || FM_CLK_SEL == FMOSC_REUSE_12MSDCLK)
    #define INTERNAL_XTAL_6218  //共晶振时增强晶振驱动能力
#endif

#pragma constseg = "FM_CONST_RTC6218"
const u8 RTC62XX_turnoff[]= {0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
                             0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
                             0x16,0xAA
                            };
const u8 RTC62XX_turnon[] = {0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
                             0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
                             0x96,0xAA
                            };
//Bk4_6 [7:0] 50k DC
const u8  RTC62XX_CKHopping[]= {0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x70,0x01, 0x20,0x00,
                                0x05,0x0F, 0x0E,0x85, 0x5A,0xA6, 0xDC,0x53, 0x80,0x00, 0x00,0xA3, 0x00,0xA3,
                                0xD0,0x08, 0x7F,0x80, 0x3C,0x08, 0xA6,0xCF, 0x81,0x00, 0x00,0x00, 0x01,/*0x30、0x20*/0x20,
                                0x47,0x00, 0x00,0x00		// TP Suggested hopping enable 20131203
                               };
const u8  RTC62XX_swbk5[]= {0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x70,0x01, 0x60,0x00,
                            0x35,0x90, 0x63,0x11, 0x30,0x08, 0x00,0x17, 0x0D,0x79, 0x7D,0x2F, 0x80,0x00,
                            0x02,0x01, 0x77,0x1F, 0x32,0x41, 0x26,0x28, 0xA5,0x16, 0x86,0x80, 0x00,0x00,
                            0x00,0x00, 0x00,0x00
                            };
// off AFC re-tune
const u8  RTC62XX_swbk1[]= {0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x70,0x01, 0x40,0x00,
                            0x14,0x08, 0x01,0x00, 0x03,0x70, 0x00,0x70, 0x00,0x52, 0x02,0xC0, 0x00,0x00,
                            0x14,0x40, 0x00,0x80, 0x0A,0x40, 0x00,0x00, 0x40,0x02, 0x80,0x5A, 0x0D,0x35,
                            0x73,0x67, 0x00,0x00
                           };

#if FM_CLOCK == CRYSTAL_32KHz
    #ifndef INTERNAL_XTAL_6218
    // last byte is spike threshold, the larger the more, recommand under 0x50, default 0x10
    const  u8 RTC62XX_power_up[] = {0xC0,0x00, 0x09,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x00, 0x00,0x00, 0x00,0x00, 0x40,0x30};
    #else
    const  u8 RTC62XX_power_up[] = {0xC0,0x00, 0x09,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x00, 0xB4,0x00, 0x00,0x00, 0x40,0x40};  // 76~108MHz
    #endif
#elif FM_CLOCK == CRYSTAL_12MHz
    #ifndef INTERNAL_XTAL_6218
    // last byte is spike threshold, the larger the more, recommand under 0x50, default 0x10
    const  u8 RTC62XX_power_up[] = {0xC0,0x00, 0x18,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x01, 0x70,0x00, 0x00,0x00, 0x40,0x20/*0x50*/}; //0x50
    #else
    const  u8 RTC62XX_power_up[] = {0xC0,0x00, 0x18,0x00, 0x00,0x00, 0x00,0x0C, 0x80,0x01, 0xB4,0x00, 0x00,0x00, 0x35,0x20};
    #endif
#else
    #error "请选择FM_CLOCK"
#endif

//const  u8 RTC62XX_rPwrOff[] = {0xC0,0x0f,0x04,0x00,0x00,0x00,0x00,0x0c,0xc0,0x00};

#if FM_CLOCK == CRYSTAL_32KHz
    #ifndef INTERNAL_XTAL_6218
    const u8 RTC62XX_Initial_value[] =         {0xC0,0x00,0x09,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x00,0xe0,0x00};	// RSSI_TH = 0x0C
    const u8 RTC62XX_Initial_gotobk3value[] =  {0xC0,0x00,0x09,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x00,0xc0,0x00};	// RSSI_TH = 0x0C
    const u8 RTC62XX_Initial_gotobk4value[] =  {0xC0,0x00,0x09,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x00,0x20,0x00};	// RSSI_TH = 0x0C
    #else
    const u8 RTC62XX_Initial_value[] =         {0xC0,0x00,0x09,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0xB4,0x00,0xe0,0x00};	// RSSI_TH = 0x0C
    const u8 RTC62XX_Initial_gotobk3value[] =  {0xC0,0x00,0x09,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x00,0xc0,0x00};	// RSSI_TH = 0x0C
    const u8 RTC62XX_Initial_gotobk4value[] =  {0xC0,0x00,0x09,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x00,0x20,0x00};	// RSSI_TH = 0x0C
    #endif
#elif FM_CLOCK == CRYSTAL_12MHz
    #ifndef INTERNAL_XTAL_6218
    const u8 RTC62XX_Initial_value[] =         {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x01,0x70,0x00,0xe0,0x00};  // RSSI_TH = 0x0C
    const u8 RTC62XX_Initial_gotobk3value[] =  {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x01,0x70,0x00,0xc0,0x00};  // RSSI_TH = 0x0C
    const u8 RTC62XX_Initial_gotobk4value[] =  {0xC0,0x00,0x09,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x00,0x20,0x00};	// RSSI_TH = 0x0C
    #else
    const u8 RTC62XX_Initial_value[] =         {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x01,0xB4,0x00,0xe0,0x00};  // RSSI_TH = 0x0C
    const u8 RTC62XX_Initial_gotobk3value[] =  {0xC0,0x00,0x18,0x00,0x00,0x00,0x00,0x0c,0x80,0x01,0xB4,0x00,0xc0,0x00};  // RSSI_TH = 0x0C
    const u8 RTC62XX_Initial_gotobk4value[] =  {0xC0,0x00,0x09,0x00,0x00,0x00,0x00,0x0c,0x80,0x00,0x00,0x00,0x20,0x00};	// RSSI_TH = 0x0C
    #endif
#else
    #error "请选择FM_CLOCK"
#endif

const u8 RTC62XX_Initial_value1[] = {0x00,0x72,0x00,0xFF,0x00,0x1F,0x03,0xFF,0x56,0xD1,0x13,0xB7,0x00,0x00};
#pragma constseg=default

u8 Spike_Diff_TH = 16;
u8 g_rssi_th=0x09;			// 20131202 bank5_0 RSSI compensation 0x02A1->0x0201, so RSSI_TH should be decreased 10db
u8 killed_by_mono_count=0, real_count=0;


#pragma location="FM_SEG_RTC6218"
void RTC6218N_Delay(u16 n)
{
    while (n--)
        asm("nop");
}

#pragma location="FM_SEG_RTC6218"
void OperationRTC6218N_write(const u8 __code *data, u8 numBytes)
{
    iic_start();
    iic_sendbyte(0xc8);
    iic_getack();
    for(u8 j = 0; j < numBytes; j++){
        iic_sendbyte(data[j]);
        iic_getack();
    }
    RTC6218N_Delay(10);		//delay_5ms(1);
    iic_stop();
}

#pragma location="FM_SEG_RTC6218"
void OperationRTC6218N_write_a(u8 *data, u8 numBytes)
{
    iic_start();
    iic_sendbyte(0xc8);
    iic_getack();
    for(u8 j = 0; j < numBytes; j++){
        iic_sendbyte(data[j]);
        iic_getack();
    }
    RTC6218N_Delay(10);		//delay_5ms(1);
    iic_stop();
}

#pragma location="FM_SEG_RTC6218"
void OperationRTC6218N_read(u8 *data, u8 numBytes)
{
    iic_start();
    iic_sendbyte(0xc9);
    iic_getack();
    for(u8 j = 0; j < numBytes; j++){
        data[j] = iic_getbyte();
        if(j==numBytes-1){
            iic_sendnack();
        }
        else{
            iic_sendack();
        }
    }
    RTC6218N_Delay(10);		//delay_5ms(1);
    iic_stop();
}

/*************************************************************
*  Radio get ID
*
*************************************************************/
#pragma location="FM_INIT_RTC6218"
u8 RTC6218_online(void)
{
    u8 RTC68XX_reg_data[16];

    for(u8 i=0; i<3; i++){      //IIC与其他引脚复用时 FM ID 改为读三次比较保险
        OperationRTC6218N_read(RTC68XX_reg_data,16);
        //printf("ID1: 0x%02x , ID2: 0x%02x\n",RTC68XX_reg_data[12],RTC68XX_reg_data[13]);  //此句注释保留在这里，方便调试
        if (RTC68XX_reg_data[12] == 0x16 && RTC68XX_reg_data[13] == 0x88){
            RTC6218_init();
            return 1;
        }
    }
    return 0;
}

#pragma location="FM_SEG_RTC6218_1"
void RTC6218_init_bk3_regf_b_stereo_lowTh(void)
{
    u8 RTC68XX_reg_data[32+16];

	//printf("RTC6218_init_bk3_regf_b set rb rf value\n\n");
    OperationRTC6218N_write(RTC62XX_Initial_gotobk3value,14);//change bk3
    RTC6218N_Delay(10);		//delay_5ms(1);
    OperationRTC6218N_read(RTC68XX_reg_data,32);
    RTC6218N_Delay(10);		//delay_5ms(1);
    //printf("bk3={\n \n");
    //for(i=0;i<4;i++){
		//printf("0x%02x%02x,0x%02x%02x,0x%02x%02x,0x%02x%02x,", \
             RTC68XX_reg_data[8*i+0],RTC68XX_reg_data[8*i+1],RTC68XX_reg_data[8*i+2], \
             RTC68XX_reg_data[8*i+3], \
			 RTC68XX_reg_data[8*i+4],RTC68XX_reg_data[8*i+5], \
             RTC68XX_reg_data[8*i+6],RTC68XX_reg_data[8*i+7]);
	//}
    //printf("}\n");
    RTC68XX_reg_data[32]=RTC68XX_reg_data[0];
	RTC68XX_reg_data[33]=RTC68XX_reg_data[1];
	RTC68XX_reg_data[34]=0x40;		// changtt : discussed with TP to rollback stereo TH : 0x10
	RTC68XX_reg_data[35]=0xff;
    RTC68XX_reg_data[36]=RTC68XX_reg_data[4];
	RTC68XX_reg_data[37]=RTC68XX_reg_data[5];
    RTC68XX_reg_data[38]=RTC68XX_reg_data[6];
	RTC68XX_reg_data[39]=RTC68XX_reg_data[7];
	RTC68XX_reg_data[40]=RTC68XX_reg_data[8];
	RTC68XX_reg_data[41]=RTC68XX_reg_data[9];
	RTC68XX_reg_data[42]=0x0c;
	RTC68XX_reg_data[43]=0x40;
    RTC68XX_reg_data[44]=RTC68XX_reg_data[12]; //reg 0
	RTC68XX_reg_data[45]=RTC68XX_reg_data[13];
	RTC68XX_reg_data[46]=RTC68XX_reg_data[14]; //reg 1
	RTC68XX_reg_data[47]=RTC68XX_reg_data[15];

    OperationRTC6218N_write_a(RTC68XX_reg_data+16,32);
    RTC6218N_Delay(10);		//delay_5ms(1);
    OperationRTC6218N_read(RTC68XX_reg_data,32);
    RTC6218N_Delay(10);		//delay_5ms(1);
    //printf("read after write low_th bk3={\n");
    //for(u8 i=0;i<4;i++){
		//printf("0x%02x%02x,0x%02x%02x,0x%02x%02x,0x%02x%02x,", \
             RTC68XX_reg_data[8*i+0],RTC68XX_reg_data[8*i+1],RTC68XX_reg_data[8*i+2], \
             RTC68XX_reg_data[8*i+3], \
			 RTC68XX_reg_data[8*i+4],RTC68XX_reg_data[8*i+5], \
             RTC68XX_reg_data[8*i+6],RTC68XX_reg_data[8*i+7]);

	//}
   // //printf("}\n");
    RTC68XX_reg_data[28]=0x00;
	RTC68XX_reg_data[29]=0x00;
	OperationRTC6218N_write_a(RTC68XX_reg_data+16,14); //change to bk0
    RTC6218N_Delay(10);		//delay_5ms(1);
}

#pragma location="FM_SEG_RTC6218_1"
void RTC6218_init_bk3_regf_b(void)
{
    u8 RTC68XX_reg_data[32+16];

	//printf("RTC6218_init_bk3_regf_b set default value\n\n\n");
    OperationRTC6218N_write(RTC62XX_Initial_gotobk3value,14);//change bk3
    RTC6218N_Delay(10);		//delay_5ms(1);
    OperationRTC6218N_read(RTC68XX_reg_data,32);
    RTC6218N_Delay(10);		//delay_5ms(1);
    //printf("bk3={\n");
    //for(u8 i=0;i<4;i++){
		//printf("0x%02x%02x,0x%02x%02x,0x%02x%02x,0x%02x%02x,", \
             RTC68XX_reg_data[8*i+0],RTC68XX_reg_data[8*i+1],RTC68XX_reg_data[8*i+2], \
             RTC68XX_reg_data[8*i+3], \
			 RTC68XX_reg_data[8*i+4],RTC68XX_reg_data[8*i+5], \
             RTC68XX_reg_data[8*i+6],RTC68XX_reg_data[8*i+7]);
	//}
    //printf("}\n\n");
    RTC68XX_reg_data[32]=RTC68XX_reg_data[0];
	RTC68XX_reg_data[33]=RTC68XX_reg_data[1];
	RTC68XX_reg_data[34]=0x40;
	RTC68XX_reg_data[35]=0x40;
    RTC68XX_reg_data[36]=RTC68XX_reg_data[4];
	RTC68XX_reg_data[37]=RTC68XX_reg_data[5];
    RTC68XX_reg_data[38]=RTC68XX_reg_data[6];
	RTC68XX_reg_data[39]=RTC68XX_reg_data[7];
	RTC68XX_reg_data[40]=RTC68XX_reg_data[8];
	RTC68XX_reg_data[41]=RTC68XX_reg_data[9];
	RTC68XX_reg_data[42]=0x0d;
	RTC68XX_reg_data[43]=0x40;
    RTC68XX_reg_data[44]=RTC68XX_reg_data[12]; //reg 0
	RTC68XX_reg_data[45]=RTC68XX_reg_data[13];
	RTC68XX_reg_data[46]=RTC68XX_reg_data[14]; //reg 1
	RTC68XX_reg_data[47]=RTC68XX_reg_data[15];

    OperationRTC6218N_write_a(RTC68XX_reg_data+16,32);
    RTC6218N_Delay(10);		//delay_5ms(1);
    OperationRTC6218N_read(RTC68XX_reg_data,32);
    RTC6218N_Delay(10);		//delay_5ms(1);
    //printf("read after write default bk3={\n");
    //for(u8 i=0;i<4;i++){
		//printf("0x%02x%02x,0x%02x%02x,0x%02x%02x,0x%02x%02x,", \
             RTC68XX_reg_data[8*i+0],RTC68XX_reg_data[8*i+1],RTC68XX_reg_data[8*i+2], \
             RTC68XX_reg_data[8*i+3], \
			 RTC68XX_reg_data[8*i+4],RTC68XX_reg_data[8*i+5], \
             RTC68XX_reg_data[8*i+6],RTC68XX_reg_data[8*i+7]);

	//}
   // //printf("}\n\n");
    RTC68XX_reg_data[28]=0x00;
	RTC68XX_reg_data[29]=0x00;
	OperationRTC6218N_write_a(RTC68XX_reg_data+16,14); //change to bk0
    RTC6218N_Delay(10);		//delay_5ms(1);
}

#pragma location="FM_INIT_RTC6218"
void RTC6218_init(void)
{
    OperationRTC6218N_write(RTC62XX_turnoff,30);
    delay_5ms(5);
    OperationRTC6218N_write(RTC62XX_turnon,30);
    delay_5ms(40);
    OperationRTC6218N_write(RTC62XX_CKHopping,46);
    RTC6218N_Delay(10);		//delay_5ms(5);
#if 0
    OperationRTC6218N_write(RTC62XX_swbk5,46);
    RTC6218N_Delay(10);		//delay_5ms(5);
    OperationRTC6218N_write(RTC62XX_swbk1,46);
    RTC6218N_Delay(10);		//delay_5ms(5);
#endif
    //RTC6218_init_bk3_regf_b();
    //delay_5ms(5);
    OperationRTC6218N_write(RTC62XX_power_up,16);
    RTC6218N_Delay(10);		//delay_5ms(40);
}

#pragma location="FM_SEG_RTC6218"
void RTC6218_off(void)
{
    //OperationRTC6218N_write(RTC62XX_rPwrOff,10);
    //delay_5ms(1);
    OperationRTC6218N_write(RTC62XX_turnoff,30);
    delay_5ms(1);
}

/**************************************

RTC6218N_Set_Property_FM_Volume()

FM_Volumn: 0~15

***************************************/
#pragma location="FM_SEG_RTC6218"
void RTC6218_set_vol(u8 FM_Volumn)
{
    // Some FM IC has 17 level, RTC serise only level 16
    /*if (FM_Volumn>0){
        FM_Volumn--;
    }*/
    u8 RTC62XX_set_property[] = {0xC0,0x0f};
    RTC62XX_set_property[1] = (RTC62XX_set_property[1] & 0xf0) | FM_Volumn;
    OperationRTC6218N_write_a(RTC62XX_set_property,2);
}

//add by cyrus for debug spike
#pragma location="FM_SEG_RTC6218_1"
void key_func_RTC6218_sub_spike(void)
{
    u8 RTC68XX_reg_data[32];
    delay_5ms(1);
    OperationRTC6218N_read(RTC68XX_reg_data,32);
    delay_5ms(10);
    //printf("key_func_RTC6218_add_spike read1 Spike bank0_r9low=%x\n", RTC68XX_reg_data[31]);
    if((0x30==RTC68XX_reg_data[31])){
		RTC68XX_reg_data[31] = 0x10;
    }
    else{
		RTC68XX_reg_data[31]+=4;
	}
    //printf("key_func_RTC6218_add_spike write Spike bank0_r9low=%x\n", RTC68XX_reg_data[31]);
    OperationRTC6218N_write_a(RTC68XX_reg_data+16,16);
    delay_5ms(40);
    OperationRTC6218N_read(RTC68XX_reg_data,32);
    //printf("key_func_RTC6218_add_spike r after w Spike bank0_r9low=%x\n", RTC68XX_reg_data[31]);
    delay_5ms(5);
}

//add by cyrus for debug spike
#pragma location="FM_SEG_RTC6218_1"
void key_func_RTC6218_sub_50kdc(void)
{
    u8 RTC68XX_reg_data[32+16];

    OperationRTC6218N_write(RTC62XX_Initial_gotobk4value,14);//change bk3
    delay_5ms(1);
    OperationRTC6218N_read(RTC68XX_reg_data,32);
	//printf("bk4={\n");
    //for(u8 i=0;i<4;i++){
		//printf("0x%02x%02x,0x%02x%02x,0x%02x%02x,0x%02x%02x,", \
            RTC68XX_reg_data[8*i+0],RTC68XX_reg_data[8*i+1],RTC68XX_reg_data[8*i+2], \
            RTC68XX_reg_data[8*i+3], \
		 	RTC68XX_reg_data[8*i+4],RTC68XX_reg_data[8*i+5], \
           	RTC68XX_reg_data[8*i+6],RTC68XX_reg_data[8*i+7]);
	//}
    //printf("}\n\n");
	if(0x40==RTC68XX_reg_data[25]){
		RTC68XX_reg_data[25]=0x10;
	}
	else{
		RTC68XX_reg_data[25]+=4;
	}

    //printf("key_func_RTC6218_add_dc will write dc =0x%02x\n", RTC68XX_reg_data[25]);
    OperationRTC6218N_write_a(RTC68XX_reg_data+16,10);

    delay_5ms(1);
    OperationRTC6218N_read(RTC68XX_reg_data,32);
	//printf("read after write bk4={\n");
    //for(u8 i=0;i<4;i++){
		//printf("0x%02x%02x,0x%02x%02x,0x%02x%02x,0x%02x%02x,", \
            RTC68XX_reg_data[8*i+0],RTC68XX_reg_data[8*i+1],RTC68XX_reg_data[8*i+2], \
            RTC68XX_reg_data[8*i+3], \
		 	RTC68XX_reg_data[8*i+4],RTC68XX_reg_data[8*i+5], \
           	RTC68XX_reg_data[8*i+6],RTC68XX_reg_data[8*i+7]);
	//}
	RTC68XX_reg_data[28]=0x00;
	RTC68XX_reg_data[29]=0x00;
	OperationRTC6218N_write_a(RTC68XX_reg_data+16,14); //change to bk0
    delay_5ms(10);
}

//add by cyrus for debug rssi threshhold
#pragma location="FM_SEG_RTC6218_1"
void key_func_RTC6218_add_rssi_th(void)
{
    //printf("key_func_RTC6218_add_rssi enter rssi=%x\n", rssi_th);
    if(0x30==g_rssi_th){
		g_rssi_th = 0x10;
    }
    else{
		g_rssi_th=+4;
	}
    //printf("key_func_RTC6218_add_rssi exit rssi=%x\n", rssi_th);
}

/**************************************
RTC6218N_FM_Tune_Freq()

channel_freq: 8750~10800
channel_space: 50,100,200
***************************************/
#pragma location="FM_SEG_RTC6218"
void RTC6218_set_freq(u16 channel_freq)
{
    //channel_freq=channel_freq*10;
    u16 freq_reg_data, loop_counter = 0;
    u8 RTC6218N_reg_data[32];
    u8 RTC6218N_channel_start_tune[] = {0xC0,0x00,0x98,0xCA};	//107.7MHz, 76~108
    u8 RTC6218N_channel_stop_tune[] = {0xC0,0x00,0x18,0xCA};
    u8 volume_temp = 0;
    //u8 RSSIValue;
    //int Spike_Count;
    //u16 DC_Count;

    // store volume setting for hardware seek complete.
    OperationRTC6218N_read(RTC6218N_reg_data, 18);
    volume_temp = RTC6218N_reg_data[17] & 0x0f;
    //printf("RTC6218_set_freq channel_freq = %d\n", channel_freq);
    RTC6218N_channel_start_tune[1] = (RTC6218N_channel_start_tune[1] & 0xf0) | volume_temp;
    RTC6218N_channel_stop_tune[1] = (RTC6218N_channel_stop_tune[1] & 0xf0) | volume_temp;

    //set tune bit
    //freq_reg_data = (channel_freq - 8750)/10;
    //freq_reg_data = (channel_freq - 8750)/5;
    freq_reg_data = (channel_freq - 7600)/5;
    RTC6218N_channel_start_tune[3] = freq_reg_data & 0xff;
    RTC6218N_channel_start_tune[2] = (RTC6218N_channel_start_tune[2] & 0xfc) | (freq_reg_data >> 8);
    RTC6218N_channel_stop_tune[3] = RTC6218N_channel_start_tune[3] ;

    RTC6218N_channel_start_tune[2]&= 0x7f;
    OperationRTC6218N_write_a(RTC6218N_channel_start_tune, 4);
    RTC6218N_Delay(10);		//delay_5ms(1);
    RTC6218N_channel_start_tune[2]|= 0x80;
    OperationRTC6218N_write_a(RTC6218N_channel_start_tune, 4);
    do{
		delay_5ms(8);			// changtt fixed - wait more than 35ms
        OperationRTC6218N_read(RTC6218N_reg_data, 2);
        loop_counter++;//printf("loop_counter: 0x%04x\n",loop_counter);
    }
    while(((RTC6218N_reg_data[0]&0x40) == 0) && (loop_counter < 70)); //0xffff  //for loop_counter, when tune, the loop time must > 60ms
    loop_counter = 0;

    //clear tune bit
    OperationRTC6218N_write_a(RTC6218N_channel_stop_tune, 4);
    //wait STC=0
    do{
        OperationRTC6218N_read(RTC6218N_reg_data, 2);
        loop_counter++;//printf("loop_counter: 0x%02x\n",loop_counter);
    }
    while(((RTC6218N_reg_data[0]&0x40) != 0) && (loop_counter < 0xff));

    //debug
    OperationRTC6218N_write(RTC62XX_Initial_value,14);  // change to bk7
    RTC6218N_Delay(10);		//delay_5ms(1);
    OperationRTC6218N_read (RTC6218N_reg_data, 24);	// Readout (BK7_5[15])
    //Spike_Count = RTC6218N_reg_data[20];
    if(RTC6218N_reg_data[22]&0x2!=0){
		//DC_Count = (~(((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23])+1) &0x1ff ; //2's complement
    }
    else{
        //DC_Count = (((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23]);
    }
    OperationRTC6218N_write(RTC62XX_Initial_value1,14); // change to bk0

    //printf("Spike_Count_1=%x,DC_Count=%x\n\n", Spike_Count,DC_Count);
	//printf("bank 7_4 = %x%x\n",RTC6218N_reg_data[20],RTC6218N_reg_data[21]);
	//printf("bank 7_5 = %x%x\n",RTC6218N_reg_data[22],RTC6218N_reg_data[23]);
    //OperationRTC6218N_read (RTC6218N_reg_data, 4);
    //RSSIValue = RTC6218N_reg_data[3];
    //printf("RSSIValue = %d\n",RSSIValue);
	//printf("\n\n\n");
}

#pragma location="FM_SEG_RTC6218_1"
void RTC6218_set_freq_for_debug_12M_tune(u16 channel_freq)
{
    //channel_freq=channel_freq*10;
    u16 freq_reg_data, loop_counter = 0;
    u8 RTC6218N_reg_data[32];
    u8 RTC6218N_channel_start_tune[] = {0xC0,0x00,0x98,0xCA};	//107.7MHz, 76~108
    u8 RTC6218N_channel_stop_tune[] = {0xC0,0x00,0x18,0xCA};
    u8 volume_temp = 0;
    //u8 RSSIValue;
    //int Spike_Count;
    //u16 DC_Count;

    //store volume setting for hardware seek complete.
    OperationRTC6218N_read(RTC6218N_reg_data, 18);
    volume_temp = RTC6218N_reg_data[17] & 0x0f;
    //printf("channel_freq = %d\n", channel_freq);
    RTC6218N_channel_start_tune[1] = (RTC6218N_channel_start_tune[1] & 0xf0) | volume_temp;
    RTC6218N_channel_stop_tune[1] = (RTC6218N_channel_stop_tune[1] & 0xf0) | volume_temp;

    //set tune bit
    //freq_reg_data = (channel_freq - 8750)/10;
    //freq_reg_data = (channel_freq - 8750)/5;
    freq_reg_data = (channel_freq - 7600)/5;
    RTC6218N_channel_start_tune[3] = freq_reg_data & 0xff;
    RTC6218N_channel_start_tune[2] = (RTC6218N_channel_start_tune[2] & 0xfc) | (freq_reg_data >> 8);
    RTC6218N_channel_stop_tune[3] = RTC6218N_channel_start_tune[3] ;

    RTC6218N_channel_start_tune[2]&= 0x7f;
    OperationRTC6218N_write_a(RTC6218N_channel_start_tune, 4);
    RTC6218N_Delay(10);		//delay_5ms(1);
    RTC6218N_channel_start_tune[2]|= 0x80;
    OperationRTC6218N_write_a(RTC6218N_channel_start_tune, 4);
    do{
		delay_5ms(2);			// changtt fixed - wait more than 35ms
        OperationRTC6218N_read(RTC6218N_reg_data, 2);
        loop_counter++;
    }
    while(((RTC6218N_reg_data[0]&0x40) == 0) && (loop_counter < 0xffff));		//for loop_counter, when tune, the loop time must > 60ms
    loop_counter = 0;

    //debug
    OperationRTC6218N_write(RTC62XX_Initial_value,14);  // change to bk7
    RTC6218N_Delay(10);		//delay_5ms(1);
    OperationRTC6218N_read (RTC6218N_reg_data, 24);	// Readout (BK7_5[15])
    //Spike_Count = RTC6218N_reg_data[20];
    if(RTC6218N_reg_data[22]&0x2!=0){
       //DC_Count = (~(((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23])+1) &0x1ff ; //2's complement
    }
    else{
       //DC_Count = (((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23]);
    }
    OperationRTC6218N_write(RTC62XX_Initial_value1,14); // change to bk0

    //printf("Spike_Count_1=%x\n", Spike_Count);
    //printf("DC_Count=%x\n", DC_Count);
	//printf("stc=1 bank 7_1 = 0x%x%x\n",RTC6218N_reg_data[14],RTC6218N_reg_data[15]);
	//printf("stc=1 bank 7_3 = 0x%x%x\n",RTC6218N_reg_data[18],RTC6218N_reg_data[19]);
	//printf("stc=1 bank 7_4 = 0x%x%x\n",RTC6218N_reg_data[20],RTC6218N_reg_data[21]);
	//printf("stc=1 bank 7_5 = 0x%x%x\n",RTC6218N_reg_data[22],RTC6218N_reg_data[23]);
    //OperationRTC6218N_read (RTC6218N_reg_data, 4);
    //RSSIValue = RTC6218N_reg_data[3];
    //printf("RSSIValue = %d\n",RSSIValue);
	
    //clear tune bit
    OperationRTC6218N_write_a(RTC6218N_channel_stop_tune, 4);
    //wait STC=0
    do{
        OperationRTC6218N_read(RTC6218N_reg_data, 2);
        loop_counter++;
    }while(((RTC6218N_reg_data[0]&0x40) != 0) && (loop_counter < 0xff));

    OperationRTC6218N_write(RTC62XX_Initial_value,14);  // change to bk7
    RTC6218N_Delay(10);		//delay_5ms(1);
    OperationRTC6218N_read (RTC6218N_reg_data, 24);	// Readout (BK7_5[15])
    //Spike_Count = RTC6218N_reg_data[20];
    if(RTC6218N_reg_data[22]&0x2!=0){
       //DC_Count = (~(((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23])+1) &0x1ff ; //2's complement
    }
    else{
       //DC_Count = (((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23]);
    }
    OperationRTC6218N_write(RTC62XX_Initial_value1,14); // change to bk0

    //printf("Spike_Count_1=%x\n", Spike_Count);
    //printf("DC_Count=%x\n", DC_Count);
	//printf("stc=0 bank 7_1 = 0x%x%x\n",RTC6218N_reg_data[14],RTC6218N_reg_data[15]);
	//printf("stc=0 bank 7_3 = 0x%x%x\n",RTC6218N_reg_data[18],RTC6218N_reg_data[19]);
	//printf("stc=0 bank 7_4 = 0x%x%x\n",RTC6218N_reg_data[20],RTC6218N_reg_data[21]);
	//printf("stc=0 bank 7_5 = 0x%x%x\n",RTC6218N_reg_data[22],RTC6218N_reg_data[23]);
	//printf("bank 7_5 = 0x%x%x\n",RTC6218N_reg_data[22],RTC6218N_reg_data[23]);
    //OperationRTC6218N_read (RTC6218N_reg_data, 4);
    //RSSIValue = RTC6218N_reg_data[3];
    //printf("RSSIValue = %d\n",RSSIValue);
    //printf("\n\n\n");
}

/**************************************
RTC6218N_myabs()
***************************************/
int myabs(int i)
{
    return i < 0 ? -i : i;
}

/**************************************
RTC6218N_FM_Tune_Freq()

channel_freq: 8750~10800
channel_space: 50,100,200
***************************************/
#if 0

#pragma location="FM_SEG_RTC6218"
u8 RTC6218_seek(u16 channel_freq)
{
    u8 RTC6218N_reg_data[32];
    u8 RSSIValue;

    if (channel_freq==960){
      return 0;
    }

    RTC6218_set_freq(channel_freq);

    OperationRTC6218N_write(RTC62XX_Initial_value,14);
    delay_5ms(1);
    OperationRTC6218N_read (RTC6218N_reg_data, 24);	// Readout (BK7_5[15])
    OperationRTC6218N_write(RTC62XX_Initial_value1,14);
    delay_5ms(1);
    if((RTC6218N_reg_data[22]&0x80)!= 0){
        return 0;
    }
    OperationRTC6218N_read (RTC6218N_reg_data, 4);
    RSSIValue = RTC6218N_reg_data[3];
		
    //printf("channel_freq=%d, RSSIValue= %d\n", channel_freq, RSSIValue); //cloud add

    if(RSSIValue > RTC62XX_Initial_value[7]){
        return 1;
    }
    else{
        return 0;
    }
}

#else

/***********************************************************************
//1、RTC6218搜台函数
//2、返回值说明：
//		0表示非真台；
//		1表示所搜频率为真台且下一个搜索频率增加步进为50KHz；
//		2表示(所搜频率+50KHz)为真台且下一个搜索频率增加步进为100KHz；
//3、补充说明：
		返回值分开1和2是为了减少重复台；
***********************************************************************/
#pragma location="TASK_FMPLAY_SEG"
__near_func u8 RTC6218_seek(u16 channel_freq)
{
	bool g_need_st_judge = 0;
    u8 RTC6218N_reg_data[24];
    u8 /*RSSIValue= 0,*/Stereo_flag= 0;
    //int Spike_Count_1=0;//, Spike_Count_2=0, Spike_Count_3=0;
    u16 DC_Count_1=0, DC_Count_2=0, /*DC_Count_3=0,*/ min_DC=0;
	u8 readCnt=10,stereoCount=0;
	u8 shiftindex=0;
    //printf("%d>",channel_freq);
#if 1  //收台时跳过一些干绕频点
    if ((channel_freq==9600  /*9595*/) || (channel_freq==10800 /*10795*/)){
      return 0;
    }
#endif

    if ((channel_freq==9000) ||(channel_freq==9595) || (channel_freq==9600) || (channel_freq == 9615) \
        || (channel_freq == 9620)|| (channel_freq == 9645)||(channel_freq >= 10790))
    {
      //return 0;
        g_need_st_judge = 1;
    }
    else
    {
        g_need_st_judge = 0;
    }


	if(8740 == channel_freq){
		RTC6218_init_bk3_regf_b_stereo_lowTh();
	}
    //printf("\t%d\n", channel_freq);
    RTC6218_set_freq(channel_freq);

    OperationRTC6218N_write(RTC62XX_Initial_value,14);  // change to bk7
    RTC6218N_Delay(10);		//delay_5ms(1);
    OperationRTC6218N_read (RTC6218N_reg_data, 24);	// Readout (BK7_5[15])
    //Spike_Count_1 = RTC6218N_reg_data[20];
    if(RTC6218N_reg_data[22]&0x2!=0){
        DC_Count_1 = (~(((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23])+1) &0x1ff; //2's complement
    }
    else{
        DC_Count_1 = (((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23]); //2's complement
    }




    OperationRTC6218N_write(RTC62XX_Initial_value1,14); // change to bk0

    if((RTC6218N_reg_data[22]&0x80)== 0)
    {
        //printf("%02d\t",real_count+killed_by_mono_count+1);
        //printf("%5d \t",channel_freq);
        //printf("0x%02x\t0x%02x\t", DC_Count_1,Spike_Count_1);

        // tune second station
        //printf("RTC6218_seek set channel_freq = %d+5 first\n", channel_freq);
        RTC6218_set_freq(channel_freq + 5);  // if seek down channel_freq - 5
        OperationRTC6218N_write(RTC62XX_Initial_value,14);  // change to bk7
    	RTC6218N_Delay(10);		//delay_5ms(1);
        OperationRTC6218N_read (RTC6218N_reg_data, 24);	// Readout (BK7_5[15])
        //Spike_Count_2 = RTC6218N_reg_data[20];
        if(RTC6218N_reg_data[22]&0x2!=0){
           DC_Count_2 = (~(((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23])+1) &0x1ff; //2's complement
        }
        else{
           DC_Count_2 = (((RTC6218N_reg_data[22]& 0x1)<<8) | RTC6218N_reg_data[23]); //2's complement
        }
        OperationRTC6218N_write(RTC62XX_Initial_value1,14); // change to bk0

		if((RTC6218N_reg_data[22]&0x80)== 0)
	    //if(myabs(Spike_Count_1-Spike_Count_2)<Spike_Diff_TH || myabs(Spike_Count_2-Spike_Count_3)<Spike_Diff_TH)
        {
            //printf("%5d \t",channel_freq+5);
            //printf("0x%02x\t0x%02x\t", DC_Count_2,Spike_Count_2);
            // select minimun DC

            //min_DC =  ((DC_Count_1<DC_Count_2)&&(DC_Count_1<DC_Count_3))?DC_Count_1:((DC_Count_2<DC_Count_3)?DC_Count_2:DC_Count_3);
			min_DC =  (DC_Count_1<DC_Count_2) ? DC_Count_1 : DC_Count_2;
	
			//printf("min_DC=%x\n",min_DC);

			if(min_DC == DC_Count_1){
              #ifdef SINGLE_SETP_SEEK
			  //printf("RTC6218_seek set channel_freq = %d second,min_DC=%d\n", channel_freq,min_DC);
              RTC6218_set_freq(channel_freq);
			  shiftindex = 0;
              //printf("+5\t");
              #endif
            }
            else if(min_DC == DC_Count_2){
              #ifdef SINGLE_SETP_SEEK
			  //printf("RTC6218_seek set channel_freq = %d+5 second,min_DC=%d\n", channel_freq,min_DC);	
              RTC6218_set_freq(channel_freq+5);
			  shiftindex = 1;
              //printf("+10\t");
              #endif
            }
        }
		else
		{
          //printf("first station got\n");
            //printf("\t\t\t");
            //printf("\t\t");
            //printf("+5\t");
		  RTC6218_set_freq(channel_freq);
		  shiftindex = 0;
		}//end of "if((RTC6218N_reg_data[22]&0x80)== 0)"

    	RTC6218N_Delay(10);		//delay_5ms(1);
    	OperationRTC6218N_read (RTC6218N_reg_data, 4);
    	//RSSIValue = RTC6218N_reg_data[3];
		//return 1+shiftindex;
		
        //printf("%d\n",RSSIValue);



        //if(RSSIValue > g_rssi_th)
        if(g_need_st_judge)
        {
        #if 1
            //delay_5ms(200);
            //delay_5ms(60);
            for(readCnt=10;readCnt>0;readCnt--)//stereo标志位需要300ms才刷新一次，且不太可靠故读10次中有3次则认为是立体声台
            {
                delay_5ms(60);//300ms
                OperationRTC6218N_read (RTC6218N_reg_data, 2);
                //printf("reg0x10= 0x%02x%02x\n",RTC6218N_reg_data[0],RTC6218N_reg_data[1]);
                Stereo_flag = (RTC6218N_reg_data[0]&0x04)>>2;
                //printf("%1d",Stereo_flag);

                stereoCount += ((Stereo_flag) ? 1:0);


                if(stereoCount >= 3)
                {
                    break;
                }
            }
			//printf("|%d\t %d \t",stereoCount,RSSIValue);
            if(stereoCount >= 3)
            {
                real_count++;
                //printf("Real \t\n");

                //printf("real and stereo real_count = %d,stereoCount=%d\n",real_count,stereoCount);
               if(10785 == channel_freq)
               {
                  RTC6218_init_bk3_regf_b();
               }
               // RTC6218_set_vol(15);    //搜索到真台后设为最大音量，程序里让其自动播放一小段时间
               return 1+shiftindex;
            }
            else
            {
                //printf("killed \t \n");

                killed_by_mono_count++;
                //printf("killed couse of mono count++ = %d\n",killed_by_mono_count);
                if(10785 == channel_freq)
                {
                    RTC6218_init_bk3_regf_b();
               	}
               return 0;
            }




        #else
          return 1;
        #endif
        }
        else // else of "if(g_need_st_judge)"
        {
            if(10785 == channel_freq)
            {
                RTC6218_init_bk3_regf_b();
            }
            real_count++;
             //printf("\n");
            return 1+shiftindex;
        }
    }
	else //else of first"if((RTC6218N_reg_data[22]&0x80)== 0)"
	{
        if(10785 == channel_freq){
			RTC6218_init_bk3_regf_b();
        }
		return 0;
	}
}

#pragma location="FM_SEG_RTC6218_1"
u8 RTC6218_seek_fortest(u16 channel_freq)
{
    //u8 RTC6218N_reg_data[24];
    //u8 RSSIValue;
    //int Spike_Count_1=0, Spike_Count_2=0, Spike_Count_3=0;
    //u16 DC_Count_1=0, DC_Count_2=0, DC_Count_3=0, min_DC=0;
	uint8 i=0;

    if ((channel_freq==9595) || (channel_freq==9600) || (channel_freq >= 10790)){
      return 0;
    }

    channel_freq= 9845;
    for(i=0;i<20;i++){
    	delay_5ms(1);
    	RTC6218_set_freq_for_debug_12M_tune(channel_freq);
	}

	channel_freq= 9850;
    for(i=0;i<20;i++){
    	delay_5ms(1);
    	RTC6218_set_freq_for_debug_12M_tune(channel_freq);
	}

	channel_freq= 9200;
    for(i=0;i<20;i++){
    	delay_5ms(1);
    	RTC6218_set_freq_for_debug_12M_tune(channel_freq);
	}

    while(1){
    };
}
#endif

#endif

