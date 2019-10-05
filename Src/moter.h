/*
 * moter.h
 *
 *  Created on: Sep 30, 2019
 *      Author: kt33_
 */

#ifndef MOTER_H_
#define MOTER_H_

#include "variable.h"

float read_vel(uint8_t RorL);
void integral_1ms(float* dis, float*vel);
void integral_ideal(run_t *ideal);

#endif /* MOTER_H_ */
