/*
 * RGB.h
 *
 *  Created on: Nov 28, 2017
 *      Author: amabo
 */

#ifndef RGB_H_
#define RGB_H_
#include "msp.h"

//Configure the LED pins
void configure_RGB();

//Turn on the Red LED
void Red_LED_On();

//Turn on the Blue LED
void Blue_LED_On();

//Turn on the Green LED
void Green_LED_On();

//Turn off all of the LEDs
void LED_Off();

#endif /* RGB_H_ */
