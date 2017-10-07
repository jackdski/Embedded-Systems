/*
 * processing.h
 *
 *  Created on: Oct 3, 2017
 *      Author: amabo
 */

#ifndef PROCESSING_H_
#define PROCESSING_H_

#include "msp.h"
#include <stdio.h>

void configurePorts();
void analyzeBuf();
void analyzeChr(uint8_t chr);
void itoa(uint8_t num, uint8_t * str);

#endif /* PROCESSING_H_ */
