
#ifndef _BTMMI_HEADER
#define _BTMMI_HEADER

enum {
  BTMMI_SKIP = 0,
  BTMMI_IDLE,  
  BTMMI_PHONE,
  BTMMI_MP3,  
  BTMMI_ALL,
};

void btMMI_Phone();
u8 btMMI_State();

#endif

