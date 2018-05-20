#ifndef USER_FFT_LED
#define USER_FFT_LED
extern void timer3_init(void);
extern void fft_led_init(void);
extern __near_func void fft_led_scan(void); //系统时钟中5ms调用一次。
extern __near_func void fft_energy_get(void);
#endif
