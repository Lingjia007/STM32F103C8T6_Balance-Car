/**
 * @file led.c
 * @brief LED操作方法实现
 */

#include "led.h"
#include "stm32f1xx_hal.h"

// LED引脚定义（需要根据实际硬件连接修改）
#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_13
#define LED_ON_LEVEL GPIO_PIN_SET    // LED点亮时的电平（根据电路设计确定）
#define LED_OFF_LEVEL GPIO_PIN_RESET // LED熄灭时的电平

/**
 * @brief 点亮LED
 */
void LED_On(void)
{
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, LED_ON_LEVEL);
}

/**
 * @brief 关闭LED
 */
void LED_Off(void)
{
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, LED_OFF_LEVEL);
}

/**
 * @brief 切换LED状态
 */
void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}
