#ifndef _RTC6218_H_
#define _RTC6218_H_

#include <include.h>

extern void RTC6218_init(void);
extern void RTC6218_set_freq(u16 TunerFrequency);
extern void RTC6218_set_vol(u8 vol);
extern void RTC6218_off(void);
extern __near_func u8 RTC6218_seek(u16 Frequency);
extern u8 RTC6218_online(void);

#ifndef _RTC62XX_H_
#define _RTC62XX_H_
typedef enum {
          WRITE,
          READ
}T_OPERA_MODE;
#endif

#endif

