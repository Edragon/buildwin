/*--------------------------------------------------------------------------*/
/**@file QN8035.C
   @brief FM模块QN8035驱动
   @details FM模块QN8035的驱动函数
   @author LZZ
   @date 2010-08-17
   @note 通过宏定义来选择驱动方式
*/
/*----------------------------------------------------------------------------*/


#include "include.h"
#include "user_fm.h"
#include "qn8035.h"

#if (FM_TYPE == FM_QN8035)

#define _USE_P33_DIGITAL_32KHz_         1   //P33输出32K方波时钟给FM OSC
#define _SHARE_CRYSTAL_32KHz_           2
#define _CRYSTAL_12MHz_                 3   //独立12M晶振或共12M晶振
#define _SHARE_CRYSTAL_24MHz_           4   //KT 没有模块
#define _CRYSTAL_32KHz_                 5   //独立32K晶振

#if(FM_CLK_SEL == P33_OUT_32K)
    #define FM_CLOCK  _USE_P33_DIGITAL_32KHz_
#elif (FM_CLK_SEL == P33_OUT_12M)
    #define FM_CLOCK  _CRYSTAL_12MHz_
#else
    #define FM_CLOCK _SHARE_CRYSTAL_32KHz_
#endif



void QND_WriteReg(u8 adr, u8 value);
u8 QND_ReadReg(u8 adr);
void QND_RXConfigAudio(u8 option) ;
void QND_RXSetTH(u8 th);
bool QND_RXValidCH(u16 freq);


#pragma location="FM_SEG_QN8035"
u16 FREQ2CHREG(u16 freq)
{
    return 	(freq-6000)/5;
}
#pragma location="FM_SEG_QN8035"
void QNF_SetMute(u8 On)
{
    QND_WriteReg(REG_DAC, On?0x1B:0x10);
}
/*#pragma location="FM_SEG_QN8035"
void qn8035_mute(void)
{
  // QNF_SetRegBit(0x14, 0x80, 0x80);  //mute
    QNF_SetMute(1);
}*/
#pragma location="FM_SEG_QN8035"
void qn8035_unmute(void)
{
    delay_5ms(5);
    //QNF_SetRegBit(0x14, 0x80, 0x00); //unmute
    QNF_SetMute(0);
}
/**********************************************************************
u8 QNF_SetCh(u16 freq)
**********************************************************************
Description: set channel frequency

Parameters:
    freq:  channel frequency to be set
Return Value:
    1: success
**********************************************************************/	
#if USING_VALID_CH
#pragma location="FM_SEG_QN8035"
void QNF_SetCh(u16 freq)
{
    u8 temp;

	 freq = FREQ2CHREG(freq);
	//writing lower 8 bits of CCA channel start index
	QND_WriteReg(CH_START, (u8)freq);
	//writing lower 8 bits of CCA channel stop index
	QND_WriteReg(CH_STOP, (u8)freq);
	//writing lower 8 bits of channel index
	QND_WriteReg(CH, (u8)freq);
	//writing higher bits of CCA channel start,stop and step index
	temp = (u8) ((freq >> 8) & CH_CH);
	temp |= ((u8)(freq >> 6) & CH_CH_START);
	temp |= ((u8) (freq >> 4) & CH_CH_STOP);
	temp |= QND_STEP_CONSTANT;//(step << 6);
	QND_WriteReg(CH_STEP, temp);
}
#else
#pragma location="FM_SEG_QN8035"
void QNF_SetCh(u16 start,u16 stop) @"QN8035_RX"
{
    u8 temp;

	 start = FREQ2CHREG(start);
	 stop = FREQ2CHREG(stop);
	//writing lower 8 bits of CCA channel start index
	QND_WriteReg(CH_START, (u8)start);
	//writing lower 8 bits of CCA channel stop index
	QND_WriteReg(CH_STOP, (u8)stop);
	//writing lower 8 bits of channel index
	QND_WriteReg(CH, (u8)start);
	//writing higher bits of CCA channel start,stop and step index
	temp = (u8) ((start >> 8) & CH_CH);
	temp |= ((u8)(start >> 6) & CH_CH_START);
	temp |= ((u8) (stop >> 4) & CH_CH_STOP);
	temp |= QND_STEP_CONSTANT;//(step << 6);
	QND_WriteReg(CH_STEP, temp);
}
#endif

