/*
*
* Wireless Bike Lock - Lock
* time.h
*
* 11-25-17
*
*/
#include "msp.h"

#ifndef TIME_H_
#define TIME_H_

// make the systick timer start counting
void startSystick();

// get hours and mins from station -- needs work!
void getTimeBluetooth();

// calculate the amount of ticks needed for the time to be checked out
uint32_t checkoutTimerTicks(uint8_t hours, uint8_t mins);

#endif
