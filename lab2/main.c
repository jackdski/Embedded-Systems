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

volatile uint16_t sysclock;           // Variable we use to get a value to calculate latency 
volatile uint32_t beamBreaks = 0;     // Counts how many beam breaks there have been
volatile float distanceTraveled = 0;  // Where the calculated distance traveled by the scooter is stored
int part_twelve = 0;                  // Global variable used to store the difference in SysTick values

void main(void)
{
  volatile unsigned int i;
  WDT_A->CTL = (WDT_A_CTL_PW | WDT_A_CTL_HOLD);   // turn off watchdog

  GPIO_configure();   // Call function that configures all the buttons, pins, etc.
  timer_a0_config();  // Call function that configures Timer A0

  __enable_irq();

#ifdef ESCOOT || PROB3B
  // SysTick Config
  SysTick->LOAD = 0X00FFFFFF;   // Give SysTick a starting value to count down from
  SysTick->CTRL = BIT0 | BIT2;  // Enable SysTick
#endif

#ifdef PROB3A
  //This is the pin set required to test the lattency of the interrupts
  P1->OUT |= BIT7;
  P1->IFG |= BIT1;
#endif

#ifdef PROB3B
  sysclock = SysTick->VAL;  // STCVR starting value
  P1->OUT |= BIT7;          // Turn P1.7 high
  P1->IFG |= BIT1;          // manually create flag
#endif

/*
 *  Probelem 12.
 *  Used SysTick to get the latency, execution time, and return time of 
 *  the square wave input 
 */
#ifdef ESCOOT
  part_twelve = SysTick->VAL;
  //P1->IFG |= BIT5;            //Set high to trigger interrupt
  part_twelve -= SysTick->VAL;
#endif

  while (1){
#ifdef ESCOOT
      // Constantly update the distance travelled by checking if there is a new value for beamBreaks
      distanceTraveled = beamBreaks * 0.9107;
#endif
    //P1->OUT ^= BIT0;
#ifdef ESCOOT
    // Manually trigger the P1.5 interrupt
    P1->IFG |= BIT5;
#endif
    // For loop delay for the blinking LED, etc. 
    //for(i = 0; i<200000; i++);
  }
}

