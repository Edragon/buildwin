#ifndef  _LED_7P7S_H_
#define  _LED_7P7S_H_

#define PIN0  BIT(6)
#define PIN1  BIT(5)
#define PIN2  BIT(4)
#define PIN3  BIT(3)
#define PIN4  BIT(2)
#define PIN5  BIT(1)
#define PIN6  BIT(0)

//7PIN 数码管扫描方式
#define SCAN_MODE_COM           0               //按COM扫描, 适用于7个IO全部有内部330R下拉, 显示更稳定
#define SCAN_MODE_SEG           1               //按SEG扫描，适用于没有330R下拉的IO, 但晃动显示屏会抖动
extern u8 pin_disp_buf[7];
void led_7p7s_init(void);
void led_7p7s_value_set(void);
__near_func void led_7p7s_scan(void);
__near_func bool led_7p7s_reuse(void);

#endif