/**********************************************************************
void QND_Init()
**********************************************************************
Description: Initialize device to make it ready to have all functionality ready for use.

Parameters:
    None
Return Value:
    1: Device is ready to use.
    0: Device is not ready to serve function.
**********************************************************************/
#pragma location="FM_INIT_QN8035"
void fm_qn8035chip_init(void)
{
    QND_WriteReg(0x00, 0x81);
    delay_5ms(1);

// change crystal frequency setting here
#if (FM_CLOCK == _USE_P33_DIGITAL_32KHz_)
    QND_WriteReg(0x01,QND_DIGITAL_CLOCK);//   方波用这个
#elif (FM_CLOCK == _SHARE_CRYSTAL_32KHz_)
    QND_WriteReg(0x01,QND_SINE_WAVE_CLOCK);//   正弦波用这个
#elif (FM_CLOCK == _CRYSTAL_12MHz_) //独立12M晶振或共12M晶振
    QND_WriteReg(0x58, 0x93);   //设为外挂晶体模式
    delay_5ms(25);              //如果不起振延迟 125 ms 或 750ms

    //使用独立晶振时，若晶振不起振，则需要在此延时2S后再进行下面的初始化
    //delay_5ms(200);WATCHDOG_CLR();delay_5ms(200);

    QND_WriteReg(0x15, 0x6e);   //12M
    QND_WriteReg(0x16, 0x01);   //时钟
    QND_WriteReg(0x17, 0x54);   //时钟
#elif (FM_CLOCK == _CRYSTAL_32KHz_) //独立32.768K晶振
    QND_WriteReg(0x58, 0x93);//mod PLL setting
    delay_5ms(25);

    //使用独立晶振时，若晶振不起振，则需要在此延时2S后再进行下面的初始化
    //delay_5ms(200);WATCHDOG_CLR();delay_5ms(200);WATCHDOG_CLR();delay_5ms(200);

    QND_WriteReg(XTAL_DIV0, QND_XTAL_DIV0);//01
    QND_WriteReg(XTAL_DIV1, QND_XTAL_DIV1);//08
    QND_WriteReg(XTAL_DIV2, QND_XTAL_DIV2);//5c
#elif (FM_CLOCK==_SHARE_CRYSTAL_24MHz_)//24M共晶振
	QND_WriteReg(0x15, 0xDC);   //24M
	QND_WriteReg(0x16, 0x02);   //时钟
	QND_WriteReg(0x17, 0x54);   // 时钟
#endif
    //QND_WriteReg(XTAL_DIV0, QND_XTAL_DIV0);
    //QND_WriteReg(XTAL_DIV1, QND_XTAL_DIV1);
    //QND_WriteReg(XTAL_DIV2, QND_XTAL_DIV2);
    QND_WriteReg(0x54, 0x47);//mod PLL setting
    //select SNR as filter3,SM step is 2db
    QND_WriteReg(0x19, 0xC6);
    QND_WriteReg(0x2d, 0xD6);//notch filter threshold adjusting
    QND_WriteReg(0x43, 0x10);//notch filter threshold enable
    QND_WriteReg(0x47,0x39); 	
    //enter receiver mode directly
    QND_WriteReg(0x00, 0x11);
    //Enable the channel condition filter3 adaptation,Let ccfilter3 adjust freely
    QND_WriteReg(0x1D,0xA9);
    QNF_SetMute(1);
}





/**********************************************************************
void qn8035_set_freq(u16 ch)
**********************************************************************
Description: Tune to the specific channel. call QND_SetSysMode() before
call this function
Parameters:
ch
Set the frequency (10kHz) to be tuned,
eg: 101.30MHz will be set to 10130.
Return Value:
None
**********************************************************************/
#pragma location="FM_SEG_QN8035"
void qn8035_set_freq(u16 ch)
{
    u8 reg;

    //ch = ch * 10; caiyue
    //increase reference PLL charge pump current.
    QND_WriteReg(REG_REF,0x7A);
	
    /********** QNF_RXInit ****************/
    QND_WriteReg(0x1B,0x70);  //Let NFILT adjust freely
    //QNF_SetRegBit(0x2C,0x3F,0x12);  ///When SNR<ccth31, ccfilt3 will work
    //setting the threshold of Filter3 will be worked.
    QND_WriteReg(0x2C,0x52);	
    //QNF_SetRegBit(0x1D,0x40,0x00);  ///Let ccfilter3 adjust freely
    //QNF_SetRegBit(0x41,0x0F,0x0A);  ///Set a hcc index to trig ccfilter3's adjust
    QND_WriteReg(0x45,0x50);        ///Set aud_thrd will affect ccfilter3's tap number
    //QNF_SetRegBit(0x40,0x70,0x70);  ///snc/hcc/sm snr_rssi_sel; snc_start=0x40; hcc_start=0x30; sm_start=0x20
    QND_WriteReg(0x40,0x70);
    //QNF_SetRegBit(0x19,0x80,0x80);  ///Use SNR for ccfilter selection criterion
    //selecting SNR as filter3 filter condition
    //QND_WriteReg(0x19, 0xC2);
    //QNF_SetRegBit(0x3E,0x80,0x80);  ///it is decided by programming this register
    //QNF_SetRegBit(0x41,0xE0,0xC0);  ///DC notching High pass filter bandwidth; remove low freqency dc signals
    QND_WriteReg(0x41,0xCA);
    QND_WriteReg(0x34,SMSTART_VAL); ///set SMSTART
    QND_WriteReg(0x35,SNCSTART_VAL); ///set SNCSTART
    QND_WriteReg(0x36,HCCSTART_VAL); ///set HCCSTART
    /********** End of QNF_RXInit ****************/
	
    QNF_SetMute(1);
#if USING_VALID_CH
    QNF_SetCh(ch);
#else
    QNF_SetCh(ch,ch);
#endif
    //enable CCA mode with user write into frequency
    QND_WriteReg(0x00, 0x13);
    //Auto tuning
    QND_WriteReg(0x4F, 0x80);
    reg = QND_ReadReg(0x4F);
    reg >>= 1;
    QND_WriteReg(0x4F, reg);
    ///avoid the "POP" noise.
    delay_5ms(CH_SETUP_DELAY_TIME/50);  //300ms
    ///decrease reference PLL charge pump current.
    QND_WriteReg(REG_REF,0x70);
    QNF_SetMute(0);
}

