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
#include "motion.h"
#include "adachi.h"
#include "buzzer.h"

void mode_0(void) {
//	uint8_t i;
	read_all_log_from_flash();
	log_output();

//	read_all_walldata();
//	adachi_map(x.goal, y.goal, 4, walldata.real);
//	output_Walldata(REAL);
}

void mode_1(void) {
//	moter_flag = 1;
//	printf("4,mode=%d,%d\n", mode, (mode & 0x80));

//	run_gain.Ki=0;

	start_led();
//	log_start();
	set_straight(90.0 * 2, 3500, 300, 0.0, 0.0);
	wait_straight();
//	HAL_Delay(500);/
//	log_flag = 0;
//	HAL_Delay(5);
//	log_flag=0;
	//	run_left_deviation.cumulative = 0.0;
//	run_right_deviation.cumulative = 0.0;
//	//	set_straight(90.0, 3500, 300, 0.0, 0.0);
////	wait_straight();
//	moter_flag = 0;
//	save_log_to_flash();
}

void mode_2(void) {

	start_led();
//	log_start();
//	adachi_search_run_known(x.goal, y.goal, 4, nomal_run.accel, nomal_run.vel_search, 0, 0);
	search_run_special(x.goal, y.goal, 4);
}

void mode_3(void) { //253.558
	rotation_gain.Kp = 0.41 / 2;
	rotation_gain.Ki = 0.005 / 10; //3
	start_led();
	log_start();
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	pass_180(nomal_run.accel, nomal_run.vel_search);
	slalom_left90(nomal_run.accel, nomal_run.vel_search);
	set_straight(45.0 * 3, nomal_run.accel, nomal_run.vel_search,
			nomal_run.vel_search, 0.0);
	wait_straight();
//	save_log_to_flash();
}

void mode_4(void) {
	start_led();
	//log_start();
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	pass_180(nomal_run.accel, nomal_run.vel_search);
	slalom_right90(nomal_run.accel, nomal_run.vel_search);
	stop90(nomal_run.accel, nomal_run.vel_search);
	//save_log_to_flash();
}

void mode_5(void) { //nomal_run.accel, nomal_run.vel_search,nomal_run.vel_search

	start_led();
	log_start();
	for (uint8_t i = 0; i < 8; i++) {
		turn_left(nomal_run.accel, nomal_run.vel_search);
	}
}

void mode_6(void) {

	start_led();
	log_start();

	front_wall_gain = 0.4;
	front_wall_flag = 1;
	while (1)
		;

}

void mode_7(void) {
//	start_led();
//	run_gain.Kp = 0.6;
//	run_gain.Ki = 0.2;
//	rotation_gain.Kp = 0.0;
//	rotation_gain.Ki = 0.001;
//	while (failsafe_flag == 0) {
//		printf("rot_Dev=%8.3f\n", rotation_deviation.cumulative);
//	}

	start_led();
	moter_flag = 0;
	log_start();
	while (log_index < 1000) {

	}
	set_buzzer(0, C_4, 600);
	log_flag = 0;
	start_led();
	moter_flag = 0;
	log_flag = 1;
	while (log_index < 2000) {

	}
	set_buzzer(0, C_4, 600);
	log_flag = 0;
	start_led();
	moter_flag = 0;
	log_flag = 1;
	while (log_index < 3000) {

	}
	set_buzzer(0, C_4, 600);
	log_flag = 0;
	start_led();
	moter_flag = 0;
	log_flag = 1;
	while (log_index < 4000) {

	}
	set_buzzer(0, C_4, 600);
	while (log_flag == 1) {

	}
	set_buzzer(0, C_4, 600);
	save_log_to_flash();

//output_SEN();

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

	if (log_index != 0) {
		save_log_to_flash();
	}
}
