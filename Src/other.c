/*
 * other.c
 *
 *  Created on: 2019/09/05
 *      Author: kt33_
 */

#include "other.h"
#include "variable.h"
#include "main.h"
#include "string.h"
#include "spi.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "buzzer.h"

#define CHATT 10000
#define SECTOR_BASE_ADRR 0x08160000ul
#define SECTOR14_BASE_ADRR 0x08140000ul
#define SECTOR13_BASE_ADRR 0x08120000ul

void chattering(void) {
	int i = 0;
	while (i < CHATT) {
		i++;
	}
	while (HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin) == 0) {
	}
	i = 0;
	while (i < CHATT) {
		i++;
	}
}

void write_flash_log(uint32_t address, uint8_t *data, uint32_t size) {
	uint32_t add = address;
	HAL_FLASH_Unlock(); //flash unlook
	if (address == SECTOR14_BASE_ADRR) {
		FLASH_EraseInitTypeDef EraseInitStruct;
		EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS; //erase only sector
		EraseInitStruct.Sector = FLASH_SECTOR_14; //sector Name
		EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; //2.7 to 3.6V
		EraseInitStruct.NbSectors = 1; //Number of sectors to be erased.
		uint32_t SectorError; //srroe sector nomber
		HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError); //erase sector
	} else if (address == SECTOR13_BASE_ADRR) {
		FLASH_EraseInitTypeDef EraseInitStruct;
		EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS; //erase only sector
		EraseInitStruct.Sector = FLASH_SECTOR_13; //sector Name
		EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; //2.7 to 3.6V
		EraseInitStruct.NbSectors = 1; //Number of sectors to be erased.
		uint32_t SectorError; //srroe sector nomber
		HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError); //erase sector
	}
	for (add = address; add < (address + size); add++) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, add, *data);
		data++;
	}
	HAL_FLASH_Lock(); //flash look
}

void save_log_to_flash(void) {
	log_flag = 0;
	write_flash_log(SECTOR14_BASE_ADRR, (uint8_t*) &mylog, sizeof(mylog));
	write_flash_log(SECTOR13_BASE_ADRR, (uint8_t*) &mylog2, sizeof(mylog2));
}

void read_all_log_from_flash(void) {
	read_flash(SECTOR14_BASE_ADRR, (uint8_t*) &mylog, sizeof(mylog));
	read_flash(SECTOR13_BASE_ADRR, (uint8_t*) &mylog2, sizeof(mylog2));
}

void erase_flash(void) {
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS; //erase only sector
	EraseInitStruct.Sector = FLASH_SECTOR_15; //sector Name
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; //2.7 to 3.6V
	EraseInitStruct.NbSectors = 1; //Number of sectors to be erased.

	uint32_t SectorError; //srroe sector nomber

	HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError); //erase sector
}

void write_flash(uint32_t address, uint8_t *data, uint32_t size) {
	uint32_t add = address;
	HAL_FLASH_Unlock(); //flash unlook
	erase_flash();
	for (add = address; add < (address + size); add++) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, add, *data);
		data++;
	}
	HAL_FLASH_Lock(); //flash look
}

void read_flash(uint32_t address, uint8_t *data, uint32_t size) {
	memcpy(data, (uint8_t*) address, size);
}

void save_all_walldata(void) {
	if (failsafe_flag == 0) {
		moter_flag=0;
//		set_buzzer(0, C_4, 400);
//		set_buzzer(1, D_4, 400);
		write_flash(SECTOR_BASE_ADRR, (uint8_t*) &walldata, sizeof(walldata));
		for (uint8_t i = 0; i < 2; i++) {
			HAL_GPIO_WritePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin,
					SET);
			set_led(5);
			HAL_Delay(200);
			HAL_GPIO_WritePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin,
					RESET);
			set_led(2);
			HAL_Delay(200);
		}
		moter_flag=1;
		HAL_Delay(200);
	}
}

