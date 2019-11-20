/*
 * motion.c
 *
 *  Created on: 2019/10/14
 *      Author: kt33_
 */

#include "adachi.h"
#include "variable.h"
#include "run.h"
#include "other.h"
#include "stdint.h"
#include "walldata.h"
#include "motion.h"
#include "buzzer.h"

void set_center_by_front_Wall(void) {
	front_wall_flag = 0;
	front_wall_value_L = (int) 2585 * 0.8;
	front_wall_value_R = (int) 4200 * 0.8;
	front_wall_gain = 0.1;
	front_wall_flag = 1;
	HAL_Delay(700);
	front_wall_flag = 0;
	front_wall_value_L = 2585;
	front_wall_value_R = 4200;
	front_wall_gain = 0.03;
	front_wall_flag = 1;
	HAL_Delay(300);
	front_wall_flag = 0;
	front_wall_value_L = 2585;
	front_wall_value_R = 4200;
	front_wall_gain = 0.1;
	front_wall_flag = 1;
	HAL_Delay(500);
	front_wall_flag = 0;
	rotation_deviation.cumulative = 0.0;
}

void go_entrance(float accel, float vel) {
//	rotation_deviation.now = 0.0;
//rotation_deviation.cumulative = 0.0;
	moter_flag = 1;
	set_straight(67.0, accel, vel, 0.0, vel);
	wait_straight();
}

void go_center(float accel, float vel) {
//	rotation_deviation.now = 0.0;
//	rotation_deviation.cumulative = 0.0;
	set_straight(22.0, accel, vel, 0.0, 0.0);
	wait_straight();
}

void turn_left(float accel, float vel) {
	if (getWall(x.now, y.now, direction, &walldata.real) == 0) {
		set_straight(45.0, accel, vel, vel, 0.0);
		wait_straight();
		HAL_Delay(200);
	} else {
		set_straight(10.0, accel, vel, vel, 0.0);
		wait_straight();
		set_center_by_front_Wall();
	}
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(200);
	set_straight(45.0, accel, vel, 0.0, vel);
	wait_straight();
}

void turn_right(float accel, float vel) {
	if (getWall(x.now, y.now, direction, &walldata.real) == 0) {
		set_straight(45.0, accel, vel, vel, 0.0);
		wait_straight();
		HAL_Delay(200);
	} else {
		set_straight(10.0, accel, vel, vel, 0.0);
		wait_straight();
		set_center_by_front_Wall();
	}
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(200);
	set_straight(45.0, accel, vel, 0.0, vel);
	wait_straight();
}

void pass_180(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, vel);
	wait_straight();
}

