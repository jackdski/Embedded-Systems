
/*
 * port.c
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo and Jack
 */
#include "port.h"

/* Handles interrupts from P1 */
void PORT1_IRQHandler(){
    //When testing latency into the function, this will turn off the pin
    if(P1->IFG & (BIT1 | BIT4)){
        P1->IE &= ~(BIT1 | BIT4);
        NVIC_EnableIRQ(TA0_0_IRQn);
    }



#if 0
    //These if statements toggle pin one, tick through the RGB LED and turn
    //off bit seven. There are used for all pre-timer parts of the lab.
    if((P1->IFG & BIT1) || (P1->IFG & BIT4)){
        P1->OUT ^= BIT0;
        P1->OUT &= ~(BIT7);
    }

    int8_t RGBMask = P2->OUT & (BIT0 | BIT1 | BIT2);

    // Left button is pressed
    if((P1->IFG & BIT1) >> 1){
        if(RGBMask == 7){
            P2->OUT &= ~(BIT0 | BIT1 | BIT2);
        }
        else {
            P2->OUT += 1;
        }
    }

    // Right button is pressed
    if((P1->IFG & BIT4) >> 4){
        if(RGBMask == 0){
                    P2->OUT |= (BIT0 | BIT1 | BIT2);
                }
                else
                    P2->OUT -= 1;
    }

#endif


    //P1->IFG &= CLEAR_FLAGS;

    //NVIC_EnableIRQ(PORT1_IRQn);
    //When testing out of the function, this will turn on the pin
    //P1->OUT |= BIT7;
}

// Configure the GPIO pins
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
  P1->SEL0 &= ~(BIT1);      // Set Port Pin Selection to General IO Mode
  P1->SEL1 &= ~(BIT1);      // Make sure not to use tertiary function of P1.1
  P1->DIR &=  ~(BIT1);      // Set P1.1 Direction to Input
  P1->REN |=    BIT1;       // Enable Pullup/Pulldown
  P1->OUT |=    BIT1;       // Enable PULLUP
  P1->IFG &=  ~(BIT1);      // Clear interrupts
  P1->IES =    BIT1;       // Set P1 IFG flag to high to low transition

  /* Right button configure*/
  P1->SEL0 &= ~(BIT4);      // Set P1.4 to General IO Mode
  P1->SEL1 &= ~(BIT4);      // Make sure not to use tertiary function of P1.4
  P1->DIR &=  ~(BIT4);      // Set P1.4 Direction to input
  P1->REN |=    BIT4;       // Enable pullup/pulldown
  P1->OUT |=    BIT4;       // Enable PULLUP
  P1->IFG &=  ~(BIT4);      // Clear interrupts
  P1->IES |=    BIT4;       // Set P1 IFG flag to high-to-low transition


  /* Configure Latency Test Output Pin (P1.7) */
  P1->SEL0 &=  ~(BIT0 | BIT7);     //
  P1->SEL1 &=  ~(BIT0 | BIT7);
  P1->DIR |=    (BIT0 | BIT7);
  P1->OUT &=   ~(BIT0 | BIT7);

  /* Configure Port 2 to be used for RGB LED*/
  P2->SEL0 &=  ~(BIT0 | BIT1 | BIT2);
  P2->SEL1 &=  ~(BIT0 | BIT1 | BIT2);
  P2->DIR |=    (BIT0 | BIT1 | BIT2);
  P2->OUT &=   ~(BIT0 | BIT1 | BIT2);


  P1->IFG = 0;
  P1->IE =  (BIT1 | BIT4);       // Enable port interrupt
  /* Enable Interrupts in the NVIC */
  NVIC_EnableIRQ(PORT1_IRQn);
  //NVIC->ISER[1] |= BIT3;
}
