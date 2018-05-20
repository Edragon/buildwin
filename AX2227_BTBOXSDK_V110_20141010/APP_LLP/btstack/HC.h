//===== HCI packet TYPE =======================================================
#define HCI_PTYPE_HC_COMMAND		0x01
#define HCI_PTYPE_ACL			0x02
//efine HCI_PTYPE_SCO			0x03
#define HCI_PTYPE_HCI_EVENT		0x04
//efine HCI_PTYPE_HCI_ERROR		0x05
//efine HCI_PTYPE_HCI_NEGOTIATE		0x06
#define HCI_PTYPE_DBG_MSG		0x07

//===== HCI COMMAND OGF =======================================================
#define OGF_LINK_CTRL			0x01
#define OGF_LINK_POLICY			0x02
#define OGF_HC_N_BASEBAND		0x03
#define OGF_INFO_PARAMS			0x04
#define OGF_STATUS_PARAMS		0x05
#define OGF_TESTING			0x06
#define OGF_LE				0x08	//LowEnergy
#define OGF_BLUETOOTH_LOGO_TESTING	0x3E
#define OGF_VENDOR_SPECIFIC		0x3F

//===== HCI COMMAND OCF with (OGF 0x01) =======================================
// OGF 0x01 link control commands
#define OCF_INQUIRY				0x001
#define OCF_INQUIRY_CANCEL			0x002
#define OCF_PERIODIC_INQ_MODE			0x003
#define OCF_EXIT_PERIODIC_INQ_MODE		0x004
#define OCF_CREATE_CONN				0x005
#define OCF_DISCONNECT				0x006
#define OCF_ADD_SCO_CONN			0x007
#define OCF_CANCEL_CREATE_CONN			0x008	//spec 1.2
#define OCF_ACCEPT_CONN_REQUEST			0x009
#define OCF_REJECT_CONN_REQUEST			0x00A
#define OCF_LINK_KEY_REQ_REPLY			0x00B
#define OCF_LINK_KEY_REQ_NEG_REPLY		0x00C
#define OCF_PIN_CODE_REQ_REPLY			0x00D
#define OCF_PIN_CODE_REQ_NEG_REPLY		0x00E
#define OCF_CHANGE_CONN_PKT_TYPE		0x00F
#define OCF_AUTH_REQUESTED			0x011
#define OCF_SET_CONN_ENCRYPT			0x013
#define OCF_CHANGE_CONN_LINKKEY			0x015
#define OCF_MASTER_LINKKEY			0x017
#define OCF_REM_NAME_REQ			0x019
#define OCF_CANCEL_REM_NAME_REQ			0x01A	//spec 1.2
#define OCF_R_REM_SUPPORT_FEATURES		0x01B
#define OCF_R_REM_EXTENDED_FEATURES		0x01C	//spec 1.2
#define OCF_R_REM_VERSION_INFO			0x01D
#define OCF_R_CLOCK_OFFSET			0x01F
#define OCF_R_LMP_HANDLE			0x020	//spec 1.2
#define OCF_SETUP_SYNCHRONOUS_CONN		0x028	//spec 1.2
#define OCF_ACCEPT_SYNCHRONOUS_CONN_REQUEST	0x029	//spec 1.2
#define OCF_REJECT_SYNCHRONOUS_CONN_REQUEST	0x02A	//spec 1.2
#define OCF_IO_CAPABILITY_REQ_REPLY		0x02B	//spec 2.1
#define OCF_USER_CONFIRM_REQ_REPLY		0x02C	//spec 2.1
#define OCF_USER_CONFIRM_REQ_NEG_REPLY		0x02D	//spec 2.1
#define OCF_USER_PASSKEY_REQ_REPLY		0x02E	//spec 2.1
#define OCF_USER_PASSKEY_REQ_NEG_REPLY		0x02F	//spec 2.1
#define OCF_REMOTE_OOB_DATA_REQ_REPLY		0x030	//spec 2.1
#define OCF_REMOTE_OOB_DATA_REQ_NEG_REPLY	0x033	//spec 2.1
#define OCF_IO_CAPABILITY_REQ_NEG_REPLY		0x034	//spec 2.1
#define OCF_CREATE_PHY_LINK			0x035	//spec 3.0
#define OCF_ACCEPT_PHY_LINK			0x036	//spec 3.0
#define OCF_DISCONNECT_PHY_LINK			0x037	//spec 3.0
#define OCF_CREATE_LOGICAL_LINK			0x038	//spec 3.0
#define OCF_ACCEPT_LOGICAL_LINK			0x039	//spec 3.0
#define OCF_DISCONNECT_LOGICAL_LINK		0x03A	//spec 3.0
#define OCF_LOGICAL_LINK_CANCEL			0x03B	//spec 3.0
#define OCF_FLOW_SPEC_MODIFY			0x03C	//spec 3.0

//===== HCI COMMAND OCF with (OGF 0x02) =======================================
// OGF 0x02 link policy commands
#define OCF_HOLD_MODE				0x001
#define OCF_SNIFF_MODE				0x003
#define OCF_EXIT_SNIFF_MODE			0x004
#define OCF_PARK_MODE				0x005
#define OCF_EXIT_PARK_MODE			0x006
#define OCF_QOS_SETUP				0x007
#define OCF_ROLE_DISCOVERY			0x009
#define OCF_SWITCH_ROLE				0x00B
#define OCF_R_LINK_POLICY_SETTINGS		0x00C
#define OCF_W_LINK_POLICY_SETTINGS		0x00D
#define OCF_R_DEFAULT_LINK_POLICY_SETTINGS	0x00E	//spec 1.2
#define OCF_W_DEFAULT_LINK_POLICY_SETTINGS	0x00F	//spec 1.2
#define OCF_FLOW_SPEC				0x010	//spec 1.2
#define OCF_SNIFF_SUBRATING			0x011	//spec 2.1

