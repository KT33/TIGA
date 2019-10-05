/*
 * buzzer.c
 *
 *  Created on: Sep 9, 2019
 *      Author: kt33_
 */

#include "buzzer.h"
#include "variable.h"

uint16_t calculation_counter(uint16_t Hz) { //タイマピリオドの計算
	uint16_t count; //設定する値
	if (Hz == 0) {
		count = 0;
	} else {
		count = (1000000 / Hz - 1);
	}
	return count;
}

void set_buzzer(uint8_t index, uint16_t Hz, uint16_t ms) {
	buzzer_index = 0;
	buzzer_count = 0;
	buzzer[index].ms = ms;
	buzzer[index].autoreload = calculation_counter(Hz);
	buzzer[index + 1].autoreload = 0xffff;
}

void set_buzzer_mode(uint8_t mode) {
	buzzer_flag = 0;
	if (mode == 0) {
		set_buzzer(0, C_4, 600);
	} else if (mode == 1) {
		set_buzzer(0, D_4, 600);
	} else if (mode == 2) {
		set_buzzer(0, E_4, 600);
	} else if (mode == 3) {
		set_buzzer(0, F_4, 600);
	} else if (mode == 4) {
		set_buzzer(0, G_4, 600);
	} else if (mode == 5) {
		set_buzzer(0, A_4, 600);
	} else if (mode == 6) {
		set_buzzer(0, B_4, 600);
	} else if (mode == 7) {
		set_buzzer(0, C_5, 600);
	}
	buzzer_flag = 1;
}
