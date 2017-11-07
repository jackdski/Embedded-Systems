/*
 * bluetooth.c
 *
 *  Created on: Oct 29, 2017
 *      Author: amabo
 */
#include "bluetooth.h"
#include "circbuf.h"
#include "conversions.h"
#include <stdint.h>

extern CircBuf_t * TXBuf;

void configure_BLUE_UART(){
    //Configure UART pins, set 2-UART pins to UART mode
    P9->SEL0 |=  (BIT6 | BIT7);
    P9->SEL1 &= ~(BIT6 | BIT7);

    EUSCI_A3->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A3->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A3->BRW = 6;                          //Set Baud Rate
    EUSCI_A3->MCTLW |= (BIT0 | BIT7);           //Set modulator bits
    EUSCI_A3->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A3->IFG &= ~(BIT1 | BIT0);
    UCA3IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA3_IRQn);
}

void EUSCIA3_IRQHandler(){
    if (EUSCI_A3->IFG & BIT0){
        //BLUART_send_byte(EUSCI_A3->RXBUF);
        uint8_t data = EUSCI_A3->RXBUF;
        //P1->OUT ^= BIT0;
    }

    if (EUSCI_A3->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
            EUSCI_A3->IFG &= ~BIT1;
              return;
        }
        BLUART_send_byte(removeItem(TXBuf));
    }

}

void BLUART_send_byte(uint8_t data){
//    while(!(EUSCI_A3->IFG & BIT1));  //While it hasn't finished transmitting
    EUSCI_A3->TXBUF = data;
}

void BLUEART_SEND_DATA(float totalDistance, float spd,uint8_t direction){
    uint8_t distanceS[7];
    uint8_t speedS[7];

    //ftoa(totalDistance,2,7,distanceS);
    //ftoa(spd,2,7,speedS);
    ftoa(totalDistance,distanceS,3);
    ftoa(spd,speedS,3);

    loadToBuf(TXBuf, "You have traveled ", 18);
    loadToBuf(TXBuf, distanceS, 7);
    loadToBuf(TXBuf, "cm", 2);
    addItemCircBuf(TXBuf, 0x0D);
    addItemCircBuf(TXBuf, 0x0A);


    loadToBuf(TXBuf, "Your speed is ", 14);
    loadToBuf(TXBuf, speedS, 7);
    loadToBuf(TXBuf, "cm/s", 4);
    addItemCircBuf(TXBuf, 0x0D);
    addItemCircBuf(TXBuf, 0x0A);


    if((uint8_t)spd == 0){
        loadToBuf(TXBuf, "You're Stopped", 15);
    }
    else{
        loadToBuf(TXBuf, "You're Traveling ", 17);
        if(direction){
            loadToBuf(TXBuf, "forwards", 8);
        }
        else{
            loadToBuf(TXBuf, "backwards", 9);
        }
    }
    addItemCircBuf(TXBuf, 0x0D);
    addItemCircBuf(TXBuf, 0x0A);


    EUSCI_A3->IFG |= BIT1;
    while(!isEmpty(TXBuf));

    NVIC_EnableIRQ(PORT1_IRQn);
    NVIC_EnableIRQ(ADC14_IRQn); // Enable ADC interrupt in NVIC module
    NVIC_EnableIRQ(PORT3_IRQn);
}