//===== HCI COMMAND OCF with (OGF 0x03) =======================================
// OGF 0x03 host controller & baseband commands
#define OCF_SET_EVENT_MASK			0x001
#define OCF_RESET				0x003
#define OCF_SET_EVENT_FILTER			0x005
#define OCF_FLUSH				0x008
#define OCF_R_PIN_TYPE				0x009
#define OCF_W_PIN_TYPE				0x00A
#define OCF_CREATE_NEW_UNIT_KEY			0x00B
#define OCF_R_STORED_LINK_KEY			0x00D
#define OCF_W_STORED_LINK_KEY			0x011
#define OCF_DELETE_STORED_LINK_KEY		0x012
#define OCF_W_LOCAL_NAME			0x013
#define OCF_R_LOCAL_NAME			0x014
#define OCF_R_CONN_ACCEPT_TIMEOUT		0x015
#define OCF_W_CONN_ACCEPT_TIMEOUT		0x016
#define OCF_R_PAGE_TIMEOUT			0x017
#define OCF_W_PAGE_TIMEOUT			0x018
#define OCF_R_SCAN_ENABLE			0x019
#define OCF_W_SCAN_ENABLE			0x01A
#define OCF_R_PAGE_SCAN_ACTIVITY		0x01B
#define OCF_W_PAGE_SCAN_ACTIVITY		0x01C
#define OCF_R_INQ_SCAN_ACTIVITY			0x01D
#define OCF_W_INQ_SCAN_ACTIVITY			0x01E
#define OCF_R_AUTH_ENABLE			0x01F
#define OCF_W_AUTH_ENABLE			0x020
#define OCF_R_ENCRYPTION_MODE			0x021	//deprecated in spec 2.1
#define OCF_W_ENCRYPTION_MODE			0x022	//deprecated in spec 2.1
#define OCF_R_CLASS_OF_DEVICE			0x023
#define OCF_W_CLASS_OF_DEVICE			0x024
#define OCF_R_VOICE_SETTING			0x025
#define OCF_W_VOICE_SETTING			0x026
#define OCF_R_AUTO_FLUSH_TIMEOUT		0x027
#define OCF_W_AUTO_FLUSH_TIMEOUT		0x028
#define OCF_R_NUM_BROADCAST_RETX		0x029
#define OCF_W_NUM_BROADCAST_RETX		0x02A
#define OCF_R_HOLD_MODE_ACTIVITY		0x02B
#define OCF_W_HOLD_MODE_ACTIVITY		0x02C
#define OCF_R_TRANSMIT_PWR_LEVEL		0x02D
#define OCF_R_SCO_FLOW_CTRL_ENABLE		0x02E
#define OCF_W_SCO_FLOW_CTRL_ENABLE		0x02F
#define OCF_SET_HC_TO_HOST_FLOW_CONTROL		0x031
#define OCF_HOST_BUFFER_SIZE			0x033
#define OCF_HOST_NUM_OF_COMPLETED_PKTS		0x035
#define OCF_R_LINK_SUPERVISION_TIMEOUT		0x036
#define OCF_W_LINK_SUPERVISION_TIMEOUT		0x037
#define OCF_R_NUM_OF_SUPPORTED_IAC		0x038
#define OCF_R_CURRENT_IAC_LAP			0x039
#define OCF_W_CURRENT_IAC_LAP			0x03A
#define OCF_R_PAGE_SCAN_PERIOD_MODE		0x03B
#define OCF_W_PAGE_SCAN_PERIOD_MODE		0x03C
#define OCF_R_PAGE_SCAN_MODE			0x03D	//???
#define OCF_W_PAGE_SCAN_MODE			0x03E	//???
#define OCF_SET_AFH_HOST_CHANNEL_CLASSIFICATION	0x03F	//spec 1.2
#define OCF_R_INQ_SCAN_TYPE			0x042	//spec 1.2
#define OCF_W_INQ_SCAN_TYPE			0x043	//spec 1.2
#define OCF_R_INQ_MODE				0x044	//spec 1.2
#define OCF_W_INQ_MODE				0x045	//spec 1.2
#define OCF_R_PAGE_SCAN_TYPE			0x046	//spec 1.2
#define OCF_W_PAGE_SCAN_TYPE			0x047	//spec 1.2
#define OCF_R_CHANNEL_ASSESSMENT_MODE		0x048	//spec 1.2
#define OCF_W_CHANNEL_ASSESSMENT_MODE		0x049	//spec 1.2
#define OCF_R_EXT_INQ_RESP			0x051	//spec 2.1
#define OCF_W_EXT_INQ_RESP			0x052	//spec 2.1
#define OCF_REFRESH_ENCRYPT_KEY			0x053	//spec 2.1
#define OCF_R_SIMPLE_PAIRING_MODE		0x055	//spec 2.1
#define OCF_W_SIMPLE_PAIRING_MODE		0x056	//spec 2.1
#define OCF_R_LOCAL_OOB_DATA			0x057	//spec 2.1
#define OCF_R_INQ_RESP_TX_POWER_LEVEL		0x058	//spec 2.1
#define OCF_W_INQUIRY_TX_POWER_LEVEL		0x059	//spec 2.1
#define OCF_R_DEFAULT_ERRO_DATA_REPORTING	0x05A	//spec 2.1
#define OCF_W_DEFAULT_ERRO_DATA_REPORTING	0x05B	//spec 2.1
#define OCF_ENHANCED_FLUSH			0x05F	//spec 2.1
#define OCF_SEND_KEYPRESS_NOTIFICATION		0x060	//spec 2.1
#define OCF_R_LOGICAL_LINK_ACCEPT_TIMEOUT	0x061	//spec 3.0
#define OCF_W_LOGICAL_LINK_ACCEPT_TIMEOUT	0x062	//spec 3.0
#define OCF_SET_PAGE_2_EVENT_MASK		0x063	//spec 3.0
#define OCF_R_LOCATION_DATA			0x064	//spec 3.0
#define OCF_W_LOCATION_DATA			0x065	//spec 3.0
#define OCF_R_FLOW_CTRL_MODE			0x066	//spec 3.0
#define OCF_W_FLOW_CTRL_MODE			0x067	//spec 3.0
#define OCF_R_ENHANCED_TX_POWER_LEVEL		0x068	//spec 3.0
#define OCF_R_BEST_EFFORT_FLUSH_TIMEOUT		0x069	//spec 3.0
#define OCF_W_BEST_EFFORT_FLUSH_TIMEOUT		0x06A	//spec 3.0
#define OCF_SHORT_RANGE_MODE			0x06B	//spec 3.0

