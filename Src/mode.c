/*
 * mode.c
 *
 *  Created on: Oct 5, 2019
 *      Author: Keisuke Takao
 */

#include "mode.h"
#include "run.h"
#include "walldata.h"
#include "other.h"

void mode_0(void) {
	uint8_t i;
	read_all_log_from_flash();
	log_output();
}

void mode_1(void) {
	moter_flag = 1;
//	printf("4,mode=%d,%d\n", mode, (mode & 0x80));
	run_gain.Kp = 0.4;
	run_gain.Ki = 0.0;
	rotation_gain.Kp = 0.0;
	rotation_gain.Ki = 0.0;
	log_start();
	set_straight(600.0, 3500, 300, 0, 0);
	wait_straight();
	save_log_to_flash();
}

void mode_2(void) {
	moter_flag = 1;
//	printf("4,mode=%d,%d\n", mode, (mode & 0x80));
	run_gain.Kp = 0.5;
	run_gain.Ki = 0.0;
	rotation_gain.Kp = 0.0;
	rotation_gain.Ki = 0.0;
	log_start();
	set_straight(600.0, 3500, 300, 0, 0);
	wait_straight();
	save_log_to_flash();
}

void mode_3(void) { //253.558
	moter_flag = 1;
//	printf("4,mode=%d,%d\n", mode, (mode & 0x80));
	run_gain.Kp = 0.6;
	run_gain.Ki = 0.0;
	rotation_gain.Kp = 0.0;
	rotation_gain.Ki = 0.0;
	log_start();
	set_straight(600.0, 3500, 300, 0, 0);
	wait_straight();
	save_log_to_flash();
}

void mode_4(void) {
	log_start();
	while (log_flag) {
		duty.left = 100;
		duty.right = 100;
		duty_to_moter();
	}
	duty.left = 0;
	duty.right = 0;

	save_log_to_flash();
}

void mode_5(void) { //nomal_run.accel, nomal_run.vel_search,nomal_run.vel_search
	log_start();
	while (log_flag) {

	}
	duty.left = 0;
	duty.right = 0;

	save_log_to_flash();
}

void mode_6(void) {
	read_all_log_from_flash();
	log_output();
}

void mode_7(void) {
//	output_SEN();

//	fan_on();

//	out_put_pass(pass);
//	while (SWITCH == 1) {
//
//	}
}

void go_mode(void) {
	uint8_t i = 0;
	printf("0,mode=%d,%d\n", mode, (mode & 0x80));
	mode = mode | 0x80;
	printf("1,mode=%d,%d\n", mode, (mode & 0x80));
	Battery_Check();
	failsafe_flag = 0;
	HAL_Delay(1000);
	ideal_translation.accel = 0.0;
	ideal_translation.vel = 0.0;
	ideal_translation.dis = 0.0;
	ideal_rotation.accel = 0.0;
	ideal_rotation.dis = 0.0;
	ideal_rotation.vel = 0.0;
	rotation_parameter.back_rightturn_flag = 0;
	rotation_deviation.now = 0.0;
	rotation_deviation.cumulative = 0.0;
	wallcontrol_value = 0.0;
	run_left_deviation.cumulative = 0.0;
	run_right_deviation.cumulative = 0.0;
	moter_flag = 0;
	x.now = 0;
	y.now = 0;
	direction = 0;
	if ((mode & 0xf) == 0) {
		mode_0();
	} else if ((mode & 0xf) == 1) {
		printf("2,mode=%d,%d\n", mode, (mode & 0x80));
		for (i = 0; i < 2; i++) {
			set_led((mode & 0xf));
			HAL_Delay(100);
			set_led(0);
			HAL_Delay(100);
		}
		mode_1();
		printf("3,mode=%d,%d\n", mode, (mode & 0x80));
	} else if ((mode & 0xf) == 2) {
		for (i = 0; i < 2; i++) {
			set_led((mode & 0xf));
			HAL_Delay(100);
			set_led(0);
			HAL_Delay(100);
		}
		mode_2();
	} else if ((mode & 0xf) == 3) {
		for (i = 0; i < 2; i++) {
			set_led((mode & 0xf));
			HAL_Delay(100);
			set_led(0);
			HAL_Delay(100);
		}
		mode_3();
	} else if ((mode & 0xf) == 4) {
		for (i = 0; i < 2; i++) {
			set_led((mode & 0xf));
			HAL_Delay(100);
			set_led(0);
			HAL_Delay(100);
		}
		mode_4();
	} else if ((mode & 0xf) == 5) {
		for (i = 0; i < 2; i++) {
			set_led((mode & 0xf));
			HAL_Delay(100);
			set_led(0);
			HAL_Delay(100);
		}
		mode_5();
	} else if ((mode & 0xf) == 6) {
		for (i = 0; i < 2; i++) {
			set_led((mode & 0xf));
			HAL_Delay(100);
			set_led(0);
			HAL_Delay(100);
		}
		mode_6();
	} else if ((mode & 0xf) == 7) {
		for (i = 0; i < 2; i++) {
			set_led((mode & 0xf));
			HAL_Delay(100);
			set_led(0);
			HAL_Delay(100);
		}
		mode_7();
	}

	HAL_Delay(200);
	ideal_translation.accel = 0.0;
	ideal_translation.vel = 0.0;

	duty.left = 0;
	duty.right = 0;
	duty_to_moter();
	x.now = 0;
	y.now = 0;
	direction = 0;
	if (failsafe_flag == 1) {
		while (failsafe_counter < 1000) {
		}
	}
	mode = mode & 0x7f;
	moter_flag = 0;
	failsafe_flag = 0;
	SEN_check_flag = 0;
	set_led(0);
}
