//===========================Button========================================================

// AVRCP value
#define BUTTON_VOL_UP					0x41
#define BUTTON_VOL_DOWN					0x42
#define BUTTON_START					0x44
#define BUTTON_STOP						0x45
#define BUTTON_PAUSE					0x46
#define BUTTON_FORWARD					0x4B
#define BUTTON_BACKWARD					0x4C


#define AVCTP_STATE_IDLE								0x00
#define AVCTP_STATE_TX_BUTTON_PUSHED_CMD				0x01
#define AVCTP_STATE_W4_RX_BUTTON_PUSHED_RES				0x02
#define AVCTP_STATE_TX_BUTTON_RELEASED_CMD				0x03
#define AVCTP_STATE_W4_RX_BUTTON_RELEASED_RES			0x04


u8 AVCTPReceiveData(u16 cid, u16 len, u8 *Data);
bool SendAVCRPButtonPushedReleased(u8 button);
bool IsEmptyButton(void);
