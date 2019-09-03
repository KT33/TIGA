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
walldate_t walldate_real, walldate_checked, walldate_adachi;
XY_t x,y;
uint16_t step_map[16][16];
