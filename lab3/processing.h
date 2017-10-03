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

uint8_t alp = 0; //Number of alphabetical chars
uint8_t pun = 0; //Number of punctuation chars
uint8_t num = 0; //Number of numerical chars
uint8_t whi = 0; //Number of white chars
uint8_t ran = 0; //Number of random chars

void configurePorts();
void analyzeChr(uint8_t chr);

uint8_t isAlphabet(uint8_t chr);
uint8_t isPunctuation(uint8_t chr);
uint8_t isNumerical(uint8_t chr);
uint8_t isWhiteSpace(uint8_t chr);




#endif /* PROCESSING_H_ */
