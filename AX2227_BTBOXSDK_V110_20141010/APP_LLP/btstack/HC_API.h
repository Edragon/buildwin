#define API_SPKAMP				0x01
#define API_TONEGEN				0x02
#define API_RSSIREAD			0x03
#define API_BATTERY				0x04
#define API_TDDCTRL				0x05
#define API_ENTERREGISTER		0x06
#define API_EXITREGISTER		0x07
#define API_REGISTER			0x08
#define API_DELREGISTER			0x09
#define API_MICAMP				0x0a
#define API_CODECRW				0x0b
#define API_EEPROMRW			0x0c
#define API_REMOTESPK			0x0d
#define API_REMOTEMIC			0x0e
#define API_CONNECTION_ST_RD	0x0f
#define API_W_XO_TRIM			0x10
#define API_SEARCH_NEIGHBOR		0x11
#define API_REGISTER_BDADDR		0x12
#define API_REGISTER_AUTO11PINS	0x13
#define API_EXIT_SNIFF			0x14
#define API_READ_GPIO			0x15
#define API_WRITE_GPIO			0x16
#define API_WRITE_LOCAL_BT_ADDRESS	0x17
#define API_IC_MODULE_TEST_MODE	0x18


#define API_STSTEVT				0xAA
#define API_REGIST_STATUSEVT	0xAB
#define API_REGIST_NOTIFYEVT	0xAC
#define API_SEARCHEVT			0xAD
/* pch[0] API command opcode[7:0]
 * pch[1] hdr_length
 * pch[2~] param[0~]
 */
bool hcReceiveAPICommand(u8 pch[]);



/* octet[0]   event opcode
 * octet[1]   param length
 * octet[2~ ] param[0~254]
 */
bool hcTransmitAPIEvent(u8 octet[]); // return 1:success 0:fail

/// EOF ///
