/*
 * ButtonConfig.c
 *
 *  Created on: Dec 2, 2017
 *      Author: Stefan
 */
#include "msp.h"
#include "ButtonConfig.h"
#include "core_cm4.h"
#include <stdint.h>
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include "clock.h"


volatile uint8_t slideNum = 1;
extern Graphics_Context g_sContext;


void GPIO_configure(void) {
/* Top and Bottom Button configure */
    P5->SEL0 &= ~(BIT1);   // General-purpose I/O
    P5->SEL1 &= ~(BIT1);   // General-purpose I/O
    P5->DIR &= ~(BIT1);   // Port configured as input
    P5->REN |= (BIT1);   // Pullup or pulldown enabled
    P5->IFG &= ~(BIT1);   // Clear interrupts for J4.33 and J4.32
    P5->IES |= (BIT1);   // Flag set with high-to-low transition
    P5->IE |= (BIT1);   // Corresponding port interrupt enabled
    P3->SEL0 &= ~(BIT5);   // General-purpose I/O
    P3->SEL1 &= ~(BIT5);   // General-purpose I/O
    P3->DIR &= ~(BIT5);   // Port configured as input
    P3->REN |= (BIT5);   // Pullup or pulldown enabled
    P3->IFG &= ~(BIT5);   // Clear interrupts for J4.33 and J4.32
    P3->IES |= (BIT5);   // Flag set with high-to-low transition
    P3->IE |= (BIT5);   // Corresponding port interrupt enabled
/* Joystick configure */
    P4->SEL0 |= BIT4;
    P4->SEL1 |= BIT4;
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;
/* Enable Interrupts in the NVIC */
    NVIC_EnableIRQ(PORT3_IRQn);
    NVIC_EnableIRQ(PORT5_IRQn);
}

void PORT3_IRQHandler(void){
    if(P3->IFG & BIT5){
        Delay0();
        Delay0();
        Delay0();
        slideNum++;
    }
    if(slideNum > 7){
        slideNum = 1;
    }
    P3->IFG &= ~(BIT5);
    P5->IFG &= ~(BIT1);
}

void PORT5_IRQHandler(void){
    if(P5->IFG & BIT1){
        Delay0();
        Delay0();
        Delay0();
        slideNum--;
    }
    if(slideNum < 1){
        slideNum = 7;
    }
    P5->IFG &= ~(BIT1);
    P3->IFG &= ~(BIT5);
}





