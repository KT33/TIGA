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

void interrupt_1ms(void) {

//buzzer
	buzzer_1ms();
//buzzer

//gyro
	real_rotation.vel=read_gyro();
	integlral_1ms(&real_rotation.dis, &real_rotation.vel);//角速度から角度に
//gyro

//encoder
	real_L.vel=read_vel(LEFT);
	real_R.vel=read_vel(RIGHT);

	integlral_1ms(&real_L.dis, &real_L.vel);
	integlral_1ms(&real_R.dis, &real_R.vel);
//encoder


	//ADC
	HAL_GPIO_WritePin(SENLED_RF_GPIO_Port, SENLED_RF_Pin, 0);
	for(uint8_t i=0;i<100;i++);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
	HAL_GPIO_WritePin(SENLED_RF_GPIO_Port, SENLED_RF_Pin, 1);
	for(uint8_t i=0;i<100;i++);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));

	HAL_GPIO_WritePin(SENLED_L_GPIO_Port, SENLED_L_Pin, 0);
	for(uint8_t i=0;i<100;i++);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
	HAL_GPIO_WritePin(SENLED_L_GPIO_Port, SENLED_L_Pin, 1);
	for(uint8_t i=0;i<100;i++);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));

	HAL_GPIO_WritePin(SENLED_R_GPIO_Port, SENLED_R_Pin, 0);
	for(uint8_t i=0;i<100;i++);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
	HAL_GPIO_WritePin(SENLED_R_GPIO_Port, SENLED_R_Pin, 1);
	for(uint8_t i=0;i<100;i++);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));

	HAL_GPIO_WritePin(SENLED_LF_GPIO_Port, SENLED_LF_Pin, 0);
	for(uint8_t i=0;i<100;i++);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
	HAL_GPIO_WritePin(SENLED_LF_GPIO_Port, SENLED_LF_Pin, 1);
	for(uint8_t i=0;i<100;i++);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));

//	printf("%d,%d,%d,%d,%d\n", g_ADCBuffer[0], g_ADCBuffer[1], g_ADCBuffer[2],
//			g_ADCBuffer[3], g_ADCBuffer[4]);
//	if ((Batt < 3.72)&&(low_batt_flag<1000)) {
//		low_batt_flag++;
//		if(low_batt_flag>1){
//			low_batt_flag=0xff;
//		}
//	} else {
//	//	low_batt_flag = 0;
//	}
	if (Batt < 3.72) {
		low_batt_flag = 0xff;
	} else {
		low_batt_flag = 0;
	}
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
