/*
 * Buzzer.c
 *
 *  Created on: Nov 23, 2017
 *      Author: amabo
 */
#include "Buzzer.h"
uint16_t timeout = 0;

void configure_Buzzer(){
    //Configure the buzzer control GPIO pin
    P5->DIR |= BIT4;

    //Configure the timer that will control the buzzer
    TIMER_A0->R = 0;                    // Clear timer count
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID__1 | TIMER_A_CTL_MC__UP;            // Set to SMCLK, Up mode (BIT9 ON;)
    TIMER_A0->CCR[0] = 2399;        // Value to count to

    //Note the interrupts are enabled in the lock button handler function
    //Enable interrupts at the NVIC
    NVIC_EnableIRQ(TA0_0_IRQn);
}

void TA0_0_IRQHandler() {
    timeout++;
    P5->OUT ^= BIT4;              //Pulse P5.4 to run it at 5kHz
    if(timeout >= 5000){
        TIMER_A0->CCTL[0] &= !TIMER_A_CCTLN_CCIE;      // TACCR0 interrupt disable
        timeout = 0;
        P6->OUT &= ~BIT1;
    }
    TIMER_A0->CCTL[0] &= ~(BIT0); //Clear the interrupts
}

