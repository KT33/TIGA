/*
 * variable.c
 *
 *  Created on: Aug 31, 2019
 *      Author: kt33_
 */
#include "variable.h"
#include "stdint.h"

uint16_t g_ADCBuffer[5];
float Batt;
uint16_t g_test=0;
walldata_t walldata;
XY_t x,y;
uint16_t step_map[16][16];
uint8_t add_wall_flag=1;
uint8_t direction=North;
sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF,SEN_F;
uint8_t mode=0;
uint16_t buzzer_count=0xffff;
buzzer_t buzzer[30];
uint8_t buzzer_index=0,buzzer_flag=0;
uint8_t low_batt_flag=0;
uint16_t before_en_val[2]={0,0};
run_t real_R,real_L,ideal,real_rotation;
run_t ideal_translation,ideal_rotation;
