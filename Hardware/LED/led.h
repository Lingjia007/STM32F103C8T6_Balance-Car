/**
 * @file led.h
 * @brief LED操作头文件
 */

#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

// LED操作函数声明
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

#endif /* __LED_H */
