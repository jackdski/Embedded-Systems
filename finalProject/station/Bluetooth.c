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

void configure_SystemClock(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.
}

//Connect 3.3 to the RX
void configure_Bluetooth(){
    //Configure UART pins, set 2-UART pins to UART mode
    P3->SEL0 |=  (BIT2 | BIT3);
    P3->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A2->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A2->BRW = 78;                          //Set Baud Rate
    EUSCI_A2->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A2->IFG &= ~(BIT0 | BIT1);
    UCA2IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA2_IRQn);
}

inline void sendByte(uint8_t data){
    EUSCI_A2->TXBUF = data;
    while(!(EUSCI_A2->IFG & BIT1));
}

void bluetooth_send_n(uint8_t * data, uint8_t length){
    //Code to iterate through the transmit data
    if(!data)
        return;
    volatile uint8_t n;
    for(n = 0; n<length; n++){
        sendByte(data[n]);
    }
}

void EUSCIA2_IRQHandler(){
    if (EUSCI_A2->IFG & BIT0){
        addItemCircBuf(RXBuf, EUSCI_A2->RXBUF);
    }
    if (EUSCI_A2->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)) {
            EUSCI_A2->IFG &= ~BIT1;
            return;
        }
        sendByte(removeItem(TXBuf));
    }
}
