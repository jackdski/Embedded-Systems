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
extern uint8_t CTime;

extern uint8_t newSID[4];
extern uint8_t SIDIterator;


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
        P5->IE &= ~BIT1;
        P3->IE &= ~BIT5;

    }
    else if(stationState == SetTime){
        TIMER_A0->CTL &=  ~(BIT1);
    }
    else if(newState == DoubleDipping){

    }


    //Turn on the new State's features
    if(newState == Standby){

        stationState = Standby;
    }
    else if(newState == Register){
        //Once we leave the locked state, we expect the beam to be broken, so disable its interrupts
        SIDIterator = 0;
        newSID[0] = '5';
        newSID[1] = '5';
        newSID[2] = '5';
        newSID[3] = '5';

        stationState = Register;
    }
    else if(newState == SetTime){
        CTime = 5;

        P3->IFG = 0;
        P5->IFG = 0;

        P5->IE |= BIT1;
        P3->IE |= BIT5;
        stationState = SetTime;
    }
    else if(newState == DoubleDipping){

    }

}
