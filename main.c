#include "msp.h"
#include "core_cm4.h"
#include "adc.h"
#include "lab4.h"

#define TEMP
#define SCB_SCR_ENABLE_SLEEPONEXIT (0X00000002)

//Holder global variables
//Temperature
volatile uint16_t TNADC = 0;

//joystick NADC vals
volatile uint16_t YNADC = 0;
volatile uint16_t XNADC = 0;

//Accelerometer NADC vals
volatile uint16_t VXNADC = 0;
volatile uint16_t VYNADC = 0;
volatile uint16_t VZNADC = 0;

//Boolean of our direction. 0 is forward 1 is backwards
volatile uint8_t  dir=0;

/**
 * main.c
 */
//Declaration of global buffers
CircBuf_t * TXBuf;
CircBuf_t * SENDBuf;

//Flags indicating we should transmit or we have a new temp
uint8_t transmit = 0;
uint8_t newTemp  = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //Create our Temperature buffer and our send buffer
    TXBuf = createCircBuf(60);
    SENDBuf = createCircBuf(360);

    //Call all of the config functions
    configure_serial_port();
    configure_clocks();
    configure_ADC();
    configure_eScooter();
    timer_a0_config();
    __enable_interrupt();

    SCB->SCR &= ~SCB_SCR_ENABLE_SLEEPONEXIT; //wake up on exit from ISR

      while(1) {

#ifdef TEMP
        //If we want to send data for problem 6, call its protocol
        if (transmit) {
            ADC14->CTL0 &= ~(ADC14_CTL0_ENC);

            transmit = 0;
            problemSix();
        }
        //If we have a new TNADC, store it in the TXBuf
        if(newTemp){
            newTemp = 0;
            addItemCircBuf(TXBuf, TNADC);
        }
#endif
#ifdef BOOSTER
        //Set the RGB LED to red if faced with high force in either direction
        if (VYNADC > 10000 || VYNADC < 6000){
            P2->OUT |= BIT0;
        }
        else{
            P2->OUT &= ~BIT0;
        }
#endif

    }
}

