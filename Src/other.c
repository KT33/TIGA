/*
 * other.c
 *
 *  Created on: 2019/09/05
 *      Author: kt33_
 */

#include "other.h"
#include "variable.h"
#include "main.h"

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

void save_walldata(uint8_t start_block){
	uint8_t i;
	for (i = 0; i < 17; i++) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
		SECTOR_BASE_ADRR + sizeof(uint32_t)*(i+start_block), &walldate_real.row[i]);
	}

}

void save_all_walldata(void) {
	uint8_t i = 0;
	HAL_FLASH_Unlock(); //flash unlook
	erase_flash();

//	uint32_t* p_data;
//
//	for (i = 0; i < 17; i++) {
//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
//		SECTOR_BASE_ADRR + sizeof(uint32_t)*i, p_data);
//	}


	HAL_FLASH_lock(); //flash look
}

void read_mapdata(void) {

}
