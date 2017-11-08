/*
 * beamBreaks.c
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */
#include "beamBreaks.h"

extern uint32_t beamBreaks;
extern uint8_t transmit;

//Configure pin 3.2 to measure the beam breaks of the wheel
void configure_beamBreaks(){
    //data from encoder (port 3.2)
         P3->SEL0 &= ~(BIT2);      // set to General IO Mode
         P3->SEL1 &= ~(BIT2);      // Make sure not to be in tertiary function
         P3->DIR &= ~(BIT2);       // set direction to input
         P3->REN |= BIT2;          // enable pullup
         P3->OUT |= BIT2;          // clear interrupts
         P3->IES |= BIT2;           // set IFT flag to high to low transition

         P3->IFG = 0;
         P3->IE  |=  (BIT2);       // Enable port interrupt

         NVIC_EnableIRQ(PORT3_IRQn);
}

//If 3.2 fires we saw a spoke, iterate our beambreaks counter.
//3.5 is configured in port.c to be a transmission button.
void PORT3_IRQHandler(){
    if (P3->IFG & BIT2) {
        beamBreaks++;
    }

    if(P3->IFG & (BIT5)){
        transmit = 1;
    }
    P3->IFG = 0;
}
