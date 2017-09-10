/*
 *
 * Author: Jack Danielski, Adam Smrekar, Riley Hadjis
 * Date Due: 9-8-2017
 *
 */

#include "lab1.h"
#include <stdint.h>

/*
 * Lab Procedure Part 5
 *
 * Report size of different data types.
 *
 */
int * report_types() {
    int sizeArray[20];

    sizeArray[0] = sizeof(char);
    sizeArray[1] = sizeof(int);
    sizeArray[2] = sizeof(short);
    sizeArray[3] = sizeof(long);
    sizeArray[4] = sizeof(long int);
    sizeArray[5] = sizeof(short int);
    sizeArray[6] = sizeof(float);
    sizeArray[7] = sizeof(double *);
    sizeArray[8] = sizeof(unsigned char);
    sizeArray[9] = sizeof(signed char);
    sizeArray[10] = sizeof(unsigned short);
    sizeArray[11] = sizeof(signed short);
    sizeArray[12] = sizeof(unsigned int);
    sizeArray[13] = sizeof(signed int);
    sizeArray[14] = sizeof(int8_t);
    sizeArray[15] = sizeof(uint8_t);
    sizeArray[16] = sizeof(int16_t);
    sizeArray[17] = sizeof(uint16_t);
    sizeArray[18] = sizeof(int32_t);
    sizeArray[19] = sizeof(uint32_t);
    return sizeArray;
}

/*
 * Lab Procedure Part 6
 *
 * Report size of different pointer types.
 *
 *
 */
int * report_pointer_types() {
    int pointerSizes[20];

    pointerSizes[0] = sizeof(char*);
    pointerSizes[1] = sizeof(int*);
    pointerSizes[2] = sizeof(short*);
    pointerSizes[3] = sizeof(long*);
    pointerSizes[4] = sizeof(long int*);
    pointerSizes[5] = sizeof(short int*);
    pointerSizes[6] = sizeof(float*);
    pointerSizes[7] = sizeof(double*);
    pointerSizes[8] = sizeof(unsigned char*);
    pointerSizes[9] = sizeof(signed char*);
    pointerSizes[10] = sizeof(unsigned short*);
    pointerSizes[11] = sizeof(signed short*);
    pointerSizes[12] = sizeof(unsigned int*);
    pointerSizes[13] = sizeof(signed int*);
    pointerSizes[14] = sizeof(int8_t*);
    pointerSizes[15] = sizeof(uint8_t*);
    pointerSizes[16] = sizeof(int16_t*);
    pointerSizes[17] = sizeof(uint16_t*);
    pointerSizes[18] = sizeof(int32_t*);
    pointerSizes[19] = sizeof(uint32_t*);
    return pointerSizes;
}

/*
 * Lab Procedure Parts 8 and 9
 *
 * Set the direction of PIN 1.7 to OUT using an OR-equals statement.
 * Set the output of pin 1.7 to ON and use a for-loop that is set to
 * have a delay that creates a square wave that has a period of 100ms.
 * The square wave is toggled using the XOR bitwise operator to turn it
 * off for 50ms and then on for 50ms.
 *
 * The square wave is measured by the oscilloscope to have a period of
 * 100.01ms.
 *
 *
 */
void make_square_wave() {
    P1_DIR |= 0x80;                 // Set
    volatile int i;

    P1_OUT |= 0x80;

    for (i = 0; i < 14395; i++);
    P1_OUT ^= 0x80;
    for(i=0; i<14395; i++);
    P1_OUT ^= 0x80;

}
