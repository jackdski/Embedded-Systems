/*
 * uart.c
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */

#include "uart.h"
#include "circbuf.h"
#include "conversions.h"

extern CircBuf_t * TXBuf;
extern uint8_t transmit;

void configure_UART(){
    //Configure UART pins, set 2-UART pins to UART mode
    P1->SEL0 |=  (BIT2 | BIT3);
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A0->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A0->BRW = 78;                         //Set Baud Rate
    EUSCI_A0->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    //Configure the port pins that will control transmission
    P1->SEL0 &= ~(BIT1 | BIT4);      // Set Port Pin Selection to General IO Mode
    P1->SEL1 &= ~(BIT1 | BIT4);      // Make sure not to use tertiary function of P1.1
    P1->DIR &=  ~(BIT1 | BIT4);      // Set P1.1 Direction to Input
    P1->REN |=   (BIT1 | BIT4);       // Enable Pullup/Pulldown
    P1->OUT |=   (BIT1 | BIT4);       // Enable PULLUP
    P1->IES =    (BIT1 | BIT4);       // Set P1 IFG flag to high to low transition


    EUSCI_A0->IFG &= ~(BIT1 | BIT0);
    UCA0IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA0_IRQn);

    P1->IFG = 0;
    P1->IE |= (BIT1 | BIT4);
    NVIC_EnableIRQ(PORT1_IRQn);
}

void UART_send_byte(uint8_t data){
    EUSCI_A0->TXBUF = data;
}

void EUSCIA0_IRQHandler(){
    if (EUSCI_A0->IFG & BIT0){
        uint8_t data = EUSCI_A0->RXBUF;
        P1->OUT ^= BIT0;
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
void PORT1_IRQHandler(){
    if(P1->IFG & (BIT1 | BIT4)){
        transmit = 1;
    }
    P1->IFG = 0;
}

void SEND_DATA(float totalDistance, float spd,uint8_t direction){
    uint8_t distanceS[7];
    uint8_t speedS[7];

    ftoa(totalDistance,2,7,distanceS);
    ftoa(spd,2,7,speedS);

    loadToBuf(TXBuf, "You have traveled ", 18);
    loadToBuf(TXBuf, distanceS, 7);
    loadToBuf(TXBuf, "cm", 2);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf, "Your speed is ", 14);
    loadToBuf(TXBuf, speedS, 7);
    loadToBuf(TXBuf, "cm/s", 2);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf, "You're Traveling ", 17);
    if(direction){
        loadToBuf(TXBuf, "backwards", 9);
    }
    else{
        loadToBuf(TXBuf, "forwards", 8);
    }
    addItemCircBuf(TXBuf, 0x0D);

    EUSCI_A0->IFG |= BIT1;
    while(!isEmpty(TXBuf));

    NVIC_EnableIRQ(PORT1_IRQn);
    NVIC_EnableIRQ(ADC14_IRQn); // Enable ADC interrupt in NVIC module
    NVIC_EnableIRQ(PORT3_IRQn);
}

