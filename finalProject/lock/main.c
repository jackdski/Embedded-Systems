#include "msp.h"
#include "Buzzer.h"
#include "LockButton.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	configure_Buzzer();
	configure_LockButton();

	P1->DIR |= BIT0;

	while(1){

	}
}
