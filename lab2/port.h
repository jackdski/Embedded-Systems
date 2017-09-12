/*
 * port.h
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo
 */

#ifndef PORT_H_
#define PORT_H_

#include "msp.h"

#define CLEAR_FLAGS (uint8_t) (~(BIT1 | BIT4))

void ISR_FUNCTION_PORTS();



void GPIO_configure(void);

#endif /* PORT_H_ */
