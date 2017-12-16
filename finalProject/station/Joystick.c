/*
 * Joystick.c
 *
 *  Created on: Dec 10, 2017
 *      Author: amabo
 */
#include "Joystick.h"
#include "State.h"
#include "Logging.h"

uint16_t joyVal;

extern State stationState;

extern uint8_t newSID[4];
extern uint8_t SIDIterator;

extern uint8_t CTime;
extern uint8_t CTimeSTR[3];


void configure_Joystick_TA0(){
    // Initialize the shared reference module
    // By default, REFMSTR=1 = REFCTL is used to configure the internal reference
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);        //If ref generator busy, WAIT
    REF_A->CTL0 = REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON;//Enable internal 1.2V ref

    // Configure ADC - Pulse sample mode; ADC14SC trigger
    // Turn off the ENC bit to enable configuration
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;

    //Configure hold time, turn it on, timer based, single channel, based on SMCLK
    ADC14->CTL0 |= ADC14_CTL0_SHT0_5 | ADC14_CTL0_ON | ADC14_CTL0_SHP
            | ADC14_CTL0_CONSEQ_0 | ADC14_CTL0_SSEL__SMCLK;
    //Set 14 bit resolution
    ADC14->CTL1 = (ADC14_CTL1_RES_3);


    //Joystick vertical configuration
    P6->SEL0 |=  BIT0;
    P6->SEL1 |=  BIT0;

    ADC14->MCTL[0] = (ADC14_MCTLN_INCH_9 | ADC14_MCTLN_VRSEL_0);

    //Enable MCTL0/MEM0(BIT0) Interrupts
    ADC14->IER0 |= ADC14_IER0_IE0 ;//| ADC14_IER0_IE1 | ADC14_IER0_IE2;

    while(!(REF_A->CTL0 & REF_A_CTL0_GENRDY)); // Wait for ref generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC; // Enable Conversions

    NVIC_EnableIRQ(ADC14_IRQn); // Enable ADC interrupt in NVIC module

    //We only check the interrupt whenever the timer fires.  So configure the timer here as well
    TIMER_A0->R = 0;                    // Clear timer count
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_3 | TIMER_A_CTL_MC__UP;            // Set to SMCLK, Up mode (BIT9 ON;)
    TIMER_A0->CCR[0] = 65000;        // Value to count to
    TIMER_A0->CCTL[0] |= 0x0010;      // TACCR0 interrupt enabled

    NVIC_EnableIRQ(TA0_0_IRQn);

}

// Pulls the acceleration value and stores it in our global variable
void ADC14_IRQHandler() {
    if(ADC14_IFGR0_IFG0){
        uint16_t data = ADC14->MEM[0];
        if(stationState == Register){
            if(data >= 12000 && joyVal < 12000 && joyVal > 4000){
                if(newSID[SIDIterator] < '9'){
                    newSID[SIDIterator]++;
                }
            }
            else if(data <= 4000 && joyVal < 12000 && joyVal > 4000){
                if(newSID[SIDIterator] > '1'){
                    newSID[SIDIterator]--;
                }
            }
        }
        if(stationState == SetTime){
            if(data >= 12000 && joyVal < 12000 && joyVal > 4000){
                if(CTime < 255){
                    CTime += 1;
                }
            }
            else if(data <= 4000 && joyVal < 12000 && joyVal > 4000){
                if(CTime > 1){
                    CTime -= 1;
                }
            }
            itos((uint32_t)CTime,3,CTimeSTR);
        }

        joyVal = data;
    }
}

void TA0_0_IRQHandler() {
    //Renable the buttons
    P5->IFG &= ~(BIT1);
    P5->IE  |= BIT1;

    P3->IFG &= ~(BIT5);
    P3->IE  |= BIT5;

    //Call for a new joystick value when in the correct states
    if(stationState == Register || stationState == SetTime ){
        if(ADC14->CTL0 & (ADC14_CTL0_ENC))
            ADC14->CTL0 |= ADC14_CTL0_SC; //Sampling and conversion start
    }

    TIMER_A0->CCTL[0] &= ~(BIT0);
}
