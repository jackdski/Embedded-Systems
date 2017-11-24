/*
*
* Wireless Bike Lock
* bluetooth.c
*
* 11-21-17
*
*/
#include "msp.h"
#include "bluetooth.h"
#include "register.h"
#include <stdint.h>

void configure_BLUE_UART(){
    //Configure UART pins, set 2-UART pins to UART mode
    P9->SEL0 |=  (BIT6 | BIT7);
    P9->SEL1 &= ~(BIT6 | BIT7);

    EUSCI_A3->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A3->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A3->BRW = 19;                         //Set Baud Rate
    EUSCI_A3->MCTLW |= (BIT0 | BIT8);           //Set modulator bits
    EUSCI_A3->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A3->IFG &= ~(BIT1 | BIT0);
    //UCA3IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
   // NVIC_EnableIRQ(EUSCIA3_IRQn);
}

void EUSCIA3_IRQHandler(){
    if (EUSCI_A3->IFG & BIT0){
        //BLUART_send_byte(EUSCI_A3->RXBUF);
        uint8_t data = EUSCI_A3->RXBUF;
        P1->OUT ^= BIT0;
    }

    if (EUSCI_A3->IFG & BIT1){
        //Transmit Stuff
        /*if(isEmpty(TXBuf)){
            EUSCI_A3->IFG &= ~BIT1;
              return;
        }*/
        //EUSCI_A3->IFG &= ~BIT1;
        //BLUART_send_byte('A');
    }

}

void BLUART_send_byte(uint8_t data){
    while(!(EUSCI_A3->IFG & BIT1));  //While it hasn't finished transmitting
    EUSCI_A3->TXBUF = data;
}
