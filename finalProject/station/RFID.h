/*
 * RFID.h
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */

#ifndef RFID_H_
#define RFID_H_
#include "msp.h"
#include <stdint.h>

void configure_RFID();
uint8_t compare_RFID(uint8_t RFIDA[16], uint8_t RFIDB[16]);



#endif /* RFID_H_ */