void turn_180(float accel, float vel) {
	set_straight(45.0, accel, vel, vel, 0.0);
	wait_straight();
	HAL_Delay(50);

//	if (getWall(x.now, y.now, direction + 1, &walldata.real) == 1) {
//		set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//		set_center_by_front_Wall();
//		set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//	} else if (getWall(x.now, y.now, direction + 3, &walldata.real) == 1) {
//		set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//		set_center_by_front_Wall();
//		set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//	} else {
	set_rotation(180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
//		set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//		set_center_by_front_Wall();
//		set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//	}

	if (getWall(x.now, y.now, (direction + 2) % 4, &walldata.real) == 1) {
		HAL_Delay(50);
		back_100();
//		if (u_turn_counter == 100) {
//
//			save_all_walldata();
//			u_turn_counter = 0;
//		} else if (u_turn_counter > 4 || (x.now == 7 && y.now == 7)
//				|| (x.now == 8 && y.now == 7) || (x.now == 7 && y.now == 8)
//				|| (x.now == 8 && y.now == 8)) {
//			save_all_walldata();
//			u_turn_counter = 0;
//		} else {
//			HAL_Delay(5);
//			u_turn_counter++;
//		}

		rotation_deviation.now = 0.0;
		rotation_deviation.cumulative = 0.0;
		wallcontrol_value = 0.0;
		run_left_deviation.cumulative = 0.0;
		run_right_deviation.cumulative = 0.0;
		wallcontrol_value = 0.0;
		go_entrance(accel, vel);
	} else {
//		if (u_turn_counter == 100) {
//			save_all_walldata();
//			u_turn_counter = 0;
//		} else if (u_turn_counter > 4 || (x.now == 7 && y.now == 7)
//				|| (x.now == 8 && y.now == 7) || (x.now == 7 && y.now == 8)
//				|| (x.now == 8 && y.now == 8)) {
//			save_all_walldata();
//			u_turn_counter = 0;
//		} else {
//			HAL_Delay(5);
//			u_turn_counter++;
//		}
		set_straight(-20.0, 600.0, 300.0, 0.0, 0.0);
		wait_straight();
		HAL_Delay(10);
		rotation_deviation.now = 0.0;
		rotation_deviation.cumulative = 0.0;
		wallcontrol_value = 0.0;
		run_left_deviation.cumulative = 0.0;
		run_right_deviation.cumulative = 0.0;
		wallcontrol_value = 0.0;
		set_straight(65.0, accel, vel, 0.0, vel);
		wait_straight();
	}
}

void ketuate(float accel, float vel) {
	set_straight(45.0, accel, vel, vel, 0.0);
	wait_straight();
//	if (getWall(x.now, y.now, direction + 1, &walldata.real) == 1) {
//		set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//		set_center_by_front_Wall();
//		set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//	} else if (getWall(x.now, y.now, direction + 3, &walldata.real) == 1) {
//		set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//		set_center_by_front_Wall();
//		set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//				0.0);
//		wait_rotation();
//	} else {
	set_rotation(180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
//	}

	HAL_Delay(50);
	back_100();
//	if (u_turn_counter == 100) {
//		save_all_walldata();
//		u_turn_counter = 0;
//	} else if (u_turn_counter > 4 || (x.now == 7 && y.now == 7)
//			|| (x.now == 8 && y.now == 7) || (x.now == 7 && y.now == 8)
//			|| (x.now == 8 && y.now == 8)) {
//		save_all_walldata();
//		u_turn_counter = 0;
//	} else {
//		HAL_Delay(5);
//		u_turn_counter++;
//	}
	rotation_deviation.cumulative = 0.0;
	rotation_deviation.now = 0.0;
	rotation_deviation.cumulative = 0.0;
	wallcontrol_value = 0.0;
	run_left_deviation.cumulative = 0.0;
	run_right_deviation.cumulative = 0.0;
	wallcontrol_value = 0.0;
	go_entrance(accel, vel);
}

void ketuate_right(float accel, float vel) {
	set_straight(45.0, accel, vel, vel, 0.0);
	wait_straight();
	HAL_Delay(5);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(5);
	back_100();
	HAL_Delay(50);
	go_center(accel, vel);
	HAL_Delay(5);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(5);
	back_100();
	HAL_Delay(5);
	rotation_deviation.cumulative = 0.0;
	go_entrance(accel, vel);
}

void ketuate_left(float accel, float vel) {
	set_straight(45.0, accel, vel, vel, 0.0);
	wait_straight();
	HAL_Delay(5);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(5);
	back_100();
	HAL_Delay(5);
	set_straight(57.0, accel, vel, 0.0, 0.0);
	wait_straight();
	HAL_Delay(5);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(5);
	back_100();
	HAL_Delay(5);
	rotation_deviation.cumulative = 0.0;
	go_entrance(accel, vel);
}

void back_100(void) {
	set_straight(-40.0, 300, 80, 0.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
}

void ketuate_goal_left(float accel, float vel) {
	set_straight(45.0, accel, vel, vel, 0.0);
	wait_straight();
	HAL_Delay(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(50);
	back_100();
	HAL_Delay(50);
	go_center(accel, vel);
	HAL_Delay(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(50);
	back_100();
	HAL_Delay(50);
	rotation_deviation.cumulative = 0.0;
}

void ketuate_goal_right(float accel, float vel) {
	set_straight(45.0, accel, vel, vel, 0.0);
	wait_straight();
	HAL_Delay(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(50);
	back_100();
	HAL_Delay(50);
	go_center(accel, vel);
	HAL_Delay(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(50);
	back_100();
	HAL_Delay(50);
	rotation_deviation.cumulative = 0.0;
}

void non_ketuate_goal_turn(float accel, float vel) {
	set_straight(45.0, accel, vel, vel, 0.0);
	wait_straight();
	HAL_Delay(50);
	set_rotation(180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	HAL_Delay(50);
	set_straight(-25.0, 500, 150, 0.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	HAL_Delay(50);
}

void non_ketuate_goal(float accel, float vel) {
	set_straight(45.0, accel, vel, vel, 0.0);
	wait_straight();
	HAL_Delay(50);
	set_straight(-25.0, 500, 150, 0.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	HAL_Delay(50);
}

void slalom_left90(float run_accel, float run_vel) {
	float rota_accel, rota_vel, in_offset, out_offset, angle_offset = 0.0;
	gain_t box;
	box.Ki = rotation_gain.Ki;
	box.Kp = rotation_gain.Kp;
//	if ((SEN_F.now < SEN_F.reference)
//				&& (getWall(x.now, y.now, direction, &walldata.real))) {
//			while (SEN_F.now < SEN_F.reference) {
//				set_buzzer(0, C_5, 200);
//				ideal_translation.dis = 0.0;
//			}
//	}
	if (run_vel == 280.0) {
		rotation_gain.Ki *= 1;
		rotation_gain.Kp *= 1;
		rota_accel = 10000;
		rota_vel = 1300;
		in_offset = 7;
		out_offset = 15;
	}

	set_straight(in_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
	set_rotation(90.0 + angle_offset, rota_accel, rota_vel, run_vel);
	wait_rotation();
	set_straight(out_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
	rotation_gain.Ki = box.Ki;
	rotation_gain.Kp = box.Kp;
}

void slalom_right90(float run_accel, float run_vel) {
	float rota_accel, rota_vel, in_offset, out_offset;
	float angle_offset = 0.0;
	gain_t box;
	box.Ki = rotation_gain.Ki;
	box.Kp = rotation_gain.Kp;
//	if ((SEN_F.now < SEN_F.reference)
//			&& (getWall(x.now, y.now, direction, &walldata.real))) {
//		while (SEN_F.now < SEN_F.reference) {
//			set_buzzer(0, C_5, 200);
//			ideal_translation.dis = 0.0;
//		}
//	}
	if (run_vel == 280.0) {
		rotation_gain.Ki *= 1.2;
		rotation_gain.Kp *= 1;
		rota_accel = 10000;
		rota_vel = 1300;
		in_offset = 9;
		out_offset = 17;
	}
	set_straight(in_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, run_vel);
	wait_rotation();
	set_straight(out_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
	rotation_gain.Ki = box.Ki;
	rotation_gain.Kp = box.Kp;
}

void stop90(float run_accel, float run_vel) {
	set_straight(45.0, run_accel, run_vel, run_vel, 0.0);
	wait_straight();
}

void slalom_left_check(float accel, float vel) {
	moter_flag = 1;
	nomal_run.vel_search = 600.0;
	go_entrance(accel, vel);
	slalom_left90(accel, vel);
	stop90(accel, vel);
}

void slalom_right_check(float accel, float vel) {
	moter_flag = 1;
	nomal_run.vel_search = 600.0;
	go_entrance(accel, vel);
	slalom_right90(accel, vel);
	stop90(accel, vel);
}

void turn_left_180_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 20.0;
		out_offset = 38.0;
		rota_accel = 6000.0;
		rota_vel = 400.0;
		kabekire_dis = 7.2;
		angle_offset = 0;
	}

	if (vel == 1000.0) {
		in_offset = 0.0;
		out_offset = 39.0;
		rota_accel = 8700.0;
		rota_vel = 642.0;
		kabekire_dis = 1.0;
		angle_offset = -1.2;
	}

	if (vel == 1200.0) {
		in_offset = 0.0;
		out_offset = 33.0;
		rota_accel = 8700.0;
		rota_vel = 792.0;
		kabekire_dis = 1.0;
		angle_offset = 0.0;
	}
	if (vel == 1300.0) {
		in_offset = 8.2;
		out_offset = 61.0;
		rota_accel = 28500.0;
		rota_vel = 840.0;
		kabekire_dis = 3.2;
		angle_offset = -1.8;
	}

	while (SEN_L.now > SEN_L.threshold) {
		ideal_translation.dis = kabekire_dis;
	}
//	Log[0]=ideal_translation.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(180.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	direction += 3;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();
	direction += 1;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();

}

void turn_right_180_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 30.0;
		out_offset = 43.0;
		rota_accel = 4700.0;
		rota_vel = 385.0;
		angle_offset = 1.0;
		kabekire_dis = 13.2;
	}
	if (vel == 1000.0) {
		in_offset = 0.0;
		out_offset = 35.0;
		rota_accel = 8700.0;
		rota_vel = 660.0;
		kabekire_dis = 4.0;
		angle_offset = 1.5;
	}

	if (vel == 1200.0) {
		in_offset = 0.0;
		out_offset = 39.0;
		rota_accel = 10500.0; //8700
		rota_vel = 790.0;
		kabekire_dis = 7.0;
		angle_offset = 3.5;
	}
	if (vel == 1300.0) {
		in_offset = 0.0;
		out_offset = 52.0;
		rota_accel = 22000.0; //8700
		rota_vel = 820.0;
		kabekire_dis = 2.6;
		angle_offset = -1.8;
	}

	while (SEN_R.now > SEN_R.threshold) {
		ideal_translation.dis = kabekire_dis;
	}
//	Log[0] = ideal_translation.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-180.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	direction += 1;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();
	direction += 3;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();
}

void turn_left_90_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 36.0;
		out_offset = 52.0;
		rota_accel = 3000.0;
		rota_vel = 350.0;
		kabekire_dis = 7.8;
		angle_offset = 0.0;
	}
	if (vel == 1000.0) {
		in_offset = 23.0;
		out_offset = 53.0;
		rota_accel = 6200.0;
		rota_vel = 1500.0;
		kabekire_dis = 3.0;
		angle_offset = 0.3;
	}
	if (vel == 1200.0) {
		in_offset = 21.0;
		out_offset = 57.0;
		rota_accel = 9500.0;
		rota_vel = 1660.0;
		kabekire_dis = -4.0;
		angle_offset = -0.4;
	}
	if (vel == 1300.0) {
		in_offset = 19.0;
		out_offset = 71.0;
		rota_accel = 13000.0;
		rota_vel = 1660.0;
		kabekire_dis = -4.0;
		angle_offset = 0.4;
	}

	while (SEN_L.now > SEN_L.threshold) {
		ideal_translation.dis = kabekire_dis;
	}
//	Log[0] = ideal_translation.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(90.0 - angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	coordinate();
}

void turn_right_90_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 36.0;
		out_offset = 50.0;
		rota_accel = 3000.0;
		rota_vel = 350.0;
		angle_offset = 1.0;
		kabekire_dis = 12.0;
	}

	if (vel == 1000.0) {
		in_offset = 26.0;
		out_offset = 55.0;
		rota_accel = 6700.0;
		rota_vel = 1700.0;
		kabekire_dis = 5.0;
		angle_offset = 1.5;
	}
	if (vel == 1200.0) {
		in_offset = 15.0;
		out_offset = 49.0;
		rota_accel = 8600.0;
		rota_vel = 1700.0;
		kabekire_dis = 3.0;
		angle_offset = 2.2;
	}
	if (vel == 1300.0) {
		in_offset = 18.0;
		out_offset = 62.0;
		rota_accel = 11500.0;
		rota_vel = 1630.0;
		kabekire_dis = 1.3;
		angle_offset = -1.5;
	}
	while (SEN_R.now > SEN_R.threshold) {
		ideal_translation.dis = kabekire_dis;
	}
//	Log[0] = ideal_translation.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	coordinate();
}

void farst_turn_right_90_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 36.0;
		out_offset = 50.0;
		rota_accel = 3000.0;
		rota_vel = 350.0;
		angle_offset = 1.0;
	}

	if (vel == 1000.0) {
		in_offset = 25.0;
		out_offset = 53.0;
		rota_accel = 6700.0;
		rota_vel = 1700.0;
		angle_offset = 1.0;
	}

	if (vel == 1200.0) {
		in_offset = 16.0;
		out_offset = 48.0;
		rota_accel = 8600.0;
		rota_vel = 1700.0;
		angle_offset = 2.2;
	}

	if (vel == 1300.0) {
		in_offset = 18.0;
		out_offset = 62.0;
		rota_accel = 11500.0;
		rota_vel = 1630.0;
//		kabekire_dis = 1.3;
		angle_offset = -1.5;
	}
	coordinate();

	set_straight(in_offset + 50.0, nomal_run.accel, vel, 0.0, vel);
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	coordinate();
}

void turn_right_45_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;

	if (vel == 1000.0) {
		in_offset = 15.0;
		out_offset = 62.0 - 20.0;
		rota_accel = 10000.0;
		rota_vel = 1000.0;
		angle_offset = 0.0;
		kabekire_dis = 4.0;
	}

	if (vel == 1200.0) {
		in_offset = 4.5;
		out_offset = 97.0 - 20.0;
		rota_accel = 21000.0;
		rota_vel = 1300.0;
		angle_offset = 5.4;
		kabekire_dis = 2.4;
	}

	if (vel == 1300.0) {
		in_offset = 12.0;
		out_offset = 84.0 - 20.0;
		rota_accel = 24000.0;
		rota_vel = 1170.0;
		angle_offset = -1.2; //3.0
		kabekire_dis = 0.0;
	}

	while (SEN_R.now > SEN_R.threshold) {
		ideal_translation.dis = kabekire_dis;
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
//	wall_control_flag = 0;
	wall_control_oblique_flag = 1;
	wait_straight();
}

void turn_left_45_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 8.0;
		out_offset = 78.0 - 20.0;
		rota_accel = 12000.0;
		rota_vel = 1100.0;
		angle_offset = 0.0;
		kabekire_dis = 3.0;
	}

	if (vel == 1200.0) {
		in_offset = 12.0;
		out_offset = 106.0 - 20.0;
		rota_accel = 28000.0;
		rota_vel = 1150.0;
		angle_offset = 0.86;
		kabekire_dis = -4.0;
	}

	if (vel == 1300.0) {
		in_offset = 11.0;
		out_offset = 123.0 - 20.0;
		rota_accel = 40000.0;
		rota_vel = 1700.0;
		angle_offset = -0.0;
		kabekire_dis = -5.0;
	}

	while (SEN_L.now > SEN_L.threshold) {
		ideal_translation.dis = kabekire_dis;
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
//	wall_control_flag = 0;
	wall_control_oblique_flag = 1;
	wait_straight();
}

void turn_right_135_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 3.0;
		out_offset = 20.0 - 20.0;
		rota_accel = 9000.0;
		rota_vel = 650.0;
		angle_offset = 0.0;
		kabekire_dis = 0.0;
	}

	if (vel == 1200.0) {
		in_offset = 15.0;
		out_offset = 47.0 - 20.0;
		rota_accel = 12700.0;
		rota_vel = 900.0;
		angle_offset = 3.6;
		kabekire_dis = 1.0;
	}

	if (vel == 1300.0) {
		in_offset = 8.0;
		out_offset = 40.0 - 20.0;
		rota_accel = 19000.0;
		rota_vel = 878.0;
		angle_offset = -2.8;
		kabekire_dis = -1.0;
	}

	while (SEN_R.now > SEN_R.threshold) {
		ideal_translation.dis = kabekire_dis;
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
//	wall_control_flag = 0;
	wall_control_oblique_flag = 1;
	wait_straight();
}

void turn_left_135_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 1.0;
		out_offset = 20.0 - 20.0;
		rota_accel = 9000.0;
		rota_vel = 658.0;
		angle_offset = 0.0;
		kabekire_dis = 0.0;
	}

	if (vel == 1200.0) {
		in_offset = 19.4;
		out_offset = 45.0 - 20.0;
		rota_accel = 13000.0;
		rota_vel = 900.0;
		angle_offset = 0.0;
		kabekire_dis = 0.0;
	}

	if (vel == 1300.0) {
		in_offset = 12.0;
		out_offset = 51.0 - 20.0;
		rota_accel = 25000.0;
		rota_vel = 900.0;
		angle_offset = -0.2;
		kabekire_dis = -2.0;
	}

	while (SEN_L.now > SEN_L.threshold) {
		ideal_translation.dis = kabekire_dis;
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
//	wall_control_flag = 0;
	wall_control_oblique_flag = 1;
	wait_straight();
}

