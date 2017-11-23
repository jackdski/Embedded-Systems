/*
*
* Wireless Bike Lock
* buttons.c
*
* 11-20-17
*
*/

#include "buttons.h"

#include "msp.h"
#include <stdint.h>

void configButtons() {
    /*
     * Button Pins:
     *  'A' : J4.33 -> P5.1
     *  'B' : J4.32 -> P3.5
     */
    // Set to General IO Mode
    P5->SEL0 &= ~(BIT1);
    P3->SEL0 &= ~(BIT5);
    // Make sure not in tertiary function
    P5->SEL1 &= ~(BIT1);
    P3->SEL1 &= ~(BIT5);
    // Set direction to input
    P5->DIR &= ~(BIT1);
    P3->DIR &= ~(BIT5);
    // Enable pullup/pulldown
    P5->REN |= BIT1;
    P3->REN |= BIT5;
    // Enable PULLUP
    P5->OUT |= BIT1;
    P3->OUT |= BIT5;
    // Clear interrupts
    P5->IFG &= ~(BIT1);
    P3->IFG &= ~(BIT5);
    /// Set IFT flag to high to low transition
    P5->IES = BIT1;
    P3->IES = BIT5;

    P5->IFG = 0;
    P3->IFG = 0;

    P5->IE = BIT1;
    P3->IE = BIT5;
    NVIC_EnableIRQ(PORT1_IRQn);
}

void configLED() {
    P2->SEL0 &=  ~(BIT0 | BIT1 | BIT2);
    P2->SEL1 &=  ~(BIT0 | BIT1 | BIT2);
    P2->DIR |=    (BIT0 | BIT1 | BIT2);
    P2->OUT &=   ~(BIT0 | BIT1 | BIT2);
}

void PORT5_IRQHandler() {
    if (P5->IFG & BIT1) {
        P2->OUT &= ~(BIT0 | BIT1 | BIT2);
        P2->OUT ^= BIT0;
    }
}

void PORT3_IRQHandler() {
    if (P3->IFG & BIT5) {
        P2->OUT &= ~(BIT0 | BIT1 | BIT2);
        P2->OUT ^= BIT1;
    }
}

void borrowBike() {
    /*
     * 1. LCD Screen
     * 2. Time Select
     * 3. Get RFID data / Student ID
     * 4. Send data to bike lock
     * 5. Return to default screen
     *
     */
}
