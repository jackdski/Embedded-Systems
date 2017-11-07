/*
 * calculations.c
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */
#include "calculations.h"

extern uint32_t beamBreaks;
extern uint16_t VXNADC;
extern uint16_t VYNADC;
extern uint16_t VZNADC;

extern volatile float totalDistance;
extern volatile uint8_t direction;
uint8_t lastSpeed = 0;

float speed(){
    float spd = 02.25114 * beamBreaks / 0.1;
    totalDistance += beamBreaks*02.25114;
    beamBreaks = 0;
    return spd;
}

uint8_t calculateDirection(float spd){
    if(lastSpeed > 0 || spd  == 0){
        lastSpeed = spd;
        return direction;
    }
    if(VYNADC > 9000){
        lastSpeed = spd;
        return 0;
    }
    lastSpeed = spd;
    return 1;

}