void turn_right_45_out(float vel) {
	float in_offset = 0.0, out_offset = 0.0, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 44.0 + 20.0;
		out_offset = 30.0;
		rota_accel = 10000.0;
		rota_vel = 1000.0;
		angle_offset = 2.0;
		kabekire_dis = 12.8;
	}

	if (vel == 1200.0) {
		in_offset = 34.0 + 20.0;
		out_offset = 21.0;
		rota_accel = 11000.0;
		rota_vel = 1100.0;
		angle_offset = 3.4;
		kabekire_dis = 10.2;
	}

	if (vel == 1300.0) {
		in_offset = 48.0 + 20.0;
		out_offset = 50.0;
		rota_accel = 24000.0;
		rota_vel = 1170.0;
		angle_offset = 1.2; //3.0
		kabekire_dis = 15.0;
	}

	while (SEN_R.now > 2000) {
		ideal_translation.dis = kabekire_dis;
//		Log[0] = ideal_translation.dis;
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
//	wall_control_flag = 0;
	oblique_Front_gain *= 1;
	oblique_Side_gain *= 1;
	wall_control_oblique_flag = 3;
	wait_straight();
//	wall_control_oblique_flag = 0;
	set_rotation(-45.0 + angle_offset, rota_accel, rota_vel, vel);
	oblique_Front_gain /= 1;
	oblique_Side_gain /= 1;
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_oblique_flag = 0;
	wall_control_flag = 1;
	wait_straight();
}

