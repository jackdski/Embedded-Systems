/*
 * timer.c
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo and Jack
 */
#include "timer.h"

volatile uint8_t activationBit = 0;
volatile uint8_t extraBit = 0;

void TA0_0_IRQHandler() {

    TIMER_A0->CTL &= ~(BIT1);  //Turn off timer interrupts

    /*In order to prevent future button presses from interrupting our timer, we
     * need to store the flag data, ie the interrupt that sent us here, in a global
     * variable. This if statement does this.
     */
    if(!activationBit && (P1->IFG & (BIT1 | BIT4))){
        activationBit = P1->IFG & (BIT1 | BIT4);
        P1->IFG = 0;
    }

    /*As the timer cannot count up to 500ms, we need to count to a multiple of 500
     * many times. This interrupt goes off every 166ms, so we need three counts before
     * taking action. Extrabit keeps count of how many times our interrupt has gone off
     * since we last took action
     */
    if(extraBit == 3){

        //These bits are used to measure the ontime of each LED.
        P1->OUT ^= (BIT7 | BIT0);  //toggle bits

        /*
         *
         */
        int8_t RGBMask = P2->OUT & (BIT0 | BIT1 | BIT2);
        if((activationBit & BIT1) && (RGBMask == 7)){
            P2->OUT &= ~(BIT0 | BIT1 | BIT2);
            NVIC_DisableIRQ(TA0_0_IRQn);
            activationBit = 0;
            P1->IFG = 0;
            P1->IE = BIT1|BIT4;
            return;
        }
        else if((activationBit & BIT4) && (RGBMask == 1)){
            P2->OUT &= ~(BIT0 | BIT1 | BIT2);
            NVIC_DisableIRQ(TA0_0_IRQn);
            activationBit = 0;
            P1->IFG = 0;
            P1->IE = BIT1|BIT4;
            return;
        }


        if(activationBit & BIT1){
            P2->OUT += 1;
        }
        else if(activationBit & BIT4) {
            P2->OUT -= 1;
        }

    }


    TIMER_A0->R = 0;
    //TIMER_A0->CTL &= ~0;//(BIT0);
    TIMER_A0->CCTL[0] &= ~(BIT0);
    TIMER_A0->CTL |=  (BIT1);

    if(extraBit == 3){
        extraBit = 0;
    }
    else
        extraBit++;

#if 0
    TIMER_A0->CTL &= ~(BIT1);  //Turn off timer interrupts

    P1->OUT ^= (BIT7 | BIT0);

#endif

    TIMER_A0->R = 0;
        //TIMER_A0->CTL &= ~0;//(BIT0);
    TIMER_A0->CCTL[0] &= ~(BIT0);
    TIMER_A0->CTL |=  (BIT1);
}

void timer_a0_config(){
    TIMER_A0->R = 0;                    // Clear timer count
    TIMER_A0->CTL = SET_CTL;            // Set to SMCLK, Up mode (BIT9 ON;)
    TIMER_A0->CCR[0] = COUNT_TO;        // Value to count to
    TIMER_A0->CCTL[0] |= SET_CCTL;      // TACCR0 interrupt enabled

    // TO DO:
    //input divider set to /1 (CTL-BITS 7-6 = 00)
    //clear pending Timer interrupt flags (CCTL-BIT0 = 0?)
    //capture compare interrupt: enabled (CCTL-BIT4 = 1)
    //capture/compare timer interrupt enabled at the peripheral
    //global interrupts enabled (BIT4 = 1 (enabled)
    //set to compare mode   (CCTL-BIT8; 0=Comp.,1=Capt.)

    /* Enable Interrupts in the NVIC */
    //NVIC_EnableIRQ(TA0_0_IRQn);
    //NVIC->ISER[1] |= BIT3;
}
