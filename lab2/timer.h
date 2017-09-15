/*
 * timer.h
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo Jack
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "msp.h"
#include <stdint.h>

#define CLEAR_TIMER (uint16_t)(0x0000)
#define SET_CTL     (TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID__1 | TIMER_A_CTL_MC__UP | TIMER_A_CTL_IE)
#define SET_CCTL    (uint16_t)(0x0010)

#define STCSR       (*(uint32_t *)(0xE000E010))
#define STCVR_CURR  (*(uint32_t *)(0xE000E018))

void TA0_0_IRQHandler();
void timer_a0_config();

#endif /* TIMER_H_ */
