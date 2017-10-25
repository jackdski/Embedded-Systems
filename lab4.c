/*
 *
 * lab4.c
 * Author: Jack Danielski and Avery Anderson
 * 10-12-17
 *
 */

#include "msp.h"
#include "lab4.h"
#include "adc_circbuf.h"
#include <stdlib.h>
uint8_t timerCount = 0;
es_V * myScooter;
uint8_t lastV;
extern CircBuf_t * SENDBuf;
extern uint16_t VYNADC;
extern uint8_t  dir;

//turns an int into a string
void itoa(uint32_t num, int8_t size, uint8_t * str ) {
    int8_t i;
    i = size;
    while (i >= 0) {
        if ( i == size) {
            str[i] = '\0';
            i--;
        }
        else {
            str[i] = num % 10;
            str[i] += 48;
            num = num / 10;
            i--;
        }
    }
}

//turns a float into a string
void ftoa(float number, uint8_t decimalPlace, uint8_t size, uint8_t * str) {
    uint8_t tens = 10;
    int8_t i = decimalPlace - 1;
    while (i > 0) {
        tens = tens * 10;
        i--;
    }
    number = number * (float)tens;
    uint16_t numberInt = (int)number;

    itoa(numberInt, size-1, str);

    int8_t deciPlace = decimalPlace +1;
    uint8_t tempStr[6];

    int j = 0;
    while (j < deciPlace) {
        tempStr[j] = str[deciPlace + j];
        j++;
    }

    str[size - decimalPlace - 1] = '.';

    deciPlace = size - decimalPlace;
    j = 0;
    while (deciPlace <= size){
        str[deciPlace] = tempStr[j];
        deciPlace++;
        j++;
    }
}

//Timer handler counts every second and stores all of the scooter data as it does
//it is also where the ADC is told to start sampling
void TA0_0_IRQHandler() {

    /*the timer cannot count up to 500ms, we need to count to a multiple of 500
     * many times. This interrupt goes off every 166ms, so we need three counts before
     * taking action.
     */

    //If I can sample
    if(ADC14->CTL0 & (ADC14_CTL0_ENC))
        ADC14->CTL0 |= ADC14_CTL0_SC; //Sampling and conversion start

    //Timercount is used to upscale the value to approximately one second
    timerCount++;
    if (timerCount == 40) {//use 40 to get one second
        P1->OUT ^= BIT0;
        timerCount = 0;

        //Store all of the scooter values
        lastV = myScooter->instantVelocity;
        myScooter->timesTimed++;
        myScooter->averageVelocity   =  (myScooter->distanceTraveled)/(0.1*(myScooter->timesTimed));
        myScooter->instantVelocity   =  ((myScooter->bBSinceLast)*(myScooter->distancePerBreak))/(0.1);
        myScooter->instantRotVel     =  ((myScooter->bBSinceLast)/((14*0.1)));
        myScooter->bBSinceLast = 0;

        //Check for changes of direction. If the V is was low and is now increasing, make the sign
        //Equal to the sign of our current aceleration
        if(lastV< 10 && myScooter->instantVelocity > 10){
            if(VYNADC < 8200)
                //loadToBuf(SENDBuf,"Forward",7);
                dir = 0;
            else
                //loadToBuf(SENDBuf,"Forward",7);
                dir = 1;
            EUSCI_A0->IFG |= BIT1;
        }
    }
    TIMER_A0->CCTL[0] &= ~(BIT0);
    TIMER_A0->CTL |=  (BIT1);
}

//Set up the timer
void timer_a0_config(){
    TIMER_A0->R = 0;                    // Clear timer count
    TIMER_A0->CTL = SET_CTL;            // Set to SMCLK, Up mode (BIT9 ON;)
    TIMER_A0->CCR[0] = COUNT_TO;        // Value to count to
    TIMER_A0->CCTL[0] |= SET_CCTL;      // TACCR0 interrupt enabled

    NVIC_EnableIRQ(TA0_0_IRQn);
}

//Initialize all of the escooter values
es_V * make_eScoot(){
    es_V * eScoot = (es_V *)malloc(sizeof(es_V));

    eScoot->distancePerBreak    = 0.9107;
    eScoot->beamBreaks          = 0;
    eScoot->bBSinceLast         = 0;
    eScoot->distanceTraveled    = 0;
    eScoot->timesTimed          = 0;
    eScoot->averageVelocity     = 0;
    eScoot->instantVelocity     = 0;

    return eScoot;
}

//Configure all of the scooter related pins and interrupts
void configure_eScooter(){

    //data from encoder (port 3.2)
      P3->SEL0 &= ~(BIT2);      // set to General IO Mode
      P3->SEL1 &= ~(BIT2);      // Make sure not to be in tertiary function
      P3->DIR &= ~(BIT2);       // set direction to input
      P3->REN |= BIT2;          // enable pullup
      P3->OUT |= BIT2;          // clear interrupts
      P3->IES = BIT2;           // set IFT flag to high to low transition

      P3->IFG = 0;
      P3->IE =  (BIT2);       // Enable port interrupt
      /* Enable Interrupts in the NVIC */
      P2->DIR |= BIT0;

      myScooter = make_eScoot();

      NVIC_EnableIRQ(PORT3_IRQn);
}

//Iterate the scooter distances whenever there is a beam break
void PORT3_IRQHandler(){

    if (P3->IFG & BIT2) {
        myScooter->beamBreaks++;
        myScooter->bBSinceLast++;
        myScooter->distanceTraveled = (myScooter->beamBreaks)*(myScooter->distancePerBreak);
        P1->OUT ^= BIT0;
    }
    P3->IFG = 0;
}
