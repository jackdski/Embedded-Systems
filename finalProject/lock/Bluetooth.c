/*
 * Wireless Bike Lock - Lock
 * Bluetooth.c
 *
 */

#include "Bluetooth.h"

#include "msp.h"
#include "Circbuf.h"

extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;

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

void sendByte(uint8_t data){
    EUSCI_A3->TXBUF = data;
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
        P1->OUT ^= BIT0;
        addItemCircBuf(RXBuf, EUSCI_A3->RXBUF);
    }
    if (EUSCI_A3->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)) {
            EUSCI_A3->IFG &= ~BIT1;
            return;
        }
        //send_byte(removeItem(TXBuf));
    }
}
