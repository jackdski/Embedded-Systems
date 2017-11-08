/*
 * port.c
 *
 *  Created on: Nov 5, 2017
 *      Author: amabo
 */
#include "port.h"
extern uint8_t transmit;

//Configure the buttons on the MSP Breakout and the Boosterpack
void configure_ports(){
    //Configure the BaseBoard's buttons so they can control transmission
    P1->SEL0 &= ~(BIT1 | BIT4);      // Set Port Pin Selection to General IO Mode
    P1->SEL1 &= ~(BIT1 | BIT4);      // Make sure not to use tertiary function of P1.1
    P1->DIR &=  ~(BIT1 | BIT4);      // Set P1.1 Direction to Input
    P1->REN |=   (BIT1 | BIT4);       // Enable Pullup/Pulldown
    P1->OUT |=   (BIT1 | BIT4);       // Enable PULLUP
    P1->IES =    (BIT1 | BIT4);       // Set P1 IFG flag to high to low transition

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

    P1->IFG = 0;
    P1->IE |= (BIT1 | BIT4);
    NVIC_EnableIRQ(PORT1_IRQn);

    P5->IFG = 0;
    P5->IE |= (BIT1);
    NVIC_EnableIRQ(PORT5_IRQn);

    P3->IFG = 0;
    P3->IE |= BIT5;
}

//If either button is pressed on the breakout board, transmit.
void PORT1_IRQHandler(){
    if(P1->IFG & (BIT1 | BIT4)){
        transmit = 1;
    }
    P1->IFG = 0;
}

//If the button on the boosterpack is pressed transmit. Note the PORT3 Handler is in
//beambreaks.c
void PORT5_IRQHandler(){
    if(P5->IFG & (BIT1)){
        transmit = 1;
    }
    P5->IFG = 0;
}
