/*
 * Wireless Bike Lock - Lock
 * Bluetooth.c
 *
 */

#include "msp.h"
#include "Bluetooth.h"
#include "Circbuf.h"
#include <stdint.h>

//The buffers used for bluetooth communication
extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;

uint8_t systickcount;

//Boolean indicating that the bikelock responded
uint8_t gotResponse = 0;

//The systick timer is used to timeout of transmissions if we don't recieve a response
void configure_Systick() {
    // starting value to count down from for 0.05s ticks
    SysTick->LOAD = 6000000;
    // Enable SysTick counter, interrupt, clock
    SysTick->CTRL = (BIT1 | BIT2);
}

//If the systick timer fires twice (approximatly 0.1 seconds)
//Flag that we did not get a response and should proceed through the transmission logic
void SysTick_Handler() {
    systickcount++;
    if(systickcount % 2) {
        SysTick->CTRL &= ~BIT0;
        gotResponse = 1;
        systickcount = 0;
    }
}

//Configure the UART Ports for bluetooth communication
void configure_Bluetooth(){
    //Configure UART pins, set 2-UART pins to UART mode
    P9->SEL0 |=  (BIT6 | BIT7);
    P9->SEL1 &= ~(BIT6 | BIT7);

    EUSCI_A3->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A3->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A3->BRW = 78;                         //Set Baud Rate
    EUSCI_A3->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A3->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A3->IFG &= ~(BIT1 | BIT0);
    UCA3IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA3_IRQn);
}

//Send a single bit of data
inline void BLUART_send_byte(uint8_t data){
    EUSCI_A3->TXBUF = data;
}

//The code controlling the transmission and reception of messages
void EUSCIA3_IRQHandler(){

    //If I recieve a message, flag that I got a response.
    if(EUSCI_A3->IFG & BIT0) {
        P1->OUT |= BIT0;
        addItemCircBuf(RXBuf,EUSCI_A3->RXBUF);
        if(isFullCircBuf(RXBuf)){
            resetCircBuf(RXBuf);
            gotResponse = 2;
        }
    }
    //This code will transmit the entire TXBuf. It stops once the Buf is empty
    if (EUSCI_A3->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
            EUSCI_A3->IFG &= ~BIT1;
            return;
        }
        BLUART_send_byte(removeItem(TXBuf));
    }
}

//This function sends a heartbeat message and enables the timer.
//It then blocks until either the timer finishes or it gets a response.
uint8_t send_Heartbeat(){
    //Set the response flag to zero and transmit
    gotResponse = 0;
    loadToBuf(TXBuf, "HXXXXXXXXXXXXXXXX",17);
    BLUART_send_byte(removeItem(TXBuf));

    //enable the Systick timer
    SysTick->CTRL |= BIT0;
    //block until either I get a response or the time finishes
    while(gotResponse == 0);

    //This is used as a boolean function. gotResponse will be 1 for no response, and 2 for
    //a recieved messsage.  Returning gotResponse -1 shifts these values down one to be better
    //represented in main.
    return gotResponse - 1;
}

//This function sends an RFID message and enables the timer.
//It automatically loads the relevant RFID data to be the 16 data chars of the message
//If we are deregistering, this data will be entirely zeros
//It then blocks until either the timer finishes or it gets a response.
uint8_t send_RFID(uint8_t RFID[16]){
    //Set the response flag to zero and transmit
    gotResponse = 0;
    addItemCircBuf(TXBuf, 'R');
    loadToBuf(TXBuf, RFID ,16);
    BLUART_send_byte(removeItem(TXBuf));

    //enable the Systick timer
    SysTick->CTRL |= BIT0;
    //block until either I get a response or the time finishes
    while(gotResponse == 0);

    //This is used as a boolean function. gotResponse will be 1 for no response, and 2 for
    //a recieved messsage.  Returning gotResponse -1 shifts these values down one to be better
    //represented in main.
    return gotResponse - 1;
}

//This function sends a warning message and enables the timer.
//It then blocks until either the timer finishes or it gets a response.
uint8_t send_Warning(){
    //Set the response flag to zero and transmit
    gotResponse = 0;
    loadToBuf(TXBuf, "WXXXXXXXXXXXXXXXX",17);
    BLUART_send_byte(removeItem(TXBuf));

    //enable the Systick timer
    SysTick->CTRL |= BIT0;
    //block until either I get a response or the time finishes
    while(gotResponse == 0);

    //This is used as a boolean function. gotResponse will be 1 for no response, and 2 for
    //a recieved messsage.  Returning gotResponse -1 shifts these values down one to be better
    //represented in main.
    return gotResponse - 1;
}
