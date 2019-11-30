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
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "pass.h"

void mode_0(void) {

//	read_all_walldata();
//	output_Walldata(ALL);
//	while(1);

	start_led();
	log_start();
	search_run_special(x.goal, y.goal, 4);
	save_all_walldata();

//	read_all_log_from_flash();
//	log_output();
//	move_pass_big_turn(nomal_run.accel, nomal_run.vel_max, 500.0);

}

void mode_1(void) {

	read_all_walldata();
	start_led();
	log_start();
	search_run_special(x.goal, y.goal, 4);
	save_all_walldata();

}

void mode_2(void) {
	read_all_walldata();
	make_pass(x.goal, y.goal, 4, 0);
	start_led();
	move_pass_big_turn(nomal_run.accel, nomal_run.vel_max, 500.0);
}

void mode_3(void) { //253.558
	start_led();
	adachi_search_run(x.goal, y.goal, 4, nomal_run.accel, nomal_run.vel_search, 1, 0);
	save_all_walldata();

}

void mode_4(void) {

//	start_led();
//	adachi_search_run(x.goal, y.goal, 4, nomal_run.accel, nomal_run.vel_search, 0, 0);
//	save_all_walldata();

	start_led();
	log_start();
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	set_straight(90.0, nomal_run.accel, nomal_run.vel_max, nomal_run.vel_search,
			nomal_run.vel_max);
	wait_straight();
	set_straight(45.0, nomal_run.accel, nomal_run.vel_max, nomal_run.vel_max,
			nomal_run.vel_max);
	HAL_GPIO_TogglePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin);
	wall_control_flag = 0;
	wait_straight();
	HAL_GPIO_TogglePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin);
	turn_right_180_big(nomal_run.vel_max);
	HAL_GPIO_TogglePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin);
	set_straight(90.0+90.0, nomal_run.accel, nomal_run.vel_max, nomal_run.vel_max,
			0.0);
	wall_control_flag = 0;
	wait_straight();


}

void mode_5(void) { //nomal_run.accel, nomal_run.vel_search,nomal_run.vel_search

//	rotation_gain.Kp = 0.2;//22
//	rotation_gain.Ki = 0.035; //45
//
//	wall_cntrol_gain.Ki = 0.05;
//	wall_cntrol_gain.Kp = 0.05;
//
//	start_led();
//	adachi_search_run(x.goal, y.goal, 4, nomal_run.accel, nomal_run.vel_search, 1, 0);
//	save_all_walldata();


	start_led();
	log_start();
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	set_straight(90.0, nomal_run.accel, nomal_run.vel_max, nomal_run.vel_search,
			nomal_run.vel_max);
	wait_straight();
	set_straight(45.0, nomal_run.accel, nomal_run.vel_max, nomal_run.vel_max,
			nomal_run.vel_max);
	HAL_GPIO_TogglePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin);
	wall_control_flag = 0;
	wait_straight();
	HAL_GPIO_TogglePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin);
	turn_left_180_big(nomal_run.vel_max);
	HAL_GPIO_TogglePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin);
	set_straight(90.0+90.0, nomal_run.accel, nomal_run.vel_max, nomal_run.vel_max,
			0.0);
	wall_control_flag = 0;
	wait_straight();


}

void mode_6(void) {
	rotation_gain.Kp = 0.17;//22
	rotation_gain.Ki = 0.03; //45

	wall_cntrol_gain.Ki = 0.0;
	wall_cntrol_gain.Kp = 0.05;

	start_led();
	adachi_search_run(x.goal, y.goal, 4, nomal_run.accel, nomal_run.vel_search, 1, 0);
	save_all_walldata();


//	start_led();
//	log_start();
//	go_entrance(nomal_run.accel, nomal_run.vel_search);
//
//	set_straight(90 * 6, nomal_run.accel, nomal_run.vel_search,
//			nomal_run.vel_search, 0);
//	wait_straight();

}

void mode_7(void) {

	known_vel=500;
	read_all_walldata();
	start_led();
	log_start();
	search_run_special(x.goal, y.goal, 4);
	save_all_walldata();


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
//	printf("1,%d,%d\n", log_flag, log_index);
	start_led();
	moter_flag = 0;
	log_flag = 1;
	while (log_index < 4000) {
		//	printf("2,%d,%d\n", log_flag, log_index);
	}
	log_flag = 0;
	set_buzzer(0, C_4, 600);
//	printf("3,%d,%d\n", log_flag, log_index);
	start_led();
//	printf("4,%d,%d\n", log_flag, log_index);
	moter_flag = 0;
	log_flag = 1;
//	printf("6,%d,%d\n", log_flag, log_index);
	while (log_flag == 1) {
		//	printf("5,%d,%d\n", log_flag,log_index);
	}
	set_buzzer(0, C_4, 600);
//	save_log_to_flash();


}

void go_mode(void) {
	uint8_t i = 0;
	wall_control_flag = 0;
	printf("0,mode=%d,%d\n", mode, (mode & 0x80));
	mode = mode | 0x80;
	printf("1,mode=%d,%d\n", mode, (mode & 0x80));
	Battery_Check();
	failsafe_flag = 0;
	HAL_Delay(500);
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
	set_buzzer(0, C_4, 400);
}
