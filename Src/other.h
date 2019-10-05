/*
 * other.h
 *
 *  Created on: 2019/09/05
 *      Author: kt33_
 */

#ifndef OTHER_H_
#define OTHER_H_
#include "variable.h"


void chattering(void);
void erase_flash(void);
void write_flash(uint32_t,uint8_t*,uint32_t);
void read_flash(uint32_t,uint8_t*,uint32_t );
void save_all_walldata(void);
void read_all_walldata(void);
void write_spi(uint8_t addr, uint8_t data);
uint8_t read_spi(uint8_t);
void init_gyro(void);
float read_gyro(void);
float read_accel(void);
void write_spi_en(uint8_t,uint16_t addr, uint16_t data);
uint16_t read_spi_en(uint8_t,uint16_t addr);
uint16_t check_parity(uint16_t val);
void set_led(uint8_t);
void Battery_Check(void);

#define WHO_AM_I 0x00



#endif /* OTHER_H_ */