//===== HCI COMMAND OCF with (OGF 0x04) =======================================
// OGF 0x04 information parameters
#define OCF_R_LOC_VERSION_INFO			0x001
#define OCF_R_LOC_SUPPORTED_COMMANDS		0x002	//spec 1.2
#define OCF_R_LOC_SUPPORTED_FEATURES		0x003
#define OCF_R_LOCAL_EXTENDED_FEATURES		0x004	//spec 1.2
#define OCF_R_BUFFER_SIZE			0x005
#define OCF_R_COUNTRY_CODE			0x007	//deprecated in spec 1.2
#define OCF_R_BD_ADDR				0x009
#define OCF_R_DATA_BLOCK_SIZE			0x00A	//spec 3.0

//===== HCI COMMAND OCF with (OGF 0x05) =======================================
// OGF 0x05 status parameters
#define OCF_R_FAILED_CONTACT_COUNTER		0x001
#define OCF_RESET_FAILED_CONTACT_COUNTER	0x002
#define OCF_GET_LINK_QUALITY			0x003
#define OCF_R_RSSI				0x005
#define OCF_R_AFH_CHANNEL_MAP			0x006	//spec 1.2
#define OCF_R_CLOCK				0x007	//spec 1.2
#define OCF_R_ENCRYPT_KEY_SIZE			0x008	//spec 3.0
#define OCF_R_LOCAL_AMP_INFO			0x009	//spec 3.0
#define OCF_R_LOCAL_AMP_ASSOC			0x00A	//spec 3.0
#define OCF_W_REMOT_AMP_ASSOC			0x00B	//spec 3.0

//===== HCI COMMAND OCF with (OGF 0x06) =======================================
// OGF 0x06 testing commands
#define OCF_R_LOOPBACK_MODE			0x001
#define OCF_W_LOOPBACK_MODE			0x002
#define OCF_ENABLE_DEV_UNDER_TEST_MODE		0x003
#define OCF_W_SIMPLE_PAIRING_DEBUG_MODE		0x004	//spec 2.1
#define OCF_ENABLE_AMP_RECEIVER_REPORTS		0x007	//spec 3.0
#define OCF_AMP_TEST				0x008	//spec 3.0
#define OCF_END_AMP_TEST			0x009	//spec 3.0
#define OCF_W_CONN_SINGLE_FREQ			0x0FD	//Conwise specific
#define OCF_M_TEST_CONTROL			0x0FE	//Conwise specific
#define OCF_S_TEST_CONTROL			0x0FF	//Conwise specific

//===== HCI COMMAND OCF with (OGF 0x08) =======================================
// OGF 0x08 LowEnergy (LE) controller commands
#define OCF_LE_SET_EVENT_MASK			0x001
#define OCF_LE_R_BUFFER_SIZE			0x002
#define OCF_LE_R_LOC_SUPPORTED_FEATURES		0x003
#define OCF_LE_SET_LOC_USED_FEATURES		0x004
#define OCF_LE_SET_RAND_ADDR			0x005
#define OCF_LE_SET_ADVERTISE_PARAM		0x006
#define OCF_LE_R_ADVERTISE_CHANNEL_TX_PWR	0x007
#define OCF_LE_SET_ADVERTISING_DATA		0x008
#define OCF_LE_SET_SCAN_RESP_DATA		0x009
#define OCF_LE_SET_ADVERTISE_ENABLE		0x00A
#define OCF_LE_SET_SCAN_PARAM			0x00B
#define OCF_LE_SET_SCAN_ENABLE			0x00C
#define OCF_LE_CREATE_CONNECTION		0x00D
#define OCF_LE_CANCEL_CREATE_CONNECTION		0x00E
#define OCF_LE_R_WHITE_LIST_SIZE		0x00F
#define OCF_LE_CLEAR_WHITE_LIST			0x010
#define OCF_LE_ADD_DEVICE_TO_WHITE_LIST		0x011
#define OCF_LE_REMOVE_DEVICE_FM_WHITE_LIST	0x012
#define OCF_LE_UPDATE_CONNECTION		0x013
#define OCF_LE_SET_HOST_CHANNEL_CLASSIFI	0x014
#define OCF_LE_R_CHANNEL_MAP			0x015
#define OCF_LE_R_REMOTE_USED_FEATURES		0x016
#define OCF_LE_ENCRYPT				0x017
#define OCF_LE_RAND				0x018
#define OCF_LE_START_ENCRYPT			0x019
#define OCF_LE_LONG_TERM_KEY_REQ_REPLY		0x01A
#define OCF_LE_LONG_TERM_KEY_REQ_NEG_REPLY	0x01B
#define OCF_LE_R_SUPPORTED_STATES		0x01C
#define OCF_LE_RECEIVER_TEST			0x01D
#define OCF_LE_TRANSMITTER_TEST			0x01E
#define OCF_LE_TEST_END				0x01F

