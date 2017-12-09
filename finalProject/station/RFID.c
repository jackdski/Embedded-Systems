/*
 * RFID.c
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */
#include "RFID.h"
#include "Circbuf.h"
#include <stdlib.h>

extern CircBuf_t * RFIDBuf;
extern uint8_t     newRFID;


/*
 * The RFID reader makes use of  the EUSCI_A2 in order to Actually read the RFID data over UART.
 *  These pins, as well as the UART communication, are configured here.
 */
void configure_RFID(){

    //In order to read our RFID data, we need to use the RX terminal of EUSCI_A2.  This is where we configure it.
    //Set the RX pin to be in UART mode
    P2->SEL0 |=  BIT2;
    P2->SEL1 &= ~BIT2;

    EUSCI_A1->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A1->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A1->BRW = 78;                         //Set Baud Rate to be 9600
    EUSCI_A1->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A1->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A1->IFG &= ~BIT0;

    //Note the interrupts for this peripheral are turned up in the state transition
    UCA1IE |= BIT0;  //Turn on interrupts for RFID RX
    NVIC_EnableIRQ(EUSCIA1_IRQn);
}

/*
 * Whenever we receive information from our RFID reader, store it for comparisons
 */
void EUSCIA1_IRQHandler(){

    if (EUSCI_A1->IFG & BIT0){
        addItemCircBuf(RFIDBuf, EUSCI_A1->RXBUF);

        if(isFullCircBuf(RFIDBuf)){
            newRFID = 1;
        }
    }
}


//This function returns a boolean in order to determine if the new RFID string matches
//Previously registered users.  True is a match, false is they are different.
uint8_t compare_RFID(uint8_t RFIDA[16], uint8_t RFIDB[16]){
    volatile int i;

    //Go through each letter of our stored userID.  If the id doesn't match the read tag
    //at any point, return false.  Otherwise return true when finished.
    for(i = 0; i < 16; i++){
        uint8_t main = RFIDA[i];
        uint8_t new  = RFIDB[i];
        if(main != new){
            return 0;
        }
    }
    resetCircBuf(RFIDBuf);
    return 1;
}