void turn_left_45_out(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 41.0 + 20.0;
		out_offset = 42.0;
		rota_accel = 12000.0;
		rota_vel = 1100.0;
		angle_offset = 0.0;
		kabekire_dis = 7.0;
	}

	if (vel == 1200.0) {
		in_offset = 28.0 + 20.0; //28
		out_offset = 35.0;
		rota_accel = 12500.0;
		rota_vel = 1170.0;
		angle_offset = 1.0; //0.0;
		////////////////////////////
		kabekire_dis = 3.2;		//7.2
		/////////////////////////////////
	}

	if (vel == 1300.0) {
		in_offset = 39.0 + 20.0;
		out_offset = 91.0;
		rota_accel = 40000.0;
		rota_vel = 1700.0;
		angle_offset = 0.6;
		kabekire_dis = 2.0;
	}

	while (SEN_L.now > 2000) {
		ideal_translation.dis = kabekire_dis;
//		LEFTFRONT=1;
//		RIGHTFRONT=1;
//		CENTERFRONT=1;
//		Log[0] = ideal_translation.dis;
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
//	wall_control_flag = 0;
	wall_control_oblique_flag = 2;
	oblique_Front_gain *= 1;
	oblique_Side_gain *= 1;
	wait_straight();
//	wall_control_oblique_flag = 0;
	set_rotation(45.0 + angle_offset, rota_accel, rota_vel, vel);
	oblique_Front_gain /= 1;
	oblique_Side_gain /= 1;
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_oblique_flag = 0;
	wall_control_flag = 1;
	wait_straight();
}