/***********************************************************************
void QND_RXSetTH(u8 th)
***********************************************************************
Description: Setting the threshold value of automatic scan channel
th:
  Setting threshold for quality of channel to be searched,
  the range of th value:CCA_SENSITIVITY_LEVEL_0 ~ CCA_SENSITIVITY_LEVEL_9
Return Value:
  None
***********************************************************************/
#pragma location="FM_SEG_QN8035"
void QND_RXSetTH(u8 th)
{
    ///increase reference PLL charge pump current.
    QND_WriteReg(REG_REF,0x7A);
    //NFILT program is enabled
    QND_WriteReg(0x1B,0x78);
    //using Filter3
    QND_WriteReg(CCA1,0x75);
    //setting CCA IF counter error range value(256).
   // QND_WriteReg(CCA_CNT2,0x01);
     QND_WriteReg(CCA_CNT2,0x03);//7.25更新
#if PILOT_CCA
    QND_WriteReg(PLT1,0x00);
#endif
	//selection the time of CCA FSM wait SNR calculator to settle:20ms
	//0x00:	    20ms(default)
	//0x40:	    40ms
	//0x80:	    60ms
	//0xC0:	    100m
	//    QNF_SetRegBit(CCA_SNR_TH_1 , 0xC0, 0x00);
    //selection the time of CCA FSM wait RF front end and AGC to settle:20ms
    //0x00:     10ms
	//0x40:     20ms(default)
    //0x80:     40ms
	//0xC0:     60ms
	//    QNF_SetRegBit(CCA_SNR_TH_2, 0xC0, 0x40);
	//    QNF_SetRegBit(CCA, 30);
       //setting CCA RSSI threshold is 30
	//QND_WriteReg(CCA,QND_ReadReg(CCA)&0xC0|25);  // 20 ~ 30
        QND_WriteReg(CCA,QND_ReadReg(CCA)&0xC0|20);  // 20 ~ 30
#if PILOT_CCA	
	QND_WriteReg(CCA_SNR_TH_1,8+th); //setting SNR threshold for CCA
#else
	QND_WriteReg(CCA_SNR_TH_1,9+th); //setting SNR threshold for CCA	 8 ~ 10
#endif	
}

/***********************************************************************
u16 QND_RXValidCH(u16 freq, u8 step);
***********************************************************************
Description: to validate a ch (frequency)(if it's a valid channel)
Freq: specific channel frequency, unit: 10Khz
  eg: 108.00MHz will be set to 10800.
Step:
  FM:
  QND_FMSTEP_100KHZ: set leap step to 100kHz
  QND_FMSTEP_200KHZ: set leap step to 200kHz
  QND_FMSTEP_50KHZ:  set leap step to 50kHz
Return Value:
  0: not a valid channel
  other: a valid channel at this frequency
***********************************************************************/
#pragma location="FM_SEG_QN8035"
bool QND_RXValidCH(u16 freq)
{
 	u8 regValue;
  	u8 timeOut;
  	u8 isValidChannelFlag;

#if PILOT_CCA
  	u8 snr,readCnt,stereoCount=0;
#endif
#if USING_VALID_CH
  	QNF_SetCh(freq);
#else
  	QNF_SetCh(freq,freq);
#endif
  	//QNF_SetRegBit(SYSTEM1,0x03,0x02);//enter channel scan mode,channel frequency is decided by internal CCA
  	//entering into RX mode and CCA mode,channels index decide by CCA.
  	QND_WriteReg(0x00, 0x12);
  	timeOut = 20;
  	do
  	{
    	regValue = QND_ReadReg(SYSTEM1);
    	delay_5ms(1);   //delay 5ms
    	timeOut--;
  	}
  	//if it seeks a potential channel or time out,the loop will be quited
  	while((regValue & CHSC) && timeOut);
  	//TRACE("CHSC:%d,timeOut:%d \n",regValue&CHSC,timeOut);
  	//reading out the rxcca_fail flag of RXCCA status
  	isValidChannelFlag = (QND_ReadReg(STATUS1) & RXCCA_FAIL ? 0:1) && timeOut;	

      //  qn8035_unmute();
  	if(isValidChannelFlag)
  	{
#if PILOT_CCA
    	QND_Delay(20);
    	snr = QND_ReadReg(SNR);
    	if(snr> 25) return 1;		
    	for(readCnt=10;readCnt>0;readCnt--)
    	{
      		QND_Delay(1);
      		stereoCount += ((QND_ReadReg(STATUS1) & ST_MO_RX) ? 0:1); 	
      		if(stereoCount >= 3) return 1;
    	} 	
#else
        qn8035_unmute();
    	return 1;
#endif
  	}
  	return 0;
}	



