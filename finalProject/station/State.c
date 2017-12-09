/*
 * State.c
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */
#include "State.h"

extern State stationState;



/*When transitioning between states we need to make sure the proper functions are
 * enabled and disabled.  For that reason check which state we are leaving and
 */
void enterState(State newState){
    //Turn off the LEDs

    //Turn off the current state's features
    if(stationState == Standby){
     //   UCA2IE &= ~BIT0;  //Turn off interrupts for RFID RX
    }
    else if(stationState == Register){
        //Once we leave the locked state, we expect the beam to be broken, so disable its interrupts
    }
    else if(stationState == SetTime){

    }
    else if(stationState == Checkout){

    }

    //Turn on the new State's features
    if(stationState == Standby){
      //  UCA2IE |= BIT0;  //Turn on interrupts for RFID RX
    }
    else if(stationState == Register){
        //Once we leave the locked state, we expect the beam to be broken, so disable its interrupts
    }
    else if(stationState == SetTime){

    }
    else if(stationState == Checkout){

    }

}
