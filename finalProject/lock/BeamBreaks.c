/*
 * beamBreaks.c
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */
#include "BeamBreaks.h"
#include "State.h"

/*
 * The beam break sensor acts as a button input.  For that reason we make all of the same configurations as a button
 * Including pullup resistors, interrupt transitions, and enabling the the interrupts
 */
void configure_BeamBreaks(){
        //data from encoder (port 3.2)
         P4->DIR  &= ~(BIT7);       // set direction to input
         P4->REN  |=   BIT7;        // enable pullup
         P4->OUT  |=   BIT7;        // clear interrupts
         P4->IES  |=   BIT7;        // set Interrupt flag to high to low transition



         //Enable the interrupt at the NVIC
         NVIC_EnableIRQ(PORT4_IRQn);
}

//Enable Beam Break Interrupts
void enable_BeamBreaks(){
    //Clear the interrupt flags
    P4->IFG = 0;
    // Enable port interrupt at the peripheral
    P4->IE  |=   BIT7;
}

//Disable Beam Break Interrupts
void disable_BeamBreaks(){
    // Disable port interrupt at the peripheral
    P4->IE  &=  ~BIT7;
}

/*
 * The Beam Breaks will only act as interrupts when in the locked state. If an interrupt fires, set the error state.
 */
void PORT4_IRQHandler(){
    //If the lock is removed unexpectedly
    if (P4->IFG & BIT7) {
        enterState(Error);
    }

    //Clear the Interrupt flags
    P4->IFG = 0;
}

/*
 * Returns a boolean of whether or not the beams have yet to be broken.
 */
inline uint8_t beams_Blocked(){
    return P4->IN;
}
