#ifndef  _LED_7P7S_H_
#define  _LED_7P7S_H_


/**** 7p7s“˝Ω≈”≥…‰ ****/
// ---------------------------------------------
// | PIN0  PIN1  PIN2  PIN3  PIN4  PIN5  PIN6  |
// |  |     |     |     |     |     |     |    |
// | P23   P24   P25   P26   P30   P31   P32   |
// ---------------------------------------------


void led_7p7s_init(void);
void led_7p7s_value_set(void);
__near_func void led_7p7s_scan(void);


#endif
