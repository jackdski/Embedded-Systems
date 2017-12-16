/*
 * Buttons.c
 *
 *  Created on: Dec 6, 2017
 *      Author: amabo
 */
#include "Buttons.h"
#include "State.h"

extern uint8_t SIDReady;
extern uint8_t newSID[4];
volatile uint8_t SIDIterator = 0;

extern State stationState;

extern uint8_t CTimeReady;
extern uint8_t CTime;

extern uint8_t LOGFlag;


/*
 * In order to configure the lock button, we need to set it as an input, enable internal resistors
 * Make it a pullup circuit, set the transition to be from high to low, and enable the interrupts.
 * This function takes care of all of this initialization
 */
void configure_Buttons(){
    //Configure the Right MSP button (1.4)
    //Configure the Left  MSP button (1.1)
    P1->DIR &=  ~(BIT1 | BIT4);      // Set P1.4 Direction to input
    P1->REN |=    BIT1 | BIT4;       // Enable pullup/pulldown hardware
    P1->OUT |=    BIT1 | BIT4;       // Make it a pullup button
    P1->IES |=    BIT1 | BIT4;       // Set P1 IFG flag to high-to-low transition

    //Configure the Top Boosterpack button (5.1)
    P5->DIR &=   ~BIT1;              // Port configured as input
    P5->REN |=    BIT1;              // Pullup or pulldown enabled
    P5->IFG &=   ~BIT1;              // Clear interrupts for J4.33 and J4.32
    P5->IES |=    BIT1;              // Flag set with high-to-low transition

    //Configure the Bottom Boosterpack button (3.5)
    P3->DIR &=   ~BIT5;              // Port configured as input
    P3->REN |=    BIT5;              // Pullup or pulldown enabled
    P3->IFG &=   ~BIT5;              // Clear interrupts for J4.33 and J4.32
    P3->IES |=    BIT5;              // Flag set with high-to-low transition

    //Enable interrupts at the peripheral for the MSP Buttons
    P1->IFG =  0;
    P1->IE  =     BIT1 | BIT4;

    //Enable interrupts at the peripheral for the MSP Buttons
    P5->IFG =  0;
    P3->IFG =  0;

    P5->IE  |=    BIT1;
    P3->IE  |=    BIT5;


    //Enable interrupts at the NVIC for the port
    NVIC_EnableIRQ(PORT1_IRQn);
    NVIC_EnableIRQ(PORT3_IRQn);
    NVIC_EnableIRQ(PORT5_IRQn);
}

/*
 * This is the lock button's handler function.  This is where we register the button's inputs so we can have effective output
 */
void PORT1_IRQHandler(){
    //Handler for the Left MSP Button
    if(P1->IFG & BIT1){

    }

    //Handler for the Right MSP Button
    if(P1->IFG & BIT4){
        LOGFlag = 1;
    }

    //Remove the port interrupt
    P1->IFG = 0;
}
void PORT3_IRQHandler(void){
    P3->IE &= ~BIT5;
    if(P3->IFG & BIT5){
        P3->IFG &= ~(BIT5);

        if(stationState == Register){
            if(SIDIterator != 0){
                SIDIterator--;
            }
        }
        else if(stationState == SetTime){
            CTimeReady = 1;
        }
    }

    //Re-enabled in Joystick.c at the TA0 handler
}

void PORT5_IRQHandler(void){
    P5->IE &= ~BIT1;

    if(P5->IFG & BIT1){
        P5->IFG &= ~(BIT1);
        if(stationState == Register){
            if(SIDIterator == 3){
                SIDReady = 1;
            }
            SIDIterator++;
        }
        else if(stationState == SetTime){
            CTimeReady = 1;
        }
    }
    //Re-enabled in Joystick.c at the TA0 handler
}



