/*****************************************************************************
 * Module    : Task
 * File      : task_hold.h
 * Author    : Hanny
 * Function  : HOLD MODE相关定义
*****************************************************************************/
#ifndef TASK_HOLD_H
#define TASK_HOLD_H

//唤醒PORT选择
#define WKUP_P07                    0
#define WKUP_P34                    1
#define WKUP_P06                    2
#define WKUP_BT_CTS                 3
#define WKUP_P01                    4
#define WKUP_BT_CDCLK               5
#define WKUP_PORT                   WKUP_BT_CTS

//选择上升沿/下降沿唤醒
#define WKUP_RISING_EDGE            0       //上升沿
#define WKUP_FALLING_EDGE           1       //下降沿
#define WKUP_EDGE                   WKUP_FALLING_EDGE

#if (WKUP_PORT == WKUP_P07)
    #define WKUP_PIN                3
    #define WKUP_PORT_INIT()        P0DIR |= BIT(7);
    #define WKUP_PORT_PULLUP()      P0PD &= ~BIT(7); P0PU |=  BIT(7)
    #define WKUP_PORT_PULLDOWN()    P0PD |=  BIT(7); P0PU &= ~BIT(7)
#elif (WKUP_PORT == WKUP_P34)
    #define WKUP_PIN                2
    #define WKUP_PORT_INIT()        PMUXCON0 &= ~(BIT(2) | BIT(3))
    #define WKUP_PORT_PULLUP()      P3PD &= ~BIT(4); P3PU |=  BIT(4)
    #define WKUP_PORT_PULLDOWN()    P3PD |=  BIT(4); P3PU &= ~BIT(4)
#elif (WKUP_PORT == WKUP_P06)
    #define WKUP_PIN                1
    #define WKUP_PORT_INIT()        PMUXCON1 &= ~BIT(1); P0DIR |= BIT(6)
    #define WKUP_PORT_PULLUP()      P0PD &= ~BIT(6); P0PU |=  BIT(6)
    #define WKUP_PORT_PULLDOWN()    P0PD |=  BIT(6); P0PU &= ~BIT(6)
#elif (WKUP_PORT == WKUP_BT_CTS)
    #define WKUP_PIN                1
    #define WKUP_PORT_INIT()        PMUXCON1 |= BIT(1)
    #define WKUP_PORT_PULLUP()
    #define WKUP_PORT_PULLDOWN()
#elif (WKUP_PORT == WKUP_P01)
    #define WKUP_PIN                0
    #define WKUP_PORT_INIT()        PMUXCON1 &= ~BIT(0); P0DIR |= BIT(1)
    #define WKUP_PORT_PULLUP()      P0PD &= ~BIT(1); P0PU |=  BIT(1)
    #define WKUP_PORT_PULLDOWN()    P0PD |=  BIT(1); P0PU &= ~BIT(1)
#elif (WKUP_PORT == WKUP_BT_CDCLK)
    #define WKUP_PIN                0
    #define WKUP_PORT_INIT()        PMUXCON1 |= BIT(0)
    #define WKUP_PORT_PULLUP()
    #define WKUP_PORT_PULLDOWN()
#endif

#endif
