/*
*
* conversions.h
* Author: Jack Danielski and Avery Anderson
* Date: 10-6-2017
*
*/
#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_


#include "msp.h"
#include "uart.h"
#include "circbuf.h"
#include "processing.h"

// Converts a string of numbers into an integer
//int atoi(char * a);
void itoa(uint32_t num, int8_t size, uint8_t * str );
void ftoa(float number, uint8_t decimalPlace, uint8_t size, uint8_t * str);

#endif
