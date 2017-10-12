/*
 *
 * lab4.h
 * Author: Jack Danielski and Avery Anderson
 * 10-12-17
 *
 */


#ifndef LAB4_H_
#define LAB4_H_


#include "msp.h"
#include <stdint.h>

// Converts a string of numbers into an integer
int  atoi(char * a);
void itoa(uint32_t num, int8_t size, uint8_t * str );
void ftoa(float number, uint8_t decimalPlace, uint8_t size, uint8_t * str);

#endif
