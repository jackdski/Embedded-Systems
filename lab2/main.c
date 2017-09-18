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

#if 0
  //This is the pin set required to test the lattency of the interrupts
  //P1->OUT |= BIT7;
  //P1->IFG |= BIT1;
#endif


  while (1){
      //P1->OUT ^= BIT0;
      //for(i = 0; i<200000; i++);
  }

}

