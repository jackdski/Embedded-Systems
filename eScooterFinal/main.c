

#include "msp.h"
#include "uart.h"
#include "circbuf.h"
#include "accelerometer.h"
#include "time.h"
#include "beamBreaks.h"

uint8_t beamBreaks = 0;
uint16_t VXNADC = 0;
uint16_t VYNADC = 0;
uint16_t VZNADC = 0;
//uint16_t accelNADC = 0;

uint16_t tempNADC = 0;
uint16_t lightNADC = 0;



uint8_t transmit = 0;

CircBuf_t * TXBuf;
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	TXBuf = createCircBuf(100);
	if(!TXBuf)
	    return;

	configure_Systick();
	configure_clocks();
	configure_ADC();
	configure_UART();
	configure_beamBreaks();

	while(1){

	}
}
