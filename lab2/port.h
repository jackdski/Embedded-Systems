/*
 * port.h
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo and Jack
 */

#ifndef PORT_H_
#define PORT_H_

#include "msp.h"

#define CLEAR_FLAGS (uint8_t) (~(BIT1 | BIT4))
#define STCSR       (*(uint32_t *)(0xE000E010))
#define STCVR_CURR  (*(uint32_t *)(0xE000E018))

void GPIO_configure(void);

#endif
