/*
 * Wireless Bikelock - Lock
 * User.h
 *
 * 12-2-17
 */

#ifndef USER_H_
#define USER_H_

#include "Circbuf.h"

typedef struct {
    uint8_t * data;
    uint8_t * studentId;
} User_t;

// see if devices are connected
//const uint8_t * ACKREQUEST = "ISCONNECTED";

// RFID data and duration data will be appended to this
//const uint8_t * NEWUSER = "USERDATA";

void acknowledgeCheck();


// parses the new user data and stores it in main's user variable
User_t * registerNewUser();

#endif
