/*--------------------------------------------------------------------------*/
/**@file QN8065.C
   @brief FM模块QN8065驱动
   @details FM模块QN8065的驱动函数
   @author NC
   @date 2013-10-23
   @note 通过宏定义来选择驱动方式
*/
/*----------------------------------------------------------------------------*/

#include "include.h"
#include "user_fm.h"
#include "qn8065.h"

#if (FM_TYPE == FM_QN8065)

//if using pilot as CCA algorithm,macro QN8065_PILOT_CCA will be set to 1
#define QN8065_PILOT_CCA					1

UINT8 QN8065_PreNoiseFloor = 40,QN8065_NoiseFloor = 40;
extern void delay_5ms(u8 cnt);

#pragma constseg="FM_SEG_QN8065_CONST"
#if 0
const u8 volume_8065table[33] = {0,2,4,6,8,10,12,13,14,15,16,17,18,19,20,21,22,
                                 23,24,25,26,27,28,29,30,32,34,36,38,41,43,45,47};  //32级音量
#else
const u8 volume_8065table[16] = {0,5,10,15,17,20,25,28,30,33,35,37,40,43,45,47};    //16级音量
#endif
#pragma constseg=default


#pragma location="FM_SEG_QN8065"
void QN8065_Delay(u16 n)
{
    while (n--)
        asm("nop");
}

#pragma location="FM_SEG_QN8065_1"
UINT16 QN8065_FREQ2CHREG(UINT16 freq)
{
    return 	(freq-6000)/5;
}

/**********************************************************************
void QN8065_SetMute(UINT8 On)
**********************************************************************
Description: on or off mute chip
Parameters:
    On: 1:on mute chip;0:off mute chip
Return Value:
    None
**********************************************************************/	
#pragma location="FM_SEG_QN8065_1"
void QN8065_SetMute(UINT8 On)
{
	//mute or unmute by holding DAC
    //and set low voltage regulator output as 0.9v
    QN8065_WriteReg(QN8065_REG_REF, On?0x14:0x10);
}

/**********************************************************************
void QN8065_SetCh(UINT16 start,UINT16 stop,UINT8 step)
**********************************************************************
Description: set channel frequency
Parameters:
    freq:  channel frequency to be set,frequency unit is 10KHZ
Return Value:
    None
**********************************************************************/	
#pragma location="FM_SEG_QN8065_1"
void QN8065_SetCh(UINT16 start,UINT16 stop,UINT8 step)
{
    UINT8 temp;
    //TRACE("start:%d,stop:%d,step:%d \n",start,stop, step);
    start = QN8065_FREQ2CHREG(start);
    //fored the stop is bigger than start
    stop = QN8065_FREQ2CHREG(stop);
	//writing higher bits of channel frequency,CCA channel start,
	//stop and step index at first
	temp = (UINT8) ((start >> 8) & QN8065_CH_CH);
	temp |= ((UINT8)(start >> 6) & QN8065_CH_CH_START);
	temp |= ((UINT8) (stop >> 4) & QN8065_CH_CH_STOP);
	temp |= (step << 6);
	QN8065_WriteReg(QN8065_CH_STEP, temp);
	//writing lower 8 bits of CCA channel start index
	QN8065_WriteReg(QN8065_CH_START, (UINT8)start);
	//writing lower 8 bits of CCA channel stop index
	QN8065_WriteReg(QN8065_CH_STOP, (UINT8)stop);
	//writing lower 8 bits of channel index
	QN8065_WriteReg(QN8065_CH, (UINT8)start);
}

