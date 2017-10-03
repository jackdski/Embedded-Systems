#include "msp.h"
#include "uart.h"
#include "circbuf.h"

CircBuf_t * TXBuf;
CircBuf_t * RXBuf;
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	TXBuf = createCircBuf(256);
	RXBuf = createCircBuf(256);

	volatile uint8_t i;

#ifdef TESTCIRCBUF
	for(i=1; i<7; i++){
	    addItemCircBuf(TXBuf, i);
	}
	uint8_t pop = removeItem(TXBuf);
	addItemCircBuf(TXBuf, 10);
#endif

	configure_clocks();
	configure_serial_port();

#ifdef  UART_TESTS
	uint8_t ptr[] = { 0xAA,'J'};
	uint8_t ptr2[] = "ABC";
#endif

#ifdef  ZOIDBERG
	uint8_t sentence[] = "Why not Zoidberg?";
	UART_send_n(sentence, 18);
#endif

#ifdef   INTERRUPT_TEST
	for(i =0; i < 18; i++){
	    addItemCircBuf(TXBuf, sentence[i]);
	}
	UART_send_byte(removeItem(TXBuf));
#endif

	while(1){
	  //  UART_send_n(ptr, 1);
	}
}
