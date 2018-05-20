#ifndef _RTC6207_H_
#define _RTC6207_H_

#include "include.h"

extern void RTC6207_init(void);
extern void RTC6207_set_freq(u16 TunerFrequency);
extern void RTC6207_set_vol(u8 vol);
extern void RTC6207_off(void);
extern u8 RTC6207_seek(u16 Frequency);
extern u8 RTC6207_online(void);

#ifndef _RTC62XX_H_
#define _RTC62XX_H_
typedef enum {
          WRITE,
          READ
}T_OPERA_MODE;
#endif

#endif
