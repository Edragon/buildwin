bool hcmd_create_connection(u8 *bd_addr/*,u16 ptype, u8 SR, u16 valid_clkoffset162, u8 allow_sw */);
bool hcmd_disconnect(u16 hdl, u8 reason);
void hcmd_w_scan_enable(u8 scan_enable);
void hcmd_authentication_request(u16 hdl);	//#ifdef _CODE_SIMPLE_PAIRING_MODE_
void hcmd_set_connection_encryption(u16 hdl);	//#ifdef _CODE_SIMPLE_PAIRING_MODE_
void hcmd_delete_stored_link_key(U8 * bd_addr);	//#ifdef _CODE_SIMPLE_PAIRING_MODE_
bool hcmd_switch_role(U8 * bd_addr, u8 new_role);	//huawei c8812 sn73
bool hcmd_role_discovery(u16 hdl);
bool hcmd_read_remote_version_info(u16 hdl);
bool hcmd_disconnect_sco(void);

bool hcmd_read_xdata(u16 addr);
bool hcmd_write_xdata(u16 addr, u8 value);	//_COVER_8202_REMOTE_NOT_ACCEPT_SNIFF_MODE_
bool hcmd_r_remote_supported_feature(u16 hdl);	//for Sony ST25i