void turn_right_135_out(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	oblique_front_box = oblique_Front_gain;
	oblique_side_box = oblique_Side_gain;

	if (vel == 1000.0) {
		in_offset = 0.0 + 20;
		out_offset = 50.0;
		rota_accel = 8000.0;
		rota_vel = 830.0;
		angle_offset = 2.0;
		kabekire_dis = 16.0;
	}

	if (vel == 1200.0) {
		in_offset = 8.0 + 20;
		out_offset = 62.0;
		rota_accel = 12700.0;
		rota_vel = 940.0;
		angle_offset = 3.2;
		kabekire_dis = 12.0;
	}

	if (vel == 1300.0) {
		in_offset = 0.0 + 20.0;
		out_offset = 66.0;
		rota_accel = 24000.0;
		rota_vel = 910.0;
		angle_offset = 0.2;
		kabekire_dis = 13.0;
	}

	while (SEN_R.now > 2000) {
		ideal_translation.dis = kabekire_dis;
//		Log[0] = ideal_translation.dis;
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
	//log_start();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
//	wall_control_flag = 0;
	oblique_Front_gain *= oblique_offset_front;
	oblique_Side_gain *= oblique_offset_side;
	wall_control_oblique_flag = 3;
	wait_straight();
//	wall_control_oblique_flag = 0;
	set_rotation(-135.0 + angle_offset, rota_accel, rota_vel, vel);
	oblique_Front_gain = oblique_front_box;
	oblique_Side_gain = oblique_side_box;
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_oblique_flag = 0;
	wall_control_flag = 1;
	wait_straight();
}

void turn_left_135_out(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}
	oblique_front_box = oblique_Front_gain;
	oblique_side_box = oblique_Side_gain;

	if (vel == 1000.0) {
		in_offset = 0.0 + 20.0;
		out_offset = 50.0;
		rota_accel = 9000.0;
		rota_vel = 710.0;
		angle_offset = 0.0;
		kabekire_dis = 10.8;
	}

	if (vel == 1200.0) {
		in_offset = 10.0 + 20.0;
		out_offset = 71.0;
		rota_accel = 12600.0;
		rota_vel = 980.0;
		angle_offset = 0.8;
		kabekire_dis = 5.2;
	}

	if (vel == 1300.0) {
		in_offset = 2.0 + 20.0;
		out_offset = 74.0;
		rota_accel = 24000.0;
		rota_vel = 920.0;
		angle_offset = -0.2;
		kabekire_dis = 3.9;
	}

	while (SEN_L.now > 2000.0) {
		ideal_translation.dis = kabekire_dis;
//		Log[0] = ideal_translation.dis;
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
//	wall_control_flag = 0;
	wall_control_oblique_flag = 2;
	oblique_Front_gain *= oblique_offset_front;
	oblique_Side_gain *= oblique_offset_side;
	wait_straight();
//	wall_control_oblique_flag = 0;
	set_rotation(135.0 + angle_offset, rota_accel, rota_vel, vel);
	oblique_Front_gain = oblique_front_box;
	oblique_Side_gain = oblique_side_box;
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_oblique_flag = 0;
	wall_control_flag = 1;
	wait_straight();
}

void turn_left_v90(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;

	oblique_front_box = oblique_Front_gain;
	oblique_side_box = oblique_Side_gain;

	if (vel == 1000.0) {
		in_offset = 0.0 + 20.0;
		out_offset = 39.0 - 20.0;
		rota_accel = 11000.0;
		rota_vel = 900.0;
		angle_offset = -1.0;
		kabekire_dis = 10.8;
		while (SEN_L.now > 2000) {
			ideal_translation.dis = kabekire_dis;
			//		Log[0] = ideal_translation.dis;
		}
	}

	if (vel == 1200.0) {
		in_offset = -3.4 + 20.0;
		out_offset = 55.0 - 20.0;
		rota_accel = 16000.0;
		rota_vel = 1350.0;
		angle_offset = 0.8;
		kabekire_dis = 6.0;
		while (SEN_L.now > 2000) {
			ideal_translation.dis = kabekire_dis;
			//		Log[0] = ideal_translation.dis;
		}
	}

	if (vel == 1300.0) {
		in_offset = -8.0 + 20.0;
		out_offset = 61.0 - 20;
		rota_accel = 22000.0;
		rota_vel = 1260.0;
		kabekire_dis = 5.2;
		angle_offset = 0.8;	//0.4
		while (SEN_L.now > 1500) {
			ideal_translation.dis = kabekire_dis;
			//		Log[0] = ideal_translation.dis;
		}
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
	wall_control_oblique_flag = 2;
	oblique_Front_gain *= oblique_offset_front;
	oblique_Side_gain *= oblique_offset_side;
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);

	wait_straight();
//	wall_control_oblique_flag = 0;
	set_rotation(90.0 + angle_offset, rota_accel, rota_vel, vel);
	oblique_Front_gain = oblique_front_box;
	oblique_Side_gain = oblique_side_box;
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_oblique_flag = 1;

	wait_straight();

}

void turn_right_v90(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;

	oblique_front_box = oblique_Front_gain;
	oblique_side_box = oblique_Side_gain;

	if (vel == 1000.0) {
		in_offset = 4.0 + 20.0;
		out_offset = 28.0 - 20.0;
		rota_accel = 12000.0;
		rota_vel = 705.0;
		angle_offset = 2.0;
		kabekire_dis = 14.3;
	}

	if (vel == 1200.0 || vel == 1300.0) {
		in_offset = -4.8 + 20.0;
		out_offset = 34.0 - 20.0;
		rota_accel = 14000.0;
		rota_vel = 1300.0;
		angle_offset = 2.2;	//2.2
		kabekire_dis = 13.2;
	}

	if (vel == 1300.0) {
		in_offset = -3.0 + 20.0;
		out_offset = 81.0 - 20.0;
		rota_accel = 30000.0;	//26000
		rota_vel = 1500.0;	//1100
		kabekire_dis = 15.6;
		angle_offset = 2.0;
	}

	while (SEN_R.now > 2000.0) {
		ideal_translation.dis = kabekire_dis;
//		Log[0] = ideal_translation.dis;
	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
//	if (vel == 1300.0) {
//		set_straight(in_offset-5.0, 12000.0, vel, vel, 1200.0);
//		rotation_gain.Kp = 0.62;
//		rotation_gain.Ki = 0.010;
//	} else {
//		set_straight(in_offset, nomal_run.accel, vel, vel, vel);
//	}

	oblique_Front_gain *= oblique_offset_front;
	oblique_Side_gain *= oblique_offset_side;
	wall_control_oblique_flag = 3;

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
//	wall_control_flag = 0;

	wait_straight();
//	wall_control_oblique_flag = 0;
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();

	//	if (vel == 1300.0) {
//		set_straight(out_offset-5.0, 14000.0, vel, 1200.0, vel);
//		rotation_gain.Kp = 0.67;
//		rotation_gain.Ki = 0.0025;
//	} else {
//		set_straight(out_offset, nomal_run.accel, vel, vel, vel);
//	}

	oblique_Front_gain = oblique_front_box;
	oblique_Side_gain = oblique_side_box;
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);

	wall_control_oblique_flag = 1;
	wait_straight();

}

void farst_turn_right_45_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel; //, kabekire_dis
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 15.0;
		out_offset = 62.0 - 20.0;
		rota_accel = 10000.0;
		rota_vel = 1000.0;
		angle_offset = 0.0;
//		kabekire_dis = 4.0;
	}

	if (vel == 1200.0) {
		in_offset = 1.5;
		out_offset = 97.0 - 20.0;
		rota_accel = 21000.0;
		rota_vel = 1300.0;
		angle_offset = 4.4;
//		kabekire_dis = 2.4;
	}

	if (vel == 1300.0) {
		in_offset = 12.0;
		out_offset = 84.0 - 20.0;
		rota_accel = 24000.0;
		rota_vel = 1170.0;
		angle_offset = -1.2; //3.0
//		kabekire_dis = 0.0;
	}

