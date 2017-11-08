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

//Speed is calcualted by dividing the distance since the last measurement by .1s
//This works as the function is called every .1 seconds. We also update our total distance
//in this function.
float speed(){
    float spd = 02.25114 * beamBreaks / 0.1;
    totalDistance += beamBreaks*02.25114;
    beamBreaks = 0;
    return spd;
}

//We use a lastSpeed variable to determine direction. If we were stopped, and are no longer,
//We then can assume we are now moving in the direction we just accelerated. The VYNADC value
//needs to be calibrated to account for the MSP's angle in the box.
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
