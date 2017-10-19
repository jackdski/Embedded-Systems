/*
 *
 * lab4.c
 * Author: Jack Danielski and Avery Anderson
 * 10-12-17
 *
 */

#include "msp.h"
#include "lab4.h"


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


// Converts a string of numbers into an integer
uint32_t atoi(uint8_t * a) {
    uint8_t count = 0;
    uint8_t finalInt = 0;

    while (a[count]){
        finalInt *= 10;
        finalInt += a[count] - '0';
    }

    return finalInt;
}


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

void TA0_0_IRQHandler() {

    /*the timer cannot count up to 500ms, we need to count to a multiple of 500
     * many times. This interrupt goes off every 166ms, so we need three counts before
     * taking action.
     */

    timerCount++;
    if (timerCount == 40) {
        P1->OUT ^= BIT0;
        timerCount = 0;
    }
    TIMER_A0->R = 0;
        //TIMER_A0->CTL &= ~0;//(BIT0);
    TIMER_A0->CCTL[0] &= ~(BIT0);
    TIMER_A0->CTL |=  (BIT1);
}

void timer_a0_config(){
    TIMER_A0->R = 0;                    // Clear timer count
    TIMER_A0->CTL = SET_CTL;            // Set to SMCLK, Up mode (BIT9 ON;)
    TIMER_A0->CCR[0] = COUNT_TO;        // Value to count to
    TIMER_A0->CCTL[0] |= SET_CCTL;      // TACCR0 interrupt enabled

    NVIC_EnableIRQ(TA0_0_IRQn);
}