void read_all_walldata(void) {
	read_flash(SECTOR_BASE_ADRR, (uint8_t*) &walldata, sizeof(walldata));
}

uint8_t read_spi(uint8_t addr) { //addrのデータを読み取る
	uint8_t data_tx[2], data_rx[2];
	data_tx[0] = (0x80 | addr); //readのMSB(1)とアドレス
	data_tx[1] = 0x00; //dummy

	HAL_GPIO_WritePin(CS_GYRO_GPIO_Port, CS_GYRO_Pin, 0);
	HAL_SPI_TransmitReceive(&hspi2, data_tx, data_rx, 2, 1);
	HAL_GPIO_WritePin(CS_GYRO_GPIO_Port, CS_GYRO_Pin, 1);

	return data_rx[1];
}

void write_spi(uint8_t addr, uint8_t data) {
	uint8_t data_rx[2], data_tx[2];
	data_tx[0] = 0x7f & addr;
	data_tx[1] = data;

	HAL_GPIO_WritePin(CS_GYRO_GPIO_Port, CS_GYRO_Pin, 0);
	HAL_SPI_TransmitReceive(&hspi2, data_tx, data_rx, 2, 1);
	HAL_GPIO_WritePin(CS_GYRO_GPIO_Port, CS_GYRO_Pin, 1);
}

void init_gyro(void) {
	read_spi(WHO_AM_I);
	HAL_Delay(10);
	if (read_spi(WHO_AM_I) != 0xe0) {
		printf("gyro init error\n");
	}
	HAL_Delay(10);
	write_spi(0x05, 0x00); //USER_CTRL いろんな機能をoffに
	HAL_Delay(10);
	write_spi(0x06, 0x09); //PWRMGMT_1 温度センサoff クロックを自動設定 スリープ解除
	HAL_Delay(10);
	write_spi(0x07, 0x3f); //PWRMGMT_2 ジャイロ・加速度センサoff

	HAL_Delay(10);
	write_spi(0x7f, 0x20); //REG_BANK_SEL bank2に移動

	HAL_Delay(10);
	write_spi(0x01, 0x06); //GYRO_CONFIG_1 +-2000dps non-filter
	HAL_Delay(10);
	write_spi(0x14, 0x00); //ACCEL_CONFIG +-2g non-filter

	HAL_Delay(10);
	write_spi(0x7f, 0x00); //REG_BANK_SEL bank0に移動

	HAL_Delay(10);
	write_spi(0x07, 0x00); //PWRMGMT_2 ジャイロ・加速度センサon
}

float read_gyro(void) {
	uint8_t data_l;
	int8_t data_h;
	int16_t value;
	data_h = read_spi(0x37);
	data_l = read_spi(0x38);
	value = (int16_t) (data_h << 8) | (int16_t) data_l;
//	if (rotation_parameter.back_rightturn_flag == 1) {
//		value = -1 * value;
//	}
	return (float) value * 0.0610370189 - angle_calibration; //*2000/(2^15-1) return deg/sec
}

float read_accel(void) {
	uint8_t data_l, data_h;
	int16_t value;
	data_h = read_spi(0x2d);
	data_l = read_spi(0x2e);
	value = (int16_t) (data_h << 8) | (int16_t) data_l;

//	acc_log[acc_log_index] = value;
//	value = LPF[0] * acc_log[((acc_log_index) % 6)]
//			+ LPF[1] * acc_log[((acc_log_index - 1 + 6) % 6)]
//			+ LPF[2] * acc_log[((acc_log_index - 2 + 6) % 6)]
//			+ LPF[3] * acc_log[((acc_log_index - 3 + 6) % 6)]
//			+ LPF[4] * acc_log[((acc_log_index - 4 + 6) % 6)]
//			+ LPF[5] * acc_log[((acc_log_index - 5 + 6) % 6)];
//	acc_log_index++;
//	if (acc_log_index == 6) {
//		acc_log_index = 0;
//	}

	return ((float) (value * 0.598568681905 * -1) - accel_calibration) * 1.02; //*9.8...*2/(2^15-1)*1000 return mm/ms^2
}

