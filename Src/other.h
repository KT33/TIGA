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

#endif /* OTHER_H_ */
