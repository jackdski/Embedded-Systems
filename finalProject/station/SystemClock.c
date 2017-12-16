/*
 * SystemClock.c
 *
 *  Created on: Nov 27, 2017
 *      Author: amabo
 */
#include "SystemClock.h"

//In order to set our 9600 BAUD Rate for UART, we run the System clock at 12MHz. This function 
//configures the system clock to run at that speed
void configure_SystemClock(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.
}
