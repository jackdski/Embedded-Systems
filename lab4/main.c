#include "msp.h"
#include "core_cm4.h"
#include "adc.h"
#include "lab4.h"

#define SCB_SCR_ENABLE_SLEEPONEXIT (0X00000002)

/**
 * main.c
 */

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	configure_ADC();
	__enable_interrupt();
	SCB->SCR &= ~SCB_SCR_ENABLE_SLEEPONEXIT; //wake up on exit from ISR

	int16_t temp = 0;
	while(1) {
	    // get temp value in ADC14MEM0?
	    ADC14->CTL0 |= ADC14_CTL0_SC; //Sampling and conversion start
	    __sleep();          //blocks here until conversion finishes
	    __no_operation();   //no-operation, good for a breakpoint
	}

}

