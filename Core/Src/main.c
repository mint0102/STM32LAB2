/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_0	0
#define LED_1	1
#define LED_2	2
#define LED_3	3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t status = LED_0;
uint8_t led_flag = 0;
const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {1, 2, 3, 4};
int hour = 15, minute = 8, second = 50;
const uint16_t pattern[8] = {0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF};
const int MAX_LED_MATRIX = 8;
int index_led_matrix = 0;
int col = 0;
uint8_t char_A[8] = {0xff,0x01,0x00,0xcc,0xcc,0x00,0x01,0xff};
uint8_t led[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
uint16_t pins_to_set[] = {GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void display7SEG(int num);
void update7SEG(int index);
void displayLEDMatrix(uint8_t matrix_buffer[8]);
void updateLEDMatrix(int index);
void testLedmatrix(uint16_t *matrixBuffer);
void scrollText(uint8_t text[8]);
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
  MX_TIM2_Init();
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  setTimer(1, 25);
  setTimer(2, 50);
  setTimer(3, 100);
  setTimer(4, 1);
  while (1)
  {
	  displayLEDMatrix(char_A);
	  if(timer_flag[1] == 1){
		  setTimer(1, 25);
		  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		  if(index_led < MAX_LED){
			  update7SEG(index_led++);
		  }
		  if(index_led >= MAX_LED){
			  index_led = 0;
		  }
	  }
	  if(timer_flag[2] == 1){
		  setTimer(2, 50);
		  HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
	  }
	  if(timer_flag[3] == 1){
		  setTimer(3, 100);
		  scrollText(char_A);
		  second++;
		  if (second >= 60){
			  second = 0;
			  minute++;
		  }
		  if(minute >= 60){
			  minute = 0;
			  hour++;
		  }
		  if(hour >=24){
			  hour = 0;
		  }
		  updateClockBuffer();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback ( TIM_HandleTypeDef * htim ){
	timerRun(1);
	timerRun(2);
	timerRun(3);
	timerRun(4);
}

void display7SEG(int num){
	switch(num){
	case 0:
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_1_Pin|SEG_2_Pin|SEG_3_Pin|SEG_4_Pin|SEG_5_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG_6_Pin,GPIO_PIN_SET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, SEG_1_Pin|SEG_2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_3_Pin|SEG_4_Pin|SEG_5_Pin|SEG_6_Pin,GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_1_Pin|SEG_3_Pin|SEG_4_Pin|SEG_5_Pin|SEG_6_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG_2_Pin|SEG_5_Pin,GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_1_Pin|SEG_2_Pin|SEG_3_Pin|SEG_6_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG_4_Pin|SEG_5_Pin,GPIO_PIN_SET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOB, SEG_1_Pin|SEG_2_Pin|SEG_5_Pin|SEG_6_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_3_Pin|SEG_4_Pin,GPIO_PIN_SET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_2_Pin|SEG_3_Pin|SEG_5_Pin|SEG_6_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG_1_Pin|SEG_4_Pin,GPIO_PIN_SET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_2_Pin|SEG_3_Pin|SEG_4_Pin|SEG_5_Pin|SEG_6_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG_1_Pin,GPIO_PIN_SET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_1_Pin|SEG_2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG_3_Pin|SEG_4_Pin|SEG_5_Pin|SEG_6_Pin,GPIO_PIN_SET);
		break;
	case 8:
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_1_Pin|SEG_2_Pin|SEG_3_Pin|SEG_4_Pin|SEG_5_Pin|SEG_6_Pin,GPIO_PIN_RESET);
		break;
	case 9:
		HAL_GPIO_WritePin(GPIOB, SEG_0_Pin|SEG_1_Pin|SEG_2_Pin|SEG_3_Pin|SEG_5_Pin|SEG_6_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG_4_Pin,GPIO_PIN_SET);
		break;
	}
}

void update7SEG(int index){
    switch (index){
        case 0:
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 0);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
			display7SEG(led_buffer[0]);
            break;
        case 1:
        	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 0);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
			display7SEG(led_buffer[1]);
            break;
        case 2:
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 0);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
			display7SEG(led_buffer[2]);
            break;
        case 3:
        	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 0);
			display7SEG(led_buffer[3]);
            break;
        default:
            break;
    }
}

void displayLEDMatrix(uint8_t matrix_buffer[8]) {
	if(timer_flag[4]){
    	setTimer(4, 1);
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, 0xFC0C);
    	// Tắt tất cả các chân
    	col = (col + 1) % MAX_LED_MATRIX;
    }
	// Bật cột (col) tương ứng bằng cách sử dụng ULN2803 với tín hiệu đầu vào bằng 0
	HAL_GPIO_WritePin(GPIOA, pins_to_set[col], GPIO_PIN_RESET);
	// Hiển thị dữ liệu của cột (matrix_buffer[col]) lên hàng (row) tương ứng
	// Sử dụng GPIO_PIN_SET cho common cathode và GPIO_PIN_RESET cho common anode
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, (matrix_buffer[col] & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, (matrix_buffer[col] & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (matrix_buffer[col] & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, (matrix_buffer[col] & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, (matrix_buffer[col] & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, (matrix_buffer[col] & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, (matrix_buffer[col] & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, (matrix_buffer[col] & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void scrollText(uint8_t text[8]){
	uint8_t temp = text[0];
	for(int i = 0; i < 8; i++){
		if(i == 7) text[7] = temp;
		else {
			text[i] = text[i+1];
		}
	}
}

void updateClockBuffer(){
	led_buffer[0] = hour/10;
	led_buffer[1] = hour%10;
	led_buffer[2] = minute/10;
	led_buffer[3] = minute%10;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
