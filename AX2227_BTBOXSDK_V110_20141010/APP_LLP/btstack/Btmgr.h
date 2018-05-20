#ifndef __BTMGR_H__
#define __BTMGR_H__

#include "global.h"

#define tmr100msGetCurrentTicks()       tmr100ms_current_ticks
#define tmr100msIsExpired(offset_ticks, Texpire)    ((u16)((tmr100ms_current_ticks)+(u16)(0x00010000-(offset_ticks))) >= (Texpire))

void HciReceiveEvent(u8 evtcode,u8 len,u8 __xdata * parm);
bool DB_Init(void);
u8* GetCurrentRemoteDev(void);
void DB_saveVol(void);

#endif