/**********************************************************************
void QN8065_Init()
**********************************************************************
Description: Initialize device to make it ready to have all functionality ready for use.
Parameters:
    None
Return Value:
    None
**********************************************************************/
#pragma location="FM_SEG_QN8065"
void QN8065_Init(void)
{
    QN8065_WriteReg(QN8065_SYSTEM1, 0x81);
    delay_5ms(10);

    /*********User sets chip working clock **********/
    //Following is where change the input clock wave type,as sine-wave or square-wave.

#if FMOSC_REUSE_SDCLK
    QN8065_WriteReg(QN8065_CCA,QN8065_DIGITAL_CLOCK);
#else
    QN8065_WriteReg(QN8065_CCA,QN8065_SINE_WAVE_CLOCK );
#endif
    //Following is where change the input clock frequency.

    QN8065_WriteReg(QN8065_XTAL_DIV0, QN8065_XTAL_DIV0_VAL);
    QN8065_WriteReg(QN8065_XTAL_DIV1, QN8065_XTAL_DIV1_VAL);
    QN8065_WriteReg(QN8065_XTAL_DIV2, QN8065_XTAL_DIV2_VAL);

    delay_5ms(10);

    /********User sets chip working clock end ********/
    QN8065_WriteReg(QN8065_CCA1, 0x96);
    QN8065_WriteReg(QN8065_CCOND1, 0xb6);
    ///select SNR for SM,SNC,HCC MPX
    QN8065_WriteReg(QN8065_SMSTART,0x80|QN8065_SMSTART_VAL); //set SMSTART
    QN8065_WriteReg(QN8065_SNCSTART,0x80|QN8065_SNCSTART_VAL); //set SNCSTART
    QN8065_WriteReg(QN8065_HCCSTART,0x80|QN8065_HCCSTART_VAL); //set HCCSTART
    //enter receiver mode directly
    QN8065_WriteReg(QN8065_SYSTEM1, 0x11);	
    QN8065_SetMute(1);
}

/**********************************************************************
void QN8065_TuneToCH(UINT16 ch)
**********************************************************************
Description: Tune to the specific channel.
Parameters:
	ch:Set the frequency (10kHz) to be tuned,
eg: 101.30MHz will be set to 10130.
Return Value:
	None
**********************************************************************/
#pragma location="FM_SEG_QN8065"
void QN8065_TuneToCH(UINT16 ch)
{
	UINT8 sysClk32kFlag = 0;
	UINT8 stereoFlag = 0;
	UINT8 reg;
    //ch = ch *10;
    QN8065_SetMute(1);
	if (QN8065_ReadReg(QN8065_XTAL_DIV0) == 0x01)
	{
		sysClk32kFlag = 1;
		stereoFlag = (QN8065_ReadReg(QN8065_STATUS1) & QN8065_ST_MO_RX) ? 0:1;
	}
#if IMPROVING_SNR
	//improving some special stations snr when working clock is 32768Hz
	if (ch == 10030 || ch == 10090 || ch == 10100 || ch == 10110
		|| ch == 10150 || ch == 10160 || ch == 10220)
	{
		//frequency is added 50k only system clock is 32K.
		if (sysClk32kFlag)
		{
			QN8065_WriteReg(QN8065_XTAL_DIV2, 0x67);
			//add 50K
			ch += 5;
		}
	}
		//restore XTAL_DIV2 right value if the station is not a special station
	else
	{
		QN8065_WriteReg(QN8065_XTAL_DIV2, QN8065_XTAL_DIV2_VAL);
	}
#endif
    QN8065_WriteReg(QN8065_REG_FLL,0x84); //enable 2k speed
    //enter idle mode and disable CCA, with user write into frequency
    QN8065_WriteReg(QN8065_SYSTEM1, 0x01);
    //step is 100K.
    QN8065_SetCh(ch, ch, 1);
    //enter rx mode and disable CCA, with user write into frequency
    QN8065_WriteReg(QN8065_SYSTEM1, 0x11);
    QN8065_Delay(QN8065_CH_SETUP_DELAY_TIME);
    QN8065_WriteReg(QN8065_REG_FLL, 0x04);//disable 2k speed
	if (sysClk32kFlag)
	{
		reg = QN8065_ReadReg(QN8065_CCA1);
		//disable bypass 32k spur rejection filter if is stereo
		if (stereoFlag)
		{
			reg &= ~0x20; 			
		}
		//enable bypass 32k spur rejection filter if is mono
		else
		{
			reg |= 0x20;
		}
		QN8065_WriteReg(QN8065_CCA1, reg);
	}	
    QN8065_SetMute(0);
}

