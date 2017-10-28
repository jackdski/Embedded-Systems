/*
 * uart.h
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */

#ifndef UART_H_
#define UART_H_

#include "msp.h"

void configure_UART();
void UART_send_byte(uint8_t data);
void EUSCIA0_IRQHandler();
void transmitData();


#endif /* UART_H_ */
