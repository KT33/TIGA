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

#define DIAMETER 12.2

typedef struct {
	int now;//今の値
	int reference;//真ん中にいるときのセンサー値
	int threshold;//閾値
	int diff;//差分
	int diff_1ms;
	int oblique_reference;
	int oblique_threshold;
} sensor_t;

typedef struct{
	float accel;//加速度・各速度
	float vel;//速度・各速度
	float dis;//距離・角度
} run_t;

typedef struct{
	float Kp;
	float Ki;
	float Kd;
} gain_t;

typedef struct{
	float i_distance;
	float accel;
//	float min_vel;
	float max_vel;
	float strat_vel;
	float end_vel;
	float triangle;
	int8_t back_rightturn_flag;
	float acceldistance;
	float deacceldistance;
	int8_t run_flag;
} trapezoid_t;

typedef struct{
	float vel_search;
	float vel_max;
	float vel_min;
	float accel;
} normal_para_t;

typedef struct{
	int left;
	int right;
} duty_t;

typedef struct{
	float now;//現在の偏差
	float cumulative;//偏差の累積値
	float difference;//偏差の差分
} deviation_t;//偏差

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



typedef struct{
	uint16_t autoreload;
	uint16_t ms;
}buzzer_t;

extern uint16_t g_ADCBuffer[5];
extern float Batt;
extern uint16_t g_test;
extern walldata_t walldata;
extern XY_t x,y;
extern uint16_t step_map[16][16];
extern uint8_t add_wall_flag;
extern uint8_t direction;
extern sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF,SEN_F;
extern uint8_t mode;
extern uint16_t buzzer_count;
extern buzzer_t buzzer[30];
extern uint8_t buzzer_index,buzzer_flag;
extern uint8_t low_batt_flag;
extern uint16_t before_en_val[2];
extern run_t real_R,real_L,ideal;

#define North 0
#define West 1
#define South 2
#define East 3

#define LEFT 0
#define RIGHT 1

#define ALL 1
#define REAL 2
#define CHECK 3
#define ADACHI 4

#endif /* VARIABLE_H_ */
