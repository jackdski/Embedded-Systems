/*
 * Wireless Bike Lock - Station
 * Bluetooth.h
 *
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "msp.h"
#include "Circbuf.h"
#include "stdint.h"

void configure_Systick();

void configure_Bluetooth();

void BLUART_send_byte(uint8_t data);

uint8_t send_Heartbeat();
uint8_t send_RFID(uint8_t RFID[17]);
uint8_t send_Warning();

#endif
