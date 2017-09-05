/*
 *
 * Author: Jack Danielski
 * Date Due: 9-8-2017
 *
 */

#include "lab1.h"
#include <stdint.h>

void report_types() {
    int char_size   =   sizeof(char);
    int int_size    =   sizeof(int);
    int short_size  =   sizeof(short);
    int long_size   =   sizeof(long);
    int longInt_size  =  sizeof(long int);
    int shortInt_size =  sizeof(short int);
    int float_size  =   sizeof(float);
    int double_size =   sizeof(double *);
    int unsignedChar_size = sizeof(unsigned char);
    int signedChar_size = sizeof(signed char);
    int unsignedShort_size = sizeof(unsigned short);
    int signedShort_size = sizeof(signed short);
    int unsignedInt_size = sizeof(unsigned int);
    int signedInt_size = sizeof(signed int);
    int int8_size = sizeof(int8_t);
    int uint8_size = sizeof(uint8_t);
    int int16_size = sizeof(int16_t);
    int uint16_size = sizeof(uint16_t);
    int int32_size = sizeof(int32_t);
    int uint32_size = sizeof(uint32_t);
}

void report_pointer_types() {
    int char_pointer = sizeof(char*);
    int int_pointer = sizeof(int*);
    int short_pointer = sizeof(short*);
    int long_pointer = sizeof(long*);
    int long_int_pointer = sizeof(long int*);
    int short_int_pointer = sizeof(short int*);
    int float_pointer = sizeof(float*);
    int double_pointer = sizeof(double*);
    int unsigned_char_pointer = sizeof(unsigned char*);
    int signed_char_pointer = sizeof(signed char*);
    int unsigned_short_pointer = sizeof(unsigned short*);
    int signed_short_pointer = sizeof(signed short*);
    int unsigned_int_pointer = sizeof(unsigned int*);
    int signed_int_pointer = sizeof(signed int*);
    int int8_pointer = sizeof(int8_t*);
    int uint8_pointer = sizeof(uint8_t*);
    int int16_pointer = sizeof(int16_t*);
    int uint16_pointer = sizeof(uint16_t*);
    int int32_pointer = sizeof(int32_t*);
    int uint32_pointer = sizeof(uint32_t*);
}

void make_square_wave(int delay) {
    P1_DIR |= 0x01;
    P1_OUT ^= 0x01;
    for (int i = 0; i < delay; i++);
    P1_OUT ^= 0x01;

}
