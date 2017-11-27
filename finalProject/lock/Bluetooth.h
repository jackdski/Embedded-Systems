/*
 * Bluetooth.h
 *
 *  Created on: Nov 26, 2017
 *      Author: amabo
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_
#include "msp.h"
#include <stdint.h>

void configure_Bluetooth();
void sendByte(uint8_t data);

#endif /* BLUETOOTH_H_ */
