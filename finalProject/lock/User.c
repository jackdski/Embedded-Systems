/*
 * Wireless Bikelock - Lock
 * User.c
 *
 * Created on: 12-2-17
 */

#include "msp.h"
#include "Circbuf.h"

extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;

/*
typedef struct {
    uint64_t data;
    uint16_t studentId;
} User_t;
*/

//extern User_t * mainuser;
extern uint8_t * mainuser;

// see if devices are connected
const uint8_t * ACKREQUEST = "ISCONNECTED"; //11 long

const uint8_t * ACKNOWLEDGE = "ACKLOCK"; //7 long

// RFID data and duration data will be appended to this
const uint8_t * NEWUSER = "USERDATA"; //8 long

void checkAvailablility() {
    int i;
    for(i=0;i<7;i++){
        addItemCircBuf(TXBuf, ACKNOWLEDGE[i]);
    }
    if(mainuser == "0000000000000000") {
        addItemCircBuf(TXBuf, "1");
    }
    else {
        addItemCircBuf(TXBuf, "0");
    }
    bluetooth_send_n(TXBuf->buffer, 8);
}

void acknowledgeCheck() {
    if (RXBuf->buffer == ACKREQUEST) {
        int i;
        for(i = 0; i < 7; i++) {
            addItemCircBuf(TXBuf, ACKNOWLEDGE[i]);
        }
        while(!isEmpty(TXBuf));
        EUSCI_A3->IFG |= BIT1;
    }
}

//void checkUser() {
    //check to see if the received data is user info data
    //use interrupts
//}

// parses the new user data and stores it in main's user variable
void registerNewUser() {
    volatile int i;
    // 33 = 8 + 9 + 16 (NEWUSER+ID+RFIDdata)
    for(i = 0; i < 8; i++) {
        if(!(RXBuf->buffer[i] == NEWUSER[i])) {
            mainuser = " ";
            //mainuser->studentId = 000000000;
            //mainuser->data = 0000000000000000;
            return;
        }
    }

    for(i = 8; i < 17; i++) {
        mainuser[i-8] = RXBuf->buffer[i];
    }
    return;


    /*
    uint8_t * sid[9];
    for(i = 8; i < 17; i++) {
        sid[i-8] = RXBuf->buffer[i];
    }
    mainuser->studentId = stoi(sid);


    uint8_t * rfdata[16];
    for(i = 17; i < 33; i++) {
        rfdata[i] = RXBuf->buffer[i];
    }
    mainuser->data = stoi(rfdata);
    */


}
