/*
 * software_timer.h
 *
 *  Created on: Sep 21, 2023
 *      Author: HaHuyen
 */


#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

extern int timer_flag[20];

void setTimer1(int index, int duration);
void timerRun(int index);

#endif /* INC_SOFTWARE_TIMER_H_ */
