/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ringbuff.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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

//用于串口接收
uint8_t recv_data = 0;

//用于存储从缓冲区读取出的数据
uint8_t read_data = 0;

//用于串口1的ringbuff句柄
ringbuff_t usart1_ringbuff;

//开辟一块内存用于缓冲区
#define USART1_BUFFDATA_SIZE 150
uint8_t usart1_buffdata[USART1_BUFFDATA_SIZE];

#define USER_BUFFDATA_SIZE  10
uint8_t User_BuffData[USER_BUFFDATA_SIZE];

/* USER CODE END 0 */
uint8_t Data_Flag = 0;
uint8_t buff_len;
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
   uint8_t len;
	
   uint8_t data_len;
	uint8_t i;
	uint8_t j;
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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */


 printf("ringbuff Port By FJT\r\n");

//初始化ringbuff句柄
if(1 != ringbuff_init(&usart1_ringbuff, (uint8_t*)usart1_buffdata, USART1_BUFFDATA_SIZE))
{
 printf("usart1 ringbuff init fail.\r\n");
}

//使能串口中断接收
HAL_UART_Receive_IT(&huart2, (uint8_t*)&recv_data, 1);
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_4);
	   HAL_Delay(100);
	  HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
	  	
		buff_len = sizeof(User_BuffData);

    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
	while((len = ringbuff_read(&usart1_ringbuff, &User_BuffData, sizeof(User_BuffData))) > 0)
	{
	
		/* 捕获起始标志 */
		if(User_BuffData[0] == 0x3F)
		{
			/*
			//读取数据字节数，最大支持0xFF
			if((len = ringbuff_read(&usart1_ringbuff, &User_BuffData, sizeof(User_BuffData))) > 0)
			{
				data_len = User_BuffData[0];
				printf("your data has %d byte(s):\r\n\t", data_len);
			}
			*/
			//提取data_len个数据
			for(i = 2; i < User_BuffData[1]; i++)
			{
				ringbuff_read(&usart1_ringbuff, &User_BuffData, sizeof(User_BuffData));	
			}
			for (j =0;j<10;j++)
			{
			
			printf("[%d]",User_BuffData[j]);
			}
			printf("over\r\n");
		}
	}
 HAL_Delay(200);


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* 中断回调函数 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* 判断是哪个串口触发的中断 */
    if(huart ->Instance == USART2)
    {
  /* 将接收到的数据写入缓冲区 */
  ringbuff_write(&usart1_ringbuff, &recv_data, 1);
		Data_Flag = 1;
        //重新使能串口接收中断
        HAL_UART_Receive_IT(huart, (uint8_t*)&recv_data, 1);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
