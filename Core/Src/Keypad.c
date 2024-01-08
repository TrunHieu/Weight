/*
 * Keypad.c
 *
 *  Created on: Jan 5, 2024
 *      Author: admin
 */
#include "Keypad.h"

#define ROW_1_Pin GPIO_PIN_7
#define ROW_2_Pin GPIO_PIN_6
#define ROW_3_Pin GPIO_PIN_5
#define ROW_4_Pin GPIO_PIN_4
#define COL_1_Pin GPIO_PIN_3
#define COL_2_Pin GPIO_PIN_2
#define COL_3_Pin GPIO_PIN_1
#define COL_4_Pin GPIO_PIN_0

GPIO_TypeDef* ROW_1_Port = GPIOA;
GPIO_TypeDef* ROW_2_Port = GPIOA;
GPIO_TypeDef* ROW_3_Port = GPIOA;
GPIO_TypeDef* ROW_4_Port = GPIOA;
GPIO_TypeDef* COL_1_Port = GPIOA;
GPIO_TypeDef* COL_2_Port = GPIOA;
GPIO_TypeDef* COL_3_Port = GPIOA;
GPIO_TypeDef* COL_4_Port = GPIOA;


void keypad_init(void)
{
  // Configure GPIO pins for keypad matrix
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = ROW_1_Pin | ROW_2_Pin | ROW_3_Pin | ROW_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ROW_1_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(ROW_2_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(ROW_3_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(ROW_4_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = COL_1_Pin | COL_2_Pin | COL_3_Pin | COL_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(COL_1_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_2_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_3_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_4_Port, &GPIO_InitStruct);
}


char keypad_scan(void)
{
  char keys[4][4] = {{'1', '2', '3', 'A'},
                     {'4', '5', '6', 'B'},
                     {'7', '8', '9', 'C'},
                     {'*', '0', '#', 'D'}};

  for(int i = 0; i < 4; i++)
  {
    // Set current column as output and low
    switch(i)
    {
      case 0:
        HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_SET);
        break;

      case 1:
        HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_SET);
		break;

	  case 2:
		HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_SET);
		break;

	  case 3:
		HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_RESET);
		break;
	}
	// Read current rows
	if(HAL_GPIO_ReadPin(ROW_1_Port, ROW_1_Pin) == GPIO_PIN_RESET){
		while(HAL_GPIO_ReadPin(ROW_1_Port, ROW_1_Pin) == GPIO_PIN_RESET){

		}
		return keys[0][i];
	}

	if(HAL_GPIO_ReadPin(ROW_2_Port, ROW_2_Pin) == GPIO_PIN_RESET){
		while(HAL_GPIO_ReadPin(ROW_2_Port, ROW_2_Pin) == GPIO_PIN_RESET){

		}
		return keys[1][i];
	}

	if(HAL_GPIO_ReadPin(ROW_3_Port, ROW_3_Pin) == GPIO_PIN_RESET){
		while(HAL_GPIO_ReadPin(ROW_3_Port, ROW_3_Pin) == GPIO_PIN_RESET){

		}
		return keys[2][i];
	}

	if(HAL_GPIO_ReadPin(ROW_4_Port, ROW_4_Pin) == GPIO_PIN_RESET){
		while(HAL_GPIO_ReadPin(ROW_4_Port, ROW_4_Pin) == GPIO_PIN_RESET){

		}
		return keys[3][i];
	}

	}
	return 0; // No key pressed
}


