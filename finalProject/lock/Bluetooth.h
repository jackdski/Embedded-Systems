/*
 * Wireless Bike Lock -Lock
 * Bluetooth.h
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_
#include "msp.h"
#include <stdint.h>

void configure_Bluetooth();

void sendByte(uint8_t data);

void bluetooth_send_n(uint8_t * data, uint8_t length);

#endif /* BLUETOOTH_H_ */
