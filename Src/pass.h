/*
 * pass.h
 *
 *  Created on: 2019/11/28
 *      Author: kt33_
 */

#ifndef PASS_H_
#define PASS_H_

#include "variable.h"

void make_pass(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale, uint8_t);
void move_pass(float accel, float vel);
void move_pass_compression(float accel, float vel);
void move_pass_big_turn(float accel, float vel, float);
void move_pass_oblique(float accel, float max_vel, float big_turn_vel, float,
		float,uint8_t);
void out_put_pass(uint8_t *pass);


#endif /* PASS_H_ */
