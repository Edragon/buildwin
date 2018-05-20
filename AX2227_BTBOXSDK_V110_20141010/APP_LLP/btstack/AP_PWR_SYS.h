#define ST_BTOFF_NORMAL					0x00
#define ST_BTOFF_PREPARE_TURN_OFF_BT	0x01
#define ST_BTOFF_W4_RF_TURN_OFF_READY	0x02
#define ST_BTOFF_TURN_OFF_BT			0x03



#define IND_GPIO_POWER_ON				1
#define IND_GPIO_POWER_OFF				0
#define IND_GPIO_SBC_STOP				1
#define IND_GPIO_SBC_START				0


#define WATCH_DOG_RESET_ENABLE				(eeprom_AP_FunctionSetting&0x01)
#define BATTERY_LOW_DETECT_ENABLE			(eeprom_AP_FunctionSetting&0x02)
#define ALLOW_REGISER_IN_IDLE_MODE			(eeprom_AP_FunctionSetting&0x04)
#define CREATE_CONNECTION_AFTER_POWER_ON	(eeprom_AP_FunctionSetting&0x08)
#define LED_ELECTRODE						(eeprom_AP_FunctionSetting&0x10)	//LED on by GPIO pull high
#define GPIO_POWER_CONTROL					(eeprom_AP_FunctionSetting&0x20)	//use button volume up/donw gpio for wakeup host and BT power state indicator.
#define SPI_EN								(eeprom_AP_FunctionSetting&0x40)	//Enable SPI and Disable SSP
#define CREATE_CONNECTION_BY_OTHER_KEY		(eeprom_AP_FunctionSetting&0x80)







//#ifdef __THIS_IS_AP_VAR__
	u8  ReadyInSniffMode;
	u8  FirstIntoSniff;
	u8  acl_detach_flag;
	u8  eeprom_AP_FunctionSetting;		
//#else
/*	extern u8 xdata ReadyInSniffMode;
	extern u8 xdata FirstIntoSniff;
	extern u8 xdata acl_detach_flag;
	extern u8 xdata eeprom_AP_FunctionSetting;
#endif
*/
