#include "msp.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	/*
	 * Joystick x and y pins:
	 *  x: J1.2     (P6.0)
	 *  y: J3.26    (P4.4)
	 *  sel: J1.5   (P4.1)
	 *
	 * Accelerometer x, y, z:
	 *  x: J3.23    (P6.1)
	 *  y: J3.24    (P4.0)
	 *  z: J3.25    (P4.2)
	 *
	 */


}
