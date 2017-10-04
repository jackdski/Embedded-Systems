#include "msp.h"
#include "uart.h"
#include "circbuf.h"
#include "processing.h"

CircBuf_t * TXBuf;
CircBuf_t * RXBuf;

uint8_t work = 0; //Flag bit. Is one if I want to process info
uint8_t transmit = 0; //Flag bit. is one if I want to send the RXBuf

uint8_t alp = 0; //Number of alphabetical chars
uint8_t pun = 0; //Number of punctuation chars
uint8_t num = 0; //Number of numerical chars
uint8_t whi = 0; //Number of white chars
uint8_t ran = 0; //Number of random chars


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	TXBuf = createCircBuf(10);
	RXBuf = createCircBuf(10);

	volatile uint8_t i;

#ifdef TESTCIRCBUF
	for(i=1; i<7; i++){
	    addItemCircBuf(TXBuf, i);
	}
	uint8_t pop = removeItem(TXBuf);
	addItemCircBuf(TXBuf, 10);
#endif
	__enable_irq();
	configure_clocks();
	configure_serial_port();
	configurePorts();


#ifdef  UART_TESTS
	uint8_t ptr[] = { 0xAA,'J'};
#endif

#ifdef  BLOCKING
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
#ifdef  UART_TESTS
	  UART_send_n(ptr, 1);
#endif
	  if(work){
	      analyzeBuf();
	      //EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
	      work = 0;
	  }
	  if(transmit){
	      transmitRX();
	      transmit = 0;
	  }
	}
}
