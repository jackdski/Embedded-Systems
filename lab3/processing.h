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
void transmitEC();
void analyzeChrEC(uint8_t chr);
uint8_t isWhiteSpace(uint8_t chr);

#endif /* PROCESSING_H_ */
