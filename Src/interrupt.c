/*
 * interrupt.c
 *
 *  Created on: 2019/09/15
 *      Author: kt33_
 */

#include "main.h"
#include "variable.h"
#include "interrupt.h"
#include "tim.h"
#include "stm32f4xx_hal_tim.h"
#include "other.h"
#include "adc.h"
#include "moter.h"

void buzzer_1ms(void);
void adc_1ms(void);

void interrupt_1ms(void) {

//buzzer
	buzzer_1ms();
//buzzer

//gyro
	real_rotation.vel = read_gyro();
	integlral_1ms(&real_rotation.dis, &real_rotation.vel); //角速度から角度に
//gyro

//encoder
	real_L.vel = read_vel(LEFT); //mm/sec
	real_R.vel = read_vel(RIGHT);

	integlral_1ms(&real_L.dis, &real_L.vel);
	integlral_1ms(&real_R.dis, &real_R.vel);
//encoder

//ADC
	adc_1ms();
//ADC


//	if (Batt < 3.72) {
//		low_batt_flag = 0xff;
//	} else {
//		//low_batt_flag = 0;
//	}
}

void buzzer_1ms(void) {
	if (buzzer[buzzer_index].autoreload != 0xffff && buzzer_flag == 1) {
		if (buzzer_count == 0) {
			if (buzzer[buzzer_index].autoreload != 0) {
				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
						buzzer[buzzer_index].autoreload / 2);
				__HAL_TIM_SET_AUTORELOAD(&htim3,
						buzzer[buzzer_index].autoreload);
				HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
			} else {
				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
			}
		}
		buzzer_count++;
	} else {
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
		buzzer_count = 0;
	}

	if (buzzer_count == buzzer[buzzer_index].ms) {
		buzzer_count = 0;
		buzzer_index++;
	}
}

void adc_1ms(void) {

	HAL_GPIO_WritePin(SENLED_RF_GPIO_Port, SENLED_RF_Pin, 0);
	for (uint8_t i = 0; i < 100; i++)
		;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
	HAL_GPIO_WritePin(SENLED_RF_GPIO_Port, SENLED_RF_Pin, 1);
	for (uint8_t i = 0; i < 100; i++)
		;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));

	SEN_RF.now = g_ADCBuffer[0] - g_ADCBuffer[1];
	SEN_RF_log.before_5ms = SEN_RF_log.before_4ms;
	SEN_RF_log.before_4ms = SEN_RF_log.before_3ms;
	SEN_RF_log.before_3ms = SEN_RF_log.before_2ms;
	SEN_RF_log.before_2ms = SEN_RF_log.before_1ms;
	SEN_RF_log.before_1ms = SEN_RF_log.now;
	SEN_RF_log.now = SEN_RF.now;
	SEN_RF.diff_1ms = SEN_RF_log.before_1ms;
	SEN_RF.diff = SEN_RF_log.before_3ms;

	HAL_GPIO_WritePin(SENLED_L_GPIO_Port, SENLED_L_Pin, 0);
	for (uint8_t i = 0; i < 100; i++)
		;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
	HAL_GPIO_WritePin(SENLED_L_GPIO_Port, SENLED_L_Pin, 1);
	for (uint8_t i = 0; i < 100; i++)
		;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));

	SEN_L.now = g_ADCBuffer[2] - g_ADCBuffer[3];
	SEN_L_log.before_5ms = SEN_L_log.before_4ms;
	SEN_L_log.before_4ms = SEN_L_log.before_3ms;
	SEN_L_log.before_3ms = SEN_L_log.before_2ms;
	SEN_L_log.before_2ms = SEN_L_log.before_1ms;
	SEN_L_log.before_1ms = SEN_L_log.now;
	SEN_L_log.now = SEN_L.now;
	SEN_L.diff_1ms = SEN_L_log.before_1ms;
	SEN_L.diff = SEN_L_log.before_3ms;

	HAL_GPIO_WritePin(SENLED_R_GPIO_Port, SENLED_R_Pin, 0);
	for (uint8_t i = 0; i < 100; i++)
		;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
	HAL_GPIO_WritePin(SENLED_R_GPIO_Port, SENLED_R_Pin, 1);
	for (uint8_t i = 0; i < 100; i++)
		;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));

	SEN_R.now = g_ADCBuffer[4] - g_ADCBuffer[5];
	SEN_R_log.before_5ms = SEN_R_log.before_4ms;
	SEN_R_log.before_4ms = SEN_R_log.before_3ms;
	SEN_R_log.before_3ms = SEN_R_log.before_2ms;
	SEN_R_log.before_2ms = SEN_R_log.before_1ms;
	SEN_R_log.before_1ms = SEN_R_log.now;
	SEN_R_log.now = SEN_R.now;
	SEN_R.diff_1ms = SEN_R_log.before_1ms;
	SEN_R.diff = SEN_R_log.before_3ms;

	HAL_GPIO_WritePin(SENLED_LF_GPIO_Port, SENLED_LF_Pin, 0);
	for (uint8_t i = 0; i < 100; i++)
		;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
	HAL_GPIO_WritePin(SENLED_LF_GPIO_Port, SENLED_LF_Pin, 1);
	for (uint8_t i = 0; i < 100; i++)
		;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));

	SEN_LF.now = g_ADCBuffer[6] - g_ADCBuffer[7];
	SEN_LF_log.before_5ms = SEN_LF_log.before_4ms;
	SEN_LF_log.before_4ms = SEN_LF_log.before_3ms;
	SEN_LF_log.before_3ms = SEN_LF_log.before_2ms;
	SEN_LF_log.before_2ms = SEN_LF_log.before_1ms;
	SEN_LF_log.before_1ms = SEN_LF_log.now;
	SEN_LF_log.now = SEN_LF.now;
	SEN_LF.diff_1ms = SEN_LF_log.before_1ms;
	SEN_LF.diff = SEN_LF_log.before_3ms;

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
	Batt = (float) g_ADCBuffer[8] / 4095 * 3.3 * 2;
}
