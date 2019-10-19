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
float test_L, test_R, test_L2, test_R2,test_float;
float en_L_table[34] = { 1.009998433, 1.007563172, 1.004827295, 1.001972749,
		0.999229336, 0.99687471, 0.99523438, 0.995031825, 0.99550266,
		0.997374733, 0.999754893, 1.001993126, 1.003682556, 1.004659447,
		1.005003202, 1.005024181, 1.003716193, 1.00224529, 1.000891179,
		0.999808296, 0.999025803, 0.998447589, 0.99785227, 0.99774053,
		0.998549181, 0.999442417, 1.000618099, 1.002036411, 1.00341987,
		1.004253315, 1.004100496, 1.003151015, 1.001933841, 1.000761267 };
float LPF[6] = { 0.400899718415940, 0.0487928848738268, 0.0503073967102330,
		0.0503073967102330, 0.0487928848738268, 0.400899718415940 };
SENLOG_t en_log_L, en_log_R;
float angle_calibration = 0.0;
uint8_t angle_calibration_flag = 0;
uint16_t angle_calibration_counter;
float angle_calibration_integral = 0.0;
float failsafe_accel = 0.0;
normal_para_t nomal_run = { 300.0, 300.0, 20.0, 3500.0 }; //search,max,min,accel
normal_para_t nomal_rotation = { 750.0, 670.0, 0.0, 1700.0 }; //deg/sec //400.0, 400.0, 0.0, 500.0
uint8_t wall_control_oblique_flag=0,wall_control_flag=0;
gain_t wall_cntrol_gain; //0.046
float oblique_Front_gain = 0.6; //0.6
float oblique_Side_gain = 0.01; //0.1
uint8_t goal_x,goal_y;
char flag;
uint8_t special_goal_flag=0;
uint8_t u_turn_counter=0;
float oblique_front_box;
float oblique_side_box;
float oblique_offset_front=1.0;
float oblique_offset_side=1.0;

uint8_t front_wall_flag=0;
float front_wall_gain;
int front_wall_value_R,front_wall_value_L;

