/*
 * Wireless Bike Lock - Station
 * Bluetooth.h
 *
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "msp.h"
#include "Circbuf.h"

void configure_SystemClock();
void configure_Systick();
void configure_serial_port();
void configAltBT();
void UART_send_byte(uint8_t data);
void UART_send_bytez(uint8_t data);
void UART_send_n(uint8_t * data, uint8_t length);
void send_registration(Bike_t bikeList) {

#endif
