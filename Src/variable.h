/*
 * variable.h
 *
 *  Created on: Aug 31, 2019
 *      Author: kt33_
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "stdint.h"
#include "stdio.h"


typedef struct{
	uint16_t row[17];
	uint16_t column[17];
}singlewalldata_t;

typedef struct{
	singlewalldata_t real;
	singlewalldata_t checked;
	singlewalldata_t adachi;
}walldata_t;


typedef struct{
	uint8_t now;
	uint8_t goal;
	uint8_t pass;
}XY_t;

typedef struct {
	int now;//今の値
	int reference;//真ん中にいるときのセンサー値
	int threshold;//閾値
	int diff;//差分
	int diff_1ms;
	int oblique_reference;
	int oblique_threshold;
} sensor_t;

extern uint16_t g_ADCBuffer[5];
extern float Batt;
extern uint16_t g_test;
extern walldata_t walldata;
extern XY_t x,y;
extern uint16_t step_map[16][16];
extern uint8_t add_wall_flag;
extern uint8_t direction;
extern sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF,SEN_F;


#define North 0
#define West 1
#define South 2
#define East 3

#define ALL 1
#define REAL 2
#define CHECK 3
#define ADACHI 4

#endif /* VARIABLE_H_ */