#if QN8065_SCAN_NOISE_FLOOR_CCA
 /***********************************************************************
Description: scan a noise floor from 87.5M to 108M by step 200K
Parameters:
Return Value:
 1: scan a noise floor successfully.
 0: chip would not normally work.
**********************************************************************/
#pragma location="FM_SEG_QN8065"
UINT8 QN8065_ScanNoiseFloor(UINT16 start,UINT16 stop)
{
    UINT8 regValue;
    UINT8 timeOut = 255; //time out is 2.55S

    QN8065_WriteReg(QN8065_CCA_TIME,0x00);
    //config CCS from start frequency to  stop frequency by step 200KHZ	
    QN8065_SetCh(start,stop,2);
    //enable CCS
    QN8065_WriteReg(QN8065_CCA2, 0x95);
    //enter CCA mode,channel index is decided by internal CCA
    QN8065_WriteReg(QN8065_SYSTEM1,0x12);	
    while(1)
    {
        regValue = QN8065_ReadReg(QN8065_SYSTEM1);
        //if it seeks a potential channel, the loop will be quited
        if((regValue & QN8065_CHSC) == 0) break;
        delay_5ms(10);   //delay 10ms
        //if it was time out,chip would not normally work.
        if((timeOut--) == 0)
        {
            //disable CCS while exit
            QN8065_WriteReg(QN8065_CCA2,0x85);
            return 0;
        }
    }	
    //disable CCS while exit
    QN8065_WriteReg(QN8065_CCA2,0x85);
    QN8065_NoiseFloor = QN8065_ReadReg(QN8065_CCS_NOISE)&0x7f;	
    if(((QN8065_PreNoiseFloor-QN8065_NoiseFloor) > 2) ||((QN8065_NoiseFloor-QN8065_PreNoiseFloor) > 2))
    {
        QN8065_PreNoiseFloor = QN8065_NoiseFloor;
    }
    //TRACE("%d-%d noiseFloor:%d,timeElapsed:%dms\n",start,stop,QN8065_NoiseFloor,(255-timeOut)*10);
    return 1;
}
#endif

/***********************************************************************
void QN8065_RXSetTH(void)
***********************************************************************
Description: Setting the threshold value of automatic scan channel
	
Return Value:
  	None
***********************************************************************/
#pragma location="FM_SEG_QN8065"
void QN8065_RXSetTH(void)
{	
#if IMPROVING_SNR
    UINT8 rssiTH;
    rssiTH = (QN8065_PreNoiseFloor-28);//(QN8065_PreNoiseFloor-28)+3;
    //restore XTAL_DIV2 right value if the station is not a special station
    QN8065_WriteReg(QN8065_XTAL_DIV2, QN8065_XTAL_DIV2_VAL);
#endif
    //set IF counter error range value is 256 = 4*64
    QN8065_WriteReg(QN8065_CCA_CNT, 8);//4
    //set CCA time
    QN8065_WriteReg(QN8065_CCA_TIME, QN8065_CCA_NAGC_40MS|QN8065_CCA_NSNR_20MS|QN8065_CCA_NRSSI_10MS);
#if IMPROVING_SNR
    //set RSSI threshold for CCA
    QN8065_WriteReg(QN8065_CCA, QN8065_ReadReg(QN8065_CCA)&~0x3f|rssiTH);
#else
    //set RSSI threshold for CCA
    QN8065_WriteReg(QN8065_CCA, QN8065_ReadReg(QN8065_CCA)&~0x3f|1); //rssiTH);
#endif
    //set QN8065_SNR threshold for CCA	
#if QN8065_PILOT_CCA
	QN8065_WriteReg(QN8065_CCA_SNR_TH, QN8065_ReadReg(QN8065_CCA_SNR_TH)&~0x3f|8);  //pilot CCA the QN8065_SNR thershold is 8
#else
	QN8065_WriteReg(QN8065_CCA_SNR_TH, QN8065_ReadReg(QN8065_CCA_SNR_TH)&~0x3f|9); 	//normall CCA the QN8065_SNR thereshold is 9
#endif
	//TRACE("rssiTH=%d, snrTH=%d\n", rssiTH,snrTH);	
}

