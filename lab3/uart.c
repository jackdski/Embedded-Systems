
/*
 * uart.c
 *
 *  Created on: Sep 26, 2017
 *      Author: amabo
 */
#include "uart.h"

void configure_serial_port(){
    //Configure UART pins, set 2-UART pins to UART mode
    P1->SEL0 |=  (BIT2 | BIT3);
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A0->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A0->BRW = 78;                         //Set Baud Rate
    EUSCI_A0->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    //EUSCI_A0->UCA0IE |= (BIT0 | BIT1)  //Turn on interrupts for RX and TX
    //NVIC_EnableIRQ(EUSCIA0_IRQn);
}

void configure_clocks(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.

}

void UART_send_n(uint8_t * data, uint32_t length){
    //Code to iterate through the transmit data
    if(!data)
        return;
    volatile uint32_t n;

    for(n = 0; n<length; n++){
        UART_send_byte(data[n]);
    }
}
void UART_send_byte(uint8_t data){
    //while(~(EUSCI_A0->IFG & BIT1));  //While it hasn't finished transmitting
    EUSCI_A0->TXBUF = data;
}

extern void EUSCIA0_IRQHandler(){
    if (EUSCI_A0->IFG & BIT0){
        //Recieve Stuff
    }
    if (EUSCI_A0->IFG & BIT1){
        //Transmit Stuff
    }
}
