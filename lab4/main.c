#include "msp.h"
#include "core_cm4.h"
#include "adc.h"
#include "lab4.h"

#define TEMP
#define SCB_SCR_ENABLE_SLEEPONEXIT (0X00000002)


volatile uint16_t TNADC = 0;
volatile uint16_t YNADC = 0;
volatile uint16_t XNADC = 0;
volatile uint16_t VXNADC = 0;
volatile uint16_t VYNADC = 0;
volatile uint16_t VZNADC = 0;
volatile uint8_t  dir=0;

/**
 * main.c
 */
CircBuf_t * TXBuf;
CircBuf_t * SENDBuf;
uint8_t transmit = 0;
uint8_t newTemp  = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    TXBuf = createCircBuf(60);
    SENDBuf = createCircBuf(360);

    configure_serial_port();
    configure_clocks();
    configure_ADC();
    configure_eScooter();

    timer_a0_config();
    __enable_interrupt();
    SCB->SCR &= ~SCB_SCR_ENABLE_SLEEPONEXIT; //wake up on exit from ISR

      while(1) {
        // get temp value in ADC14MEM0?

#ifdef TEMP
        if (transmit) {
            ADC14->CTL0 &= ~(ADC14_CTL0_ENC);

            transmit = 0;
            problemSix();
        }
        if(newTemp){
            newTemp = 0;
            addItemCircBuf(TXBuf, TNADC);
        }
#endif
        if (VYNADC > 10000 || VYNADC < 6000){
            P2->OUT |= BIT0;
        }
        else{
            P2->OUT &= ~BIT0;
        }
    }

}

