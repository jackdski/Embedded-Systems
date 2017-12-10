/*
*
* Wireless Bike Lock - Station
* Checkout.c
*
* 12-9-17
*
*/

#include "msp.h"
#include <stdint.h>
#include "Checkout.h"

extern uint8_t timerFull;
extern uint32_t checkoutTicks;
//extern uint32_t overtime;
extern uint8_t hours;
extern uint8_t mins;

void configSystick() {
    /*
     * 500,000us == 0.5s
     * f(t) = (t/3) *from Lab 2 write-up*
     *  500,000 = (t/12)
     *  t = 6,000,000
     */

    // starting value to count down from for 0.5s ticks
    SysTick->LOAD = 6000000;
    // Enable SysTick counter, interrupt, clock
    SysTick->CTRL = (BIT0 | BIT1 | BIT2);
}

void SysTick_Handler() {
    checkoutTicks--;
    if(checkoutTicks == 0) {
        // Buzzer
        timerFull = 1;
        P1->OUT |= BIT0;
        P2->OUT |= BIT2;
    }
    /* report this back to the station to be charged for overtime?
    if(systickCounter > checkoutTimerTicksVal) {
        overtime++;
    }
    */

}

uint32_t checkoutTimerTicks(uint8_t hours, uint8_t mins) {
    uint16_t ticks = mins;
    uint16_t hoursToMins = hours * 60; // convert hours to minutes
    ticks += hoursToMins; // add the hours time in minutes
    ticks *= 60; // convert minutes to seconds
    ticks *= 2; // measuring in 1/2 seconds
    return ticks;
}
