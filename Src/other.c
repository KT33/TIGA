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

#define CHATT 10000
#define SECTOR_BASE_ADRR 0x08160000ul

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

//	data_tx = 0xffff;

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

//	printf("1st rx0=%d,rx1=%d\n", data_rx[0], data_rx[1]);

//	if (le_ri == LEFT) {
//		HAL_GPIO_WritePin(CS_L_EN_GPIO_Port, CS_L_EN_Pin, 0);
//	} else if (le_ri == RIGHT) {
//		HAL_GPIO_WritePin(CS_R_EN_GPIO_Port, CS_R_EN_Pin, 0);
//	}
//	HAL_SPI_TransmitReceive(&hspi3, (uint8_t*) &data_tx[1],
//			(uint8_t*) &data_rx[1], 1, 100);
//
//	HAL_GPIO_WritePin(CS_L_EN_GPIO_Port, CS_L_EN_Pin, 1);
//	HAL_GPIO_WritePin(CS_R_EN_GPIO_Port, CS_R_EN_Pin, 1);
//	printf("2nd rx0=%d,rx1=%d\n", data_rx[0], data_rx[1]);

//	return (uint16_t) ((uint16_t) (data_rx[0] << 8) + (uint16_t) data_rx[1]);
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
	if (Batt < 3.72) { //7.7
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
