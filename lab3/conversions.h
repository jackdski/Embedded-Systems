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
int atoi(char * a);
void itoa(uint16_t num, int size, char * str );
void ftoa(float number, int decimalPlace, int size, char * str);

#endif
