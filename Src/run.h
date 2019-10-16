/*
 * run.h
 *
 *  Created on: 2019/10/03
 *      Author: kt33_
 */

#ifndef RUN_H_
#define RUN_H_

#include "variable.h"
#include "main.h"
#include "other.h"

void set_straight(float i_distance, float accel, float max_vel, float strat_vel,
		float end_vel);
void set_rotation(float i_angle, float accel, float max_vel, float center_vel);

void trapezoid_preparation(trapezoid_t *trapezoid, float i_distance,
		float accel, float max_vel, float strat_vel, float end_vel);
void wait_straight(void);
void wait_rotation(void);
void control_accel(run_t *ideal, trapezoid_t *trapezoid, uint8_t);
void duty_to_moter(void);
void PID_control(run_t *ideal, run_t *left, run_t *right,
		deviation_t *left_deviation, deviation_t *right_deviation, gain_t *gain,
		trapezoid_t *parameter, duty_t *duty, uint8_t rotation_flag);
float read_vel(uint8_t RorL);
void integral_1ms(float* dis, float*vel);
void integral_ideal(run_t *ideal);
void wall_control(void);
void coordinate(void);


#endif /* RUN_H_ */
