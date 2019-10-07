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
	write_flash(SECTOR_BASE_ADRR, (uint8_t*) &walldata, sizeof(walldata));
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
	write_spi(0x14, 0x04); //ACCEL_CONFIG +-8g non-filter

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
	return (float) value * 0.0610370189; //*2000/(2^15-1) return deg/sec
}

float read_accel(void) {
	uint8_t data_l, data_h;
	int16_t value;
	data_h = read_spi(0x31);
	data_l = read_spi(0x32);
	value = (int16_t) (data_h << 8) | (int16_t) data_l;
	return (float) value * 0.00239427472762; //*9.8...*8/(2^15-1) return m/s^2
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

	HAL_Delay(100);
	for (uint8_t i = 0; i < 9; i++) {
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*) g_ADCBuffer,
				sizeof(g_ADCBuffer) / sizeof(uint16_t));
	}
	HAL_Delay(10);
	Batt = (float) g_ADCBuffer[8] / 4095 * 3.3 * 2;
	printf("%4.2f\n", Batt);

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
		HAL_GPIO_WritePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin, SET);
	} else {
		HAL_GPIO_WritePin(UI_LED_LEFT_BO_GPIO_Port, UI_LED_LEFT_BO_Pin, RESET);
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
		log_index = 0;
	} else if (log_often_count == log_how_often) {
		mylog.log_1[log_index] = real_L.vel;
		mylog.log_2[log_index] = real_R.vel;
		mylog.log_3[log_index] = ideal_translation.vel;
		mylog.log_4[log_index] = (float) duty.left / 800;
		mylog.log_5[log_index] = (float) duty.right / 800;
		mylog2.log_1[log_index] = (real_L.dis + real_R.dis) / 2;
		mylog2.log_2[log_index] = test_L;
		mylog2.log_3[log_index] = test_R;
		mylog2.log_4[log_index] = test_L2;
		mylog2.log_5[log_index] = test_R2;

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
		printf("%4.2f,", mylog.log_1[i]);
		printf("%4.2f,", mylog.log_2[i]);
		printf("%4.2f,", mylog.log_3[i]);
		printf("%4.2f,", mylog.log_4[i]);
		printf("%4.2f,", mylog.log_5[i]);
		printf("%4.2f,", mylog2.log_1[i]);
		printf("%4.2f,", mylog2.log_2[i]);
		printf("%4.2f,", mylog2.log_3[i]);
		printf("%4.2f,", mylog2.log_4[i]);
		printf("%4.2f,", mylog2.log_5[i]);
		printf("\n");
	}
	SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0,
	SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}
