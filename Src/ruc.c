/*
 * ruc.c
 *
 *  Created on: 2019/10/03
 *      Author: kt33_
 */

#include "run.h"
#include "variable.h"
#include "tim.h"

void set_straight(float i_distance, float accel, float max_vel, float strat_vel,
		float end_vel) {

	trapezoid_preparation(&translation_parameter, i_distance, accel, max_vel,
			strat_vel, end_vel);
	wall_control_flag = 1;
	if (i_distance < 0) {
		wall_control_flag = 0;
	}
	translation_parameter.run_flag = 1;
	ideal_translation.vel = translation_parameter.strat_vel;
}

void set_rotation(float i_angle, float accel, float max_vel, float center_vel) {
	rotation_parameter.back_rightturn_flag = 0;
	trapezoid_preparation(&rotation_parameter, i_angle, accel, max_vel, 0.0,
			0.0);

	wall_control_flag = 0;
	ideal_rotation.vel = 0.0;
	ideal_translation.accel = 0.0;
	ideal_translation.vel = center_vel;
	rotation_deviation.now = 0.0;
	if ((i_angle > 80.0) && (i_angle < 100.0)) {
		direction++;
	} else if ((i_angle < -80.0) && (i_angle > -100.0)) {
		direction += 3;
	} else if (((i_angle > 170.0) && (i_angle < 190.0))
			|| ((i_angle < -170.0) && (i_angle > -190.0))) {
		direction += 2;
	}
	if (direction > 3) {
		direction -= 4;
	}
	rotation_parameter.run_flag = 1;
//	log_start();
}

void wait_straight(void) {
//LEFTEING = 1;
//	printf("wait_st%6.2f\n", ideal_translation.vel);
	while (translation_parameter.run_flag == 1 && failsafe_flag == 0) {

	}
//LEFTFRONT = 1;
//translation_parameter.run_flag=1;
	ideal_translation.accel = 0.0;
	ideal_translation.dis = 0.0;
	ideal_translation.vel = translation_parameter.end_vel;
	translation_parameter.back_rightturn_flag = 0;
	run_left_deviation.now = 0.0;
	run_left_deviation.difference = 0.0;
	run_right_deviation.now = 0.0;
	run_right_deviation.difference = 0.0;
	rotation_parameter.back_rightturn_flag = 0;
}

void wait_rotation(void) {
//	LEFTEING = 1;
	while (rotation_parameter.run_flag == 1 && failsafe_flag == 0) {

	}
//	LEFTFRONT = 1;
//	rotation_gain.Ki = 0.0;
	ideal_translation.dis = 0.0;
	ideal_rotation.accel = 0.0;
	ideal_rotation.dis = 0.0;
	ideal_rotation.vel = 0.0;
//	rotation_parameter.back_rightturn_flag = 0;
	rotation_deviation.now = 0.0;
//	rotation_deviation.cumulative = 0.0;
//	duty.left = 0;
//	duty.right = 0;
//	duty_to_moter();
}

void trapezoid_preparation(trapezoid_t *trapezoid, float i_distance,
		float accel, float max_vel, float strat_vel, float end_vel) {
//任意のパラメータから台形加速の概要を計算
//trapezoidはポインタでとってる

	trapezoid->back_rightturn_flag = 0;

	if (i_distance < 0) {
		i_distance = i_distance * -1;
		trapezoid->back_rightturn_flag = 1;
	}

	trapezoid->accel = accel;
	trapezoid->end_vel = end_vel;
	trapezoid->i_distance = i_distance;
	trapezoid->max_vel = max_vel;
	trapezoid->strat_vel = strat_vel;
//	trapezoid->triangle = triangle;
//	trapezoid->triangle_flag = triangle_flag;
//↑任意の値を代入してる

	if (accel != 0.0) {

		trapezoid->acceldistance = (max_vel * max_vel - strat_vel * strat_vel)
				/ (2.0 * accel);
		trapezoid->deacceldistance = (max_vel * max_vel - end_vel * end_vel)
				/ (2.0 * accel);
		while (i_distance
				< trapezoid->acceldistance + trapezoid->deacceldistance) {
			max_vel = max_vel * 0.95;
			trapezoid->acceldistance = (max_vel * max_vel
					- strat_vel * strat_vel) / (2.0 * accel);
			trapezoid->deacceldistance = (max_vel * max_vel - end_vel * end_vel)
					/ (2.0 * accel);
		}
		trapezoid->max_vel = max_vel;
	} else {
		trapezoid->acceldistance = 0.0;
		trapezoid->deacceldistance = 0.0;
	}

}

