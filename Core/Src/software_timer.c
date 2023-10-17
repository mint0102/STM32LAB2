/*
 * software_timer.c
 *
 *  Created on: Sep 21, 2023
 *      Author: HaHuyen
 */
#include "software_timer.h"

int timer_counter[20] = {0};
int timer_flag[20] = {0};

void setTimer(int index, int duration){
	timer_counter[index] = duration;
	timer_flag[index] = 0;
}

void timerRun(int index){
	if(timer_counter[index] > 0){
		timer_counter[index]--;
		if(timer_counter[index] <= 0){
			timer_flag[index] = 1;
		}
	}
}
