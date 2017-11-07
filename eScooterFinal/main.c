

#include "msp.h"
#include "uart.h"
#include "circbuf.h"
#include "accelerometer.h"
#include "time.h"
#include "beamBreaks.h"
#include "bluetooth.h"
#include "calculations.h"
#include "port.h"

volatile uint8_t beamBreaks = 0;
volatile uint16_t VYNADC = 0;
//uint16_t accelNADC = 0;

volatile uint16_t tempNADC = 0;
volatile uint16_t lightNADC = 0;

volatile float totalDistance = 0;
volatile uint8_t direction = 0;
volatile float spd = 0;
//volatile float speed = 0;

uint8_t measure = 0;
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

    configure_ADC();
	configure_clocks();
	configure_UART();
	configure_beamBreaks();
	configure_Systick();
	configure_ports();

	configure_BLUE_UART();
	__enable_irq();

	P1->DIR |= BIT0;

	P2->DIR |= BIT0 | BIT1;

	while(1){
	    if(measure){
	        measure = 0;
	        //P1->OUT ^= BIT0;
	        //Update distance and calculate speed
	        spd = speed();
	        direction = calculateDirection(spd);
	    }

	    if(transmit){
	        NVIC_DisableIRQ(PORT1_IRQn);
	        NVIC_DisableIRQ(ADC14_IRQn); // Enable ADC interrupt in NVIC module
	        NVIC_DisableIRQ(PORT3_IRQn);
            //SEND_DATA(totalDistance,spd,direction);
            BLUEART_SEND_DATA(totalDistance,spd,direction);
            transmit = 0;
	    }

	    if(VYNADC > 9800){
	        P2->OUT &= ~(BIT1);
	        P2->OUT |=   BIT0;
	    }
	    else if(VYNADC < 7800){
	        P2->OUT &= ~(BIT0);
	        P2->OUT |=   BIT1;
	    }
	    else{
	        P2->OUT &= ~(BIT0 | BIT1);
	    }

	   /* if(beamBreaks == 10){
	        BLUART_send_byte('A');
	        UART_send_byte('A');
	        beamBreaks = 0;
	    }*/
	    if(direction){
	        P1->OUT |=  BIT0;
	    }
	    else{
	        P1->OUT &= ~BIT0;
	    }

	}
}
