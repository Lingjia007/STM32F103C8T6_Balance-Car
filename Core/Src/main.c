/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
u8 Way_Angle = 2;                                                                                                // 获取角度的算法，1：四元数  2：卡尔曼  3：互补滤波
u8 Flag_front, Flag_back, Flag_Left, Flag_Right, Flag_velocity = 2;                                              // 蓝牙遥控相关的变量
u8 Flag_Stop = 0, Flag_Show = 0;                                                                                 // 电机停止标志位和显示标志位  默认停止 显示打开
int Motor_Left, Motor_Right;                                                                                     // 电机PWM变量 应是Motor的 向Moto致敬
float Temperature;                                                                                               // 温度变量
int Voltage, Middle_angle;                                                                                       // 电池电压采样相关的变量
float Angle_Balance, Gyro_Balance, Gyro_Turn;                                                                    // 平衡倾角 平衡陀螺仪 转向陀螺仪
u8 LD_Successful_Receive_flag;                                                                                   // 雷达成功接收数据标志位
u8 Mode = 0;                                                                                                     // 模式选择，默认是普通的控制模式
u8 CCD_Zhongzhi, CCD_Yuzhi;                                                                                      // CCD中值和阈值
u16 ADV[128] = {0};                                                                                              // 存放CCD的数据的数组
u16 determine;                                                                                                   // 雷达跟随模式的一个标志位
float Move_X, Move_Z;                                                                                            // 遥控控制的速度
u32 Distance;                                                                                                    // 超声波测距
u8 PID_Send;                                                                                                     // 调参相关变量
u8 Flag_follow = 0, Flag_avoid = 0;                                                                              // 超声波跟随、超声波壁障标志位
float Acceleration_Z;                                                                                            // Z轴加速度计
volatile u8 delay_flag, delay_50;                                                                                // 提供延时的变量
float Balance_Kp = 25500, Balance_Kd = 135, Velocity_Kp = 16000, Velocity_Ki = 80, Turn_Kp = 4200, Turn_Kd = 60; // PID参数（放大100倍）
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  OLED_HAL_Init(); // 初始化OLED
  OLED_HAL_Clear();
  delay_init();
  // MPU6050_initialize(); // MPU6050初始化
  // DMP_Init();           // 初始化DMP

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // OLED_HAL_ShowString(0, 0, (uint8_t *)"ABC", 8, 1);   // 6*8 “ABC”
    // OLED_HAL_ShowString(0, 8, (uint8_t *)"DEF", 12, 1);  // 6*12 “ABC”
    // OLED_HAL_ShowString(0, 20, (uint8_t *)"GHI", 16, 1); // 8*16 “ABC”
    // OLED_HAL_ShowString(0, 36, (uint8_t *)"JKL", 24, 1); // 12*24 “ABC”
    // OLED_HAL_Refresh();
    // printf("OLED_HAL_ShowString\r\n");
    char voltage_str[20];
    Voltage = GetADC_Power() * 330 * 10.97 / 4096;
    sprintf(voltage_str, "%d\r\n", Voltage);
    HAL_UART_Transmit(&huart3, (uint8_t *)voltage_str, strlen(voltage_str), 1000);
    delay_ms(500);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
