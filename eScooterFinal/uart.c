/*
 * uart.c
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */

#include "uart.h"
#include "circbuf.h"

extern CircBuf_t * TXBuf;

void configure_UART(){
    //Configure UART pins, set 2-UART pins to UART mode
    P1->SEL0 |=  (BIT2 | BIT3);
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A0->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A0->BRW = 78;                         //Set Baud Rate
    EUSCI_A0->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A0->IFG &= ~(BIT1 | BIT0);
    UCA0IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA0_IRQn);
}

void UART_send_byte(uint8_t data){
    EUSCI_A0->TXBUF = data;
}

void EUSCIA0_IRQHandler(){/*
    if (EUSCI_A0->IFG & BIT0){
        if(currentChar == 26){
            transmitEC();
        }
        else{
            work = 1;
        }
        if(words ==0){
            time = 0;
            systickCounter = 0;
        }
    }*/

    if (EUSCI_A0->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
            EUSCI_A0->IFG &= ~BIT1;
              return;
        }
        UART_send_byte(removeItem(TXBuf));
    }

}

void transmitData(){

}

