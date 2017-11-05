/*
 * beamBreaks.c
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */
#include "beamBreaks.h"

extern uint8_t beamBreaks;

void configure_beamBreaks(){
    //data from encoder (port 3.2)
         P3->SEL0 &= ~(BIT2);      // set to General IO Mode
         P3->SEL1 &= ~(BIT2);      // Make sure not to be in tertiary function
         P3->DIR &= ~(BIT2);       // set direction to input
         P3->REN |= BIT2;          // enable pullup
         P3->OUT |= BIT2;          // clear interrupts
         P3->IES = BIT2;           // set IFT flag to high to low transition

         P3->IFG = 0;
         P3->IE =  (BIT2);       // Enable port interrupt

         NVIC_EnableIRQ(PORT3_IRQn);
}

void PORT3_IRQHandler(){

    if (P3->IFG & BIT2) {
        beamBreaks++;
        //P1->OUT ^= BIT0;
    }
    P3->IFG = 0;
}
