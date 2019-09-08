/*
 * buzzer.c
 *
 *  Created on: Sep 9, 2019
 *      Author: kt33_
 */

#include "buzzer.h"

uint16_t calculation_counter(uint16_t Hz){//タイマピリオドの計算
	uint16_t count;//設定する値
	if(Hz==0){
		count=0;
	}else{
		count=(1000000/Hz-1);
	}
	return count;
}