uint16_t check_parity(uint16_t val) {
	val ^= val >> 8;
	val ^= val >> 4;
	val ^= val >> 2;
	val ^= val >> 1;
	return val & 0x0001;
}

uint16_t read_spi_en(uint8_t le_ri, uint16_t addr) { //addrのデータを読み取る
	uint16_t data_tx;
	uint16_t data_rx;

	data_tx = (0x4000 | addr); //14bitが1でread 0でwrite
	data_tx = data_tx | (check_parity(data_tx) << 15); //偶パリティ

	if (le_ri == LEFT) { //select CS
		HAL_GPIO_WritePin(CS_L_EN_GPIO_Port, CS_L_EN_Pin, 0);
	} else if (le_ri == RIGHT) {
		HAL_GPIO_WritePin(CS_R_EN_GPIO_Port, CS_R_EN_Pin, 0);
	}
	HAL_SPI_TransmitReceive(&hspi3, (uint8_t*) &data_tx, (uint8_t*) &data_rx, 1,
			100);
	HAL_GPIO_WritePin(CS_L_EN_GPIO_Port, CS_L_EN_Pin, 1);
	HAL_GPIO_WritePin(CS_R_EN_GPIO_Port, CS_R_EN_Pin, 1);
	return data_rx;
}

void write_spi_en(uint8_t le_ri, uint16_t addr, uint16_t data) {
	uint8_t data_tx[2];
	uint16_t data_rx[2];
	data_tx[0] = 0xBFFF & addr; //14bitが1でread 0でwrite
	data_tx[0] = data_tx[0] & (check_parity(data_tx[0]) << 15);

	data_tx[1] = data;

	if (le_ri == LEFT) {
		HAL_GPIO_WritePin(CS_L_EN_GPIO_Port, CS_L_EN_Pin, 0);
	} else if (le_ri == RIGHT) {
		HAL_GPIO_WritePin(CS_R_EN_GPIO_Port, CS_R_EN_Pin, 0);
	} else {
	}
	HAL_SPI_TransmitReceive(&hspi3, data_tx, (uint8_t*) data_rx, 2, 1);
	if (le_ri == LEFT) {
		HAL_GPIO_WritePin(CS_L_EN_GPIO_Port, CS_L_EN_Pin, 1);
	} else if (le_ri == RIGHT) {
		HAL_GPIO_WritePin(CS_R_EN_GPIO_Port, CS_R_EN_Pin, 1);
	} else {

	}
}

void Battery_Check(void) {

	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	HAL_GPIO_WritePin(SENLED_RF_GPIO_Port, SENLED_RF_Pin, SET);
	HAL_GPIO_WritePin(SENLED_LF_GPIO_Port, SENLED_LF_Pin, SET);
	HAL_GPIO_WritePin(SENLED_R_GPIO_Port, SENLED_R_Pin, SET);
	HAL_GPIO_WritePin(SENLED_L_GPIO_Port, SENLED_L_Pin, SET);

	SEN_check_flag = 1;
	HAL_Delay(10);
	SEN_check_flag = 0;
//	for (uint8_t i = 0; i < 9; i++) {
//		HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
//				sizeof(g_ADCBuffer) / sizeof(uint16_t));
//	}

//	Batt = (float) g_ADCBuffer[8] / 4095 * 3.3 * 2;
//	printf("%4.2f\n", Batt);

	if ((Batt < 3.72 && Batt > 3.35) || (Batt < 3.25)) { //7.7
		while (1) {
			set_led(3);
			HAL_Delay(500);
			set_led(6);
			HAL_Delay(500);
			//	myprintf("%6.2f\n",Battery);
		}
	}
}

