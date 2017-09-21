
/*
 * port.c
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo and Jack
 */
#include "port.h"

extern int beamBreaks;
extern int part_twelve;
extern int latency_twelve;
extern int execution;

/* Handles interrupts from P1 */
void PORT1_IRQHandler(){
#ifdef PROB3b
  sysclock -= SysTick->VAL;
#endif

#ifdef PROB12
    int latency_twelve = 0;
    int execution = 0;
#endif

    //When testing latency into the function, this will turn off the pin
    if(P1->IFG & (BIT1 | BIT4)){
        P1->IE &= ~(BIT1 | BIT4);
        NVIC_EnableIRQ(TA0_0_IRQn);
    }

    // Prob. 11 P1.5 IFG
#ifdef PROB12
    if (P1->IFG & BIT5) {
        latency_twelve = part_twelve - SysTick->VAL;
        beamBreaks++;
        P1->OUT ^= BIT0;
    }
#endif

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



    //NVIC_EnableIRQ(PORT1_IRQn);
    //When testing out of the function, this will turn on the pin
    //P1->OUT |= BIT7;
#ifdef PROB12
    execution = part_twelve - SysTick->VAL;
    P1->IFG &= CLEAR_FLAGS;
#endif
}

// Configure the GPIO pins
void GPIO_configure(void) {

  //data from encoder (port 1.5)
  P1->SEL0 &= ~(BIT5);      // set to General IO Mode
  P1->SEL1 &= ~(BIT5);      // Make sure not to be in tertiary function
  P1->DIR &= ~(BIT5);       // set direction to input
  P1->REN |= BIT5;          // enable pullup
  P1->OUT |= BIT5;          // clear interrupts
  P1->IFG &= ~(BIT5);       // clear interrupts
  P1->IES = BIT5;           // set IFT flag to high to low transition
  P1->IE = BIT5;            // Enable port interrupt

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
