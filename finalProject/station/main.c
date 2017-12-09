#include <stdlib.h>

#include "msp.h"
#include "Student.h"
#include "Circbuf.h"
#include "State.h"
#include "RFID.h"
#include "SystemClock.h"
#include "Buttons.h"

Student_t * registry;
CircBuf_t * RFIDBuf;
uint8_t     newRFID = 0;

State stationState;
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//Initialize the registry to have no students
	registry = NULL;

	configure_RFID();
	configure_SystemClock();
	configure_LockButton();


	//Create an RFIDBuffer to hold our 16 chars of RFID data
	RFIDBuf = createCircBuf(16);

	while(1){
	    if(newRFID){
	        newRFID = 0;

	        //Create a string to hold our new data, and create an iterator to populate it
	        uint8_t readRFID[16];
	        uint8_t i = 0;

	        //Store each character in its corresponding place in the string and clear the buff.
	        for(i = 0; i < 16; i++){
	            readRFID[i] = removeItem(RFIDBuf);
	        }
            resetCircBuf(RFIDBuf);


	        //Check to see if we have already registered this student and enter the appropriate state
	        if(!findStudent(readRFID)){
	            enterState(Register);
	            registerStudent(readRFID);
	            //Enter Stefan's register student joystick stuff
	        }

	    }

	}
}
