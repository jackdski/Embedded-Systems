/*
 * main.c
 *
 *  Created on: Sep 7, 2017
 *      Author: amabo and Jack
 */

#include "msp.h"
#include <stdint.h>
#include "timer.h"
#include "port.h"

volatile uint16_t sysclock;
volatile uint32_t beamBreaks = 0;
int part_twelve = 0;

void main(void)
{
  volatile unsigned int i;
  WDT_A->CTL = (WDT_A_CTL_PW | WDT_A_CTL_HOLD);

  GPIO_configure();
  timer_a0_config();

  __enable_irq();
  
  // SysTick Config
  SysTick->LOAD = 0X00FFFFFF;   // Give SysTick a starting value to count down from
  SysTick->CTRL = BIT0 | BIT2;  // Enable SysTick

#ifdef PROB3A
  //This is the pin set required to test the lattency of the interrupts
  //P1->OUT |= BIT7;
  //P1->IFG |= BIT1;
#endif
  
#ifdef PROB3B
  sysclock = SysTick->VAL;  // STCVR starting value
  P1->OUT |= BIT7;          // Turn P1.7 high
  P1->IFG |= BIT1;          // manually create flag
#endif

  part_twelve = SysTick->VAL;
  P1->IFG |= BIT5;
  part_twelve -= SysTick->VAL;
  

  while (1){
      //P1->OUT ^= BIT0;
      P1->IFG |= BIT5;
      //for(i = 0; i<200000; i++);
  }

}

