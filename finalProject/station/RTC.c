/*
 * RTC.c
 *
 *  Created on: Dec 9, 2017
 *      Author: amabo
 */
#include "RTC.h"
#include "Bike.h"

extern Bike_t * bikeList;

void configure_RTC(){

    //Set the RTC to have event interrupts, also set the key
    RTC_C->CTL0   = RTC_C_KEY | RTC_C_CTL0_TEVIE;
    RTC_C->CTL13 &= ~RTC_C_CTL13_HOLD;

    //Enable the interrupt at the NVIC
    NVIC_EnableIRQ(RTC_C_IRQn);
}

void RTC_C_IRQHandler(){
    //RTC_C_CTL0_TEVIFG The event pin that will fire every minute
    Bike_t * runner = bikeList;
    while(runner){
        runner->bikeCounter++;

        runner = runner->nextBike;
    }
    P1->OUT ^= BIT0;
    RTC_C->CTL0 = RTC_C_KEY;
    RTC_C->CTL0 = RTC_C_KEY | RTC_C_CTL0_TEVIE;

}