void duty_to_moter(void) {
	int duty_left = 0, duty_right = 0;

	if (duty.left >= 0) {
		HAL_GPIO_WritePin(MOTER_L_CWCCW_GPIO_Port, MOTER_L_CWCCW_Pin, RESET);
		duty_left = duty.left;
	} else {
		HAL_GPIO_WritePin(MOTER_L_CWCCW_GPIO_Port, MOTER_L_CWCCW_Pin, SET);
		duty_left = (duty.left * -1);
	}
	if (duty.right >= 0) {
		HAL_GPIO_WritePin(MOTER_R_CWCCW_GPIO_Port, MOTER_R_CWCCW_Pin, SET);
		duty_right = duty.right;
	} else {
		HAL_GPIO_WritePin(MOTER_R_CWCCW_GPIO_Port, MOTER_R_CWCCW_Pin, RESET);
		duty_right = (duty.right * -1);
	}

	test_L = duty_left;
	test_R = duty_right;

	if (duty_left >= 800) {
		duty_left = 800 - 1;
	}
	if (duty_right >= 800) {
		duty_right = 800 - 1;
	}
	if (duty_left <= -800) {
		duty_left = -800 + 1;
	}
	if (duty_right <= -800) {
		duty_right = -800 + 1;
	}

	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
	if (duty_left == 0 && duty_right == 0) {

	} else {
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty_right); //MOTER_R
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty_left); //MOTER_L
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	}

	duty.left = 0;
	duty.right = 0;
}

void control_accel(run_t *ideal, trapezoid_t *trapezoid, uint8_t rotation_flag) {
	if (trapezoid->back_rightturn_flag == 1) {
		ideal->dis *= -1.0;
		ideal->vel *= -1.0;
		ideal->accel *= -1.0;
	}

	if (ideal->dis < trapezoid->acceldistance) {
//		UI_LED1 = 1;
		if (ideal->vel < trapezoid->max_vel) {
			ideal->accel = trapezoid->accel;
		} else {
			ideal->vel = trapezoid->max_vel;
			ideal->accel = 0;
		}

	} else if (ideal->dis
			< (trapezoid->i_distance - trapezoid->deacceldistance)) {
//		UI_LED2 = 1;
		ideal->accel = 0;

		ideal->vel = trapezoid->max_vel;
	} else if (ideal->vel > trapezoid->end_vel) {
		ideal->accel = -trapezoid->accel;

	} else {
		trapezoid->run_flag = 0;
		ideal->accel = 0;
		ideal->vel = trapezoid->end_vel;
	}

	if (trapezoid->back_rightturn_flag == 1) {
		ideal->dis *= -1.0;
		ideal->vel *= -1.0;
		ideal->accel *= -1.0;
	}

//	printf("ideal_vel=%4.2f,ideal_dis=%4.2f\n", ideal->vel, ideal->dis);

}

void PID_control(run_t *ideal, run_t *left, run_t *right,
		deviation_t *left_deviation, deviation_t *right_deviation, gain_t *gain,
		trapezoid_t *parameter, duty_t *duty, uint8_t rotation_flag) {
	int duty_left, duty_right;
	float Kp, Ki;

	Ki = gain->Ki;
	Kp = gain->Kp;

	left->vel = (left->vel + right->vel) / 2;
	right->vel = left->vel;

	if (rotation_flag == 1) {
		right->vel += wallcontrol_value;
		left->vel = right->vel;
	}

//	if (parameter->back_rightturn_flag == 1) {
//		ideal->vel *= -1.0;
//	}

	left_deviation->now = (ideal->vel - left->vel);
	right_deviation->now = (ideal->vel - right->vel);
	if (rotation_flag == 0) {
		left_deviation->cumulative += left_deviation->now;
		right_deviation->cumulative += right_deviation->now;
	} else if (rotation_flag == 1) {
		left_deviation->cumulative += left_deviation->now;
		right_deviation->cumulative = left_deviation->cumulative;
	}
	duty_left = (int) left_deviation->now * Kp
			+ left_deviation->cumulative * Ki;
	duty_right = (int) right_deviation->now * Kp
			+ right_deviation->cumulative * Ki;

	if (rotation_flag == 1) {
		duty_left = duty_left * -1;

	}
//	if (parameter->back_rightturn_flag == 1) {
//		duty_left = duty_left * -1;
//		duty_right = duty_right * -1;
//	}
	duty->left += duty_left;
	duty->right += duty_right;
}

