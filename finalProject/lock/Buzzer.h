/*
 * Buzzer.h
 *
 *  Created on: Nov 23, 2017
 *      Author: amabo
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include "msp.h"

//Configure the pins needed for the Buzzer
void configure_Buzzer();

//Create one long beep
void long_buzz();

//Create a set of 4 beeps
void short_buzzes();

#endif /* BUZZER_H_ */
