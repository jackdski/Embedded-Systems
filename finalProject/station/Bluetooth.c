/*
 * Wireless Bike Lock - Lock
 * Bluetooth.c
 *
 */

#include "msp.h"
#include "Bluetooth.h"
#include "Circbuf.h"
#include <stdint.h>

extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;
extern CircBuf_t * TESTtxBuf;
extern CircBuf_t * TESTrxBuf;

void configure_SystemClock(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.
}

//Connect 3.3 to the RX
void configure_serial_port(){
    //Configure UART pins, set 2-UART pins to UART mode
    P3->SEL0 |=  (BIT2 | BIT3);
    P3->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A2->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A2->BRW = 78;                         //Set Baud Rate
    EUSCI_A2->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A2->IFG &= ~(BIT1 | BIT0);
    UCA2IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA2_IRQn);
}

void configAltBT(){
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

inline void UART_send_byte(uint8_t data){
    EUSCI_A2->TXBUF = data;
}

inline void UART_send_bytez(uint8_t data){
    EUSCI_A0->TXBUF = data;
}

void UART_send_n(uint8_t * data, uint8_t length){
    //Code to iterate through the transmit data
    if(!data)
        return;
    volatile uint8_t n;

    for(n = 0; n<length; n++){
        UART_send_byte(data[n]);
    }
}


void EUSCIA0_IRQHandler(){
//echo
    /*
    if (EUSCI_A0->IFG & BIT0){
        //addItemCircBuf(TXBuf, EUSCI_A0->RXBUF);
        addItemCircBuf(TESTrxBuf, EUSCI_A0->RXBUF);
        //if(TXBuf->num_items == 1){
         //   EUSCI_A0->IFG |= BIT1;
        //}
    }
    */
    if(EUSCI_A0->IFG & BIT0) {
        P1->OUT |= BIT0;
        addItemCircBuf(RXBuf,EUSCI_A0->RXBUF);
    }
    if(EUSCI_A0->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
            EUSCI_A0->IFG &= ~BIT1;
            return;
        }
        //UART_send_bytez(removeItem(TESTtxBuf));
        UART_send_bytez(removeItem(TXBuf));
    }
}

// use echo stuff to return data to station
void EUSCIA2_IRQHandler(){
/*
    if(EUSCI_A2->IFG & BIT0) {
        //addItemCircBuf(TXBuf, EUSCI_A2->RXBUF);
        addItemCircBuf(RXBuf, EUSCI_A2->RXBUF);
        //if(TXBuf->num_items == 1){
          //  EUSCI_A2->IFG |= BIT1;
        //}
        addItemCircBuf(RXBuf, EUSCI_A2->RXBUF);
    }
    if (EUSCI_A2->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
            EUSCI_A2->IFG &= ~BIT1;
            return;
        }
        UART_send_byte(removeItem(TXBuf));
    }*/
    //echo

    if(EUSCI_A2->IFG & BIT0) {
        P1->OUT |= BIT0;
        addItemCircBuf(RXBuf,EUSCI_A2->RXBUF);
    }
    if (EUSCI_A2->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
            EUSCI_A2->IFG &= ~BIT1;
            return;
        }
        //UART_send_bytez(removeItem(TESTtxBuf));
        UART_send_byte(removeItem(TXBuf));
    }
}
