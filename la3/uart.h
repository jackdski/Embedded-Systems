/*
 * uart.h
 *
 *  Created on: Sep 26, 2017
 *      Author: amabo
 */
#ifndef UART_H_
#define UART_H_
#include "msp.h"

void UART_send_n(unint8_t * data, uint32_t length);
void UART_send_byte(unint8_t data);

#endif
