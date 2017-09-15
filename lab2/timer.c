/*
 * timer.c
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo and Jack
 */
#include "timer.h"

void TA0_0_IRQHandler() {       // possibly N instead of 0
    //int flag = (TIMER_A0->CTL & BIT0);
    P1->OUT ^= (BIT7 | BIT0);
    TIMER_A0->CTL &= ~(BIT0 | BIT2);   // Clear flag
}

void timer_a0_config(){
    TIMER_A0->R = 0;                // Clear timer count
    TIMER_A0->CTL |= SET_CTL;        // Set to SMCLK, Up mode (BIT9 ON;)
    TIMER_A0->CCR[0] = 1000;        // Value to count to
    TIMER_A0->CCTL[0] &= ~(BIT1);    // TACCR0 interrupt enabled
    TIMER_A0->CCTL[0] |= CCIE;

    // TO DO:
    //input divider set to /1 (CTL-BITS 7-6 = 00)
    //clear pending Timer interrupt flags (CCTL-BIT0 = 0?)
    //capture compare interrupt: enabled (CCTL-BIT4 = 1)
    //capture/compare timer interrupt enabled at the peripheral
    //global interrupts enabled (BIT4 = 1 (enabled)
    //set to compare mode   (CCTL-BIT8; 0=Comp.,1=Capt.)

    /* Enable Interrupts in the NVIC */
    NVIC_EnableIRQ(TA0_0_IRQn);
}
