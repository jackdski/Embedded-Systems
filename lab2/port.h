/*
 * port.h
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo and Jack
 */

#ifndef PORT_H_
#define PORT_H_
//#define PROB12

#include "msp.h"

#define CLEAR_FLAGS (uint8_t) (~(BIT1 | BIT4 | BIT5))

//void ISR_FUNCTION_PORTS();

void GPIO_configure(void);

#endif /* PORT_H_ */
