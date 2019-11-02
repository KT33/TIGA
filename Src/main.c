/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "stdint.h"
#include "stdio.h"
#include "variable.h"
#include "walldata.h"
#include "other.h"
#include "buzzer.h"
#include "mode.h"
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

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_ADC1_Init();
	MX_SPI2_Init();
	MX_SPI3_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM6_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim6);
	SEGGER_RTT_Init();
	SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	printf("\x1b[0m");
	init_walldata();
	HAL_GPIO_WritePin(CS_GYRO_GPIO_Port, CS_GYRO_Pin, SET);
	HAL_GPIO_WritePin(CS_L_EN_GPIO_Port, CS_L_EN_Pin, 1);
	HAL_GPIO_WritePin(CS_R_EN_GPIO_Port, CS_R_EN_Pin, 1);
	init_gyro();
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	set_led(0);
	HAL_GPIO_WritePin(SENLED_RF_GPIO_Port, SENLED_RF_Pin, SET);
	HAL_GPIO_WritePin(SENLED_LF_GPIO_Port, SENLED_LF_Pin, SET);
	HAL_GPIO_WritePin(SENLED_R_GPIO_Port, SENLED_R_Pin, SET);
	HAL_GPIO_WritePin(SENLED_L_GPIO_Port, SENLED_L_Pin, SET);
	clear_Map(&walldata.real);
	clear_Map(&walldata.checked);
	clear_adachiMap(&walldata.adachi);
	walldata.checked.row[1] = 0x1;

	HAL_Delay(10);
	Battery_Check();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	u_int16_t i, j;
//	uint8_t test = 1;
//	float pi = 3.14;

//	walldata.real.column[1]++;

//	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
//			sizeof(g_ADCBuffer) / sizeof(uint16_t));

	for (i = 0; i < 17; i++) {
		for (j = 0; j < 17; j++) {
			step_map[i][j] = i + j * 16;
		}
		printf("i=%d,pari=%d\n", i, check_parity(i));
	}

	for (i = 0; i < 50; i++) {
		enc_buff_l[i] = 0;
		enc_buff_r[i] = 0;
		acc_buff[i] = 0;
	}
	for (i = 50; i < 100; i++) {
		enc_buff_l[i] = 0;
		enc_buff_r[i] = 0;
	}

//	output_Walldata(REAL);
	mode = 0;
	mode_select_dis = 10;

	SEN_L.reference = 376;
	SEN_L.threshold = 61;
	SEN_R.reference = 394;
	SEN_R.threshold = 100;
	SEN_LF.reference = 496;
	SEN_LF.threshold = 374;
	SEN_RF.reference = 550;
	SEN_RF.threshold = 380;

	SEN_F.threshold = (int) (SEN_RF.threshold + SEN_LF.threshold) / 2 * 0.45;
	SEN_F.reference = (int) (SEN_RF.reference + SEN_LF.reference) / 2;

	run_gain.Kp = 0.6;
	run_gain.Ki = 0.15;
	rotation_gain.Kp = 0.41;
	rotation_gain.Ki = 0.005; //3
	wall_cntrol_gain.Kp = 0.05;
	wall_cntrol_gain.Kd = 0.0;

	nomal_run.vel_search = 280.0;
	nomal_run.accel = 1000.0;

	known_acc = 1000.0;
	known_vel = 350.0;

	nomal_rotation.vel_search = 750.0;
	nomal_rotation.accel = 1700.0;

	x.goal = 2;
	y.goal = 0;
	while (1) {
//		while (1) {
//			set_led(2);
//			HAL_Delay(500);
//			set_led(5);
//			HAL_Delay(500);
//		}

//		printf("mode:sel_dis=%3.2f,vel=%3.2f\n", mode_select_dis, real_R.vel);
//		printf("R=%8.2f,	L=%8.2f\n",test_R,test_L);
//		printf("%8.3f\n",real_acc);

		set_led(mode);
		if (mode_select_dis > 30) {
			mode_select_dis = 0;
			mode++;
			if (mode >= 8) {
				mode = 0;
			}
			set_buzzer_mode(mode);
		}

		if (mode_select_dis < -30) {
			mode_select_dis = 0;
			if (mode == 0) {
				mode = 8;
			}
			mode--;
			set_buzzer_mode(mode);
		}

		if (HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin) == 0) {
			mode_select_dis = 0;
			set_led(0);
			set_buzzer_mode(mode);
			chattering();
			go_mode();
		}
		HAL_GPIO_TogglePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin);

//		if (HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin) == 0) {
//			mode++;
//			mode = mode % 8;
//			printf("%d\n", mode);
//			chattering();
//			set_led(mode);
//			set_buzzer_mode(mode);
//			real_rotation.dis = 0;
//
//			if (mode == 2) {
//				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
//				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
//				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 400);
//				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 400);
//				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
//				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
//			} else {
//				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
//				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
//			}
//		}
//
//		if (low_batt_flag == 0xff) {
//			//		while(1){
//			set_led(0);
//			HAL_Delay(100);
//			set_led(7);
//			HAL_Delay(100);
//			//		}
//		} else {
//			set_led(mode);
//		}
//
//		HAL_GPIO_TogglePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin);
//
//		printf("batt=%4.2f\n", Batt);
//		HAL_Delay(100);
//
//		if (mode == 0) {
////			printf("LEFT=%4.2f,RIGHT=%4.8f\n",read_vel(LEFT),read_vel(RIGHT));
//
//		} else if (mode == 1) {
////			printf("%4.2f,%4.2f\n",real_rotation.dis,real_rotation.vel);
//			printf("%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\n", g_ADCBuffer[0],
//					g_ADCBuffer[1], g_ADCBuffer[2], g_ADCBuffer[3],
//					g_ADCBuffer[4], g_ADCBuffer[5], g_ADCBuffer[6],
//					g_ADCBuffer[7], g_ADCBuffer[8]);
//		} else if (mode == 2) {
//			HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
//			HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
//			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 200);
//			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 200);
//			HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
//			HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
//		} else if (mode == 3) {
//			printf("LEFT=%4.2f,RIGHT=%4.2f\n", read_vel(LEFT), read_vel(RIGHT));
//		} else if (mode == 4) {
//
//		} else if (mode == 5) {
//
//		} else if (mode == 6) {
//
//		} else if (mode == 7) {
//
//		}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE()
	;
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 100;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