/***********************************************************************
INT8 QN8065_RXValidCH(UINT16 freq);
***********************************************************************
Description: to validate a ch (frequency)(if it's a valid channel)
Freq: specific channel frequency, unit: 10Khz
  eg: 108.00MHz will be set to 10800.
Return Value:
  0: it's not a valid channel
  1: it's a valid channel
 -1: chip does not normally work.
***********************************************************************/
#pragma constseg="FM_SEG_QN8065_CONST"
IAR_CONST u16 qn8065_start[] =
{
    8750,  8810,  9010,  9210,
    9410,  9610,  9810,  10010,
    10210, 10410, 10610
};
IAR_CONST u16 qn8065_stop[] =
{
    8800,  9000,  9200,  9400,
    9600,  9800,  10000, 10200,
	10400, 10600, 10800
};
#pragma constseg=default


#pragma location="FM_SEG_QN8065"
u8 QN8065_RXValidCH(u16 freq)
{
    UINT8 regValue;
    UINT8 timeOut;
    UINT8 isValidChannelFlag;
#if QN8065_PILOT_CCA
    UINT8 snr,readCnt,stereoCount=0;
#endif

#if QN8065_SCAN_NOISE_FLOOR_CCA
#if 1
    for(u8 i=0; i<21; i++) {
        if(freq == qn8065_start[i]) {
            QN8065_ScanNoiseFloor(qn8065_start[i], qn8065_stop[i]);
            QN8065_RXSetTH();
            break;
        }
    }
    QN8065_WriteReg(QN8065_SYSTEM1, 0x01);
#else
    switch(freq)
    {
    case 8750:
        QN8065_ScanNoiseFloor(8750,8800);	
        QN8065_RXSetTH();
        break;
    case 8810:
        QN8065_ScanNoiseFloor(8810,9000);	
        QN8065_RXSetTH();
        break;
    case 9010:
        QN8065_ScanNoiseFloor(9010,9200);	
        QN8065_RXSetTH();
        break;
    case 9210:
        QN8065_ScanNoiseFloor(9210,9400);
        QN8065_RXSetTH();
        break;
    case 9410:
        QN8065_ScanNoiseFloor(9410,9600);
        QN8065_RXSetTH();
        break;
    case 9610:
        QN8065_ScanNoiseFloor(9610,9800);
        QN8065_RXSetTH();
        break;
    case 9810:
        QN8065_ScanNoiseFloor(9810,10000);
        QN8065_RXSetTH();
        break;	
    case 10010:
        QN8065_ScanNoiseFloor(10010,10200);
        QN8065_RXSetTH();
        break;
    case 10210:
        QN8065_ScanNoiseFloor(10210,10400);
        QN8065_RXSetTH();
        break;
    case 10410:
        QN8065_ScanNoiseFloor(10410,10600);
        QN8065_RXSetTH();
        break;	
    case 10610:
        QN8065_ScanNoiseFloor(10610,10800);
        QN8065_RXSetTH();
        break;		
    default:
        //delay_5ms(100); //this delay can improve user display effect
        break;
    }
    //enter idle mode and disable CCA, with user write into frequency
    QN8065_WriteReg(QN8065_SYSTEM1, 0x01);
#endif
    //only support 100K and 200K step.
    QN8065_SetCh(freq,freq+10,2);
#else
   // printf("freq=%d;", freq);
    QN8065_RXSetTH();
    //only support 100K and 200K step.
    QN8065_SetCh(freq,freq+5,1);
#endif
    //entering into RX mode and CCA mode,channels index decide by CCA.
    QN8065_WriteReg(QN8065_SYSTEM1, 0x12);
#if !FMOSC_REUSE_SDCLK
    delay_5ms(10);
#endif
    timeOut = 60;  // time out is 300ms
    while(1)
    {
        regValue = QN8065_ReadReg(QN8065_SYSTEM1);
        //if it seeks a potential channel, the loop will be quited
        if((regValue & QN8065_CHSC) == 0) break;
        delay_5ms(5);   //delay 5ms
        //if it was time out,chip would not normally work.
        if((timeOut--) == 0) return 0;
    }
    //reading out the rxcca_fail flag of RXCCA status
    isValidChannelFlag = (QN8065_ReadReg(QN8065_STATUS1) & QN8065_RXCCA_FAIL ? 0:1);
    if(isValidChannelFlag)
    {
        QN8065_SetMute(0);      //搜到真台后不MUTE，程序里让其自动播放一小段时间
#if QN8065_PILOT_CCA
        //set pilot threshold as 1/256
        QN8065_WriteReg(QN8065_CCOND2,0x91);
        delay_5ms(100);
        snr = QN8065_ReadReg(QN8065_SNR);
#if !FMOSC_REUSE_SDCLK
        if(snr< 25)  //25
#else
        if(snr< 12)  //25
#endif
        {
            isValidChannelFlag = 0;
            for(readCnt=10;readCnt>0;readCnt--)
            {
                delay_5ms(2);
                stereoCount += ((QN8065_ReadReg(QN8065_STATUS1) & QN8065_ST_MO_RX) ? 0:1);
                if(stereoCount >= 3)
                {
                    isValidChannelFlag =1;
                    break;
                }
            }
        }
        //restore pilot threshold as 2/256
        QN8065_WriteReg(QN8065_CCOND2,0x92);
        return (isValidChannelFlag);
#else
        return 1;
#endif
    }
    return 0;
}	