//===== HCI COMMAND OCF with (OGF 0x3F) =======================================
// OGF 0x3F verdor specific debug commands
#define OCF_R_CSR_BLUESOLEIL_SECURITY_KEY	0x000
#define OCF_R_CODEC_CTRL_REGISTER		0x001
#define OCF_W_CODEC_CTRL_REGISTER		0x002
#define OCF_R_DSP_CTRL_REGISTER			0x003
#define OCF_W_DSP_CTRL_REGISTER			0x004
#define OCF_R_EEPROM				0x005
#define OCF_W_EEPROM				0x006
#define OCF_R_XDATA				0x007
#define OCF_W_XDATA				0x008
#define OCF_R_RF_SPI_REG			0x009
#define OCF_W_RF_SPI_REG			0x00A
#define OCF_R_SIW_BLUESOLEIL_SECURITY_KEY	0x00B	//SiliconWave, notice: value cannot be changed
#define OCF_R_IVT_BLUESOLEIL_SECURITY_KEY	0x00C	//Conwise, notice: value cannot be changed
#define OCF_W_IVT_BLUESOLEIL_SECURITY_KEY	0x00D	//Conwise, notice: value cannot be changed
#define OCF_W_VIRTUAL_BD_ADDR			0x00E
#define OCF_FCC_TEST_CTRL			0x00F
#define OGF3F_OCF_W_CONN_SINGLE_FREQ		0x010	//same as OCF_W_CONN_SINGLE_FREQ of OGF_0x06
#define OGF3F_OCF_S_TEST_CONTROL		0x011	//same as OCF_S_TEST_CONTROL of OGF_0x06
#define OGF3F_OCF_M_TEST_CONTROL		0x012	//same as OCF_M_TEST_CONTROL of OGF_0x06
#define OCF_R_REMOTE_EEPROM			0x013
#define OCF_W_REMOTE_EEPROM			0x014
#define OCF_R_REMOTE_CODE_VERSION		0x015	//YS
#define OCF_R_BYTE				0x016
#define OCF_W_BYTE				0x017
#define OCF_W_GPIO				0x18
#define OCF_R_IVT_BLUESOLEIL_SECUKEY_ISSC	0x01B	//ISSC, notice: value cannot be changed
#define OCF_W_IVT_BLUESOLEIL_SECUKEY_ISSC	0x01C	//ISSC, notice: value cannot be changed
#define OCF_R_IVT_BLUESOLEIL_2608_SECUKEY_ISSC	0x029	//ISSC, notice: value cannot be changed, BlueSoleil 2.6.0.8
#define OCF_W_IVT_BLUESOLEIL_2608_SECUKEY_ISSC	0x02A	//ISSC, notice: value cannot be changed, BlueSoleil 2.6.0.8

#define OCF_BC_CMD				0x0BC	//CSR BC
#define OCF_CC_CMD				0x0CC	//Conwsie Cell phone command

