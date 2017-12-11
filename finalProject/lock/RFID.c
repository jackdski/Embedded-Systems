/*
 * RFID.c
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */
#include "RFID.h"
#include "Circbuf.h"
#include <stdlib.h>
#include "State.h"
#include "Buzzer.h"
#include "RGB.h"

extern CircBuf_t * RFIDBuf;
extern State       lockState;
extern uint8_t     mainUser[16]; // stores registered RFID data
extern uint8_t     newRFID;             // Flags that we have stored a new RFID tag


/*
 * The RFID reader makes use of one GPIO pin to control the reset input of the reader, and it makes use of the EUSCI_A2 in order to
 * Actually read the RFID data over UART.  These pins, as well as the UART communication, are configured here.
 */
void configure_RFID(){
    //Pin 6.0 is connected to the reset pin on the RFID Reader.  We need to set it as an output and then immediately strap it to high voltage
    P6->DIR |= BIT0; //Make the pin an output
    P6->OUT |= BIT0; //Write the pin to be high

    //In order to read our RFID data, we need to use the RX terminal of EUSCI_A2.  This is where we configure it.
    //Set the RX pin to be in UART mode
    P3->SEL0 |=  BIT2;
    P3->SEL1 &= ~BIT2;

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A2->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A2->BRW = 78;                         //Set Baud Rate to be 9600
    EUSCI_A2->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A2->IFG &= ~BIT0;
    UCA2IE |= BIT0;  //Turn on interrupts for RX
    NVIC_EnableIRQ(EUSCIA2_IRQn);
}

/*
 * Whenever we receive information from our RFID reader, store it for comparisons
 */
void EUSCIA2_IRQHandler(){

    if (EUSCI_A2->IFG & BIT0){

        if(lockState != Locked){
            Red_LED_On();
            short_buzzes();
            EUSCI_A2->RXBUF;
        }
        else{
            addItemCircBuf(RFIDBuf, EUSCI_A2->RXBUF);
        }
        if(isFullCircBuf(RFIDBuf)){
            newRFID = 1;
        }

    }
}

//This function returns a boolean in order to determine if the new RFID string matches
//Our registered user.  True is a match, false is they are different.
uint8_t compare_RFID(){
    volatile uint8_t i;

    //Go through each letter of our stored userID.  If the id doesn't match the read tag
    //at any point, return false.  Otherwise return true when finished.
    for(i = 0; i < 16; i++){
        uint8_t main = mainUser[i];
        uint8_t new  = RFIDBuf->buffer[i];//= removeItem(RFIDBuf);
        if(main != new){
            resetCircBuf(RFIDBuf);
            return 0;
        }
    }
    resetCircBuf(RFIDBuf);
    return 1;
}
