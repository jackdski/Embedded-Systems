/*
 * beamBreaks.c
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */
#include <BeamBreaks.h>

/*
 * The beam break sensor acts as a button input.  For that reason we make all of the same configurations as a button
 * Including pullup resistors, interrupt transitions, and enabling the the interrupts
 */
void configure_beamBreaks(){
        //data from encoder (port 3.2)
         P4->DIR  &= ~(BIT7);       // set direction to input
         P4->REN  |=   BIT7;        // enable pullup
         P4->OUT  |=   BIT7;        // clear interrupts
         P4->IES  |=   BIT7;        // set Interrupt flag to high to low transition

         //Clear the interrupt flags
         P4->IFG = 0;
         // Enable port interrupt at the peripheral (ONLY IN LOCKED)
         P4->IE  |=  (BIT7);

         //Enable the interrupt at the NVIC
         NVIC_EnableIRQ(PORT4_IRQn);
}

/*
 * The Beam Breaks will only act as interrupts when in the locked state. If an interrupt fires, set the error state.
 */
void PORT4_IRQHandler(){

    if (P4->IFG & BIT7) {
        P1->OUT ^= BIT0;
    }


    P4->IFG = 0;
}