float read_vel(uint8_t RorL) {
	float vel;
	uint16_t val;
	float val2;
	float val_cor;
//	uint8_t table_index;
	uint8_t i;
	for (i = 0; i < 50; i++)
		;
	read_spi_en(RorL, 0x3fff);
	for (i = 0; i < 50; i++)
		;
	val = (0x3fff & read_spi_en(RorL, 0x3fff));
	for (i = 0; i < 50; i++)
		;

//	table_index = val / 500;
	if (RorL == LEFT) {
		en_log_L.before_5ms = en_log_L.before_4ms;
		en_log_L.before_4ms = en_log_L.before_3ms;
		en_log_L.before_3ms = en_log_L.before_2ms;
		en_log_L.before_2ms = en_log_L.before_1ms;
		en_log_L.before_1ms = en_log_L.now;
		en_log_L.now = val;

		val_cor = LPF[0] * en_log_L.now + LPF[1] * en_log_L.before_1ms
				+ LPF[2] * en_log_L.before_2ms + LPF[3] * en_log_L.before_3ms
				+ LPF[4] * en_log_L.before_4ms + LPF[5] * en_log_L.before_5ms;

		val_cor = (float) val;
//		test_L = (float) val;
//		test_L = val_cor;
//		test_L2 = val_cor - before_en_val[RorL];

	} else {
		en_log_R.before_5ms = en_log_R.before_4ms;
		en_log_R.before_4ms = en_log_R.before_3ms;
		en_log_R.before_3ms = en_log_R.before_2ms;
		en_log_R.before_2ms = en_log_R.before_1ms;
		en_log_R.before_1ms = en_log_R.now;
		en_log_R.now = val;

		val_cor = LPF[0] * en_log_R.now + LPF[1] * en_log_R.before_1ms
				+ LPF[2] * en_log_R.before_2ms + LPF[3] * en_log_R.before_3ms
				+ LPF[4] * en_log_R.before_4ms + LPF[5] * en_log_R.before_5ms;
		val_cor = (float) val;
//		test_R = (float) val;
//		test_R = val_cor;
//		test_R2 = val_cor - before_en_val[RorL];

	}

	val2 = (float) ((val_cor - before_en_val[RorL]));
	if (val2 < -8000) {
		val2 += 16384;
	}
	if (val2 > 8000) {
		val2 -= 16384;
	}
	before_en_val[RorL] = val_cor;

	vel = ((float) (val2)) / 16384.0 * (3.1415926 * DIAMETER) * 1000;

	if (RorL == LEFT) {
		vel *= -1;
	}

//	if (translation_parameter.back_rightturn_flag == 1) { //バックするとき速度マイナスにするバカ
//		vel *= -1;
//	}

	return vel;
}

//float read_vel(uint8_t RorL) {
//	float vel;
//	uint16_t val;
//	int16_t val2;
//	uint8_t i;
//	for (i = 0; i < 50; i++)
//		;
//	read_spi_en(RorL, 0x3fff);
//	for (i = 0; i < 50; i++)
//		;
//	val = (0x3fff & read_spi_en(RorL, 0x3fff));
//	for (i = 0; i < 50; i++)
//		;
//
//	if(RorL==LEFT){
//		test_L=(float)val;
//	}else{
//		test_R=(float)val;
//	}
//
//	val2 = (int16_t) ((val - before_en_val[RorL]));
//	if (val2 < -8000) {
//		val2 += 16383;
//	}
//	if (val2 > 8000) {
//		val2 -= 16383;
//	}
//	before_en_val[RorL] = val;
//
//
//	vel = ((float) (val2)) / 16384.0 * ( 3.1415926 * DIAMETER) * 1000;
//		if (RorL == LEFT) {
//			vel *= -1;
//		}
//	return vel;
//}

