#include "msp.h"
#include "Buttons.h"
#include "BeamBreaks.h"
#include "Buzzer.h"
#include "Bluetooth.h"
#include "SystemClock.h"
#include "RGB.h"
#include "Solenoid.h"
#include "RFID.h"
#include "State.h"
#include "User.h"
#include "Circbuf.h"

State lockState;

/*
volatile CircBuf_t * TXBuf;
volatile CircBuf_t * RXBuf;
volatile CircBuf_t * RFIDBuf;
*/

CircBuf_t * TXBuf;
CircBuf_t * RXBuf;
CircBuf_t * RFIDBuf;


extern uint8_t timedOut;
extern uint8_t checkBeamBreak;

uint8_t mainUser[16];   // // stores RFID data
uint8_t newRFID = 0;    // Flags that we have stored a new RFID tag

uint8_t unexpectedBeamBreak = 0;


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    TXBuf   = createCircBuf(17);
    RXBuf   = createCircBuf(17);
    RFIDBuf = createCircBuf(16);

    /*
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
    */

    //Call all of our configuration functions
    configure_SystemClock();
    configure_Buzzer();
    configure_LockButton();
    configure_Bluetooth();
    configure_RGB();
    configure_BeamBreaks();
    configure_Solenoid();
    configure_RFID();

    // heartbeat message
    uint8_t heartbeat[17] = "HXXXXXXXXXXXXXXXX";
    uint8_t warning[17] = "WXXXXXXXXXXXXXXXX";
    uint8_t clearRFID[17] = "R0000000000000000";
    //uint8_t testRFID[17] = "R1234567890abcd";

    //Initialize LED 1.0 for debug purposes
    P1->DIR |= BIT0;
    //Initialize lockState to be locked
    enterState(Locked);
    volatile uint8_t rfidData[16];
    volatile uint8_t hbCheck = 0;
    volatile uint8_t i;

        /*
    // test stuff
    loadToBuf(TXBuf, testRFID, 17);
    if(!(isEmpty(TXBuf))) {
        EUSCI_A3->IFG |= BIT1;
    }
    */
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
       // if something has been received check here
       if(isFullCircBuf(RXBuf)) {
           for(i=0;i<17;i++) {
               rfidData[i] = RXBuf->buffer[i];
           }
           if(rfidData[0] == 'W') {
               short_buzzes();
               loadToBuf(TXBuf, rfidData, 17);
               if(!(isEmpty(TXBuf))) {
                   EUSCI_A3->IFG |= BIT1;
               }
               resetCircBuf(RXBuf);
           }
           // if a heartbeat is received send it back
           //if((rfidData[0] == 'H') && (hbCheck == 17)) {
             if(rfidData[0] == 'H') {
               loadToBuf(TXBuf, heartbeat, 17);
               if(!(isEmpty(TXBuf))) {
                   EUSCI_A3->IFG |= BIT1;
               }
               resetCircBuf(RXBuf);
           }
           // if rfid data is received set mainUser to it and send it back
           if(rfidData[0] == 'R') {
               for(i=0;i<16;i++) {
                   mainUser[i] = rfidData[i+1];
               }
               loadToBuf(TXBuf, rfidData, 17);
               if(!(isEmpty(TXBuf))) {
                   EUSCI_A3->IFG |= BIT1;
               }
               resetCircBuf(RXBuf);
           }
       }
    }
}
