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
uint16_t g_test=0;
walldata_t walldata;
XY_t x,y;
uint16_t step_map[16][16];
uint8_t add_wall_flag=1;
uint8_t direction=North;
sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF,SEN_F;
SENLOG_t SEN_R_log, SEN_RF_log, SEN_L_log, SEN_LF_log,SEN_F_log;
uint8_t mode=0;
uint16_t buzzer_count=0xffff;
buzzer_t buzzer[30];
uint8_t buzzer_index=0,buzzer_flag=0;
uint8_t low_batt_flag=0;
uint16_t before_en_val[2]={0,0};
run_t real_R,real_L,real_rotation;
run_t ideal_translation,ideal_rotation;
uint8_t wall_control_flag;
trapezoid_t translation_parameter,rotation_parameter;
uint8_t failsafe_flag=0;
deviation_t run_right_deviation;
deviation_t run_left_deviation;
deviation_t rotation_deviation;
float mode_select_dis=0.0;
float wallcontrol_value = 0.0;
duty_t duty;
uint16_t failsafe_counter=0;
uint8_t moter_flag=0;
uint8_t SEN_check_flag=0;
gain_t rotation_gain = { 0.62, 0.010, 0.0 };
gain_t run_gain = { 0.8, 0.4, 0.0 }; //p,i,d
LOG_t mylog,mylog2;
uint16_t log_index=0,log_how_often=0,log_often_count=0;
uint8_t log_flag=0;
