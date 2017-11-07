/*
 * conversions.h
 *
 *  Created on: Nov 4, 2017
 *      Author: amabo
 */

#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_
#include <stdint.h>

//void itoa(uint32_t num, int8_t size, uint8_t * str );
//void ftoa(float number, uint8_t decimalPlace, uint8_t size, uint8_t * str);
void ftoa(float n, char *res, int afterpoint);

#endif /* CONVERSIONS_H_ */
