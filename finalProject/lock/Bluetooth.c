/*
 * Bluetooth.c
 *
 *  Created on: Nov 26, 2017
 *      Author: amabo
 */
#include "Bluetooth.h"

//Connect 9.7 to the RX
void configure_Bluetooth(){
    //Configure UART pins, set 2-UART pins to UART mode
    P9->SEL0 |=  (BIT6 | BIT7);
    P9->SEL1 &= ~(BIT6 | BIT7);

    EUSCI_A3->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A3->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A3->BRW = 78;                          //Set Baud Rate
    EUSCI_A3->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A3->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A3->IFG &= ~(BIT1 | BIT0);
    UCA3IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA3_IRQn);

    sendByte('A');
}

uint8_t deletethis = 0;
void EUSCIA3_IRQHandler(){
    if (EUSCI_A3->IFG & BIT0){
        uint8_t data = EUSCI_A3->RXBUF;
        P1->OUT ^= BIT0;
    }

    if (EUSCI_A3->IFG & BIT1){
        //Transmit Stuff
        EUSCI_A3->IFG &= ~BIT1;

        /*if(isEmpty(TXBuf)){
              return;
        }*/
       /* if(deletethis%2)
            sendByte('A');
        else
            EUSCI_A3->IFG &= ~BIT1;
        deletethis++;
        */
    }

}

void sendByte(uint8_t data){
    EUSCI_A3->TXBUF = data;
}

