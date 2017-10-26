

#include "accelerometer.h"

void configure_ADC() {
    // Initialize the shared reference module
    // By default, REFMSTR=1 = REFCTL is used to configure the internal reference
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);        //If ref generator busy, WAIT
    REF_A->CTL0 = REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON;//Enable internal 1.2V ref
    REF_A->CTL0 &=  ~REF_A_CTL0_TCOFF;              // Turn on Temperature sensor

    // Configure ADC - Pulse sample mode; ADC14SC trigger
    // ADC ON, temperature sample period > 30us
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;
    //Add sequence of channels
    ADC14->CTL0 |= ADC14_CTL0_SHT0_5 | ADC14_CTL0_ON | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1;

    ADC14->IER0 |= ADC14_IER0_IE0; //Enable MCTL0/MEM0(BIT0) Interrupts

    //Accelerometer Configuration
    P4->SEL0 |= (BIT0 | BIT2);
    P4->SEL1 |= (BIT0 | BIT2);
    P6->SEL0 |=  BIT1;
    P6->SEL1 |=  BIT1;
    ADC14->MCTL[3] = (ADC14_MCTLN_INCH_14 | ADC14_MCTLN_VRSEL_0 );
    ADC14->MCTL[4] = (ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_0 );
    ADC14->MCTL[5] = (ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_0 | ADC14_MCTLN_EOS);

    while(!(REF_A->CTL0 & REF_A_CTL0_GENRDY)); // Wait for ref generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC; // Enable Conversions

    //ADC14->IFGR0 &= ~ADC14_IFGR0_IFG0;
    NVIC_EnableIRQ(ADC14_IRQn); // Enable ADC interrupt in NVIC module
    NVIC_EnableIRQ(PORT1_IRQn); // Enable port 1 buttons
}

// Stores accelerometer data in MEM[3-5]
void ADC_IRQHandler() {
    if(ADC14_IFGR0_IFG3){
         VXNADC = ADC14->MEM[3];
     }
     if(ADC14_IFGR0_IFG4){
         VYNADC = ADC14->MEM[4];
     }
     if(ADC14_IFGR0_IFG5){
         VZNADC = ADC14->MEM[5];
     }
 }
}
