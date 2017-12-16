/*
 * Wireless Bike Lock - Lock
 * Bluetooth.c
 *
 */

#include "msp.h"
#include "Bluetooth.h"
#include "Circbuf.h"
#include <stdint.h>

extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;
extern CircBuf_t * TESTtxBuf;
extern CircBuf_t * TESTrxBuf;

uint8_t systickcount;

uint8_t gotResponse = 0;

void configure_Systick() {
    /*
     * 500,000us == 0.5s
     * f(t) = (t/12) *from Lab 2 write-up*
     *  500,000 = (t/12)
     *  t = 6,000,000
     */

    // starting value to count down from for 0.5s ticks
    SysTick->LOAD = 6000000;
    // Enable SysTick counter, interrupt, clock
    SysTick->CTRL = (BIT1 | BIT2);
}

void SysTick_Handler() {
    systickcount++;
    if(systickcount % 2) {
        SysTick->CTRL &= ~BIT0;
        gotResponse = 1;
        systickcount = 0;
    }
}

//Connect 3.3 to the RX
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

inline void BLUART_send_byte(uint8_t data){
    EUSCI_A3->TXBUF = data;
}

// use echo stuff to return data to station
void EUSCIA3_IRQHandler(){

    if(EUSCI_A3->IFG & BIT0) {
        P1->OUT |= BIT0;
        addItemCircBuf(RXBuf,EUSCI_A3->RXBUF);
        if(isFullCircBuf(RXBuf)){
            resetCircBuf(RXBuf);
            gotResponse = 2;
        }
    }
    if (EUSCI_A3->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
            EUSCI_A3->IFG &= ~BIT1;
            return;
        }
        BLUART_send_byte(removeItem(TXBuf));
    }
}

uint8_t send_Heartbeat(){
    //Set the response flag to zero and transmit
    gotResponse = 0;
    loadToBuf(TXBuf, "HXXXXXXXXXXXXXXXX",17);
    BLUART_send_byte(removeItem(TXBuf));

    //enable the Systick timer
    SysTick->CTRL |= BIT0;
    //block until either I get a response or the time finishes
    while(gotResponse == 0);

    return gotResponse - 1;
}

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

    return gotResponse - 1;
}

uint8_t send_Warning(){
    //Set the response flag to zero and transmit
    gotResponse = 0;
    loadToBuf(TXBuf, "WXXXXXXXXXXXXXXXX",17);
    BLUART_send_byte(removeItem(TXBuf));

    //enable the Systick timer
    SysTick->CTRL |= BIT0;
    //block until either I get a response or the time finishes
    while(gotResponse == 0);

    return gotResponse - 1;
}
