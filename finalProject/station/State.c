/*
 * State.c
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */
#include "State.h"
#include "Circbuf.h"

extern State stationState;
extern CircBuf_t * RFIDBuf;



/*When transitioning between states we need to make sure the proper functions are
 * enabled and disabled.  For that reason check which state we are leaving and
 */
void enterState(State newState){
    //Turn off the LEDs

    //Turn off the current state's features
    if(stationState == Standby){
        resetCircBuf(RFIDBuf);
    }
    else if(stationState == Register){
        //Once we leave the locked state, we expect the beam to be broken, so disable its interrupts
    }
    else if(stationState == SetTime){

    }
    else if(stationState == Checkout){

    }

    //Turn on the new State's features
    if(newState == Standby){

        stationState = Standby;
    }
    else if(newState == Register){
        //Once we leave the locked state, we expect the beam to be broken, so disable its interrupts
        stationState = Register;
    }
    else if(newState == SetTime){

        stationState = SetTime;
    }
    else if(newState == Checkout){

        stationState = Checkout;
    }

}
