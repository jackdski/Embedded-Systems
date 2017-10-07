/*
 * uart.h
 *
 *  Created on: Sep 26, 2017
 *      Author: amabo
 */
#ifndef UART_H_
#define UART_H_
#include "msp.h"
#include <stdint.h>

void configure_serial_port();
void configure_clocks();
void UART_send_n(uint8_t * data, uint8_t length);
void UART_send_byte(uint8_t data);
void transmitRX();

#endif
 
