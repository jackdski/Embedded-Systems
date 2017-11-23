/*
*
* Wireless Bike Lock
* bluetooth.h
*
* 11-22-17
*
*/

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

void configure_BLUE_UART();
void EUSCIA3_IRQHandler();
void BLUART_send_byte(uint8_t data);

#endif
