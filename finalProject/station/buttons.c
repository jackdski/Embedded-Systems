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

      //Configure the BoosterPack's buttons so they can control transmission
      P3->SEL0 &= ~(BIT5);      // Set Port Pin Selection to General IO Mode
      P3->SEL1 &= ~(BIT5);      // Make sure not to use tertiary function of P1.1
      P3->DIR &=  ~(BIT5);      // Set P1.1 Direction to Input
      P3->REN |=   (BIT5);       // Enable Pullup/Pulldown
      P3->OUT |=   (BIT5);       // Enable PULLUP
      P3->IES |=   (BIT5);       // Set P1 IFG flag to high to low transition
      //Configure the BoosterPack's buttons so they can control transmission
      P5->SEL0 &= ~(BIT1);      // Set Port Pin Selection to General IO Mode
      P5->SEL1 &= ~(BIT1);      // Make sure not to use tertiary function of P1.1
      P5->DIR &=  ~(BIT1);      // Set P1.1 Direction to Input
      P5->REN |=   (BIT1);       // Enable Pullup/Pulldown
      P5->OUT |=   (BIT1);       // Enable PULLUP
      P5->IES =    (BIT1);       // Set P1 IFG flag to high to low transition

      P5->IFG = 0;
      P5->IE |= (BIT1);
      NVIC_EnableIRQ(PORT5_IRQn);

      P3->IFG = 0;
      P3->IE |= BIT5;
      NVIC_EnableIRQ(PORT3_IRQn);
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
    P5->IFG = 0;
}

void PORT3_IRQHandler() {
    if (P3->IFG & BIT5) {
        P2->OUT &= ~(BIT0 | BIT1 | BIT2);
        P2->OUT ^= BIT1;
    }
    P3->IFG = 0;
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
