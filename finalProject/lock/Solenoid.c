/*
 * Solenoid.c
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */
#include "Solenoid.h"
#include "State.h"
#include "BeamBreaks.h"

extern State lockState;

uint8_t checkBeamBreak = 0;

//Extra counter used to extend the period that the Solenoid is activated
uint8_t solCount = 0;

/*
 * The solenoid is controlled by a single output pin and a timer.  This function configures both
 */
void configure_Solenoid(){
    //Configure the enable pin to be an output
    P6->DIR |=  BIT1;
    P6->OUT &= ~BIT1;

    //Configure TimerA1 to control the lock
    TIMER_A1->R = 0;                    // Clear timer count
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID__8 | TIMER_A_CTL_MC__UP;            // Set to SMCLK, Up mode (BIT9 ON;)
    TIMER_A1->CCR[0] = 65535;        // Value to count to

    //Note the interrupts are enabled in the lock button handler function
    //Enable interrupts at the NVIC
    NVIC_EnableIRQ(TA1_0_IRQn);
}

/*
 * In order to lock and unlock the solenoid, we need to pull it back.  We do this by writing our enable pin high, and starting our timer
 * The timer will count out the period, and turn off the solenoid.
 */
inline void pullLatch(){
    P6->OUT |= BIT1;
    TIMER_A1->R = 0;                              // Clear timer count
    TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_CCIE;      // Timer 1 interrupt disable
}

/*
 * In order to turn off the solenoid we need to use a timer.  Our period extends beyond the amount of time the timer can count to by itself
 * so we use solCount to increase the solenoid's period.  Once it has incremented solCount 75 times, solCount is reset, the solenoid is turned
 * off, and the timer is disabled.
 */
void TA1_0_IRQHandler() {
    //Increment solCount
    solCount++;

    //If solCount has reached 75, reset it, turn off the solenoid, and turn off the timer
    if(solCount == 75){
        solCount = 0;
        if(lockState == Unlocked)
            checkBeamBreak = 1;

        P6->OUT &= ~BIT1;
        TIMER_A1->CCTL[0] &= !TIMER_A_CCTLN_CCIE;      // TACCR0 interrupt disable
    }

    //Remove the timer interrupt
    TIMER_A1->CCTL[0] &= ~(BIT0); //Clear the interrupts
}
