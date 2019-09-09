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

#define CHATT 10000
#define SECTOR_BASE_ADRR 0x08160000ul

void chattering(void) {
	int i = 0;
	while (i < CHATT) {
		i++;
	}
	while (HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin) == 0) {
	}
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

void write_spi(uint8_t addr,uint8_t data){
	uint8_t data_rx[2],data_tx[2];
	data_tx[0]=0x7f&addr;
	data_tx[1]=data;

	HAL_GPIO_WritePin(CS_GYRO_GPIO_Port, CS_GYRO_Pin, 0);
	HAL_SPI_TransmitReceive(&hspi2, data_tx, data_rx, 2, 1);
	HAL_GPIO_WritePin(CS_GYRO_GPIO_Port, CS_GYRO_Pin, 1);
}

void init_gyro(void){
		read_spi(WHO_AM_I);
	HAL_Delay(10);
	if(read_spi(WHO_AM_I)!=0xe0){
		printf("gyro init error\n");
	}
	HAL_Delay(10);





}
