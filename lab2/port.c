/*
 * port.c
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo
 */
#include "port.h"

void PORT1_IRQHandler(){
    int8_t flag1 = (P1->IFG & BIT1) >> 1;
    int8_t flag2 = (P1->IFG & BIT4) >> 4;

    if(flag1){
        P1->OUT ^= BIT0;
    }
    if(flag2){
        P1->OUT ^= BIT0;
    }

    P1->IFG &= CLEAR_FLAGS;

}

// Configure the GPIO
void GPIO_configure(void) {

    /*
  //data from encoder
  P1->SEL0 = ???;
  P1->SEL1 = ???;
  P1->DIR = ???;
  P1->REN = ???;
  P1->OUT = ???;
  P1->IFG = ???;
  P1->IES = ???;
  P1->IE = ???;*/

  /* Left button configure */
  P1->SEL0 &= ~(BIT1);
  P1->SEL1 &= ~(BIT1);
  P1->DIR &=  ~(BIT1);
  P1->REN |=    BIT1;
  P1->OUT |=    BIT1;
  P1->IFG &=  ~(BIT1);
  P1->IES |=    BIT1;
  P1->IE  |=    BIT1;

  /* Right button configure*/
  P1->SEL0 &= ~(BIT4);
  P1->SEL1 &= ~(BIT4);
  P1->DIR &=  ~(BIT4);
  P1->REN |=    BIT4;
  P1->OUT |=    BIT4;
  P1->IFG &=  ~(BIT4);
  P1->IES |=    BIT4;
  P1->IE |=     BIT4;


  /* Configure Latency Test Output Pin*/
  P1->SEL0 &=  ~(BIT0);
  P1->SEL1 &=  ~(BIT0);
  P1->DIR |=     BIT0;
  P1->OUT &=   ~(BIT0);

  /* Enable Interrupts in the NVIC */
  NVIC_EnableIRQ(PORT1_IRQn);
}
