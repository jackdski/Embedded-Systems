/*
*
* Wireless Bike Lock
* main.c
*
* 11-12-17
*
*/
#include "msp.h"

#include "bluetooth.h"
#include "buttons.h"
#include "lcd.h"
#include "register.h"

void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    // configs
    configButtons();
    configLED();
    configure_BLUE_UART();

    P1->SEL0 &= ~(BIT0);
    P1->SEL1 &= ~(BIT0);
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;

    while(1) {
        P1->OUT ^= BIT0;
        volatile int i;
        for (i = 0; i < 30000; i++);
    }
}
