/*
 * RGB.c
 *
 *  Created on: Nov 28, 2017
 *      Author: amabo
 */
#include "RGB.h"
#include "State.h"
#include "BeamBreaks.h"

extern State lockState;

//Extra counter used to extend the period that the LED is activated
uint8_t RGBCount = 0;

uint8_t timedOut = 0;

/*
 * In order to configure the LEDs, we only need to set their direction to be outputs.
 * In this function we do that, and then make sure the LEDs start off.  We also
 * initialize a timer to turn off the LEDs after user interactions.
 */
void configure_RGB(){
    //Set all of the LEDs to be outputs
    P6->DIR |=  (BIT4 | BIT5);
    P5->DIR |=  (BIT5);

    //Call the LED_Off function to make sure all of the LEDs are off
    LED_Off();

    //Configure TimerA2 to so we can turn off the LEDs when to save power
    TIMER_A2->R = 0;                    // Clear timer count
    TIMER_A2->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID__8 | TIMER_A_CTL_MC__UP;            // Set to SMCLK, Up mode (BIT9 ON;)
    TIMER_A2->CCR[0] = 65535;        // Value to count to

    //Note the interrupts are enabled in the lock button handler function
    //Enable interrupts at the NVIC
    NVIC_EnableIRQ(TA2_0_IRQn);
}

//Turn on the Red LED
inline void Red_LED_On(){
    P5->OUT |= BIT5;
    TIMER_A2->CCTL[0] |= TIMER_A_CCTLN_CCIE;      // Timer 2 interrupt disable
}

//Turn on the Blue LED
inline void Blue_LED_On(){
    P6->OUT |= BIT4;
    TIMER_A2->CCTL[0] |= TIMER_A_CCTLN_CCIE;      // Timer 2 interrupt disable
}

//Turn on the Green LED
inline void Green_LED_On(){
    P6->OUT |= BIT5;
    TIMER_A2->CCTL[0] |= TIMER_A_CCTLN_CCIE;      // Timer 2 interrupt disable
}

//Turn off all of the LEDs
inline void LED_Off(){
    P6->OUT &= ~(BIT4 | BIT5);
    P5->OUT &= ~(BIT5);
}

void TA2_0_IRQHandler() {
    //Increment solCount
    RGBCount++;

    //
    if(RGBCount >= 40 && lockState != Unlockable){
        RGBCount = 0;
        LED_Off();
        TIMER_A2->CCTL[0] &= !TIMER_A_CCTLN_CCIE;      // TACCR0 interrupt disable
    }

    if(RGBCount >= 80 && lockState == Unlockable){
        timedOut = 1;
        RGBCount = 0;
        LED_Off();
        TIMER_A2->CCTL[0] &= !TIMER_A_CCTLN_CCIE;      // TACCR0 interrupt disable
    }

    //Remove the timer interrupt
    TIMER_A2->CCTL[0] &= ~(BIT0); //Clear the interrupts
}

