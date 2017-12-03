/*
 * RFID.c
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */
#include "RFID.h"

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
        uint8_t data = EUSCI_A2->RXBUF;
        P1->OUT ^= BIT0;
    }
}
