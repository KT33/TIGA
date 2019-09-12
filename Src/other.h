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
uint8_t read_spi(uint8_t);
void init_gyro(void);
float read_gyro(void);
float read_accel(void);
void set_led(uint8_t);

#define WHO_AM_I 0x00


#endif /* OTHER_H_ */
