#ifndef __AT_CMD_H__
#define __AT_CMD_H__

#include "global.h"
typedef struct{
	u8 loaded;
	u8 n;
	u8 *cmd;
	u16 cid;
	u8 dlci;
}   AT_CmdCon;

bool Added_AT_Command(u16 cid, u8 dlci, u16 len, u8 Info[]);

void set_codec(void);
void At_CmdConReset(void);
void wait125us(void);
u8 get_gain(u8 Info[]);
#endif

