/*
 * Buzzer.h
 *
 *  Created on: Nov 23, 2017
 *      Author: amabo
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include "msp.h"

void configure_Buzzer();
void long_buzz();
void short_buzzes(uint8_t count);

#endif /* BUZZER_H_ */
