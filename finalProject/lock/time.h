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

void startSystick();
void getTimeBluetooth();
uint32_t checkoutTimerTicks(uint8_t hours, uint8_t mins);

#endif
