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

//#define PROB3A
//#define PROB3B
//#define PROB4
//#define PROB10
//#define ESCOOT

volatile uint16_t sysclock;
volatile uint32_t beamBreaks = 0;
volatile float distanceTraveled = 0;
int part_twelve = 0;

void main(void)
{
  volatile unsigned int i;
  WDT_A->CTL = (WDT_A_CTL_PW | WDT_A_CTL_HOLD);

  GPIO_configure();
  timer_a0_config();

  __enable_irq();

#ifdef ESCOOT || PROB3B
  // SysTick Config
  SysTick->LOAD = 0X00FFFFFF;   // Give SysTick a starting value to count down from
  SysTick->CTRL = BIT0 | BIT2;  // Enable SysTick
#endif

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

#ifdef ESCOOT
  part_twelve = SysTick->VAL;
  //P1->IFG |= BIT5;
  part_twelve -= SysTick->VAL;
#endif

  while (1){
#ifdef ESCOOT
      distanceTraveled = beamBreaks * 0.9107;
#endif
      //P1->OUT ^= BIT0;
#ifdef ESCOOT
      P1->IFG |= BIT5;
#endif
      //for(i = 0; i<200000; i++);

  }

}

