/*
 * variable.c
 *
 *  Created on: Aug 31, 2019
 *      Author: kt33_
 */
#include "variable.h"
#include "stdint.h"

uint16_t g_ADCBuffer[9];
float Batt;
uint16_t g_test = 0;
walldata_t walldata;
XY_t x, y;
uint16_t step_map[16][16];
uint8_t add_wall_flag = 1;
uint8_t direction = North;
sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF, SEN_F;
SENLOG_t SEN_R_log, SEN_RF_log, SEN_L_log, SEN_LF_log, SEN_F_log;
uint8_t mode = 0;
uint16_t buzzer_count = 0xffff;
buzzer_t buzzer[30];
uint8_t buzzer_index = 0, buzzer_flag = 0;
uint8_t low_batt_flag = 0;
float before_en_val[2] = { 0, 0 };
run_t real_R, real_L, real_rotation;
run_t ideal_translation, ideal_rotation;
trapezoid_t translation_parameter, rotation_parameter;
uint8_t failsafe_flag = 0;
deviation_t run_right_deviation;
deviation_t run_left_deviation;
deviation_t rotation_deviation;
float mode_select_dis = 0.0;
float wallcontrol_value = 0.0;
duty_t duty;
uint16_t failsafe_counter = 0;
uint8_t moter_flag = 0;
uint8_t SEN_check_flag = 0;
gain_t rotation_gain = { 0.62, 0.010, 0.0 };
gain_t run_gain = { 0.8, 0.4, 0.0 }; //p,i,d
LOG_t mylog, mylog2;
uint16_t log_index = 0, log_how_often = 0, log_often_count = 0;
uint8_t log_flag = 0;
float test_L, test_R, test_L2, test_R2, test_float;
float en_L_table[34] = { 0.9995869892557080, 1.0002069971889400,
		0.9906386122687360, 0.9791746384518470, 0.9709183885641870,
		0.9683533203755940, 0.9719126726745640, 0.9805491013429930,
		0.9923043154309130, 1.0048787132312400, 1.0162010183544900,
		1.0249979158035500, 1.0286904193815400, 1.0294418512187900,
		1.0260670252765300, 1.0200915393135300, 1.0127801310623400,
		1.0051541871702500, 0.9980092521403580, 0.9919325372725490,
		0.9873204296045050, 0.9843960008527270, 0.9832265163535420,
		0.9837409440041100, 0.9857474632034430, 0.9889509737934180,
		0.9929706049997780, 0.9973572243731430, 1.0016109467300400,
		1.0051986430938900, 1.0075714496360400, 1.0081822766167500,
		1.0065033173262400, 1.0033499208262800 };
float LPF[6] = { 0.400899718415940, 0.0487928848738268, 0.0503073967102330,
		0.0503073967102330, 0.0487928848738268, 0.400899718415940 };
float angle_calibration = 0.0;
uint8_t angle_calibration_flag = 0;
uint16_t angle_calibration_counter;
float angle_calibration_integral = 0.0;
float failsafe_accel = 0.0;
normal_para_t nomal_run = { 300.0, 300.0, 20.0, 3500.0 }; //search,max,min,accel
normal_para_t nomal_rotation = { 750.0, 670.0, 0.0, 1700.0 }; //deg/sec //400.0, 400.0, 0.0, 500.0
uint8_t wall_control_oblique_flag = 0, wall_control_flag = 0;
gain_t wall_cntrol_gain; //0.046
float oblique_Front_gain = 0.6; //0.6
float oblique_Side_gain = 0.01; //0.1
uint8_t goal_x, goal_y;
char flag;
uint8_t special_goal_flag = 0;
uint8_t u_turn_counter = 0;
float oblique_front_box;
float oblique_side_box;
float oblique_offset_front = 1.0;
float oblique_offset_side = 1.0;

uint8_t front_wall_flag = 0;
float front_wall_gain;
int front_wall_value_R, front_wall_value_L;
float known_acc, known_vel;

int32_t enc_buff_l[100], enc_buff_r[100];
uint8_t enc_buff_index = 0;
float acc_buff[50];
uint8_t acc_buff_index = 0;
uint16_t left_enc_before, right_enc_before;
int32_t enc_sum_l = 0, enc_sum_r = 0;
float acc_sum = 0;

float real_acc;
float real_vel_from_acc = 0, real_diss_from_acc = 0;
float accel_calibration_integral = 0.0, accel_calibration = 0.0;

float en_log_L[6] = { 0, 0, 0, 0, 0, 0 }, en_log_R[6] = { 0, 0, 0, 0, 0, 0 },
		acc_log[6] = { 0, 0, 0, 0, 0, 0 };
int8_t en_log_index = 0, acc_log_index = 0;

