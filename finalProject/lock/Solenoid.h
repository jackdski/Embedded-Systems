/*
 * Solenoid.h
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */

#ifndef SOLENOID_H_
#define SOLENOID_H_

#include "msp.h"
#include <stdint.h>

//Configure the solenoid circuit
void configure_solenoid();

//Function to activate the solenoid
void pullLatch();

#endif /* SOLENOID_H_ */
