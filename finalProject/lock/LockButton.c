/*
 * LockButton.c
 *
 *  Created on: Nov 23, 2017
 *      Author: amabo
 */

#include "LockButton.h"

void configure_LockButton(){
    //Configure the right button to be the lock button
    P1->SEL0 &= ~(BIT4);      // Set P1.4 to General IO Mode
    P1->SEL1 &= ~(BIT4);      // Set P1.4 to General IO Mode
    P1->DIR &=  ~(BIT4);      // Set P1.4 Direction to input
    P1->REN |=    BIT4;       // Enable pullup/pulldown
    P1->OUT |=    BIT4;       // Enable PULLUP
    P1->IFG &=  ~(BIT4);      // Clear interrupts
    P1->IES |=    BIT4;       // Set P1 IFG flag to high-to-low transition

    //Enable interrupts at the peripheral
    P1->IFG =  0;
    P1->IE  =  BIT4;
    //Enable interrupts at the NVIC
    NVIC_EnableIRQ(PORT1_IRQn);
}

void PORT1_IRQHandler(){
    if(P1->IFG & BIT4){
        TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;  //Toggle Buzzer's pwm interrupts
        //EUSCI_A3->IFG |= BIT1;
        P6->OUT |= BIT1;
    }

    P1->IFG = 0;
}
