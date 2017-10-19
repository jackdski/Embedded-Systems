/*
 *
 * adc.h
 * Author: Jack Danielski and Avery Anderson
 * 10-12-17
 *
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include "msp.h"
#include "adc_circbuf.h"

void configure_ADC();
void configure_serial_port();
void configure_clocks();
void UART_send_n(uint8_t * data, uint8_t length);
void UART_send_byte(uint8_t data);
void printTemps();
void problemSix();

#endif
