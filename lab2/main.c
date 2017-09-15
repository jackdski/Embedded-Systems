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

void main(void)
{
  volatile unsigned int i;
  WDT_A->CTL = (WDT_A_CTL_PW | WDT_A_CTL_HOLD);

  GPIO_configure();
  timer_a0_config();

  __enable_irq();

  //This is the pin set required to test the lattency of the interrupts

  STCSR |= BIT0;        // Enable SysTick

  P1->OUT |= BIT7;      // Turn P1.7 high
  volatile uint32_t startSysclock = *(uint32_t *)(0xE000E018);  //STCVR starting value
  P1->IFG |= BIT1;      // Manually create flag


  while (1){
      P1->OUT ^= BIT0;
      for(i = 0; i<200000; i++);
  }

}

