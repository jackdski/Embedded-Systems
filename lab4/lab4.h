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

#define CLEAR_TIMER (uint16_t)(0x0000)
#define SET_CTL     (uint16_t)(TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_3 | TIMER_A_CTL_MC__UP)
#define COUNT_TO    (uint16_t)(37822) //37775
#define SET_CCTL    (uint16_t)(0x0010)

typedef struct values{
    float    distancePerBreak;
    volatile uint32_t beamBreaks;
    volatile uint32_t bBSinceLast;
    volatile float    distanceTraveled;
    volatile uint32_t timesTimed;
    volatile float    averageVelocity;
    volatile float    instantVelocity;
    volatile float    instantRotVel;
} es_V; //eScooter Values

// Converts a string of numbers into an integer
//uint32_t  atoi(uint8_t * a);
void itoa(uint32_t num, int8_t size, uint8_t * str );
void ftoa(float number, uint8_t decimalPlace, uint8_t size, uint8_t * str);
void timer_a0_config();
es_V * make_eScoot();
void configure_eScooter();

#endif