/************************************************************************
void QN8065_RXConfigAudio(UINT8 volume )
*************************************************************************
Description: config audio
Parameters:
  volume:control the volume gain,range : 0~47(-47db~0db)
Return Value:
    None
**********************************************************************/
#pragma location="FM_SEG_QN8065"
void QN8065_RXConfigAudio(UINT8 option)
{
	UINT8 regVal;
	
	option = option>47 ? 47:option;
	regVal = QN8065_ReadReg(QN8065_VOL_CTL);
	regVal = (regVal&0xc0)|(option/6)|((5-option%6)<<3);
	QN8065_WriteReg(QN8065_VOL_CTL,regVal);
}

#pragma location="FM_SEG_QN8065"
void qn8065_set_vol(u8 volume)
{
    if(volume == 0)
        QN8065_SetMute(1);
    else
        QN8065_SetMute(0);
    QN8065_RXConfigAudio(volume_8065table[volume]);
}

#pragma location="FM_SEG_QN8065"
void qn8065_off(void)
{
    QN8065_WriteReg(QN8065_SYSTEM1, 0x20);
    delay_5ms(10);
}

#pragma location="FM_SEG_QN8065"
u8 qn8065_seek(u16 freq)
{
    QN8065_SetMute(1);
    return QN8065_RXValidCH(freq/**10*/);//qn8035 step frequency unit is 10KHZ
}

////////////////////////////////////////////
#pragma location="FM_SEG_QN8065"
UINT8 QN8065_ReadReg(UINT8 adr)
{
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
    QN8065_Delay(60);

    return  I2CReadResult;
}

#pragma location="FM_SEG_QN8065"
void QN8065_WriteReg(UINT8 adr, UINT8 value)
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
    QN8065_Delay(60);
}


#pragma location="FM_SEG_QN8065"
u8 qn8065_online(void)
{
    u8 temp;
    QN8065_WriteReg(QN8065_SYSTEM1, 0x81);
    for(u8 i=0; i<3; i++){      //IIC与其他引脚复用时 FM ID 改为读三次比较保险
        temp = QN8065_ReadReg(0x06) & 0xFC;
        //printf("QN_id=%02x\n",temp);
        if(temp == 0x8c){
            QN8065_Init();
            return 1;
        }
    }
    return 0;
}

#pragma location="FM_SEG_QN8065_1"
u16 GetIFCount(void)
{
	UINT8 r_temp;
	UINT8 IF_CNT1;
	UINT16 IF_CNT;
    IF_CNT1 = QN8065_ReadReg(0x1D);
	IF_CNT = QN8065_ReadReg(0x1E);
    IF_CNT = IF_CNT<<8;
    IF_CNT = IF_CNT + IF_CNT1;
	r_temp = QN8065_ReadReg(0x11);
	r_temp= r_temp & 0xfd;
	QN8065_WriteReg(0x11, r_temp);
	r_temp= r_temp | 0x02;
	QN8065_WriteReg(0x11, r_temp);
    return IF_CNT;
}

#endif
