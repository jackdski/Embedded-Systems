#include <stdlib.h>

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

#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <stdint.h>
#include "display.h"

Graphics_Context g_sContext;

volatile Student_t * registry;
volatile Bike_t    * bikeList;

volatile CircBuf_t * RFIDBuf;
volatile uint8_t     newRFID = 0;

volatile CircBuf_t * LOGBuf;
volatile uint8_t LOGFlag;

volatile uint8_t  transmitFlag = 0;

volatile State stationState;
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
	configure_Buttons();
	configure_RTC();
	configure_TerminalUART();
	configure_Joystick_TA0();

	GPIO_configure();
	LCD_configure();
	__enable_irq();


	makeBike(1234);

	P1->DIR |= BIT0;


	//Create an RFIDBuffer to hold our 16 chars of RFID data
	RFIDBuf = createCircBuf(16);
	LOGBuf  = createCircBuf(600);


	enterState(Standby);

	while(1){
	    if(transmitFlag){
	        transmitFlag = 0;
	        //call transmit function
	    }
	    if(newRFID){
	        newRFID = 0;

	        //Create a string to hold our new data, and create an iterator to populate it
	        uint8_t readRFID[16];
	        uint8_t i = 0;

	        //Store each character in its corresponding place in the string and clear the buff.
	        for(i = 0; i < 16; i++){
	            readRFID[i] = removeItem(RFIDBuf);
	        }


            Student_t * activeStudent = findStudent(readRFID);

	        //Check to see if we have already registered this student and enter the appropriate state
	        if(!activeStudent){
	            enterState(Register);
	            activeStudent = registerStudent(readRFID);
	        }

	        //If we successfully registered the new student proceed to checkout
	        if(activeStudent){
	            if(activeStudent->HASBIKE){
	                enterState(DoubleDipping);
	            }
	            else{
	                enterState(SetTime);
	                checkOutBike(activeStudent);
	            }
	        }

	        enterState(Standby);


	    }
	    flagDeregister();

	    if(LOGFlag){
	        LOGFlag = 0;
	        sendLog();
	    }

	}
}
