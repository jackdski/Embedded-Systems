/*
 * timer.h
 *
 *  Created on: Sep 7, 2017
 *      Author: Avery and Jack
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "msp.h"
#include <stdint.h>

#define CLEAR_TIMER (uint16_t)(0x0000)
#define SET_CTL     (uint16_t)(TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_3 | TIMER_A_CTL_MC__UP)
#define COUNT_TO    (uint16_t)(37799) //Value for 5Hz is 37499 by experiment  47255 for 500ms on time
#define SET_CCTL    (uint16_t)(0x0010)

void timer_a0_config();

#endif /* TIMER_H_ */
