/*
 * port.h
 *
 *  Created on: Sep 7, 2017
 *      Author: Avery Anderson and Jack Danielski
 */

#ifndef PORT_H_
#define PORT_H_

#include "msp.h"

#define CLEAR_FLAGS (uint8_t) (~(BIT1 | BIT4 | BIT5))

void GPIO_configure(void);

#endif 