//	while (SEN_R.now > SEN_R.threshold) {
//		ideal_translation.dis = kabekire_dis;
//	}

//	Log[0] = ideal_translation.dis;
//	log_sampling();
	set_straight(in_offset + 52.0, nomal_run.accel, vel, 0.0, vel);
	wait_straight();
	set_rotation(-45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_oblique_flag = 1;
	wait_straight();
}

void farst_turn_right_135_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel; //, kabekire_dis
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 3.0;
		out_offset = 20.0 - 20.0;
		rota_accel = 9000.0;
		rota_vel = 650.0;
		angle_offset = 0.0;
//		kabekire_dis = 5.0;
	}

	if (vel == 1200.0) {
		in_offset = 15.0;
		out_offset = 47.0 - 20.0;
		rota_accel = 12700.0;
		rota_vel = 900.0;
		angle_offset = 3.6;
//		kabekire_dis = 0.0;
	}

	if (vel == 1300.0) {
		in_offset = 5.0;
		out_offset = 40.0 - 20.0;
		rota_accel = 19000.0;
		rota_vel = 878.0;
		angle_offset = -2.8;
//		kabekire_dis = -1.0;
	}

//	while (SEN_R.now > SEN_R.threshold) {
////		ideal_translation.dis = kabekire_dis;
//	}

//	Log[0] = ideal_translation.dis;
	log_sampling();
	set_straight(in_offset + 50, nomal_run.accel, vel, 0.0, vel);
	wait_straight();
	set_rotation(-135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_oblique_flag = 1;
	wait_straight();
}
