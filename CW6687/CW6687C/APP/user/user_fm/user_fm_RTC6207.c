/*--------------------------------------------------------------------------*/
/**@file RTC6207.C
   @brief FM module
   @date 2014-02-18
   @note none
*/
/*----------------------------------------------------------------------------*/
#include "include.h"
#include "RTC6207.h"
#include "user_fm.h"

#if (FM_TYPE == FM_RTC6207)

#define INTERNAL_XTAL_6207
//#define FM_50KHZ_STEP
/*
If using stand alone x'tal or shared x'tal from MCU
the macro "INTERNAL_XTAL_6207" should be defined.

In shared x'tal case, we should add 300pF capacitor on series path in front of RCLK pin
*/

#pragma constseg = "FM_CONST_RTC6207"
const u8 RTC62XX_CKHoppingOn[] =
{
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x20,0x00,
    0x05,0x0F, 0x40,0x00, 0x10,0x40, 0x00,0x11, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0xD0,0x09, 0x7F,0x80, 0x3C,0x01, 0x96,0xC0, 0x01,0x00, 0x00,0x00, 0x01,0x40,
    0x47,0x00, 0x00,0x00
};

const u8 RTC62XX_SWBank2[] =
{
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x80,0x00, 0x80,0x00,
    0x01,0x01, 0xC8,0xC8, 0x07,0x2D, 0x05,0x1C, 0x03,0x0B, 0x03,0x0B, 0x07,0x2D,
    0x00,0x83, 0x7F,0x8A, 0xC8,0x01, 0xC8,0xC8, 0x14,0x00, 0x40,0x00, 0x00,0x01,
    0x32,0x01, 0x00,0x00
};


#ifndef INTERNAL_XTAL_6207

#ifndef FM_50KHZ_STEP
const u8 RTC6207_power_up[] = {0xC0,0x01,0x00,0x00,0x90,0x04,0x0c,0x1f,0x00,0x48,0x01,0x00,0x00,0x00,0x40,0x50};
#else
const u8 RTC6207_power_up[] = {0xC0,0x01,0x00,0x00,0x90,0x04,0x0c,0x2f,0x00,0x48,0x01,0x00,0x00,0x00,0x40,0x50};//space 50
#endif

#else

#ifndef FM_50KHZ_STEP
const u8 RTC6207_power_up[] = {0xC0,0x01,0x00,0x00,0x90,0x04,0x0c,0x1f,0x00,0x48,0x81,0x00,0x00,0x00,0x40,0x50};
#else
const u8 RTC6207_power_up[] = {0xC0,0x01,0x00,0x00,0x90,0x04,0x0c,0x2f,0x00,0x48,0x81,0x00,0x00,0x00,0x40,0x50};//space 50
#endif

#endif

