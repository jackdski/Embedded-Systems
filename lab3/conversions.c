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
#include "conversions.h"

#include <stdint.h>

// homemade itoa function
/*
void itoa(uint16_t num, int8_t size, uint8_t * str ) {
    int8_t i;
    i = size;
    while (i >= 0) {
        if ( i == size) {
            str[i] = '\0';
            i--;
        }
        else {
            str[i] = num % 10;
            str[i] += 48;
            num = num / 10;
            i--;
        }
    }
}
*/

// Converts a string of numbers into an integer
int16_t atoi(char * a) {
    uint16_t returnArray [3];

    int8_t count = 0;
    while (count < 3) {
        returnArray[count] = a[count] - 48;
        count++;
    }

    int8_t finalInt = 0;
    finalInt = returnArray[2] + (10 * returnArray[1]) + (100 * returnArray[0]);
    return finalInt;
}


void ftoa(float number, int8_t decimalPlace, int8_t size, uint8_t * str) {
    uint8_t tens = 10;
    int8_t i = decimalPlace - 1;
    while (i > 0) {
        tens = tens * 10;
        i--;
    }
    number = number * (float)tens;
    uint16_t numberInt = (int)number;

    itoa(numberInt, size-1, str);

    int8_t deciPlace = decimalPlace +1;
    uint8_t tempStr[6];

    int j = 0;
    while (j < deciPlace) {
        tempStr[j] = str[deciPlace + j];
        j++;
    }

    str[size - decimalPlace - 1] = '.';

    deciPlace = size - decimalPlace;
    j = 0;
    while (deciPlace <= size){
        str[deciPlace] = tempStr[j];
        deciPlace++;
        j++;
    }
}