void set_led(uint8_t num) {
	if ((0x01 & num) != 0) {
		HAL_GPIO_WritePin(UI_LED_RIGHT_GPIO_Port, UI_LED_RIGHT_Pin, SET);
	} else {
		HAL_GPIO_WritePin(UI_LED_RIGHT_GPIO_Port, UI_LED_RIGHT_Pin, RESET);
	}
	if ((0x02 & num) != 0) {
		HAL_GPIO_WritePin(UI_LED_CENTER_GPIO_Port, UI_LED_CENTER_Pin, SET);
	} else {
		HAL_GPIO_WritePin(UI_LED_CENTER_GPIO_Port, UI_LED_CENTER_Pin, RESET);
	}
	if ((0x04 & num) != 0) {
		HAL_GPIO_WritePin(UI_LED_LEFT_GPIO_Port, UI_LED_LEFT_Pin, SET);
	} else {
		HAL_GPIO_WritePin(UI_LED_LEFT_GPIO_Port, UI_LED_LEFT_Pin, RESET);
	}
	if ((0x08 & num) != 0) {
//		HAL_GPIO_WritePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin, SET);
	} else {
//		HAL_GPIO_WritePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin, RESET);
	}
}

void log_start(void) {
	log_often_count = 0;
	log_index = 0;
	log_how_often = 1;
	log_flag = 1;
}

void log_sampling(void) {
	log_often_count++;
	if (log_index == LOG_MAX - 1) {
		log_flag = 0;
//		log_index = 0;
	} else if (log_often_count == log_how_often) {
		mylog.log_1[log_index] = real_L.vel;
		mylog.log_2[log_index] = real_R.vel;
		mylog.log_3[log_index] = ideal_translation.vel;
		mylog.log_4[log_index] = real_rotation.vel;
		mylog.log_5[log_index] = ideal_rotation.vel;
		mylog2.log_1[log_index] = real_rotation.dis;
		mylog2.log_2[log_index] = ideal_rotation.dis;
		mylog2.log_3[log_index] = wallcontrol_value;
		mylog2.log_4[log_index] = test_R;
		mylog2.log_5[log_index] = test_L2;

//		mylog.log_1[log_index] = 1;
//		mylog.log_2[log_index] = 2;
//		mylog.log_3[log_index] = 3;
//		mylog.log_4[log_index] = 4;
//		mylog.log_5[log_index] = 5;
//		mylog2.log_1[log_index] = 6;
//		mylog2.log_2[log_index] = 7;
//		mylog2.log_3[log_index] = 8;
//		mylog2.log_4[log_index] = 9;
//		mylog2.log_5[log_index] = 10;

		//	mylog.log_1[log_index] = real_acc;

//		mylog.log_1[log_index] = real_L.vel;
//		mylog.log_2[log_index] = real_R.vel;
//		mylog.log_3[log_index] = ideal_translation.vel;
//		mylog.log_4[log_index] = test_L;
//		mylog.log_5[log_index] = test_L2;
//		mylog2.log_1[log_index] = test_R;
//		mylog2.log_2[log_index] = test_R2;
//		mylog2.log_3[log_index] = rotation_deviation.cumulative;
//		mylog2.log_4[log_index] = run_left_deviation.cumulative;
//		mylog2.log_5[log_index] = run_right_deviation.cumulative;
//
//
//		mylog.log_1[log_index] = (float) SEN_L.now;
//		mylog.log_2[log_index] = (float) SEN_R.now;
//		mylog.log_3[log_index] = (float) SEN_LF.now;
//		mylog.log_4[log_index] = (float) SEN_RF.now;
//		mylog.log_5[log_index] = (float) SEN_F.now;
//		mylog2.log_1[log_index] = ideal_translation.dis;
//		mylog2.log_2[log_index] = test_R;
//		mylog2.log_3[log_index] = SEN_F.now;
//		mylog2.log_4[log_index] = test_float;
//		mylog2.log_5[log_index] = 0;

//		mylog.log_1[log_index] = real_L.vel;
//		mylog.log_2[log_index] = real_R.vel;
//		mylog.log_3[log_index] = ideal_translation.vel;
//		mylog.log_4[log_index] = real_acc;
//		mylog.log_5[log_index] = real_vel_from_acc;
//		mylog2.log_1[log_index] = real_diss_from_acc;
//		mylog2.log_2[log_index] = ideal_rotation.dis * 0;
//		mylog2.log_3[log_index] = test_L;
//		mylog2.log_4[log_index] = test_R;
//		mylog2.log_5[log_index] = test_L2;

		log_index++;
		log_often_count = 0;
		//	printf(",%4.8f,%4.8f,%4.8f,%4.8f\n",real_L.vel,real_R.vel,real_L.dis,real_R.dis);

	}
}

