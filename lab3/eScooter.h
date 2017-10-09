/*
 * eScooter.h
 *
 *  Created on: Oct 7, 2017
 *      Author: amabo
 */

#ifndef ESCOOTER_H_
#define ESCOOTER_H_

#include "msp.h"
#include <stdint.h>
#include <stdlib.h>

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

es_V * make_eScoot();

void configure_eScooter();
void transmitDistance();


#endif /* ESCOOTER_H_ */
