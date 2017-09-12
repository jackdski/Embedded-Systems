/*
 * timer.c
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo
 */
#include "timer.h"

void ISR_FUNCTION_TIMER()
{

}

void timer_a0_config(){
    TIMER_A0->R = CLEAR_TIMER;         // Reset count
    TIMER_A0->CTL = SET_CTL;       // SMCLK, Up mode
    //TIMER_A0->CCR[0] = ???;    // Value to count to
    //TIMER_A0->CCTL[0] = ???;   // TACCR0 interrupt enabled

    /* Enable Interrupts in the NVIC */
    NVIC_EnableIRQ(TA0_0_IRQn);
}
