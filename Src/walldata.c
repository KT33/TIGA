/*
 * walldata.c
 *
 *  Created on: 2019/09/04
 *      Author: kt33_
 */
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "stdint.h"
#include "stdio.h"
#include "variable.h"
#include "walldata.h"
#include "main.h"

void add_West_wall(singlewalldata_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= y.now;
	walldate->column[x.now] |= new;
}

void add_South_wall(singlewalldata_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= x.now;
	walldate->row[y.now] |= new;
}

void add_East_wall(singlewalldata_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= y.now;
	walldate->column[x.now + 1] |= new;
}

void add_North_wall(singlewalldata_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= x.now;
	walldate->row[y.now + 1] |= new;
}

void pull_West_wall(singlewalldata_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= y.now;
	walldate->column[x.now] &= ~new;
}

void pull_South_wall(singlewalldata_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= x.now;
	walldate->row[y.now] &= ~new;
}

void pull_East_wall(singlewalldata_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= y.now;
	walldate->column[x.now + 1] &= ~new;
}

void pull_North_wall(singlewalldata_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= x.now;
	walldate->row[y.now + 1] &= ~new;
}

void add_walldata_one(uint8_t dire, singlewalldata_t* walldata) {
	dire = dire % 4;
	if (dire == North) {
		add_North_wall(walldata);
	} else if (dire == West) {
		add_West_wall(walldata);
	} else if (dire == South) {
		add_South_wall(walldata);
	} else if (dire == East) {
		add_East_wall(walldata);
	}
}

void pull_walldata_one(uint8_t dire, singlewalldata_t* walldata) {
	dire = dire % 4;
	if (dire == North) {
		pull_North_wall(walldata);
	} else if (dire == West) {
		pull_West_wall(walldata);
	} else if (dire == South) {
		pull_South_wall(walldata);
	} else if (dire == East) {
		pull_East_wall(walldata);
	}
}

void addWall(void) {
//	uint16_t new;
	if (add_wall_flag == 1) {
		if (direction == North) {
			add_West_wall(&walldata.checked);
			add_East_wall(&walldata.checked);
			add_North_wall(&walldata.checked);
		} else if (direction == West) {
			add_West_wall(&walldata.checked);
			add_South_wall(&walldata.checked);
			add_North_wall(&walldata.checked);
		} else if (direction == South) {
			add_West_wall(&walldata.checked);
			add_South_wall(&walldata.checked);
			add_East_wall(&walldata.checked);
		} else if (direction == East) {
			add_North_wall(&walldata.checked);
			add_South_wall(&walldata.checked);
			add_East_wall(&walldata.checked);
		}

		if (SEN_F.now > SEN_F.threshold) {
			add_walldata_one(direction, &walldata.real);
		} else if (getWall(x.now, y.now, direction, &walldata.real)==0) {
			pull_walldata_one(direction, &walldata.adachi);
		}

		if (SEN_R.now > SEN_R.threshold) {
			add_walldata_one(direction+3, &walldata.real);
		} else if (getWall(x.now, y.now, direction+3, &walldata.real)==0) {
			pull_walldata_one(direction+3, &walldata.adachi);
		}

		if (SEN_L.now > SEN_L.threshold) {
			add_walldata_one(direction+1, &walldata.real);
		} else if (getWall(x.now, y.now, direction+1, &walldata.real)==0) {
			pull_walldata_one(direction+1, &walldata.adachi);
		}

	}
}

void clear_Map(singlewalldata_t *walldate) {
	uint8_t i;
	for (i = 0; i < 16; i++) {
		walldate->column[i] = 0;
		walldate->row[i] = 0;
	}
	walldate->column[0] = 0xffff;
	walldate->row[0] = 0xffff;
	walldate->column[16] = 0xffff;
	walldate->row[16] = 0xffff;
	walldate->column[1] = 0x0001;
}

void clear_adachiMap(singlewalldata_t *walldate) {
	uint8_t i;
	for (i = 0; i < 17; i++) {
		walldate->column[i] = 0xffff;
		walldate->row[i] = 0xffff;
		walldate->row[1] = 0xfffe;
	}
}

