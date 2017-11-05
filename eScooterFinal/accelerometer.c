/*
 * accelerometer.c
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */



#include "msp.h"
#include "accelerometer.h"

extern uint16_t VYNADC;

void configure_ADC() {
    // Initialize the shared reference module
    // By default, REFMSTR=1 = REFCTL is used to configure the internal reference
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);        //If ref generator busy, WAIT
    REF_A->CTL0 = REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON;//Enable internal 1.2V ref

    // Configure ADC - Pulse sample mode; ADC14SC trigger
    // ADC ON, temperature sample period > 30us
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;
    //Add sequence of channels
    ADC14->CTL0 |= ADC14_CTL0_SHT0_5 | ADC14_CTL0_ON | ADC14_CTL0_SHP
                    | ADC14_CTL0_CONSEQ_0 | ADC14_CTL0_SSEL__SMCLK;
    ADC14->CTL1 = (ADC14_CTL1_RES_3);
    //Accelerometer Configuration
    P4->SEL0 |= (BIT0);
    P4->SEL1 |= (BIT0);
    ADC14->MCTL[0] = (ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_EOS);

    //Enable MCTL0/MEM0(BIT0) Interrupts
    ADC14->IER0 |= ADC14_IER0_IE0 ;//| ADC14_IER0_IE1 | ADC14_IER0_IE2;

    while(!(REF_A->CTL0 & REF_A_CTL0_GENRDY)); // Wait for ref generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC; // Enable Conversions

    NVIC_EnableIRQ(ADC14_IRQn); // Enable ADC interrupt in NVIC module
    //ADC14->CTL0 |= ADC14_CTL0_ENC|ADC14_CTL0_SC;
}

// Stores accelerometer data in MEM[3-5]
void ADC14_IRQHandler() {
    if(ADC14_IFGR0_IFG0){
         VYNADC = ADC14->MEM[0];
     }
}

