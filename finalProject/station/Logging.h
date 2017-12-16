/*
 * Logging.h
 *
 *  Created on: Dec 10, 2017
 *      Author: amabo
 */

#ifndef LOGGING_H_
#define LOGGING_H_
#include "msp.h"
#include "CircBuf.h"

void configure_TerminalUART();

void itos(uint32_t num, int8_t size, uint8_t * str );

void newLine(CircBuf_t * Buf);

void sendLog();

void Terminal_send_byte(uint8_t data);

#endif /* LOGGING_H_ */
