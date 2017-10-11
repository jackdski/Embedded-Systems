/*
 * eScooter.c
 *
 *  Created on: Oct 7, 2017
 *      Author: amabo
 */
#include "eScooter.h"
#include "circbuf.h"
#include "conversions.h"
#include "uart.h"

es_V * myScooter;
extern CircBuf_t * TXBuf;
extern uint8_t updateDistance;

void PORT3_IRQHandler(){

    if (P3->IFG & BIT2) {
        myScooter->beamBreaks++;
        myScooter->bBSinceLast++;
        myScooter->distanceTraveled = (myScooter->beamBreaks)*(myScooter->distancePerBreak);
        P1->OUT ^= BIT0;
    }
    if(myScooter->beamBreaks%(5*14) == 0){
        updateDistance = 1;
    }
    P3->IFG = 0;
}

// used to calculate the veloctiy 
#ifdef ESCOOT
void SysTick_Handler (){

    myScooter->timesTimed++;
    myScooter->averageVelocity   =  (myScooter->distanceTraveled)/(0.1*(myScooter->timesTimed));
    myScooter->instantVelocity   =  ((myScooter->bBSinceLast)*(myScooter->distancePerBreak))/(0.1);
    myScooter->instantRotVel     =  ((myScooter->bBSinceLast)/((14*0.1)));
    myScooter->bBSinceLast = 0;
    //P1->OUT ^= BIT0 ;
}
#endif

// used to initialize the es_V type
es_V * make_eScoot(){
    es_V * eScoot = (es_V *)malloc(sizeof(es_V));

    eScoot->distancePerBreak    = 0.9107;
    eScoot->beamBreaks          = 0;
    eScoot->bBSinceLast         = 0;
    eScoot->distanceTraveled    = 0;
    eScoot->timesTimed          = 0;
    eScoot->averageVelocity     = 0;
    eScoot->instantVelocity     = 0;

    return eScoot;
}

void configure_eScooter(){
    SysTick->LOAD = 1210000;   // Give SysTick a starting value to count down from
    SysTick->CTRL = BIT0 | BIT1 | BIT2;  // Enable SysTick

    //data from encoder (port 3.2)
      P3->SEL0 &= ~(BIT2);      // set to General IO Mode
      P3->SEL1 &= ~(BIT2);      // Make sure not to be in tertiary function
      P3->DIR &= ~(BIT2);       // set direction to input
      P3->REN |= BIT2;          // enable pullup
      P3->OUT |= BIT2;          // clear interrupts
      P3->IES = BIT2;           // set IFT flag to high to low transition

      P3->IFG = 0;
      P3->IE =  (BIT2);       // Enable port interrupt
      /* Enable Interrupts in the NVIC */

      myScooter = make_eScoot();

      NVIC_EnableIRQ(PORT3_IRQn);
}

// transmit how far the scooter has gone over UART
void transmitDistance(){
    addItemCircBuf(TXBuf, 0x0A);
    addItemCircBuf(TXBuf, 0x0D);
    loadToBuf(TXBuf, "Distance: ", 9);
    uint8_t dist[7];
    ftoa(myScooter->distanceTraveled, 2, 7, dist);
    loadToBuf(TXBuf,dist ,7);
    addItemCircBuf(TXBuf, 0x0A);
    addItemCircBuf(TXBuf, 0x0D);

    UART_send_byte(removeItem(TXBuf));

}

