/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : 主程序体
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/*
移植自LwGPS Implementation for Ublox-NEO6M on STM32F4   https://github.com/Squieler/NEO6M---LwGPS---STM32-Example
这是为由 Tilen MAJERLE 开发的 LwGPS NMEA 句子解析库的一个示例文件：https://github.com/MaJerle/lwgps
这个示例配置为使用 Ublox-NEO6M 来收集信息：时间、纬度、经度、海拔和速度。
有关库的更多信息，请查看文档：https://docs.majerle.eu/projects/lwgps/en/latest/
示例用法
在示例项目中，USART3 外设配置为：
波特率：9600 比特/秒
DMA 设置：USART1_RX，peripheral to memory, circular mode with high priority
NVIC 设置：启用 USART1 全局中断，其余设置为默认。如果遇到任何问题，请检查示例项目。
USART2 外设配置为向计算机传输消息。（已被注释）
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "lwgps/lwgps.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h> // 用于变长参数函数
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SIZE 200 // 定义缓冲区大小
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t gpsRx[SIZE]; // GPS接收缓冲区
uint8_t flag = 0;    // 标志位

/* GPS句柄 */
lwgps_t hgps;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* 如果huart1缓冲区已满，标记flag */
  flag = 1;
}

void serialPrint(const char *fmt, ...)
{
  static char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);
  // HAL_UART_Transmit(&huart2, (uint8_t*) buffer, strlen(buffer), 15); // 通过huart2与计算机进行串行通信。
}

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
  MX_DMA_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  /* 接收缓冲区 */
  memset(gpsRx, 0, SIZE); // 清空缓冲区。

  /* 启动DMA；循环模式 */
  HAL_UART_Receive_DMA(&huart3, gpsRx, SIZE);

  /* 初始化GPS */
  lwgps_init(&hgps);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //注释在这一页的最上面
    if (flag == 1)
    {

      /* 如果缓冲区已满，处理接收到的数据 */
      lwgps_process(&hgps, gpsRx, SIZE);

      /* 打印信息 */
      serialPrint("---\r\n");
      serialPrint("Valid status: %d\r\n", hgps.is_valid);                              // 有效状态
      serialPrint("Time: %02d:%02d:%02d\r\n", hgps.hours, hgps.minutes, hgps.seconds); // 时间
      serialPrint("Latitude: %f degrees\r\n", hgps.latitude);                          // 纬度
      serialPrint("Longitude: %f degrees\r\n", hgps.longitude);                        // 经度
      serialPrint("Altitude: %f meters\r\n", hgps.altitude + hgps.geo_sep);            // 海拔
      serialPrint("Speed: %.2f km/h\r\n", hgps.speed * 1.852);                         // 速度

      flag = 0;
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  /* 用户可以添加自己的实现来报告HAL错误返回状态 */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
  /* 用户可以添加自己的实现来报告文件名和行号，
     例如: printf("错误参数值: 文件 %s 在行 %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
