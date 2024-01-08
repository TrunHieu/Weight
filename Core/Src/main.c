/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
  HX711:
  DT-PA9
  SCK PA8

  LCD-I2C:
  SDA-PB7
  SCL-PB6

  keypad: PA0->PA7
  Coi chip: PC15
  LED_LCD: PB0

#include "main.h"
#include <stdio.h>
#include "hx711.h"
#include "i2c-lcd.h"
#include "Keypad.h"
#include <stdlib.h>

  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include "hx711.h"
#include "i2c-lcd.h"
#include "Keypad.h"
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
float weight = 0.00;
hx711_t loadcell;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define R1_Port GPIOA           //row1 port (ngang)
#define R1_Pin GPIO_PIN_7		//row1 Pin

#define R2_Port GPIOA
#define R2_Pin GPIO_PIN_6

#define R3_Port GPIOA
#define R3_Pin GPIO_PIN_5

#define R4_Port GPIOA
#define R4_Pin GPIO_PIN_4

#define C1_Port GPIOA
#define C1_Pin GPIO_PIN_3

#define C2_Port GPIOA
#define C2_Pin GPIO_PIN_2

#define C3_Port GPIOA
#define C3_Pin GPIO_PIN_1

#define C4_Port GPIOA
#define C4_Pin GPIO_PIN_0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
char LCD_send[10];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t key = 0;
uint8_t shift = 8; // Biến khai báo vị trí đàu tiên của giá ti�?n
uint8_t buffer[10];
uint32_t Total = 0;
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  hx711_init(&loadcell, GPIOA, GPIO_PIN_8, GPIOA, GPIO_PIN_9);
  hx711_coef_set(&loadcell, 478); // read afer calibration 475
  hx711_tare(&loadcell, 10);

  lcd_init();
  keypad_init();

  lcd_gotoxy(2,0);
  lcd_send_string("ELECTRONIC SCALE");

  lcd_gotoxy(0,1);
  lcd_send_string("Weight:");

  lcd_gotoxy(0,2);
  lcd_send_string("Price :");
  lcd_gotoxy(17,2);
  lcd_send_string("VND");

  lcd_gotoxy(0,3);
  lcd_send_string("Total :");
  lcd_gotoxy(17,3);
  lcd_send_string("VND");
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
/*LoadCell-HX711-----------------------------------------------------------------*/
	  weight = hx711_weight(&loadcell, 10);
	  if(weight < 1)weight = 0.00;
	  {
		  lcd_gotoxy(8,1);
		  sprintf(LCD_send, "%.3f", weight);  //weight
		  lcd_send_string(LCD_send);
		  lcd_gotoxy(14,1);
		  lcd_send_string("   Gam");
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,0);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,0);
	  }
	  if(weight > 1)
	  {
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,1);
	  }
	  if(weight > 3000)
	  {
		  lcd_gotoxy(8,1);
		  lcd_send_string("OVERLOAD    ");
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,1);
	  }
/*LCD-Keypad 4x4 ------------------------------------------------------------------*/
	  buffer[key] = keypad_scan();
	  if(buffer[key] != 0 && buffer[key] != 'A' && buffer[key] != 'B'&& buffer[key] != 'C')
	  {
		  lcd_gotoxy(0,2);
		  lcd_send_string("Price :");
		  lcd_gotoxy(shift, 2);
		  lcd_send_data(buffer[key]);
		  shift++;						 //Tăng giá trị vị trí hiển thị số tiếp theo
		  if(shift > 19) shift = 8;
		  key++;						 //Tăng giá trị phần tử mảng sau khi nhấn nút
	  }
/*Trừ bì khi nhấn nút A--------------------------------------------------------------*/
	  if(buffer[key] == 'A')
	  {
		  NVIC_SystemReset();
	  }
/* Xóa giá tri khi nhấn nút B-------------------------------------------------------*/
	  if(buffer[key] == 'B')
	  {
		  shift = 8;					// dua con tro vi tri gia tri dau tien 7
		  for(int i = 0; i< 10; i ++)	//xóa dữ liệu trong mảng buffer[]
		  {
			  buffer[i] = 0;
		  }
		  key = 0;						// Xóa giá trị phần tử
		  lcd_gotoxy(0,2);
		  lcd_send_string("Price :          ");
		  lcd_gotoxy(0, 3);
		  lcd_send_string("Total :          ");
	  }
/*Tính tổng khi nhấn nút C-----------------------------------------------------------*/
	  if(buffer[key] == 'C')
	  {
		  //buffer[0] - 48 là để chuyển giá trị char thành giá trị int (Tham khảo bảng mã ASCII)
		  Total = (buffer[0] - 48)*10000 + (buffer[1] - 48)*1000+ (buffer[2] - 48)*100 + (buffer[3] - 48)*10 + (buffer[4] - 48)*1;
		  Total = Total * weight;
		  sprintf(LCD_send,"%ld",Total);
		  lcd_gotoxy(0, 3);
		  lcd_send_string("Total : ");
		  lcd_gotoxy(8, 3);
		  lcd_send_string(LCD_send);
	  }
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7
                           PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
