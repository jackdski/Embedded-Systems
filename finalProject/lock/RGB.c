/*
 * RGB.c
 *
 *  Created on: Nov 28, 2017
 *      Author: amabo
 */
#include "RGB.h"

void configure_RGB(){
    P6->DIR |=  (BIT4 | BIT5);
    P5->DIR |=  (BIT5);

    P6->OUT &= !(BIT4 | BIT5);
    P5->OUT &= !(BIT5);
}