void log_output(void) {
	SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0,
	SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
	for (uint16_t i = 0; i < LOG_MAX; i++) {
		printf(",");
		printf("%8.2f,", mylog.log_1[i]);
		printf("%8.2f,", mylog.log_2[i]);
		printf("%8.2f,", mylog.log_3[i]);
		printf("%8.2f,", mylog.log_4[i]);
		printf("%8.2f,", mylog.log_5[i]);
		printf("%8.2f,", mylog2.log_1[i]);
		printf("%8.2f,", mylog2.log_2[i]);
		printf("%8.2f,", mylog2.log_3[i]);
		printf("%8.2f,", mylog2.log_4[i]);
		printf("%8.2f,", mylog2.log_5[i]);
		printf("\n");
	}
	SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0,
	SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

void start_led(void) {
	uint8_t i;
	SEN_check_flag = 1;
	wall_control_flag = 0;
	while (SEN_R.now < 1000 || SEN_RF.now < 1000) {
//			||SEN_L.now < 1000&& SEN_LF.now < 1000) {

	}
	set_buzzer(0, C_5, 800);
	set_led(7);
	HAL_Delay(200);
	set_led(0);
	HAL_Delay(200);
	set_led(7);
	HAL_Delay(200);
	set_led(0);
	HAL_Delay(200);

//	log_start();

	angle_calibration_integral = 0.0;
	angle_calibration = 0.0;
	accel_calibration_integral = 0.0;
	accel_calibration = 0.0;
	angle_calibration_counter = 0;
	angle_calibration_flag = 1;
	while (angle_calibration_flag == 1) {

	}
	angle_calibration = angle_calibration_integral / 2048.0;
	accel_calibration = accel_calibration_integral / 2048.0;
	real_rotation.dis = 0.0;
	ideal_translation.vel = 0.0;

	for (i = 0; i < 50; i++) {
		enc_buff_l[i] = 0;
		enc_buff_r[i] = 0;
		acc_buff[i] = 0;
	}
	for (i = 50; i < 100; i++) {
		enc_buff_l[i] = 0;
		enc_buff_r[i] = 0;
	}

	rotation_deviation.cumulative = 0;
	set_buzzer(0, E_5, 800);
	moter_flag = 1;
	wall_control_flag = 1;
}

void output_SEN(void) {
	SEN_check_flag = 1;
	while (HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin) == 1) {
		printf("LF=%5d,L=%5d,R=%5d,RF=%5d\n", SEN_LF.now, SEN_L.now, SEN_R.now,
				SEN_RF.now);
//		printf("%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d\n", g_ADCBuffer[0],
//				g_ADCBuffer[1], g_ADCBuffer[2], g_ADCBuffer[3], g_ADCBuffer[4],
//				g_ADCBuffer[5], g_ADCBuffer[6], g_ADCBuffer[7], g_ADCBuffer[8]);
		HAL_Delay(100);
	}
	SEN_check_flag = 0;

}