//===== HCI EVENT opcode ======================================================
#define HCI_INQUIRY_COMPLETE				0x01
#define HCI_INQUIRY_RESULT				0x02
#define HCI_CONN_COMPLETE				0x03
#define HCI_CONN_REQUEST				0x04
#define HCI_DISCONN_COMPLETE				0x05
#define HCI_AUTH_COMPLETE				0x06
#define HCI_REM_NAME_REQUEST_COMPLETE			0x07
#define HCI_ENCRYPTION_CHANGE				0x08
#define HCI_CHANGE_CONN_LINK_KEY_COMPLETE		0x09
#define HCI_MASTER_LINK_KEY_COMPLETE			0x0A
#define HCI_R_REM_FEATURES_COMPLETE			0x0B
#define HCI_READ_REM_VER_INFO_COMPLETE			0x0C
#define HCI_QOS_SETUP_COMPLETE				0x0D
#define HCI_CMD_COMPLETE				0x0E
#define HCI_CMD_STATUS					0x0F
#define HCI_HARDWARE_ERROR				0x10	//TODO: host-to-dongle uart synchronization is lost
#define HCI_FLUSH_OCCURRED				0x11
#define HCI_ROLE_CHANGE					0x12
#define HCI_NUM_OF_COMPLETED_PKTS			0x13
#define HCI_MODE_CHANGE					0x14
#define HCI_RETURN_LINK_KEYS				0x15
#define HCI_PIN_CODE_REQUEST				0x16
#define HCI_LINK_KEY_REQUEST				0x17
#define HCI_LINK_KEY_NOTIFY				0x18
#define HCI_LOOPBACK_COMMAND				0x19
#define HCI_DATA_BUFFER_OVERFLOW			0x1A
#define HCI_MAX_SLOTS_CHANGE				0x1B
#define HCI_READ_CLOCK_OFFSET_COMPLETE			0x1C
#define HCI_CONN_PKT_TYPE_CHANGED			0x1D
#define HCI_QOS_VIOLATION				0x1E
#define HCI_PAGE_SCAN_MODE_CHANGE			0x1F	//obsolete, deprecated in spec 1.2
#define HCI_PAGE_SCAN_REPETITION_MODE_CHANGE		0x20
#define HCI_FLOW_SPEC_COMPLETE				0x21	//spec 1.2
#define HCI_INQUIRY_RESULT_WITH_RSSI			0x22	//spec 1.2
#define HCI_R_REM_EXTEND_FEATURES_COMPLETE		0x23	//spec 1.2
#define HCI_SYNCHRONOUS_CONN_COMPLETE			0x2C	//spec 1.2
#define HCI_SYNCHRONOUS_CONN_CHANGED			0x2D	//spec 1.2	TODO:unused
#define HCI_SNIF_SUBRATING_CHANGED			0x2E	//spec 2.1
#define HCI_EXTENDED_INQ_RESULT				0x2F	//spec 2.1
#define HCI_ENCRYPTION_KEY_REFRESH_COMPLETE		0x30	//spec 2.1
#define HCI_IO_CAPABILITY_REQUEST			0x31	//spec 2.1
#define HCI_IO_CAPABILITY_RESPONSE			0x32	//spec 2.1
#define HCI_USER_CONFIRM_REQUEST			0x33	//spec 2.1
#define HCI_USER_PASSKEY_REQUEST			0x34	//spec 2.1
#define HCI_REMOTE_OOB_DATA_REQUEST			0x35	//spec 2.1
#define HCI_SIMPLE_PAIRING_COMPLETE			0x36	//spec 2.1
#define HCI_LINK_SUPERVISION_TIMEOUT_CHANGED		0x38	//spec 2.1
#define HCI_ENHANCED_FLUSH_COMPLETE			0x39	//spec 2.1
#define HCI_USER_PASSKEY_NOTIFICATION			0x3B	//spec 2.1
#define HCI_KEYPRESS_NOTIFICATION			0x3C	//spec 2.1
#define HCI_REMOTE_HOST_SUPPORTED_FEAT_NOTIFICATION	0x3D	//spec 2.1
#define HCI_LE_META_EVENT_3E				0x3E
#define HCI_PHYSICAL_LINK_COMPLETE			0x40	//spec 3.0
#define HCI_CHANNEL_SELECTED				0x41	//spec 3.0
#define HCI_DISCONNECTION_PHYSICAL_LINK_COMPLETE	0x42	//spec 3.0
#define HCI_PHYSICAL_LINK_LOSS_EARLY_WARNING		0x43	//spec 3.0
#define HCI_PHYSICAL_LINK_RECOVERY			0x44	//spec 3.0
#define HCI_LOGICAL_LINK_COMPLETE			0x45	//spec 3.0
#define HCI_DISCONNECTION_LOGICAL_LINK_COMPLETE		0x46	//spec 3.0
#define HCI_FLOW_SPEC_MODIFY_COMPLETE			0x47	//spec 3.0
#define HCI_NUM_OF_COMPLETED_DATA_BLOCKS		0x48	//spec 3.0
#define HCI_AMP_START_TEST				0x49	//spec 3.0
#define HCI_AMP_TEST_END				0x4A	//spec 3.0
#define HCI_AMP_RECEIVER_REPORT				0x4B	//spec 3.0
#define HCI_SHORT_RANGE_MODE_CHANGE_COMPLETE		0x4C	//spec 3.0
#define HCI_AMP_STATUS_CHANGE				0x4D	//spec 3.0
#define HCI_CSR_SIW_IVT_BLUESOLEIL_SECURITY_KEY		0xFF

//===== LE SUB-EVENT opcode ===================================================
#define HCI_LE_CONN_COMPLETE				0x01
#define HCI_LE_ADVERTISE_REPORT				0x02
#define HCI_LE_CONN_UPDATE_COMPLETE			0x03
#define HCI_LE_R_REMOTE_USED_FEATURES			0x04
#define HCI_LE_LONG_TERM_KEY_REQUEST			0x05

