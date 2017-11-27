/*
*
* Wireless Bike Lock - Lock
* main.c
*
* 11-26-17
*
*/

#include "msp.h"
#include "Buzzer.h"
#include "LockButton.h"
#include "time.h"
#include "bluetooth.h"
#include <stdint.h>

uint32_t systickCounter = 0;
uint32_t checkoutTimerTicksVal = 0;
uint32_t overtime = 0;
uint8_t hours = 0;
uint8_t mins = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    checkoutTimerTickVal = checkoutTimerTicks(hours, mins); // calculate time to count to

    // configs
    configure_Buzzer();
    configure_LockButton();
    configSystick();

    // if time has been set use this
    //startSysTick();


    P1->DIR |= BIT0;

    while(1){

    }
}
