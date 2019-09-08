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

	uint16_t SectorError; //srroe sector nomber

	HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError); //erase sector
}

void write_flash(uint32_t address,uint8_t *data,uint32_t size){
	uint32_t add=address;
	HAL_FLASH_Unlock(); //flash unlook
	erase_flash();
	for(add=address;add<(address+size);add++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, add, *data);
		data++;
	}
	HAL_FLASH_Lock(); //flash look
}

void save_all_walldata(void) {
	uint8_t i = 0;
//write_flash(SECTOR_BASE_ADRR, data, size)

}

void read_flash(uint32_t address,uint8_t *data,uint32_t size) {
	memcpy(data,(uint8_t*)address,size);
}
