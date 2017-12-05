#include <Buttons.h>
#include "BeamBreaks.h"
#include "Checkout.h"
#include "msp.h"
#include "Buzzer.h"
#include "Bluetooth.h"
#include "SystemClock.h"
#include "RGB.h"
#include "Solenoid.h"
#include "RFID.h"
#include "State.h"
#include "User.h"
#include "Circbuf.h"


uint32_t systickCounter = 0;        //counts how man 0.5s have passed
uint32_t checkoutTimerTicksVal = 0; //how many ticks the bike is being taken out for
uint32_t overtime = 0;              //how many ticks over the above time the bike is out
uint8_t  hours = 0;                 //how many hours the bike is out for
uint8_t  mins = 0;                  //how many mins the bike is out for

State lockState;

volatile CircBuf_t * TXBuf;
volatile CircBuf_t * RXBuf;
volatile CircBuf_t * RFIDBuf;

extern uint8_t timedOut;
extern uint8_t checkBeamBreak;

uint8_t * mainUser;   //[13]; // stores RFID data
uint8_t   newRFID = 0;  // Flags that we have stored a new RFID tag

uint8_t unexpectedBeamBreak = 0;
/**
 * main.c
 */
void main(void)
{
    TXBuf   = createCircBuf(20);
    RXBuf   = createCircBuf(20);
    RFIDBuf = createCircBuf(16);

    //Delete This.  It is for debug purposes only
    uint8_t test[16];
    test[0]  = 0x02;
    test[1]  = 0x31;
    test[2]  = 0x38;
    test[3]  = 0x30;
    test[4]  = 0x30;
    test[5]  = 0x38;
    test[6]  = 0x33;
    test[7]  = 0x43;
    test[8]  = 0x44;
    test[9]  = 0x34;
    test[10] = 0x31;
    test[11] = 0x31;
    test[12] = 0x37;
    test[13] = 0x0D;
    test[14] = 0x0A;
    test[15] = 0x03;
    mainUser = test;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//Call all of our configuration functions
	configure_SystemClock();
	configure_Buzzer();
	configure_LockButton();
	configure_Bluetooth();
	configure_RGB();
	configure_BeamBreaks();
	configure_Solenoid();
	configure_RFID();

	//Initialize LED 1.0 for debug purposes
	P1->DIR |= BIT0;

	//Initialize lockState to be locked
	enterState(Locked);

	while(1){
	   if(lockState == Error){
	       Red_LED_On();
	       short_buzzes();
	    }
	    else if(lockState == Locked){
	        if(newRFID){
	            newRFID = 0;

	            if(compare_RFID()){
	                enterState(Unlockable);
	            }
	            else{
	                Red_LED_On();
	                short_buzzes();
	            }
	        }

	    }
	    else if(lockState == Unlockable){
	        if(timedOut){
	            enterState(Locked);
	        }
	    }
	    else if(lockState == Unlocked){
	         if(checkBeamBreak){
	             if(beams_Blocked()){
	                 enterState(Locked);
	             }
	         }
	    }
	}
}
