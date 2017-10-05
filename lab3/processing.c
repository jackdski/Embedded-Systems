/*
 * processing.c
 *
 *  Created on: Oct 3, 2017
 *      Author: amabo
 */
#include "processing.h"
#include "circbuf.h"
#include "uart.h"

extern uint8_t transmit;
extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;

extern uint8_t alp; //Number of alphabetical chars
extern uint8_t pun; //Number of punctuation chars
extern uint8_t num; //Number of numerical chars
extern uint8_t whi; //Number of white chars
extern uint8_t ran; //Number of random chars


void configurePorts(){
    P1->SEL0 &= ~(BIT1 | BIT4);
    P1->SEL1 &= ~(BIT1 | BIT4);
    P1->DIR  &= ~(BIT1 | BIT4);
    P1->REN  |=  (BIT1 | BIT4);
    P1->OUT  |=  (BIT1 | BIT4);
    P1->IES  |=  (BIT1 | BIT4);

    P1->DIR |= BIT0;

    P1->IFG   = 0;
    P1->IE   |=  (BIT1 | BIT4);
    NVIC_EnableIRQ(PORT1_IRQn);
}

void PORT1_IRQHandler(){
    if(P1->IFG & BIT1){
        resetCircBuf(RXBuf);
    }
    else if(P1->IFG & BIT4){
        transmit = 1;
    }
    P1->OUT ^= BIT0;
    P1->IFG = 0;
}

void analyzeBuf(){
    if(isEmpty(RXBuf)){
        return;
    }
    volatile uint32_t length = RXBuf->num_items;
    volatile uint32_t i;
    uint8_t temp;

    for(i = 0; i < length; i++){
        temp = removeItem(RXBuf);
        addItemCircBuf(RXBuf, temp);
        analyzeChr(temp);
    }



    UART_send_byte(removeItem(TXBuf));
}


void analyzeChr(uint8_t chr){
    if (chr >= 'A' || chr <= 'Z') {
        alp++;
    }
    else if (chr >= 'a' || chr <= 'z') {
        alp++;
    }
    else if (chr == 33 || chr == 46 || chr == 63) {
        pun++;
    }
    else if (char >= 48 || chr <= 57) {
        num++;
    }

    else if (chr == 32 || chr == 9) {
        whi++;
    }
    else {
        ran++;
    }
}

/*
if(TXBuf->num_items == 1){
            EUSCI_A0->IFG |= BIT1;
        }
        */

