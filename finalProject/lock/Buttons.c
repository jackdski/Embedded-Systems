/*
 * LockButton.c
 *
 *  Created on: Nov 23, 2017
 *      Author: amabo
 */

#include "LockButton.h"
#include "Buzzer.h"
#include "Solenoid.h"

/*
 * In order to configure the lock button, we need to set it as an input, enable internal resistors
 * Make it a pullup circuit, set the transition to be from high to low, and enable the interrupts.
 * This function takes care of all of this initialization
 */
void configure_LockButton(){
    //Configure the right button to be the lock button
    P1->DIR &=  ~(BIT4);      // Set P1.4 Direction to input
    P1->REN |=    BIT4;       // Enable pullup/pulldown hardware
    P1->OUT |=    BIT4;       // Make it a pullup button
    P1->IES |=    BIT4;       // Set P1 IFG flag to high-to-low transition

    //Enable interrupts at the peripheral
    P1->IFG =  0;
    P1->IE  =  BIT4;
    //Enable interrupts at the NVIC
    NVIC_EnableIRQ(PORT1_IRQn);
}

/*
 * This is the lock button's handler function.  This is where we register the button's inputs so we can have effective output
 */
void PORT1_IRQHandler(){
    if(P1->IFG & BIT4){
        /*
         * REMOVE THE FUNCTION CALLS FROM HERE BEFORE WE FINISH
         */
        //short_buzzes();
        long_buzz();

        pullLatch();

    }

    //Remove the port interrupt
    P1->IFG = 0;
}
