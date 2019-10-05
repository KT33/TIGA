/*
 * ruc.c
 *
 *  Created on: 2019/10/03
 *      Author: kt33_
 */

#include "run.h"
#include "variable.h"

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
//	log_start();

}

void set_rotation(float i_angle, float accel, float max_vel, float center_vel) {
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
	while (translation_parameter.run_flag == 1 && failsafe_flag == 0) {
		//	myprintf("%6.2f", rotation_ideal.velocity);
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
//	rotation_deviation.now = 0.0;
//	rotation_deviation.cumulative = 0.0;

//	duty.left = 0;
//	duty.right = 0;
//	duty_to_moter();
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
	rotation_parameter.back_rightturn_flag = 0;
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

void control_accel(run_t *ideal, trapezoid_t *trapezoid, uint8_t rotation_flag) {
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

		//		RIGHTWING = 1;
	}

}
