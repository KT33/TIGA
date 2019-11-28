/*
 * ruc.c
 *
 *  Created on: 2019/10/03
 *      Author: kt33_
 */

#include "run.h"
#include "variable.h"
#include "tim.h"
#include "walldata.h"

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

//	test_L = duty_left;
//	test_R = duty_right;

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
		left_deviation->cumulative += left_deviation->now; //+ wallcontrol_value
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
	uint8_t table_index, table_value;
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
//		en_log_L.before_5ms = en_log_L.before_4ms;
//		en_log_L.before_4ms = en_log_L.before_3ms;
//		en_log_L.before_3ms = en_log_L.before_2ms;
//		en_log_L.before_2ms = en_log_L.before_1ms;
//		en_log_L.before_1ms = en_log_L.now;
//		en_log_L.now = val;
//
//		val_cor = LPF[0] * (float) en_log_L.now
//				+ LPF[1] * (float) en_log_L.before_1ms
//				+ LPF[2] * (float) en_log_L.before_2ms
//				+ LPF[3] * (float) en_log_L.before_3ms
//				+ LPF[4] * (float) en_log_L.before_4ms
//				+ LPF[5] * (float) en_log_L.before_5ms;

//		table_index = val / 500;
//		table_value = val % 500;
//
//		if (table_index + 1 < 33) {
//			val_cor =
//					(float) (((en_L_table[table_index + 1]
//					- en_L_table[table_index]) * (float) table_value / 500)
//					+ en_L_table[table_index]) * val;
//		} else {
//			val_cor = (float) ((en_L_table[table_index + 1]
//					- en_L_table[table_index]) * (float) table_value / 335
//					+ en_L_table[table_index]) * val;
//		}

		val_cor = (float) val;