void integral_1ms(float* dis, float*vel) {
	*dis += *vel * 0.001;
}

void integral_ideal(run_t *ideal) {
	ideal->vel += ideal->accel * 0.001;
	ideal->dis += ideal->vel * 0.001 + ideal->accel * 0.001 * 0.001 / 2;
}

void wall_control(void) {
//	test_L = (float) SEN_L.diff;
//	test_R = (float) SEN_R.diff;
//	test_L2 = (float) SEN_F.reference;

	if ((wall_control_flag == 1) && (wall_control_oblique_flag == 0)) {
		test_R2 = 1;
		if (((ideal_translation.vel) > 100.0) && (SEN_L.diff < 8)
				&& (SEN_R.diff < 8) && (SEN_F.now < SEN_F.reference)) { //&& (SEN_L.diff < 2000) && (SEN_R.diff < 2000)&& (SEN_F.now < SEN_F.threshold * 100))
			test_R2 = 2;
			if (SEN_L.now > SEN_L.threshold && SEN_R.now > SEN_R.threshold) {
				test_R2 = 3;
				wallcontrol_value = wall_cntrol_gain.Kp
						* (((float) SEN_L.now - (float) SEN_L.reference)
								- ((float) SEN_R.now - (float) SEN_R.reference))
						+ wall_cntrol_gain.Kd
								* (float) (SEN_L.diff_1ms - SEN_R.diff_1ms);
				set_led(5);
			} else if (SEN_L.now < SEN_L.threshold
					&& SEN_R.now > SEN_R.threshold) {
				test_R2 = 4;
				wallcontrol_value = -2.0 * wall_cntrol_gain.Kp
						* ((float) SEN_R.now - (float) SEN_R.reference)
						+ wall_cntrol_gain.Kd * (float) (-2 * SEN_R.diff_1ms);
				set_led(4);
			} else if (SEN_L.now > SEN_L.threshold
					&& SEN_R.now < SEN_R.threshold) {
				test_R2 = 5;
				wallcontrol_value = 2.0 * wall_cntrol_gain.Kp
						* ((float) SEN_L.now - (float) SEN_L.reference)
						+ wall_cntrol_gain.Kd * (float) (2 * SEN_L.diff_1ms);
				set_led(1);
			} else {
				test_R2 = 6;
				wallcontrol_value = 0.0;
				set_led(2);
			}
		}
//		  else if ((SEN_L.now > SEN_L.reference)
//				&& ((ideal_translation.vel) < 1800.0)
//				&& (SEN_F.now < SEN_F.threshold)) {
//			wallcontrol_value = 2.0 * wall_cntrol_gain.Kp
//					* ((float) SEN_L.now - (float) SEN_L.reference)
//					+ wall_cntrol_gain.Kd * (float) (2 * SEN_L.diff_1ms);
//			if (ideal_translation.vel > 1600.0) {
//				wallcontrol_value *= 0.2;
//			}
//		} else if ((SEN_R.now > SEN_R.reference)
//				&& ((ideal_translation.vel) < 1800.0)
//				&& (SEN_F.now < SEN_F.threshold)) {
//			wallcontrol_value = -2.0 * wall_cntrol_gain.Kp
//					* ((float) SEN_R.now - (float) SEN_R.reference)
//					+ wall_cntrol_gain.Kd * (float) (-2 * SEN_R.diff_1ms);
//			if (ideal_translation.vel > 1600.0) {
//				wallcontrol_value *= 0.2;
//			}
//		} else {
//			wallcontrol_value = 0.0;
//
//		}
	} else if ((wall_control_flag == 1) && (wall_control_oblique_flag >= 1)) {
		test_R2 = 7;
		wallcontrol_value = 0.0; //ここに斜め壁制御を書く
		if (wall_control_oblique_flag == 1 || wall_control_oblique_flag == 2) {
			test_R2 = 8;
			if (SEN_L.now > SEN_L.oblique_threshold && SEN_L.diff_1ms < 180) {
				test_R2 = 9;
				wallcontrol_value += (float) oblique_Side_gain
						* (float) (SEN_L.now - SEN_L.oblique_reference);
			}
			if (SEN_LF.now > SEN_LF.oblique_threshold && SEN_LF.diff < 20) {
				test_R2 = 10;
				wallcontrol_value += (float) oblique_Front_gain
						* (float) (SEN_LF.now - SEN_LF.oblique_reference);
			}
		}
		if (wall_control_oblique_flag == 1 || wall_control_oblique_flag == 3) {
			test_R2 = 11;
			if (SEN_R.now > SEN_R.oblique_threshold && SEN_R.diff_1ms < 180) {
				test_R2 = 12;
				wallcontrol_value -= (float) oblique_Side_gain
						* (float) (SEN_R.now - SEN_R.oblique_reference) * 2.0;

			}
			if (SEN_RF.now > SEN_RF.oblique_threshold && SEN_RF.diff < 30) {
				test_R2 = 13;
				wallcontrol_value -= (float) oblique_Front_gain
						* (float) (SEN_RF.now - SEN_RF.oblique_reference);
			}
		}
		if (wallcontrol_value > 300.0) {
			wallcontrol_value = 300.0;
		}
		if (wallcontrol_value < -300.0) {
			wallcontrol_value = -300.0;
		}

	} else {
		wallcontrol_value = 0.0;

	}
	test_float = wallcontrol_value;

}

