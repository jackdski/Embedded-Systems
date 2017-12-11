/*
 * Wireless Bikelock - Lock
 * User.h
 *
 * 12-2-17
 */

#ifndef USER_H_
#define USER_H_

#include "msp.h"
#include "Circbuf.h"

void checkAvailablility();
void acknowledgeCheck();
void registerNewUser();


// parses the new user data and stores it in main's user variable
//User_t * registerNewUser();

#endif
