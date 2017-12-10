/*
*
* Wireless Bike Lock - Station
* Checkout.h
*
* 12-9-17
*
*/

#include "msp.h"

#ifndef CHECKOUT_H_
#define CHECKOUT_H_

// make the systick timer start counting
void configSystick();

// calculate the amount of ticks needed for the time to be checked out
uint32_t checkoutTimerTicks(uint8_t hours, uint8_t mins);

#endif