const u8 RTC62XX_power_down[] = {0x00,0x41,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

#ifndef INTERNAL_XTAL_6207
const u8 RTC6207_Initial_value[] = {0xC0,0x01,0x00,0x00,0x90,0x04,0x0c,0x1f,0x00,0x48,0x00,0x00,0xe0,0x00};	// RSSI_TH = 0x0c
#else
const u8 RTC6207_Initial_value[] = {0xC0,0x01,0x00,0x00,0x90,0x04,0x0c,0x1f,0x00,0x48,0x80,0x00,0xe0,0x00};	// RSSI_TH = 0x0c
#endif

const u8 RTC6207_Initial_value1[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00};

#pragma constseg=default


#pragma location="FM_SEG_RTC6207"
void OperationRTC6207_write(const u8 __code *data, u8 numBytes)
									
{
    iic_start();
    iic_sendbyte(0x20);
    iic_getack();
    for(u8 j = 0; j < numBytes; j++)
    {
        iic_sendbyte(data[j]);
        iic_getack();
    }
    delay_5ms(1);
    iic_stop();
}

#pragma location="FM_SEG_RTC6207"
void OperationRTC6207_write_a(u8 *data, u8 numBytes)
{
    iic_start();
    iic_sendbyte(0x20);
    iic_getack();
    for(u8 j = 0; j < numBytes; j++)
    {
        iic_sendbyte(data[j]);
        iic_getack();
    }
    delay_5ms(1);
    iic_stop();
}

#pragma location="FM_SEG_RTC6207"
void OperationRTC6207_read(u8 *data, u8 numBytes)
{
    iic_start();
    iic_sendbyte(0x21);
    iic_getack();
    for(u8 j = 0; j < numBytes; j++)
    {
        data[j] = iic_getbyte();
        if(j== (numBytes-1)){
            iic_sendnack();
        }
        else{
            iic_sendack();
        }
    }
    delay_5ms(1);
    iic_stop();
}

/*************************************************************
*  Radio get ID
*
*************************************************************/
#pragma location="FM_INIT_RTC6207"
u8 RTC6207_online(void)
{
    u8 RTC62XX_reg_data[16];
    for(u8 i=0; i<3; i++){      //IIC与其他引脚复用时 FM ID 改为读三次比较保险
        OperationRTC6207_read(RTC62XX_reg_data, 16);
        //printf("ID1: 0x%02x , ID2: 0x%02x\n",RTC62XX_reg_data[12],RTC62XX_reg_data[13]);
        if (RTC62XX_reg_data[12] == 0x16 && RTC62XX_reg_data[13]== 0x88){
            RTC6207_init();
            return 1;
        }
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/**@brief FM Module RTC6207 Initialize
   @param  none
   @return  none	
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_INIT_RTC6207"
void RTC6207_init(void)
{
	OperationRTC6207_write(RTC62XX_CKHoppingOn, 46);
    delay_5ms(10);
	OperationRTC6207_write(RTC62XX_SWBank2, 46);
	delay_5ms(10);
	OperationRTC6207_write(RTC6207_power_up, 16);
	delay_5ms(10);
}

/*----------------------------------------------------------------------------*/
/**@brief FM Module set frequency
   @param frequency
   @return none
   @note Range 875~1080
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_SEG_RTC6207"
void RTC6207_set_freq(u16 channel_freq)
{
    //channel_freq=channel_freq*10;
    u16 freq_reg_data, loop_counter = 0;
    u8 RTC6207_reg_data[32];
    u8 RTC6207_channel_start_tune[] = {0xC0,0x01,0x80,0xCA};	//107.7MHz
    u8 RTC6207_channel_stop_tune[] = {0xC0,0x01,0x00,0xCA};	
    u16 start_freq = 8750;

    //printf("channel_freq = %d\n", channel_freq);
    //set tune bit

    for(u8 i = 0; i < 7; i++)
    {
        RTC6207_reg_data[i] = RTC6207_power_up[i];
    }
    if(channel_freq >= 8750)
    {
        delay_5ms(10);
        #ifndef FM_50KHZ_STEP
        RTC6207_reg_data[7] = 0x1f;
        #else
        RTC6207_reg_data[7] = 0x2f;
        #endif
	    OperationRTC6207_write_a(RTC6207_reg_data, 8);
        start_freq = 8750;
    }
    else
    {
        delay_5ms(10);
        #ifndef FM_50KHZ_STEP
        RTC6207_reg_data[7] = 0x9f;
        #else
        RTC6207_reg_data[7] = 0xaf;
        #endif
        OperationRTC6207_write_a(RTC6207_reg_data, 8);
        start_freq = 7600;
    }

#ifndef FM_50KHZ_STEP
    freq_reg_data = (channel_freq - start_freq)/10;
#else
    freq_reg_data = (channel_freq - start_freq)/5;
#endif

    RTC6207_channel_start_tune[3] = freq_reg_data & 0xff;
    RTC6207_channel_start_tune[2] = (RTC6207_channel_start_tune[2] & 0xfc) | (freq_reg_data >> 8);
    RTC6207_channel_stop_tune[3] = RTC6207_channel_start_tune[3] ;

    OperationRTC6207_write_a(RTC6207_channel_start_tune, 4);
    do
    {
        OperationRTC6207_read(RTC6207_reg_data, 2);
        loop_counter++;
    }while(((RTC6207_reg_data[0]&0x40) == 0) && (loop_counter < 0xffff));		//for loop_counter, when tune, the loop time must > 60ms

    loop_counter = 0;
    OperationRTC6207_write_a(RTC6207_channel_stop_tune, 4);
    //wait STC=0
    do
    {
        OperationRTC6207_read(RTC6207_reg_data, 2);
        loop_counter++;
    }while(((RTC6207_reg_data[0]&0x40) != 0) && (loop_counter < 0xff));
}

/*----------------------------------------------------------------------------*/
/**@brief FM Module RTC6207 set volume
   @param volumn
   @return none
   @note   Range 0~15
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_SEG_RTC6207"
void RTC6207_set_vol(u8 FM_Volumn)
{
	// Some FM IC has 17 level, RTC serise only level 16
    if (FM_Volumn>0){
        FM_Volumn--;
    }

#ifndef FM_50KHZ_STEP
    u8 RTC62XX_set_property[] = {0xC0,0x01,0x00,0x00,0x90,0x04,0x0c,0x1f};	//RSSI threshold = 0x0C = 12dB
#else
    u8 RTC62XX_set_property[] = {0xC0,0x01,0x00,0x00,0x90,0x04,0x0c,0x2f};	//step 50K
#endif

    RTC62XX_set_property[7] = (RTC62XX_set_property[7] & 0xf0) | FM_Volumn;
    OperationRTC6207_write_a(RTC62XX_set_property, 8);
}

/*----------------------------------------------------------------------------*/
/**@brief FM Module RTC6207 off
   @param none
   @return none
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_INIT_RTC6207"
void RTC6207_off(void)
{
	OperationRTC6207_write(RTC62XX_power_down, 12);
    delay_5ms(4);
}

/*----------------------------------------------------------------------------*/
/**@brief FM Module RTC6207 Auto Search
   @param Frequency which you want to check is valid or not
   @return If there is a station return 1 o.w., return 0.
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_SEG_RTC6207"
u8 RTC6207_seek(u16 channel_freq)
{
    RTC6207_set_freq(channel_freq);
    //u8 RSSIValue;
    u8 RTC6207_reg_data[32];
	
	OperationRTC6207_write(RTC6207_Initial_value, 14);
    delay_5ms(1);
    OperationRTC6207_read(RTC6207_reg_data, 24); // Readout (BK7_5[15])
	OperationRTC6207_write(RTC6207_Initial_value1, 14);
    delay_5ms(1);

    if((RTC6207_reg_data[22]&0x80)!= 0){
        return 0;
    }

    OperationRTC6207_read(RTC6207_reg_data, 2);
    //RSSIValue = RTC6207_reg_data[1];
    //if(RSSIValue > RTC6207_Initial_value[6])
    //{
        //printf("RSSIValue=%d\n",RSSIValue);
        //printf("channel_freq=%d\n",channel_freq);
        return 1;
    //}
    //else
        //return 0;
}

#endif
