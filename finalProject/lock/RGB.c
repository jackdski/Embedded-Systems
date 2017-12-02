/*
 * RGB.c
 *
 *  Created on: Nov 28, 2017
 *      Author: amabo
 */
#include "RGB.h"

/*
 * In order to configure the LEDs, we only need to set their direction to be outputs.
 * In this function we do that, and then make sure the LEDs start off.
 */
void configure_RGB(){
    P6->DIR |=  (BIT4 | BIT5);
    P5->DIR |=  (BIT5);

    LED_Off();
}

//Turn on the Red LED
void Red_LED_On(){
    P5->OUT |= BIT5;
}

//Turn on the Blue LED
void Blue_LED_On(){
    P6->OUT |= BIT4;
}

//Turn on the Green LED
void Green_LED_On(){
    P6->OUT |= BIT5;
}

//Turn off all of the LEDs
void LED_Off(){
    P6->OUT &= ~(BIT4 | BIT5);
    P5->OUT &= ~(BIT5);
}
