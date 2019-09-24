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

void interrupt_1ms(void) {

	//buzzer
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

	read_gyro();

<<<<<<< HEAD
	g_test=read_spi_en(LEFT, 0xFFFc);
=======
	g_test = read_spi_en(LEFT, 0xFFFf);
>>>>>>> 9bde7ac4382873975e1aa609f10b650c56efe70c

	//ADC
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
			sizeof(g_ADCBuffer) / sizeof(uint16_t));
//	if ((Batt < 3.72)&&(low_batt_flag<1000)) {
//		low_batt_flag++;
//		if(low_batt_flag>1){
//			low_batt_flag=0xff;
//		}
//	} else {
//	//	low_batt_flag = 0;
//	}
	if(Batt<3.72){
		low_batt_flag=0xff;
	}else{
		low_batt_flag=0;
	}
}

