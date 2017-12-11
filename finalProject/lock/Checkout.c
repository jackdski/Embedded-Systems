/*
*
* Wireless Bike Lock - Lock
* checkout.c
*
* 11-26-17
*

#include <Checkout.h>
#include "msp.h"

#include <stdint.h>

extern uint32_t systickCounter;
extern uint32_t checkoutTimerTicksVal;
extern uint32_t overtime;
extern uint8_t hours;
extern uint8_t mins;

void startSystick() {
    /
     * 500,000us == 0.5s
     * f(t) = (t/3) *from Lab 2 write-up*
     *  500,000 = (t/3)
     *  t = 1,500,000


    // starting value to count down from for 0.5s ticks
    SysTick->LOAD = 1500000;
    // Enable SysTick counter, interrupt, clock
    SysTick->CTRL = (BIT0 | BIT1 | BIT2);
}

void SysTick_Handler() {
    systickCounter++;
    if(systickCounter == checkoutTimerTicksVal) {
        // Buzzer, etc.
    }
    // report this back to the station to be charged for overtime?
    if(systickCounter > checkoutTimerTicksVal) {
        overtime++;
    }

}

void getTimeBluetooth() {
    return;
}

uint32_t checkoutTimerTicks(uint8_t hours, uint8_t mins) {
    uint16_t ticks = mins;
    uint16_t hoursToMins = hours * 60; // convert hours to minutes
    ticks += hoursToMins; // add the hours time in minutes
    ticks *= 60; // convert minutes to seconds
    ticks *= 2; // measuring in 1/2 seconds
    return ticks;
}*/
