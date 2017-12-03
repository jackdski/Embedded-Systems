/*
 * State.h
 *
 *  Created on: Dec 2, 2017
 *      Author: amabo
 */

#ifndef STATE_H_
#define STATE_H_

typedef enum state{
    Error = -1,
    Locked = 0,
    Unlockable = 1,
    Unlocked = 2
}State;

//We only want to stay unlockable for a short period of time.  If we don't
//transition quickly enough, reset to locked
void configure_UnlockableTimer();

//This function is the only way we will allow the program to change state
void enterState(State newState);

#endif /* STATE_H_ */
