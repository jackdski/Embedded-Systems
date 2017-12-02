/*
 * Buzzer.c
 *
 *  Created on: Nov 23, 2017
 *      Author: amabo
 */
#include "Buzzer.h"

#define TIMEOUT  5000

//Boolean used to distinguish between the two pulse modes (one beep or five)
uint8_t  pulseType;

//Counter used to create the longer period of toggling over the higher frequency that drives the buzzer.
uint16_t count = 0;
/*
 * The buzzer needs an output pin, and a timer to control it.  This function
 * configures both
 */
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

/*
 * This is the Buzzer's Timer's Handler function.  It can produce two outputs on
 * the buzzer's output.  It either has a constant wave for its entire period making one
 * long beep, or it will produce four short beeps.  Once the period is over, count is
 * reset and the timer is disabled
 */
void TA0_0_IRQHandler() {
    //Increment our counter to determine the period
    count++;

    //If we want one long beep, toggle the pin every time
    if(!pulseType)
    {
        P5->OUT ^= BIT4;              //Pulse P5.4 to run it at 5kHz
    }

    //If we want 4 short beeps, enable and disable the toggling every 500 counts
    if(pulseType){
        if(count % (TIMEOUT/5) < (TIMEOUT/10))
        {
            P5->OUT ^= BIT4;         //Pulse P5.4 to run it at 5kHz
        }
    }

    //If we have reached the end of our period, reset the count and turn off the timer
    if(count >= TIMEOUT){
        TIMER_A0->CCTL[0] &= !TIMER_A_CCTLN_CCIE;      // TACCR0 interrupt disable
        count = 0;
    }

    //Clear the timer interrupt
    TIMER_A0->CCTL[0] &= ~(BIT0);
}

/*
 * This function sets the pulseType to be one long beep, and enables the timer
 */
void long_buzz(){
    pulseType = 0;
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;  //Toggle Buzzer's pwm interrupts
}

/*
 * This function sets the pulseType to be four short beeps, and enables the timer
 */
void short_buzzes(){
    pulseType = 1;
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;  //Toggle Buzzer's pwm interrupts
}

