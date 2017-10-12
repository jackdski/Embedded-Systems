#include "msp.h"
#include "adc.c"

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

	while(1) {
	    ADC14->CTL0 |= ; //Sampling and conversion start
	    __sleep();          //blocks here until conversion finishes
	    __no_operation();   //no-operation, good for a breakpoint
	}

}

