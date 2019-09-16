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

void interrupt_1ms(void) {

	//buzzer
	if (buzzer_count != 0xffff) {
		if (buzzer_count == 0) {
			HAL_TIM_PWM_Stop(&htim6, TIM_CHANNEL_2);
			if (buzzer[buzzer_index].freq == 0) {
				__HAL_TIM_SET_COMPARE(&htim6, TIM_CHANNEL_1, 0);
				__HAL_TIM_SET_AUTORELOAD(&htim6, 0);
			} else
				__HAL_TIM_SET_COMPARE(&htim6, TIM_CHANNEL_1,
						(uint16_t )((1000000.0 / buzzer[buzzer_index].freq - 1)
								/ 2));
			__HAL_TIM_SET_AUTORELOAD(&htim6,
					(uint16_t )(1000000.0 / buzzer[buzzer_index].freq - 1));
			HAL_TIM_PWM_Start(&htim6, TIM_CHANNEL_2);
		}
	}
	buzzer_count++;

	if (buzzer_count == buzzer[buzzer_index].ms) {
		buzzer_count = 0;
	}

	if (buzzer[buzzer_index].freq == 0xffff) {
		buzzer_count = 0xffff;
	}

}


