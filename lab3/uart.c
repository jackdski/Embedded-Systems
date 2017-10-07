/*
 * uart.c
 *
 *  Created on: Sep 26, 2017
 *      Author: amabo
 */
#include "uart.h"
#include "circbuf.h"


extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;
extern uint8_t work;

void configure_serial_port(){
    //Configure UART pins, set 2-UART pins to UART mode
    P1->SEL0 |=  (BIT2 | BIT3);
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A0->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A0->BRW = 78;                         //Set Baud Rate
    EUSCI_A0->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

#ifndef BLOCKING
    EUSCI_A0->IFG &= ~(BIT1 | BIT0);
    UCA0IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA0_IRQn);
#endif
}

void configure_clocks(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.

}

#ifdef  BLOCKING
void UART_send_n(uint8_t * data, uint8_t length){
    //Code to iterate through the transmit data
    if(!data)
        return;
    volatile uint8_t n;

    for(n = 0; n<length; n++){
        UART_send_byte(data[n]);
    }
}
#endif

void UART_send_byte(uint8_t data){
#ifdef  BLOCKING
    while(!(EUSCI_A0->IFG & BIT1));  //While it hasn't finished transmitting
#endif
    EUSCI_A0->TXBUF = data;
}

void EUSCIA0_IRQHandler(){

#ifdef ECHO
    if (EUSCI_A0->IFG & BIT0){
        addItemCircBuf(TXBuf, EUSCI_A0->RXBUF);
        if(TXBuf->num_items == 1){
            EUSCI_A0->IFG |= BIT1;
        }
    }
    if (EUSCI_A0->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
           EUSCI_A0->IFG &= ~BIT1;
           return;
        }
        UART_send_byte(removeItem(TXBuf));
    }
#endif

    if (EUSCI_A0->IFG & BIT0){
        uint8_t data = EUSCI_A0->RXBUF;
        if(data == '\n' || (RXBuf->length == RXBuf->num_items)){
            work = 1;
            //EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset to pause RX
        }
        else
            addItemCircBuf(RXBuf, data);

    }
    if (EUSCI_A0->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
            EUSCI_A0->IFG &= ~BIT1;
              return;
        }
        UART_send_byte(removeItem(TXBuf));
    }

}

void transmitRX(){
    if(isEmpty(RXBuf)){
        return;
    }
    volatile uint32_t length = RXBuf->num_items;
    volatile uint32_t i;
    uint8_t temp;

    for(i = 0; i < length; i++){
        temp = removeItem(RXBuf);
        addItemCircBuf(RXBuf, temp);
        addItemCircBuf(TXBuf, temp);
    }
    UART_send_byte(removeItem(TXBuf));
}
