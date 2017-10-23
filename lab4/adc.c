/*
 *
 * acd.c
 * Author: Jack Danielski and Avery Anderson
 * 10-12-17
 *
 */

#include "adc.h"
#include "lab4.h"

#define TEMP


extern CircBuf_t * TXBuf;
extern CircBuf_t * SENDBuf;
extern uint16_t TNADC;
extern uint16_t XNADC;
extern uint16_t YNADC;
extern uint8_t transmit;
extern uint8_t newTemp;
extern uint16_t VXNADC;
extern uint16_t VYNADC;
extern uint16_t VZNADC;


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
    //conf internal temp sensor channel, set resolution to 14
    ADC14->CTL1 = (ADC14_CTL1_TCMAP | ADC14_CTL1_RES_3);

    // Map temp analog channel to MEM0/MCTL0, set 3.3v ref
    ADC14->MCTL[0] = (ADC14_MCTLN_INCH_22 | ADC14_MCTLN_VRSEL_1);
    ADC14->IER0 |= ADC14_IER0_IE0; //Enable MCTL0/MEM0(BIT0) Interrupts

    //Joystick Configuration
    P4->SEL0 |=  BIT4;
    P4->SEL1 |=  BIT4;
    P6->SEL0 |=  BIT0;
    P6->SEL1 |=  BIT0;
    ADC14->MCTL[1] = (ADC14_MCTLN_INCH_15 | ADC14_MCTLN_VRSEL_0);
    ADC14->MCTL[2] = (ADC14_MCTLN_INCH_9  | ADC14_MCTLN_VRSEL_0);

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

    //Enable Port one button
    P1->SEL0 &= ~(BIT1 | BIT4);
    P1->SEL1 &= ~(BIT1 | BIT4);
    P1->DIR  &= ~(BIT1 | BIT4);
    P1->REN  |=  (BIT1 | BIT4);
    P1->OUT  |=  (BIT1 | BIT4);
    P1->IES  |=  (BIT1 | BIT4);

    P1->DIR |= BIT0;
    P1->OUT &=  ~BIT0;


    P1->IFG   = 0;
    P1->IE   |=  (BIT1 | BIT4);

    //ADC14->IFGR0 &= ~ADC14_IFGR0_IFG0;
    NVIC_EnableIRQ(ADC14_IRQn); // Enable ADC interrupt in NVIC module
    NVIC_EnableIRQ(PORT1_IRQn); // Enable port 1 buttons
}

void configure_serial_port(){
    //Configure UART pins, set 2-UART pins to UART mode
    P1->SEL0 |=  (BIT2 | BIT3);
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A0->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A0->BRW = 78;                         //Set Baud Rate
    EUSCI_A0->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

#ifndef BLOCKING
    EUSCI_A0->IFG &= ~(BIT1 | BIT0);
    UCA0IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA0_IRQn);
#endif
}

void configure_clocks(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.

}

void UART_send_n(uint8_t * data, uint8_t length){
    //Code to iterate through the transmit data
    if(!data)
        return;
    volatile uint8_t n;

    for(n = 0; n<length; n++){
        UART_send_byte(data[n]);
    }
}


void UART_send_byte(uint8_t data){
    EUSCI_A0->TXBUF = data;
}

void EUSCIA0_IRQHandler(){
    if (EUSCI_A0->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(SENDBuf)){
            EUSCI_A0->IFG &= ~BIT1;
            ADC14->CTL0 |= (ADC14_CTL0_ENC);
            return;
        }
        UART_send_byte(removeItem(SENDBuf));
    }

}

void ADC14_IRQHandler(){

    if(ADC14_IFGR0_IFG0){
        TNADC = ADC14->MEM[0];
        newTemp = 1;
    }
    if(ADC14_IFGR0_IFG1){
        YNADC = ADC14->MEM[1];
       }
    if(ADC14_IFGR0_IFG2){
        XNADC = ADC14->MEM[2];
    }
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
void PORT1_IRQHandler(){
#ifndef TEMP
    if(P1->IFG & BIT1 || P1->IFG & BIT4){
        P1->OUT ^= BIT0;
        printTemps();
    }
#endif
#ifdef TEMP
    if (P1->IFG & BIT1 || P1->IFG & BIT4) {
        transmit = 1;
    }
#endif
    P1->IFG = 0;
}

void printTemps(){
    float Ctemp = 0;
    uint8_t CString[7];

    uint8_t KString[9];

    uint8_t FString[9];

    Ctemp = (0.0381)*(float)(TNADC)-360.5;
    ftoa(Ctemp,2,5,CString);
    CString[6] = 0x0D;
    CString[5] = 0x0A;

    float Ftemp = (Ctemp*1.8)+32.0;

    ftoa((Ctemp+273),2,7,KString);
    KString[8] = 0x0D;
    KString[7] = 0x0A;

    ftoa(Ftemp,2,7,FString);
    FString[8] = 0x0D;
    FString[7] = 0x0A;

    loadToBuf(SENDBuf,"Temps in C, K and F",19);
    addItemCircBuf(SENDBuf, 0x0A);
    addItemCircBuf(SENDBuf, 0x0D);

    loadToBuf(SENDBuf,CString,7);
    loadToBuf(SENDBuf,KString,9);
    loadToBuf(SENDBuf,FString,9);

    if(!isEmpty(SENDBuf)){
        EUSCI_A0->IFG |= BIT1;
    }
}

void problemSix() {
    while(!isEmpty(TXBuf)){
        float Ftemp = (0.0381)*(float)(removeItem(TXBuf))-360.5;
        uint8_t FString[6];
        ftoa((Ftemp+273),2,5,FString);
        FString[5] = 0x0D;
        loadToBuf(SENDBuf,FString,6);
    }

    loadToBuf(SENDBuf,"The joystick is in the ",23);

    if(XNADC > 9000){
        loadToBuf(SENDBuf,"upper ",6);
    }
    else{
        loadToBuf(SENDBuf,"lower ",6);
    }
    if(YNADC > 9000){
        loadToBuf(SENDBuf,"right ",6);
    }
    else{
        loadToBuf(SENDBuf,"left ",5);
    }
    loadToBuf(SENDBuf,"quadrant.",10);
    addItemCircBuf(SENDBuf, 0x0D);


    EUSCI_A0->IFG |= BIT1;
    while(!isEmpty(SENDBuf));
    resetCircBuf(TXBuf);
    P1->OUT &= ~BIT0;
}
