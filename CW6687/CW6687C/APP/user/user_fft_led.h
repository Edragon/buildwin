#ifndef USER_FFT_LED
#define USER_FFT_LED
extern void timer3_init(void);
extern void fft_led_init(void);
extern __near_func void fft_led_scan(void); //ϵͳʱ����5ms����һ�Ρ�
extern __near_func void fft_energy_get(void);
#endif