//===== ERROR CODES ===========================================================
#define ERR_UNKNOWN_HCI_COMMAND				0x01
#define ERR_NO_CONNECTION				0x02	//unknown connection identifier
#define ERR_HARDWARE_FAILURE				0x03
#define ERR_PAGE_TIMEOUT				0x04
#define ERR_AUTH_FAILURE				0x05
#define ERR_PIN_OR_KEY_MISSING				0x06
#define ERR_MEMORY_CAPACITY_EXCEEDED			0x07
#define ERR_CONNECTION_TIMEOUT				0x08
#define ERR_MAX_NUM_OF_CONNS				0x09
#define ERR_MAX_NUM_OF_SCO_CONNS_TO_A_DEVICE		0x0A
#define ERR_ACL_CONN_ALREADY_EXISTS			0x0B
#define ERR_COMMAND_DISALLOWED				0x0C
#define ERR_HOST_REJECT_LIMITED_RESOURCES		0x0D
#define ERR_HOST_REJECT_SECURITY_REASONS		0x0E
#define ERR_HOST_REJECT_REM_DEV_IS_ONLY_A_PERSONAL_DEV	0x0F
#define ERR_HOST_TIMEOUT				0x10
#define ERR_UNSUPPORTED_FEATURE_OR_PARAM_VALUE		0x11	//param in HCI command not supported. Not used in an LMP PDU
#define ERR_INVALID_HCI_CMD_PARAMS			0x12
#define ERR_OTHER_END_TERMINATED_CONN_USER_END		0x13
#define ERR_OTHER_END_TERMINATED_CONN_LOW_RESOURCES	0x14
#define ERR_OTHER_END_TERMINATED_CONN_POWER_OFF		0x15
#define ERR_CONN_TERMINATED_BY_LOC_HOST			0x16
#define ERR_REPEATED_ATTEMPTS				0x17
#define ERR_PAIRING_NOT_ALLOWED				0x18
#define ERR_UNKNOWN_LMP_PDU				0x19
#define ERR_UNSUPPORTED_LMP_FEATURES			0x1A	//or unsupported remote feature
#define ERR_SCO_OFFSET_REJECTED				0x1B
#define ERR_SCO_INTERVAL_REJECTED			0x1C
#define ERR_SCO_AIR_MODE_REJECTED			0x1D
#define ERR_INVALID_LMP_PARAMS				0x1E	//
#define ERR_UNSPECIFIED_ERROR				0x1F
#define ERR_UNSUPPORTED_LMP_PARAM_VALUE			0x20	//
#define ERR_ROLE_CHANGE_NOT_ALLOWED			0x21
#define ERR_LMP_RESPONSE_TIMEOUT			0x22
#define ERR_LMP_ERROR_TRANSACTION_COLLISION		0x23
#define ERR_LMP_PDU_NOT_ALLOWED				0x24
#define ERR_ENCR_MODE_NOT_ACCEPTABLE			0x25
#define ERR_UNIT_KEY_USED				0x26
#define ERR_QOS_NOT_SUPPORTED				0x27
#define ERR_INSTANT_PASSED				0x28
#define ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED		0x29
#define ERR_DIFFERENT_TRANSACTION_COLLISION		0x2A
#define ERR_QOS_UNACCEPTED_PARAMS			0x2C
#define ERR_QOS_REJECTED				0x2D
#define ERR_CHANNEL_CLASSIFICATION_NOT_SUPPORTED	0x2E
#define ERR_INSUFFICIENT_SECURITY			0x2F
#define ERR_PARAM_OUT_OF_MANDATORY_RANGE		0x30
#define ERR_ROLE_SWITCH_PENDING				0x32
#define ERR_RESERVED_SLOT_VIOLATION			0x34
#define ERR_ROLE_SWITCH_FAILED				0x35
#define ERR_EXTENDED_INQ_RESPONSE_TOO_LARGE		0x36	//spec 2.1
#define ERR_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST	0x37	//spec 2.1
#define ERR_HOST_BUSY_PAIRING				0x38	//spec 2.1
#define ERR_CONN_REJECTED_DUE_TO_NO_SUITABLE_CH_FOUND	0x39	//spec 3.0
#define ERR_CONTROLLER_BUSY				0x3A	//spec 3.0


//======= FUNCTIONS PROTOTYPE DECLARATION =====================================
/* octet[0]   event opcode
 * octet[1]   param length
 * octet[2~ ] param[0~254]
 */
bool hcTransmitHCIEvent(u8 octet[]); //return 1:success 0:fail
bool hcTxHCIEventToHost(u8 octet[], bool tohost);
bool xmit_hcmd_status(u8 status, u16 cmd_opcode); //return 1:success 0:fail
bool xmit_hcmd_status_tohost(u8 status, u16 cmd_opcode, bool tohost);
bool xmit_hcmd_complete0(u8 cmd_opcode70, u8 cmd_opcodef8, u8 status);
bool xmit_hcmd_complete1(u8 cmd_opcode70, u8 cmd_opcodef8, u8 status, u8 param);

void receive_cobs_byte(u8 ch);				// uart
void receive_cobs_block(u8 count, u8 *pch);	// dma1
void prepare_sent_acl(u8 acl_index);			//#ifdef _YS_PARSE_DMA11_AT_TMR1_

u8   hcAllowedCmdPkts(void);
#define HC_ALLOWED_CMD_PKTS	0x01
//#define HC_ALLOWED_CMD_PKTS	hcAllowedCmdPkts()


/* return 0x00:fail 0x01~0xFF:success
 */
u8   hcTxPacketToHost(u8 ptype, u8 *pch, u16 len, bool copy);

//------------------------------------------------------------------------------
u16  tmr625usGetCurrentTicks(void);//CurrentTicks is used as later offset_ticks
bool tmr625usIsExpired(u16 offset_ticks, u16 Texpire);//Texpire in unit of 625us


//==============================================================================
//link control
#ifdef __EMBEDDED__
void hcmd_inquiry(u8 lapL,u8 lapM,u8 lapH,u8 length,u8 num_inq_resp,bool cmdfromhost);	//0x0401
#else
void hcmd_inquiry(u8 lapL,u8 lapM,u8 lapH,u8 length,u8 num_inq_resp);	//0x0401
#endif
void hcmd_inquiry_cancel(void);						//0x0402
void hcmd_periodic_inq_mode(u16 max_period_len,u16 min_period_len,	//0x0403
		u8 lapL,u8 lapM,u8 lapH,u8 inq_length,u8 num_inq_resp);
