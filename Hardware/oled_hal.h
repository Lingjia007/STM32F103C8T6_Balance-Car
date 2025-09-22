#ifndef __OLED_HAL_H
#define __OLED_HAL_H

#include "stm32f1xx_hal.h"
#include "stdlib.h"

#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif

//-----------------OLED引脚定义----------------
#define OLED_RES_PORT    GPIOA
#define OLED_RES_PIN     GPIO_PIN_3
#define OLED_DC_PORT     GPIOA
#define OLED_DC_PIN      GPIO_PIN_2
#define OLED_CS_PORT     GPIOA
#define OLED_CS_PIN      GPIO_PIN_4

#define OLED_RES_Clr()   HAL_GPIO_WritePin(OLED_RES_PORT, OLED_RES_PIN, GPIO_PIN_RESET)
#define OLED_RES_Set()   HAL_GPIO_WritePin(OLED_RES_PORT, OLED_RES_PIN, GPIO_PIN_SET)
#define OLED_DC_Clr()    HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_RESET)
#define OLED_DC_Set()    HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_SET)
#define OLED_CS_Clr()    HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_RESET)
#define OLED_CS_Set()    HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_SET)

#define OLED_CMD  0   //写命令
#define OLED_DATA 1   //写数据

//函数声明
extern SPI_HandleTypeDef hspi1;

void OLED_HAL_WR_Byte(u8 dat, u8 mode);
void OLED_HAL_DisPlay_On(void);
void OLED_HAL_DisPlay_Off(void);
void OLED_HAL_Refresh(void);
void OLED_HAL_Clear(void);
void OLED_HAL_DrawPoint(u8 x, u8 y, u8 t);
void OLED_HAL_ShowChar(u8 x, u8 y, u8 chr, u8 size1, u8 mode);
void OLED_HAL_ShowString(u8 x, u8 y, u8 *chr, u8 size1, u8 mode);
void OLED_HAL_Init(void);

#endif
