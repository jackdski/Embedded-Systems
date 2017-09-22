/*
 * timer.c
 *This code configures and handles all timer operations within the code.
 *By default it will only be turned on by button presses and will toggle through all of
 *the P2 LEDs in different sequences depending on which button was pressed.
 *  Created on: Sep 7, 2017
 *      Author: Avery and Jack
 */
#include "timer.h"

//These bits are used to store information for the timers operation between calls.
//Activation bit stores the P1 flag that started the interrupt, and extraBit is used
//to run through the function multiple times until we reach 500ms on each light.
volatile uint8_t activationBit = 0;
volatile uint8_t extraBit = 0;

void TA0_0_IRQHandler() {

    TIMER_A0->CTL &= ~(BIT1);  //Turn off timer interrupts

  #ifdef PROB10 
    /*In order to prevent future button presses from interrupting our timer, we
     * need to store the flag data, ie the interrupt that sent us here, in a global
     * variable. This if statement does this.
     */
    if(!activationBit && (P1->IFG & (BIT1 | BIT4))){
        activationBit = P1->IFG & (BIT1 | BIT4);
        P1->IFG = 0;
    }

    /*As the timer cannot count up to 500ms, we need to count to a multiple of 500
     * many times. This interrupt goes off every 125ms, so we need three counts before
     * taking action. Extrabit keeps count of how many times our interrupt has gone off
     * since we last took action
     */
    if(extraBit == 3){

        //These bits are used to measure the ontime of each LED.
        P1->OUT ^= (BIT7 | BIT0);  //toggle bits

        /*
         *These if statements will fire if the LED is one value away from
         *overflowing into other port pins, they handle counting up and down
         *respectively.  Inside the functions they turn off the RGB LED, disable
         *timer interrupts, reset the activation bit, and reinitialize the port interrupts
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

        /*
        *If we are not yet at the end, iterate the direction we want to.
        */
        if(activationBit & BIT1){
            P2->OUT += 1;
        }
        else if(activationBit & BIT4) {
            P2->OUT -= 1;
        }

    }
    
    //If I am exiting the function, reset extrabit to zero. Otherwise iterate it
    if(extraBit == 3){
        extraBit = 0;
    }
    else
        extraBit++;
#endif 

    //Clear the timer flag
    TIMER_A0->CCTL[0] &= ~(BIT0);
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
    
    //Note we do not call the enable NVIC function as that operation is tied to the ports

}
