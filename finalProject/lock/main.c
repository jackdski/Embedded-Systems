#include "msp.h"
#include "Buzzer.h"
#include "LockButton.h"
#include "Bluetooth.h"
#include "SystemClock.h"
#include "RGB.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	configure_SystemClock();
	configure_Buzzer();
	configure_LockButton();
	configure_Bluetooth();
	configure_RGB();

	P1->DIR |= BIT0;
	P5->OUT |= BIT5;
	P6->DIR |= BIT0 | BIT1;
	P6->OUT |= (BIT0 | BIT4 | BIT5);

	while(1){

	}
}
