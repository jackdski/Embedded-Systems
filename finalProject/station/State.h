/*
 * State.h
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */

#ifndef STATE_H_
#define STATE_H_
#include "msp.h"

typedef enum state{
    Standby  = 0,
    Register = 1,
    SetTime  = 2,
    Checkout = 3,
}State;

//This function is the only way we will allow the program to change state
void enterState(State newState);

#endif /* STATE_H_ */