//		test_L = (float) val;
//		test_L = val_cor;
	} else {
//		en_log_R.before_5ms = en_log_R.before_4ms;
//		en_log_R.before_4ms = en_log_R.before_3ms;
//		en_log_R.before_3ms = en_log_R.before_2ms;
//		en_log_R.before_2ms = en_log_R.before_1ms;
//		en_log_R.before_1ms = en_log_R.now;
//		en_log_R.now = val;
//		val_cor = LPF[0] * (float) en_log_R.now
//				+ LPF[1] * (float) en_log_R.before_1ms
//				+ LPF[2] * (float) en_log_R.before_2ms
//				+ LPF[3] * (float) en_log_R.before_3ms
//				+ LPF[4] * (float) en_log_R.before_4ms
//				+ LPF[5] * (float) en_log_R.before_5ms;
		val_cor = (float) val;
//		test_R = (float) val;
//		test_R = val_cor;
	}

	val2 = (float) ((val_cor - before_en_val[RorL]));
	before_en_val[RorL] = val_cor;
	if (val2 < -8000) {
		val2 += 16384;
	}
	if (val2 > 8000) {
		val2 -= 16384;
	}

	if (RorL == LEFT) {
		test_L2 = val2;
	} else {
		test_R2 = val2;
	}

	vel = ((float) (val2)) / 16384.0 * (3.1415926 * DIAMETER) * 1000;

	if (RorL == LEFT) {
		en_log_L[en_log_index] = vel;
//		test_L = vel * -1;
//		vel = LPF[0] * en_log_L[((en_log_index) % 6)]
//				+ LPF[1] * en_log_L[((en_log_index - 1 + 6) % 6)]
//				+ LPF[2] * en_log_L[((en_log_index - 2 + 6) % 6)]
//				+ LPF[3] * en_log_L[((en_log_index - 3 + 6) % 6)]
//				+ LPF[4] * en_log_L[((en_log_index - 4 + 6) % 6)]
//				+ LPF[5] * en_log_L[((en_log_index - 5 + 6) % 6)];

	} else {
		en_log_R[en_log_index] = vel;
//		test_R = vel;
//		vel = LPF[0] * en_log_R[((en_log_index) % 6)]
//				+ LPF[1] * en_log_R[((en_log_index - 1 + 6) % 6)]
//				+ LPF[2] * en_log_R[((en_log_index - 2 + 6) % 6)]
//				+ LPF[3] * en_log_R[((en_log_index - 3 + 6) % 6)]
//				+ LPF[4] * en_log_R[((en_log_index - 4 + 6) % 6)]
//				+ LPF[5] * en_log_R[((en_log_index - 5 + 6) % 6)];
		en_log_index++;
		if (en_log_index == 6) {
			en_log_index = 0;
		}
	}

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
	float L_error = (float) SEN_L.now - (float) SEN_L.reference;
	float R_error = (float) SEN_R.now - (float) SEN_R.reference;

	float error_max = 400;

	if (L_error < 0) {
		L_error *= 3;
	}
	if (R_error < 0) {
		R_error *= 3;
	}

	if (L_error > error_max) {
		L_error = error_max;
	}
	if (R_error > error_max) {
		R_error = error_max;
	}
	if (L_error < -error_max) {
		L_error = -error_max;
	}
	if (R_error < -error_max) {
		R_error = -error_max;
	}
	float L_error_diff = (L_error - SEN_L.error_before);
	float R_error_diff = (R_error - SEN_R.error_before);
	SEN_L.error_before = ((float) SEN_L.now - (float) SEN_L.diff)
			- (float) SEN_L.reference;
	SEN_R.error_before = ((float) SEN_R.now - (float) SEN_R.diff)
			- (float) SEN_R.reference;
	test_L = L_error_diff;
	test_R = R_error_diff;
	if ((wall_control_flag == 1) && (wall_control_oblique_flag == 0)) { //通常

		if (((ideal_translation.vel) > 40.0) && (SEN_L.diff < 18)
				&& (SEN_R.diff < 18) && (SEN_F.now < SEN_F.reference)) { //&& (SEN_L.diff < 2000) && (SEN_R.diff < 2000)&& (SEN_F.now < SEN_F.threshold * 100))
			if (SEN_L.now > SEN_L.threshold && SEN_R.now > SEN_R.threshold) {
				wallcontrol_value = wall_cntrol_gain.Kp
						* ((L_error) - (R_error))
						+ wall_cntrol_gain.Kd
								* (float) (L_error_diff - R_error_diff);
//				set_led(5);
			} else if (SEN_L.now < SEN_L.threshold
					&& SEN_R.now > SEN_R.threshold) {
				wallcontrol_value = -2.0 * wall_cntrol_gain.Kp * (R_error)
						+ wall_cntrol_gain.Kd * (float) (-2 * R_error_diff);
//				set_led(4);
			} else if (SEN_L.now > SEN_L.threshold
					&& SEN_R.now < SEN_R.threshold) {
				wallcontrol_value = 2.0 * wall_cntrol_gain.Kp * (L_error)
						+ wall_cntrol_gain.Kd * (float) (2 * L_error_diff);
//				set_led(1);
			} else {
				wallcontrol_value = 0.0;
//				set_led(2);
			}
		}
		if ((kushi_control_flag == 1) && ideal_translation.vel > 150.0) {
			set_led(7);
			if ((SEN_RF.now > SEN_RF.front_kusi)
					&& ((float) SEN_RF.now < (float) SEN_RF.reference * 1)
					&& (SEN_RF.diff > 10) && (SEN_R.now < SEN_R.threshold)) { //
				wallcontrol_value -= wall_cntrol_gain.Ki
						* (SEN_RF.now - SEN_RF.front_kusi);
			}
			if ((SEN_LF.now > SEN_LF.front_kusi)
					&& ((float) SEN_LF.now < (float) SEN_LF.reference * 1)
					&& (SEN_LF.diff > 10) && (SEN_L.now < SEN_L.threshold)) { //
				wallcontrol_value += wall_cntrol_gain.Ki
						* (SEN_LF.now - SEN_LF.front_kusi);
			}
		} else {
			set_led(2);
		}

//		  else if ((SEN_L.now > SEN_L.reference)
//				&& ((ideal_translation.vel) < 1800.0)
//				&& (SEN_F.now < SEN_F.threshold)) {
//			wallcontrol_value = 2.0 * wall_cntrol_gain.Kp
//					* (L_error)
//					+ wall_cntrol_gain.Kd * (float) (2 * L_error_diff);
//			if (ideal_translation.vel > 1600.0) {
//				wallcontrol_value *= 0.2;
//			}
//		} else if ((SEN_R.now > SEN_R.reference)
//				&& ((ideal_translation.vel) < 1800.0)
//				&& (SEN_F.now < SEN_F.threshold)) {
//			wallcontrol_value = -2.0 * wall_cntrol_gain.Kp
//					* (R_error)
//					+ wall_cntrol_gain.Kd * (float) (-2 * R_error_diff);
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
			if (SEN_L.now > SEN_L.oblique_threshold && SEN_L.diff < 180) {
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
			if (SEN_R.now > SEN_R.oblique_threshold && SEN_R.diff < 180) {
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

	} else if (wall_control_flag == 10) { //壁キレ区間
		if (SEN_L.now > SEN_L.reference && SEN_R.now > SEN_R.reference) {
			wallcontrol_value = wall_cntrol_gain.Kp * ((L_error) - (R_error))
					+ wall_cntrol_gain.Kd
							* (float) (L_error_diff - R_error_diff);
//				set_led(5);
		} else if (SEN_L.now < SEN_L.threshold  && SEN_R.now > SEN_R.reference) {
			wallcontrol_value = -2.0 * wall_cntrol_gain.Kp * (R_error)
					+ wall_cntrol_gain.Kd * (float) (-2 * R_error_diff);
//				set_led(4);
		} else if (SEN_L.now > SEN_L.reference && SEN_R.now < SEN_R.threshold) {
			wallcontrol_value = 2.0 * wall_cntrol_gain.Kp * (L_error)
					+ wall_cntrol_gain.Kd * (float) (2 * L_error_diff);
//				set_led(1);
		} else {
			wallcontrol_value = 0.0;
//				set_led(2);
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

void coordinate_pass(void) {
	if (direction_pass == North) {
		y.pass++;
	} else if (direction_pass == West) {
		x.pass--;
	} else if (direction_pass == South) {
		y.pass--;
	} else if (direction_pass == East) {
		x.pass++;
	}
}

void read_vel2(float*left_vel, float*right_vel) {
	uint8_t i;
	uint16_t left_enc_val, right_enc_val;
	int32_t left_diff, right_diff;
	float vel_enc_l, vel_enc_r, vel_acc;
	float l_vel_box;
	float r_vel_box;
	l_vel_box = *left_vel;
	r_vel_box = *right_vel;
//	uint8_t table_index;

	real_acc = read_accel();
	acc_sum += real_acc - acc_buff[acc_buff_index];
	acc_buff[acc_buff_index] = real_acc;
	acc_buff_index++;
	if (acc_buff_index == 50) {
		acc_buff_index = 0;
	}
	vel_acc = acc_sum * 0.001;
	test_L2 = vel_acc;

//leftの実際の測定値
	for (i = 0; i < 50; i++)
		;
	read_spi_en(LEFT, 0x3fff);
	for (i = 0; i < 50; i++)
		;
	left_enc_val = (0x3fff & read_spi_en(LEFT, 0x3fff));
	for (i = 0; i < 25; i++)
		;

//rightの実際の測定値
	for (i = 0; i < 25; i++)
		;
	read_spi_en(RIGHT, 0x3fff);
	for (i = 0; i < 50; i++)
		;
	right_enc_val = (0x3fff & read_spi_en(RIGHT, 0x3fff));
	for (i = 0; i < 50; i++)
		;

	left_diff = (int32_t) (left_enc_val - left_enc_before) * -1;
	right_diff = (int32_t) (right_enc_val - right_enc_before);
	left_enc_before = left_enc_val;
	right_enc_before = right_enc_val;

	if (left_diff < -8000) {
		left_diff += 16384;
	}
	if (left_diff > 8000) {
		left_diff -= 16384;
	}

	if (right_diff < -8000) {
		right_diff += 16384;
	}
	if (right_diff > 8000) {
		right_diff -= 16384;
	}

	enc_sum_l += (left_diff - enc_buff_l[enc_buff_index]);
	enc_sum_r += (right_diff - enc_buff_r[enc_buff_index]);
	enc_buff_l[enc_buff_index] = left_diff;
	enc_buff_r[enc_buff_index] = right_diff;
	enc_buff_index++;
	if (enc_buff_index == 100) {
		enc_buff_index = 0;
	}

//	test_L = (float) ((enc_sum_l / 100) / 16384.0 * (3.1415926 * DIAMETER)
//			* 1000) + vel_acc; //+ vel_acc
//	test_R = (float) ((enc_sum_r / 100) / 16384.0 * (3.1415926 * DIAMETER)
//			* 1000) + vel_acc; //+ vel_acc
	vel_enc_l = (float) ((enc_sum_l / 100) / 16384.0 * (3.1415926 * DIAMETER)
			* 1000) + vel_acc; //+ vel_acc
	vel_enc_r = (float) ((enc_sum_r / 100) / 16384.0 * (3.1415926 * DIAMETER)
			* 1000) + vel_acc; //+ vel_acc
//	enc_sum_l = (int32_t)left_diff * 100;
//	enc_sum_r = (int32_t)right_diff * 100;
//
//	vel_enc_l = (float) (enc_sum_l / 100) / 16384.0 * (3.1415926 * DIAMETER)
//			* 1000;
//	vel_enc_r = (float) (enc_sum_r / 100) / 16384.0 * (3.1415926 * DIAMETER)
//			* 1000;

//	vel_enc_l = (float) (left_diff) / 16384.0 * (3.1415926 * DIAMETER) * 1000;
//	vel_enc_r = (float) (right_diff) / 16384.0 * (3.1415926 * DIAMETER) * 1000;

	*left_vel = vel_enc_l; //vel_acc +
	*right_vel = vel_enc_r; //vel_acc +

//	vel = ((float) (val2)) / 16384.0 * (3.1415926 * DIAMETER) * 1000;

//	if (RorL == LEFT) {
//		vel *= -1;
//	}

//	return vel;
}
