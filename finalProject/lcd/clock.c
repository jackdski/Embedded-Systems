/*
 * clock.c
 *
 *  Created on: Dec 7, 2017
 *      Author: Stefan
 */
#include "msp.h"
#include "clock.h"

void configure_SystemClock(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.
}

void Delay0(void)
{
    volatile int i;
    for (i=0;i < 100 ; i ++);
}
void Delay1(void)
{
    volatile int i;
    for (i=0;i < 1700000 ; i ++);
}
void Delay2(void)
{
    volatile int i;
    for (i=0;i < 3400000 ; i ++);
}
void Delay3(void)
{
    volatile int i;
    for (i=0;i < 5100000 ; i ++);
}
void Delay4(void)
{
    volatile int i;
    for (i=0;i < 6800000 ; i ++);
}
void Delay5(void)
{
    volatile int i;
    for (i=0;i < 8500000 ; i ++);
}



