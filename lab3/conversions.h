/*
*
* conversions.h
* Author: Jack Danielski and Avery Anderson
* Date: 10-6-2017
* 
*/

#include "msp.h"
#include "uart.h"
#include "circbuf.h"
#include "processing.h"

// Converts a string of numbers into an integer
int atoi(char * a);

void itoa(uint8_t num, int size, char * str );
