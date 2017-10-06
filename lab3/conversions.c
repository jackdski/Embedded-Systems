/*
*
* conversions.c
* Author: Jack Danielski and Avery Anderson
* Date: 10-6-2017
* 
*/

#include "msp.h"
#include "uart.h"
#include "circbuf.h"
#include "processing.h"

// Converts a string of numbers into an integer
int atoi(char * a) {
	int returnArray [3]; 

	int count = 0; 
	while (count < 3) {
		returnArray[count] = a[count] - 48;
		count++;
	}

	int finalInt = 0;
	finalInt = returnArray[2] + (10 * returnArray[1]) + (100 * returnArray[0]);
	return finalInt;
}