void hcmd_exit_periodic_inq_mode(void);					//0x0404
bool hcmd_create_connection(u8 *bd_addr/*,u16 ptype,u8 SR,u16 valid_clkoffset162,u8 allow_sw */);
bool hcmd_disconnect(u16 hdl, u8 reason);				//0x0406
void hcmd_disconnect_acl(u8 *bd_addr);
//void hcmd_add_sco_connection(u16 hdl, u8 pkt_type);			//0x0407
bool hcmd_add_sco_connection(void);
void hcmd_cancel_create_connection(u8 *bd_addr);			//0x0408 spec 1.2
void hcmd_accept_connection_request(u8 *bd_addr, bool role);	//0x0409
void hcmd_reject_connection_request(u8 *bd_addr, u8 reason);	//0x040A
void hcmd_link_key_request_reply(u8 *bd_addr, u8 *key);	//0x040B
void hcmd_link_key_request_neg_reply(u8 *bd_addr);		//0x040C
void hcmd_pin_code_request_reply(u8 *bd_addr,u8 len,u8 *);	//0x040D
void hcmd_pin_code_request_neg_reply(u8 *bd_addr);		//0x040E
//void hcmd_change_conn_pkt_type(u16 hdl, u16 pkt_type);			//0x040F
void hcmd_authentication_requested(u16 hdl);				//0x0411
void hcmd_set_conn_encryption(u16 hdl, u8 on);				//0x0413
void hcmd_change_conn_link_key(u16 hdl);				//0x0415
void hcmd_master_link_key(u8 key_flag);					//0x0417
#ifdef __EMBEDDED__
void hcmd_remote_name_request(u8 *bd_addr,u8 SR,u16 offset,bool);	//0x0419
#else
void hcmd_remote_name_request(u8 *bd_addr,u8 SR,u16 offset);	//0x0419
#endif
void hcmd_cancel_remote_name_request(u8 *bd_addr);		//0x041A spec 1.2
void hcmd_r_remote_support_features(u16 hdl);				//0x041B
void hcmd_r_remote_extended_features(u16 hdl, u8 page_number);		//0x041C spec 1.2
void hcmd_r_remote_ver_info(u16 hdl);					//0x041D
void hcmd_r_clock_offset(u16 hdl);					//0x041F
void hcmd_r_lmp_handle(u16 hdl);					//0x0420 spec 1.2
void hcmd_setup_synchronous_connection(u8 *);			//0x0428 spec 1.2
void hcmd_accept_synchronous_connection_request(u8 *bd_addr);	//0x0429 spec 1.2
void hcmd_reject_synchronous_connection_request(u8 *bd_addr,u8 reason);	//0x042A spec 1.2
void hcmd_io_capability_request_reply(u8 *bd_addr, u8, u8, u8);	//0x042B spec 2.1
void hcmd_user_confirmation_request_reply(u8 *bd_addr);		//0x042C spec 2.1
void hcmd_user_confirmation_request_negative_reply(u8 *bd_addr);	//0x042D spec 2.1
void hcmd_user_passkey_request_reply(u8 *bd_addr, u8 *);	//0x042E spec 2.1
void hcmd_user_passkey_request_negative_reply(u8 *bd_addr);	//0x042F spec 2.1
void hcmd_remote_oob_data_request_reply(u8 *bd_addr, u8 *);	//0x0430 spec 2.1
void hcmd_remote_oob_data_request_negative_reply(u8 *bd_addr);	//0x0433 spec 2.1
void hcmd_io_capability_request_neg_reply(u8 *bd_addr, u8 reason);//0x0434 spec 2.1

//link policy
void hcmd_hold_mode(u16 hdl,u16 max_interval,u16 min_interval);		//0x0801
//void hcmd_sniff_mode(u16 hdl,u16 Tmax,u16 Tmin,u16 attempt,u16 TO);	//0x0803
//void hcmd_exit_sniff_mode(u16 hdl);					//0x0804
void hcmd_qos_setup(u16 hdl,u8 type, u8 *qos4x4);			//0x0807
bool hcmd_role_discovery(u16 hdl);					//0x0809
bool hcmd_switch_role(u8 *bd_addr, u8 new_role);		//0x080B
void hcmd_r_link_policy_settings(u16 hdl);				//0x080C
void hcmd_w_link_policy_settings(u16 hdl, u8 settings);			//0x080D
void hcmd_r_default_link_policy_settings(void);				//0x080E spec 1.2
void hcmd_w_default_link_policy_settings(u8 settings);			//0x080F spec 1.2
void hcmd_flow_specification(u16 hdl, u8 *param);			//0x0810 spec 1.2
void hcmd_sniff_subrating(u16 hdl, u16 latency, u16 , u16 );		//0x0811 spec 2.1

