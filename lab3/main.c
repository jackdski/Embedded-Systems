
#include "msp.h"
#include "uart.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	configure_clocks();
	configure_serial_port();

	uint8_t data = 0xAA;
	uint8_t ptr[] = {0xAA,0xBB};
		//UART_send_byte(data);


	while(1){
	    UART_send_n(ptr, 1);
	}
}
