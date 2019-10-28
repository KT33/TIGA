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

#define DIAMETER 12.5

typedef struct {
	int now; //今の値
	int reference; //真ん中にいるときのセンサー値
	int threshold; //閾値
	int diff; //差分
	int diff_1ms;
	int oblique_reference;
	int oblique_threshold;
} sensor_t;

typedef struct {
	int before_1ms;
	int before_2ms;
	int before_3ms;
	int before_4ms;
	int before_5ms;
	int now;
} SENLOG_t;

typedef struct {
	float accel; //加速度・各速度
	float vel; //速度・各速度
	float dis; //距離・角度
} run_t;

typedef struct {
	float Kp;
	float Ki;
	float Kd;
} gain_t;

typedef struct {
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

typedef struct {
	float vel_search;
	float vel_max;
	float vel_min;
	float accel;
} normal_para_t;

typedef struct {
	int left;
	int right;
} duty_t;

typedef struct {
	float now; //現在の偏差
	float cumulative; //偏差の累積値
	float difference; //偏差の差分
} deviation_t; //偏差

typedef struct {
	uint16_t row[17];
	uint16_t column[17];
} singlewalldata_t;

typedef struct {
	singlewalldata_t real;
	singlewalldata_t checked;
	singlewalldata_t adachi;
} walldata_t;

typedef struct {
	uint8_t now;
	uint8_t goal;
	uint8_t pass;
} XY_t;

typedef struct {
	uint16_t autoreload;
	uint16_t ms;
} buzzer_t;

#define LOG_MAX 5000

typedef struct {
	float log_1[LOG_MAX];
	float log_2[LOG_MAX];
	float log_3[LOG_MAX];
	float log_4[LOG_MAX];
	float log_5[LOG_MAX];
} LOG_t;

typedef struct{
	float in_offset;
	float out_offset;
}sla_t;

typedef struct{
	float accel;
	float max_vel;
	sla_t left;
	sla_t right;
}slarom_para_t;

extern uint16_t g_ADCBuffer[9];
extern float Batt;
extern uint16_t g_test;
extern walldata_t walldata;
extern XY_t x, y;
extern uint16_t step_map[16][16];
extern uint8_t add_wall_flag;
extern uint8_t direction;
extern sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF, SEN_F;
extern SENLOG_t SEN_R_log, SEN_RF_log, SEN_L_log, SEN_LF_log, SEN_F_log;
extern uint8_t mode;
extern uint16_t buzzer_count;
extern buzzer_t buzzer[30];
extern uint8_t buzzer_index, buzzer_flag;
extern uint8_t low_batt_flag;
extern float before_en_val[2];
extern SENLOG_t en_log_L, en_log_R;
extern run_t real_R, real_L, real_rotation;
extern run_t ideal_translation, ideal_rotation;
extern trapezoid_t translation_parameter, rotation_parameter;
extern deviation_t rotation_deviation;
extern uint8_t failsafe_flag;
extern deviation_t run_right_deviation;
extern deviation_t run_left_deviation;
extern float mode_select_dis;
extern float wallcontrol_value;
extern duty_t duty;
extern uint16_t failsafe_counter;
extern uint8_t moter_flag;
extern uint8_t SEN_check_flag;
extern gain_t run_gain;
extern gain_t rotation_gain;
extern LOG_t mylog, mylog2;
extern uint16_t log_index, log_how_often, log_often_count;
extern uint8_t log_flag;
extern float test_L, test_R, test_L2, test_R2, test_float;
extern float en_L_table[34];
extern float LPF[6];
extern float angle_calibration;
extern uint8_t angle_calibration_flag;
extern uint16_t angle_calibration_counter;
extern float angle_calibration_integral;
extern uint8_t failsafe_flag;
extern uint16_t failsafe_counter;
extern float failsafe_accel;
extern normal_para_t nomal_run;
extern normal_para_t nomal_rotation;
extern uint8_t wall_control_oblique_flag, wall_control_flag;
extern gain_t wall_cntrol_gain;
extern float oblique_Front_gain; //0.6
extern float oblique_Side_gain; //0.1
extern uint8_t goal_x, goal_y;
extern char flag;
extern uint8_t special_goal_flag;
extern uint8_t u_turn_counter;
extern float oblique_front_box;
extern float oblique_side_box;
extern float oblique_offset_front;
extern float oblique_offset_side;
extern float front_wall_gain;
extern uint8_t front_wall_flag;
extern int front_wall_value_R,front_wall_value_L;
extern float known_acc,known_vel;
extern float real_acc;

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
