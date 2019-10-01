/*
 * moter.c
 *
 *  Created on: Sep 30, 2019
 *      Author: kt33_
 */

#include "moter.h"
#include "main.h"
#include "variable.h"
#include "tim.h"
#include "other.h"

float read_vel(uint8_t RorL) {
	float vel;
	uint16_t val;
	int16_t val2;
	read_spi_en(RorL, 0x3fff);
	val = (0x3fff & read_spi_en(RorL, 0x3fff));
	val2 = (int16_t) ((val - before_en_val[RorL]));
	if (val2 < -8000) {
		val2 += 16384;
	}
	if (val2 > 8000) {
		val2 -= 16384;
	}
	before_en_val[RorL] = val;
//	vel = ((float) (val2)) / 16384.0;
	return vel = ((float) (val2)) / 16384.0 * (0.5 * 3.14 * DIAMETER * DIAMETER);
}

void integlral_1ms(float* dis, float*vel) {
	*dis += *vel ;
}
