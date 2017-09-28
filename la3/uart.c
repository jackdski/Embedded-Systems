/*
 * uart.c
 *
 *  Created on: Sep 26, 2017
 *      Author: amabo
 */
#include "uart.h"

void configure_serial_port(){
    P1->SEL0 |=  (BIT2 | BIT3);
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_PEN | EUSCI_A_CTLW0_MSB | 
                         EUSCI_A_CTLW0_SEVENBIT | EUSCI_A_CTLW0_SPB);
   // EUSCI_A0->CTLW0 |= 
    EUSCI_A0->UCA0BRW = 0;
    EUSCI_A0->MCTLW |= 0;
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);
}

void UART_send_n(unint8_t * data, uint32_t length){
    
}
void UART_send_byte(unint8_t data){
    
}
