/*
 * LockButton.c
 *
 *  Created on: Nov 23, 2017
 *      Author: amabo
 */

#include "Buttons.h"
#include "Buzzer.h"
#include "Solenoid.h"
#include "State.h"
#include "RGB.h"

extern State lockState;
extern uint8_t RGBCount;

/*
 * In order to configure the lock button, we need to set it as an input, enable internal resistors
 * Make it a pullup circuit, set the transition to be from high to low, and enable the interrupts.
 * This function takes care of all of this initialization
 */
void configure_LockButton(){
    //Configure the right button (1.4) to be the Admin Button
    //Configure the right button (1.1) to be the Lock button
    P1->DIR &=  ~(BIT1 | BIT4);      // Set P1.4 Direction to input
    P1->REN |=    BIT1 | BIT4;       // Enable pullup/pulldown hardware
    P1->OUT |=    BIT1 | BIT4;       // Make it a pullup button
    P1->IES |=    BIT1 | BIT4;       // Set P1 IFG flag to high-to-low transition

    //Enable interrupts at the peripheral for the Admin Button
    P1->IFG =  0;
    P1->IE  =     BIT1 | BIT4;

    //Enable interrupts at the NVIC for the port
    NVIC_EnableIRQ(PORT1_IRQn);
}

/*
 * This is the lock button's handler function.  This is where we register the button's inputs so we can have effective output
 */
void PORT1_IRQHandler(){
    //Handler for the Admin Button
    if(P1->IFG & BIT1){
        if(lockState == Unlockable || lockState == Unlocked){
            long_buzz();
            pullLatch();
            LED_Off();
            RGBCount = 0;
            Green_LED_On();
        }
        else{
            short_buzzes();
            LED_Off();
            Red_LED_On();
        }
    }

    //Handler for the Admin Button
    if(P1->IFG & BIT4){
        /*
         * REMOVE THE FUNCTION CALLS FROM HERE BEFORE WE FINISH
         */
        if(lockState == Error){
            enterState(Locked);
        }
        else if(lockState == Locked){
            enterState(Unlockable);
        }
        else if(lockState == Unlockable){
            enterState(Unlocked);
        }
        else if(lockState == Unlocked){
            enterState(Error);
        }
    }

    //Remove the port interrupt
    P1->IFG = 0;
}
