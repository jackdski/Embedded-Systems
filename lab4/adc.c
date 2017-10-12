/*
 *
 * acd.c
 * Author: Jack Danielski and Avery Anderson
 * 10-12-17
 *
 */

void configure_ADC() {
    // Initialize the shared reference module
    // By default, REFMSTR=1 = REFCTL is used to configure the internal reference
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);        //If ref generator busy, WAIT
    REF_A->CTL0 = REF_A_CLT0_VSEL_0 | REF_A_CTL0_ON;//Enable internal 1.2V ref
    REF_A->CTL0 &=  ~REF_A_CTL0_TCOFF;              // Turn on Temperature sensor

    // Configure ADC - Pulse sample mode; ADC14SC trigger
    // ADC ON, temperature sample period > 30us
    ADC->CTL0 |= ADC14_CLT0_SHT0_5 | ADC14_CTL0_ON | ADC14_CTL0_SHP;
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;
    //conf internal temp sensor channel, set resolution to 14
    ADC14->CTL1 |= (ADC14_CTL1_TCMAP | ADC14_CTL1_RES_3);

    // Map temp analog channel to MEM0/MCTL0, set 3.3v ref
    ADC14->MCTL0 = (ADC14_MCTLN_INCH_22 | ADC14_MCTLN_VRSEL0);
    ADC14->IER0 |= ADC14_IER0_IE0; //Enable MCTL0/MEM0(BIT0) Interrupts

    while(!(REF_A->CTL0 & REF_A_CTL0_GENRDY)); // Wait for ref generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC; // Enable Conversions
    NVIC_EnableIRQ(ADC14_IRQn); // Enable ADC interrupt in NVIC module
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
#ifdef  BLOCKING
    while(!(EUSCI_A0->IFG & BIT1));  //While it hasn't finished transmitting
#endif
    EUSCI_A0->TXBUF = data;
}

void EUSCIA0_IRQHandler(){
    if (EUSCI_A0->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)){
            EUSCI_A0->IFG &= ~BIT1;
              return;
        }
        UART_send_byte(removeItem(TXBuf));
    }

}