void coordinate(void) {
	if (direction == North) {
		y.now++;
	} else if (direction == West) {
		x.now--;
	} else if (direction == South) {
		y.now--;
	} else if (direction == East) {
		x.now++;
	}
}

void read_vel2(float*left_vel,float*right_vel) {
	uint8_t RorL;
	float vel;
	uint16_t val;
	float val2;
	float val_cor;
//	uint8_t table_index;
	uint8_t i;
	for (i = 0; i < 50; i++)
		;
	read_spi_en(RorL, 0x3fff);
	for (i = 0; i < 50; i++)
		;
	val = (0x3fff & read_spi_en(RorL, 0x3fff));
	for (i = 0; i < 50; i++)
		;

//	table_index = val / 500;
	if (RorL == LEFT) {
		en_log_L.before_5ms = en_log_L.before_4ms;
		en_log_L.before_4ms = en_log_L.before_3ms;
		en_log_L.before_3ms = en_log_L.before_2ms;
		en_log_L.before_2ms = en_log_L.before_1ms;
		en_log_L.before_1ms = en_log_L.now;
		en_log_L.now = val;

		val_cor = LPF[0] * en_log_L.now + LPF[1] * en_log_L.before_1ms
				+ LPF[2] * en_log_L.before_2ms + LPF[3] * en_log_L.before_3ms
				+ LPF[4] * en_log_L.before_4ms + LPF[5] * en_log_L.before_5ms;

		val_cor = (float) val;
	} else {
		en_log_R.before_5ms = en_log_R.before_4ms;
		en_log_R.before_4ms = en_log_R.before_3ms;
		en_log_R.before_3ms = en_log_R.before_2ms;
		en_log_R.before_2ms = en_log_R.before_1ms;
		en_log_R.before_1ms = en_log_R.now;
		en_log_R.now = val;

		val_cor = LPF[0] * en_log_R.now + LPF[1] * en_log_R.before_1ms
				+ LPF[2] * en_log_R.before_2ms + LPF[3] * en_log_R.before_3ms
				+ LPF[4] * en_log_R.before_4ms + LPF[5] * en_log_R.before_5ms;
		val_cor = (float) val;

	}

	val2 = (float) ((val_cor - before_en_val[RorL]));
	if (val2 < -8000) {
		val2 += 16384;
	}
	if (val2 > 8000) {
		val2 -= 16384;
	}
	before_en_val[RorL] = val_cor;

	vel = ((float) (val2)) / 16384.0 * (3.1415926 * DIAMETER) * 1000;

	if (RorL == LEFT) {
		vel *= -1;
	}

//	return vel;
}