void init_walldata(void) {
	clear_Map(&walldata.real);
	clear_Map(&walldata.checked);
	clear_adachiMap(&walldata.adachi);
}

void output_Walldata(uint8_t i) {
	if (i == ALL || i == REAL) {
		read_Walldata(&walldata.real);
	}
	if (i == ALL || i == CHECK) {
		read_Walldata(&walldata.checked);
	}
	if (i == ALL || i == ADACHI) {
		read_Walldata(&walldata.adachi);
	}
}

void read_Walldata(singlewalldata_t *walldate) {
	int i = 0;
	SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0,
	SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
	int x_check, y_check;
	printf("\n");
	printf("\x1b[36m");
	printf("\x1b[1m");
	printf("  ");
	for (i = 1; i < 17; i++) {
		printf("  %2d  ", i);
	}
	printf("\x1b[0m");
	printf("\n");
	for (y_check = 15; y_check >= 0; y_check--) {
		printf("\x1b[31m");
		printf("  +");
		for (x_check = 0; x_check < 16; x_check++) {
			if (getWall(x_check, y_check, North, walldate) == 1) {
				//
				printf("-----");
			} else {
				printf("     ");
			}
			//
			printf("+");
		}
		printf("\n");
		printf("\x1b[1m");
		printf("\x1b[36m");
		printf("%2d", y_check);
		printf("\x1b[0m");
		for (x_check = 0; x_check < 16; x_check++) {
			if (getWall(x_check, y_check, West, walldate) == 1) {
				printf("\x1b[31m");
				printf("|");
				printf("\x1b[0m");
			} else {
				printf(" ");
			}
			if (step_map[x_check][y_check] < 1000) {
				printf(" %3d ", step_map[x_check][y_check]); //step_Map[x_check][y_check]
			} else {
				printf(" %3d ", 999);
			}
		}
		if (getWall(15, y_check, East, walldate) == 1) {
			printf("\x1b[31m");
			printf("|");
			printf("\x1b[0m");
		} else {
			printf(" ");
		}
		printf("\x1b[1m");
		printf("\x1b[36m");
		printf("%2d", y_check);
		printf("\x1b[0m");
		printf("\n");
	}
	printf("\x1b[31m");
	printf("  +");
	printf("\x1b[0m");
	y_check = 0;
	for (x_check = 0; x_check < 16; x_check++) {
		if (getWall(x_check, y_check, South, walldate) == 1) {
			printf("\x1b[31m");
			printf("-----");
			printf("\x1b[0m");
		} else {
			printf("     ");
		}
		printf("\x1b[31m");
		printf("+");
		printf("\x1b[0m");
	}
	printf("\n");
	printf("  ");
	printf("\x1b[1m");
	printf("\x1b[36m");
	for (i = 1; i < 17; i++) {
		printf("  %2d  ", i);
	}
	printf("\x1b[0m");

	printf("\n");

	SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0,
	SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	printf("\x1b[0m");
}
//
int getWall(int x_check, int y_check, int direction_check,
		singlewalldata_t *walldate) { //(見たい座標のx,y,とその座標からの方角１～４壁があれば１
	int check_wall = 1;

	direction_check = direction_check % 4;

	if (direction_check == North) {
		check_wall <<= x_check;
		check_wall &= walldate->row[y_check + 1];
		if (check_wall != 0) {
			check_wall = 1;
		}
	} else if (direction_check == East) {
		check_wall <<= y_check;
		check_wall &= walldate->column[x_check + 1];
		if (check_wall != 0) {
			check_wall = 1;
		}
	} else if (direction_check == South) {
		check_wall <<= x_check;
		check_wall &= walldate->row[y_check];
		if (check_wall != 0) {
			check_wall = 1;
		}
	} else if (direction_check == West) {
		check_wall <<= y_check;
		check_wall &= walldate->column[x_check];
		if (check_wall != 0) {
			check_wall = 1;
		}
	}
	return check_wall;
}

