#include "msp.h"
#include <stdint.h>
#include "timer.h"
#include "port.h"

void main(void)
{
  unsigned int i;
  WDT_A->CTL = (WDT_A_CTL_PW | WDT_A_CTL_HOLD);

  GPIO_configure();
  timer_a0_config();

  __enable_irq();

    while (1){
        P1->OUT ^= BIT0;
        for(i = 0; i<200000; i++);
    }
}


