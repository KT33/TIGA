/*
 * interrupt.c
 *
 *  Created on: 2019/09/15
 *      Author: kt33_
 */

#include "main.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "stdint.h"
#include "stdio.h"
#include "variable.h"
#include "walldata.h"
#include "other.h"
#include "buzzer.h"
#include "mode.h"
#include "tim.h"
#include "adc.h"
#include "interrupt.h"

void buzzer_1ms(void);
void adc_1ms(void);

void interrupt_1ms(void) {

//buzzer
	buzzer_1ms();
//buzzer

//ADC
	if (SEN_check_flag == 1) {
		adc_1ms();
	}
//ADC

//failsafe判定 //|| failsafe_accel > 3.0  real_R.vel > 2000.0 || real_L.vel > 2000.0||
	if ((rotation_deviation.cumulative > 200000.0
			|| rotation_deviation.cumulative < -200000.0)
			&& translation_parameter.back_rightturn_flag == 0&&failsafe_flag==0) { //|| failsafe_accel > 39.2
		failsafe();
//		printf("R_vel=%6.2f,L_vel=%6.2f,rot_dev=%9.2f,acc=%5.2f\n", real_R.vel,
//				real_L.vel, rotation_deviation.cumulative, failsafe_accel);
	}

	//failsafe処理
	if (failsafe_flag == 1 && failsafe_counter < 1000) {
		//	ui_led_3bit(7);
		ideal_translation.accel = 0.0;
		ideal_translation.vel = 0.0;
		run_left_deviation.cumulative = 0.0;
		run_right_deviation.cumulative = 0.0;
		failsafe_counter++;

		PID_control(&ideal_translation, &real_L, &real_R, &run_left_deviation,
				&run_right_deviation, &run_gain, &translation_parameter, &duty,
				0);
		duty_to_moter();
	}

	if (mode & 0x80) { //in_mode　モード中
		//gyro
		real_rotation.vel = read_gyro();
		integral_1ms(&real_rotation.dis, &real_rotation.vel); //角速度から角度に
		//gyro
		failsafe_accel = read_accel();

		//encoder
		real_L.vel = read_vel(LEFT); //mm/sec
		real_R.vel = read_vel(RIGHT);
		integral_1ms(&real_L.dis, &real_L.vel);
		integral_1ms(&real_R.dis, &real_R.vel);
		//encoder

		if (log_flag == 1) {
			log_sampling();
		}

		if (angle_calibration_flag == 1) {
			angle_calibration_counter++;
			angle_calibration_integral += real_rotation.vel;
			if (angle_calibration_counter == 2000) {
				angle_calibration_flag = 0;
			}
		}

		if (moter_flag == 1 && failsafe_flag == 0) {
			if (translation_parameter.run_flag == 1) {

				control_accel(&ideal_translation, &translation_parameter, 0);
			}
			if (rotation_parameter.run_flag == 1) {
				control_accel(&ideal_rotation, &rotation_parameter, 1);
				integral_ideal(&ideal_rotation);
			}

			PID_control(&ideal_translation, &real_L, &real_R,
					&run_left_deviation, &run_right_deviation, &run_gain,
					&translation_parameter, &duty, 0);
			if (translation_parameter.back_rightturn_flag == 0
					|| ideal_translation.vel > 100.0) {
				PID_control(&ideal_rotation, &real_rotation, &real_rotation,
						&rotation_deviation, &rotation_deviation,
						&rotation_gain, &rotation_parameter, &duty, 1);
			}
			integral_ideal(&ideal_translation);

			duty_to_moter();
		}

	} else { //mode_out モード選択
//		printf("test3\n");
		real_R.vel = read_vel(RIGHT);
		integral_1ms(&mode_select_dis, &real_R.vel);

		//	printf("inter:sel_dis=%3.2f,vel=%3.2f\n",mode_select_dis,real_R.vel);
	}

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

void failsafe(void) {
	failsafe_flag = 1;
	ideal_translation.accel = 0.0;
	ideal_translation.vel = 0.0;
	duty_to_moter();
	set_buzzer(0, C_4, 300);
	set_buzzer(1, 0, 300);
	set_buzzer(2, D_4, 300);
	set_buzzer(3, 0, 300);
	set_buzzer(4, E_4, 300);
	set_buzzer(5, 0, 300);
	set_buzzer(6, F_4, 300);
	x.now = 0;
	y.now = 0;
	direction = 0;

	failsafe_flag = 1;
	failsafe_counter = 0;
}
