/*
 * processing.c
 *
 *  Created on: Oct 3, 2017
 *      Author: amabo
 */
#include "processing.h"

void configurePorts(){
    P1->SEL0 &= ~(BIT1 | BIT4);
    P1->SEL1 &= ~(BIT1 | BIT4);
    P1->DIR  &= ~(BIT1 | BIT4);
    P1->REN  |=  (BIT1 | BIT4);
    P1->OUT  |=  (BIT1 | BIT4);
    P1->IES  |=  (BIT1 | BIT4);

    P1->IFG   = 0;
    P1->IE   |=  (BIT1 | BIT4);
    NVIC_EnableIRQ(PORT1_IRQn);
}

void PORT1_IRQHandler(){


}

