/*
 * timer.h
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo and Jack
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "msp.h"
#include <stdint.h>

#define CLEAR_TIMER (uint16_t)(0x0000)
#define SET_CTL     (uint16_t)(0x0212)
#define SET_CCTL    (uint16_t)(0x0010)

void Timer_Counter_Compare();
void timer_a0_config();

#endif /* TIMER_H_ */
