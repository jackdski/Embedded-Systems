/*
 * Wireless Bike Lock - Lock
 * Bluetooth.c
 *
 */

#include "msp.h"
#include <stdint.h>
#include "Bluetooth.h"
#include "Circbuf.h"

extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;
extern CircBuf_t * RFIDBuf;

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
}

inline void sendByte(uint8_t data){
    EUSCI_A3->TXBUF = data;
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

void EUSCIA3_IRQHandler(){
    if (EUSCI_A3->IFG & BIT0){
        //P1->OUT ^= BIT0;
        addItemCircBuf(RXBuf, EUSCI_A3->RXBUF);
    }
    if (EUSCI_A3->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)) {
            EUSCI_A3->IFG &= ~BIT1;
            return;
        }
        sendByte(removeItem(TXBuf));
    }
}
