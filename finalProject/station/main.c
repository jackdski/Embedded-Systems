#include <stdlib.h>

//Include all of our relevant files
#include "msp.h"
#include "Student.h"
#include "Circbuf.h"
#include "State.h"
#include "RFID.h"
#include "SystemClock.h"
#include "Buttons.h"
#include "RTC.h"
#include "Bike.h"
#include "Logging.h"
#include "Joystick.h"
#include "Bluetooth.h"

//Create the heads of our linked lists
volatile Student_t * registry;
volatile Bike_t    * bikeList;

//Create the RFID circular buffer and new RFID flag
volatile CircBuf_t * RFIDBuf;
volatile uint8_t     newRFID = 0;

//Create the Logging circular buffer and send log flag
volatile CircBuf_t * LOGBuf;
volatile uint8_t LOGFlag;

//Create circular buffers to control the transmission and reception of information between the station and lock
volatile CircBuf_t * RXBuf;
volatile CircBuf_t * TXBuf;
volatile uint8_t  transmitFlag = 0;

//Create the State variable
volatile State stationState;
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//Initialize the registry to have no students
	registry = NULL;

	//Configure all of our peripherals
	configure_RFID();
	configure_SystemClock();
	configure_Buttons();
	configure_RTC();
	configure_TerminalUART();
	configure_Joystick_TA0();
	configure_Bluetooth();
	configure_Systick();

	//Add our bike to the bike list with SN: 1234
	makeBike(1234);

	//Set the direction of an onboard LED for debug purposes
	P1->DIR |= BIT0;

	//Create an our circular buffers
	RFIDBuf = createCircBuf(16);
	LOGBuf  = createCircBuf(256);
	RXBuf   = createCircBuf(17);
	TXBuf   = createCircBuf(17);

	//Make the initial state of the system be the Standby state
	enterState(Standby);

	while(1){
	    //If we want to transmit meaning we have a new user or a minute has passed
	    if(transmitFlag && stationState == Standby){
	        transmitFlag = 0;
	        //loop through all of the bikes we have
	        volatile Bike_t * myBike = bikeList;

	        while(myBike){
	            //Depending on the state of the bike, vary the message sent
	            //If bike is in state 0 or 1 send a heartbeat to see if it is connected
	            if(myBike->inUse == 0 || myBike->inUse == 1){
	                //I succeeded in making a connection
	                if(send_Heartbeat()){
	                    myBike->isPresent = 1;
	                }
	                //Else I failed to get a response
	                else{
	                    myBike->isPresent = 0;
	                }
	            }
	            else if(myBike->inUse == 2){
	                //If I registered the bike. Finish storing the information of the users
	                if(send_RFID(myBike->user->RFID)){
	                    //Tell the bike that it is now in use
	                    myBike->inUse = 1;
	                    //Pair the student with the bike
	                    myBike->user->HASBIKE = myBike->SN;
	                }
	                else{
	                    //Something went wrong, as this state should never be reached.
	                    //Reset the bike's use
	                    myBike->inUse = 0;
	                }
	            }
		    //If the bike is overtime and needs a warning
	            else if(myBike->inUse == 3){
	                //If I get a response, I am ready to deregister the bike at the next transmission
	                if(send_Warning()){
	                    myBike->inUse = 4;
	                }
	            }
		    //If the bike is overtime and is ready to be deregistered
	            else if(myBike->inUse == 4){
	                //If I get a response, the bike is now deregistered. Reset bike and student
	                if(send_RFID("0000000000000000")){
	                    myBike->user->HASBIKE = 0;

	                    myBike->inUse = 0;
	                }
	            }
	            myBike = myBike->nextBike;
	        }
	    }
   	    //If the station receives RFID data begin progressing through the RFID recived logic
	    if(newRFID){
	        newRFID = 0;

	        //Create a string to hold our new data, and create an iterator to populate it
	        uint8_t readRFID[16];
	        uint8_t i = 0;

	        //Store each character in its corresponding place in the string and clear the buff.
	        for(i = 0; i < 16; i++){
	            readRFID[i] = removeItem(RFIDBuf);
	        }

	    //Attempt to find the student we are working with
            Student_t * activeStudent = findStudent(readRFID);

	        //If we didn't find the student, register this new RFID card. Enter Register state
	        if(!activeStudent){
	            enterState(Register);
	            activeStudent = registerStudent(readRFID);
	        }

	        //If we successfully registered, or had already found a student, proceed to checkout
	        if(activeStudent){
		    //Don't let users check out more than one bike
	            if(activeStudent->HASBIKE){
	                enterState(DoubleDipping);
	            }
	            else{
	                enterState(SetTime);
	                checkOutBike(activeStudent);
	            }
	        }
		
		//After we have updated the bike information, return to standby
	        enterState(Standby);


	    }
	    
	    //Constantly poll to see if the bikes need to be deregistered based off of the RTC's last updates 
	    flagDeregister();

	    //If we had a log button interrupt, send the log
	    if(LOGFlag){
	        LOGFlag = 0;
	        sendLog();
	    }

	}
}