//host controller & baseband
void hcmd_set_event_mask(u8 *mask);				//0x0C01
void hcmd_reset(void);							//0x0C03
void hcmd_set_event_filter(u8 filter,u8 type,u8 *pCondition);	//0x0C05
void hcmd_flush(u16 conn_hdl);						//0x0C08
void hcmd_r_pin_type(void);						//0x0C09
void hcmd_w_pin_type(u8 pin_type);					//0x0C0A
void hcmd_create_new_unit_key(void);					//0x0C0B
void hcmd_r_stored_link_key(u8 *bd_addr, u8 read_all_flag);	//0x0C0D
void hcmd_w_stored_link_key(u8 num_keys_to_write, u8 *octet);	//0x0C11
void hcmd_del_stored_link_key(u8 *bd_addr, u8 del_all_flag);	//0x0C12
void hcmd_w_local_name(u8 *pName);				//0x0C13
void hcmd_r_local_name(void);						//0x0C14
void hcmd_r_conn_accept_timeout(void);					//0x0C15
void hcmd_w_conn_accept_timeout(u16 timeout);				//0x0C16
void hcmd_r_page_timeout(void);						//0x0C17
void hcmd_w_page_timeout(u16 page_timeout);				//0x0C18
void hcmd_r_scan_enable(void);						//0x0C19
void hcmd_w_scan_enable(u8 scan_enable);				//0x0C1A
void hcmd_r_page_scan_activity(void);					//0x0C1B
void hcmd_w_page_scan_activity(u16 interval,u16 window);		//0x0C1C
void hcmd_r_inq_scan_activity(void);					//0x0C1D
void hcmd_w_inq_scan_activity(u16 interval,u16 window);			//0x0C1E
void hcmd_r_auth_enable(void);						//0x0C1F
void hcmd_w_auth_enable(u8 auth_enable);				//0x0C20
void hcmd_r_encryption_mode(void);					//0x0C21 (deprecated in spec 2.1)
void hcmd_w_encryption_mode(u8 encr_mode);				//0x0C22 (deprecated in spec 2.1)
void hcmd_r_class_of_device(void);					//0x0C23
void hcmd_w_class_of_device(u8 *pCoD);				//0x0C24
void hcmd_r_voice_setting(void);					//0x0C25
void hcmd_w_voice_setting(u8 setting70, u8 setting98);			//0x0C26
void hcmd_r_automatic_flush_timeout(u16 hdl);				//0x0C27
void hcmd_w_automatic_flush_timeout(u16 hdl, u16 TO);			//0x0C28
void hcmd_r_num_broadcast_retransmit(void);				//0x0C29
void hcmd_w_num_broadcast_retransmit(u8);				//0x0C2A
void hcmd_r_hold_mode_activity_suspended(void);				//0x0C2B
void hcmd_w_hold_mode_activity_suspended(u8);				//0x0C2C
void hcmd_r_transmit_power_level(u16 hdl,u8 type);			//0x0C2D
void hcmd_r_synchronous_flow_ctrl_enable(void);				//0x0C2E
void hcmd_w_synchronous_flow_ctrl_enable(u8 enable);			//0x0C2F
void hcmd_set_hc_2_host_flow_ctrl(u8 enable);				//0x0C31
void hcmd_host_buffer_size(u16, u8, u16, u16);				//0x0C33
void hcmd_host_num_of_completed_pkts(u8 num_of_hdl, u8 *);	//0x0C35
void hcmd_r_link_supervision_timeout(u16 hdl);				//0x0C36
void hcmd_w_link_supervision_timeout(u16 hdl, u16 TO);			//0x0C37
void hcmd_r_num_of_support_iac(void);					//0x0C38
void hcmd_r_current_iac_lap(void);					//0x0C39
void hcmd_w_current_iac_lap(u8 num_current_iac, u8 *iac_lap);	//0x0C3A
void hcmd_r_page_scan_period_mode(void);				//0x0C3B
void hcmd_w_page_scan_period_mode(u8 SP);				//0x0C3C
void hcmd_r_page_scan_mode(void);					//0x0C3D
void hcmd_w_page_scan_mode(u8 page_scan_mode);				//0x0C3E
void hcmd_set_AFH_host_channel_classification(u8 *chanclass);	//0x0C3F spec 1.2
void hcmd_r_inq_scan_type(void);					//0x0C42 spec 1.2
void hcmd_w_inq_scan_type(u8 scan_type);				//0x0C43 spec 1.2
void hcmd_r_inquiry_mode(void);						//0x0C44 spec 1.2
void hcmd_w_inquiry_mode(u8 inq_mode);					//0x0C45 spec 1.2
void hcmd_r_page_scan_type(void);					//0x0C46 spec 1.2
void hcmd_w_page_scan_type(u8 scan_type);				//0x0C47 spec 1.2
void hcmd_r_channel_assessment_mode(void);				//0x0C48 spec 1.2
void hcmd_w_channel_assessment_mode(u8 mode);				//0x0C49 spec 1.2
void hcmd_read_extended_inquiry_response(void);				//0x0C51 spec 2.1
void hcmd_write_extended_inquiry_response(u8 fec_required, u8 *);	//0x0C52 spec 2.1
void hcmd_refresh_encryption_key(u16 hdl);				//0x0C53 spec 2.1
void hcmd_r_simple_pairing_mode(void);					//0x0C55 spec 2.1
void hcmd_w_simple_pairing_mode(u8 mode);				//0x0C56 spec 2.1
void hcmd_r_local_oob_data(void);					//0x0C57 spec 2.1
void hcmd_r_inquiry_response_transmit_power_level(void);		//0x0C58 spec 2.1
void hcmd_w_inquiry_transmit_power_level(u8 tx_power);			//0x0C59 spec 2.1
void hcmd_r_default_erroneous_data_reporting(void);			//0x0C5A spec 2.1
void hcmd_w_default_erroneous_data_reporting(u8 reporting);		//0x0C5B spec 2.1
void hcmd_enhanced_flush(u16 hdl, u8 pkt_type);				//0x0C5F spec 2.1
void hcmd_send_keypress_notification(u8 *bd_addr, u8 type);	//0x0C60 spec 2.1

//information parameters
void hcmd_r_loc_ver_info(void);						//0x1001
void hcmd_r_loc_support_commands(void);					//0x1002 spec 1.2
void hcmd_r_loc_support_features(void);					//0x1003
void hcmd_r_local_extended_features(u8 page_number);			//0x1004 spec 1.2
void hcmd_r_buffer_size(void);						//0x1005
void hcmd_r_country_code(void);						//0x1007 (deprecated in spec 1.2)
void hcmd_r_bd_addr(void);						//0x1009

//status parameters
void hcmd_r_failed_contact_counter(u16 hdl);				//0x1401
void hcmd_reset_failed_contact_counter(u16 hdl);			//0x1402
void hcmd_get_link_quality(u16 hdl);					//0x1403
void hcmd_read_rssi(u16 hdl);						//0x1405
void hcmd_read_AFH_channel_map(u16 hdl);				//0x1406
void hcmd_read_clock(u16 hdl, u8 which_clock);				//0x1407

//testing
void hcmd_r_loopback_mode(void);					//0x1801
void hcmd_w_loopback_mode(u8 mode);					//0x1802
void hcmd_enable_device_under_test_mode(void);				//0x1803
void hcmd_w_simple_pairing_debug_mode(u8 mode);				//0x1804 spec 2.1
void hcmd_w_test_control(u8 ctrl[]);
void hcmd_m_test_control(u8 len, u8 ctrl[]);
void hcmd_w_conn_single_freq(u8 flags, u8 TRFreq);//2400+...MHz
void hcmd_fcc_test(u8 len_params, u8 *ctrl);

/// EOF ///
