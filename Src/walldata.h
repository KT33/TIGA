/*
 * walldata.h
 *
 *  Created on: 2019/09/04
 *      Author: kt33_
 */

#ifndef WALLDATA_H_
#define WALLDATA_H_

#include "variable.h"
#include "stdint.h"

void addWall(void);
void clear_Map(singlewalldata_t *walldate);
void clear_adachiMap(singlewalldata_t *walldate);
void init_walldata(void);
void read_Walldata(singlewalldata_t*);
int getWall(int x_check, int y_check, int direction_check, singlewalldata_t*); //(見たい座標のx,y,とその座標からの方角１～４壁があれば１
void output_Walldata(uint8_t);
void add_West_wall(singlewalldata_t *walldate);
void add_East_wall(singlewalldata_t *walldate);
void add_North_wall(singlewalldata_t *walldate);
void add_South_wall(singlewalldata_t *walldate);
void pull_West_wall(singlewalldata_t *walldate);
void pull_East_wall(singlewalldata_t *walldate);
void pull_North_wall(singlewalldata_t *walldate);
void pull_South_wall(singlewalldata_t *walldate);

#endif /* WALLDATA_H_ */