////////////////////////////////////////////
#pragma location="FM_SEG_QN8035"
u8 QND_ReadReg(u8 adr)
{
	//u16 QNData;
	// to be filled by user API
   	u8  I2CReadResult;

    iic_start();                    //I2C启动

    iic_sendbyte(0x20);             //写命令
    iic_getack();

    iic_sendbyte(adr);       //写地址
    iic_getack();

    iic_start();                    //写转为读命令，需要再次启动I2C

    iic_sendbyte(0x21);             //读命令
    iic_getack();

    I2CReadResult = iic_getbyte();
    iic_sendnack();                      //发送ack命令

    iic_stop();                     //I2C停止
    delay_5ms(1);

    return  I2CReadResult;
	//return (u8)QNData;
}
#pragma location="FM_SEG_QN8035"
void QND_WriteReg(u8 adr, u8 value)
{
  // to be filled by user API
    iic_start();                //I2C启动

    iic_sendbyte(0x20);         //写命令
    iic_getack();


    iic_sendbyte(adr);   //写地址
    iic_getack();



    iic_sendbyte(value);      //写数据
    iic_getack();

    iic_stop();                 //I2C停止时序
    delay_5ms(1);
}

#pragma location="FM_SEG_QN8035"
bool fm_qn8035_set_freq(u16 freq)
{
    QNF_SetMute(1);
    QND_RXSetTH(0);
    return QND_RXValidCH(freq);// caiyue QND_RXValidCH(freq*10);//qn8035 step frequency unit is 10KHZ
}



/*----------------------------------------------------------------------------*/
/**@brief FM模块QN8035检测
   @param 无
   @return 检测到QN8035模块返回1,并初始化fm芯片，否则返回0
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_INIT_QN8035"
bool fm_qn8035_init(void)
{
    u8 temp;
    //QND_WriteReg(0x00, 0x81);
    for(u8 i=0; i<3; i++){      //IIC与其他引脚复用时 FM ID 改为读三次比较保险
        temp = QND_ReadReg(0x06) & 0xFC;
        //printf("id:%02x\n",temp);
        if (temp == 0x84 ){
            fm_qn8035chip_init();
            return 1;
        }
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块QN8035关闭
   @param 无
   @return 无
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_INIT_QN8035"
void fm_qn8035_off(void)
{
    QND_WriteReg(0x00, 0x21);
    delay_5ms(1);
}

/************************************************************************
void QND_RXConfigAudio(u8 optiontype, u8 option )
*************************************************************************
Description: config audio
Parameters:
  optiontype: option
    QND_CONFIG_MONO; ‘option’control mono, 0: stereo receive mode ,1: mono receiver mode
    QND_CONFIG_MUTE; ‘option’control mute, 0:mute disable,1:mute enable
    QND_CONFIG_VOLUME: 'option' control the volume gain,range : 0~47(-47db~0db)

Return Value:
    none
**********************************************************************/
#pragma location="FM_SEG_QN8035"
void QND_RXConfigAudio(u8 option )// @"QN8035_RX"
{
    u8 regVal;
    regVal = QND_ReadReg(VOL_CTL);
	regVal = (regVal&0xC0)|(option/6)|(5-option%6<<3);
	QND_WriteReg(VOL_CTL,regVal);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块QN8035设置音量
   @param volume 音量值
   @return 无
   @note 音量范围0~15
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_SEG_QN8035"
void qn8035_set_vol(u8 volume)//@"QN8035_RX"
{

    volume = volume*3;
    if(volume>47){
        volume = 47;//qn8035 volume range is from 0~47
    }
    QND_RXConfigAudio(volume);
}

#endif
