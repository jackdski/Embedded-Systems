#include <Buttons.h>
#include "BeamBreaks.h"
#include "msp.h"
#include "Buzzer.h"
#include "Bluetooth.h"
#include "SystemClock.h"
#include "RGB.h"
#include "Solenoid.h"
#include "RFID.h"
#include "State.h"
#include "Circbuf.h"

State lockState;

volatile CircBuf_t * TXBuf;
volatile CircBuf_t * RXBuf;
volatile CircBuf_t * RFIDBuf;

extern uint8_t timedOut;
extern uint8_t checkBeamBreak;
extern uint8_t RGBCount;

uint8_t mainUser[16];   //[13]; // stores RFID data
uint8_t   newRFID = 0;  // Flags that we have stored a new RFID tag

uint8_t unexpectedBeamBreak = 0;
/**
 * main.c
 */
void main(void)
{
    TXBuf   = createCircBuf(17);
    RXBuf   = createCircBuf(17);
    RFIDBuf = createCircBuf(16);

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    /*//Delete This.  It is for debug purposes only
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
    mainUser = test;*/

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
    //P1->DIR |= BIT0;

    // heartbeat message
    uint8_t heartbeat[17] = "HXXXXXXXXXXXXXXXX";
    uint8_t warning[17] = "WXXXXXXXXXXXXXXXX";
    uint8_t clearRFID[17] = "R0000000000000000";
    volatile uint8_t rfidData[16];
    volatile uint8_t hbCheck = 0;
    volatile uint8_t i;

    //loadToBuf((CircBuf_t *)RXBuf, "R1234567890ABCDEF", 17);
    //if(!(isEmpty((CircBuf_t *)TXBuf))) {
    //    EUSCI_A3->IFG |= BIT1;
    //}


    //Initialize lockState to be locked
    enterState(Locked);

    while(1){
       if(lockState == Error){
           Red_LED_On();
           short_buzzes();
        }
        else if(lockState == Locked){
            if(newRFID){
                if(compare_RFID()){
                    enterState(Unlockable);
                }
                else{
                    Red_LED_On();
                    short_buzzes();
                }
                newRFID = 0;
            }

        }
        else if(lockState == Unlockable){
            if(timedOut){
                enterState(Locked);
            }
            if(checkBeamBreak){
                checkBeamBreak = 0;

                if(beams_Blocked()){
                    RGBCount = 0;
                    LED_Off();
                    Blue_LED_On();
                }
                else{
                    enterState(Unlocked);
                }
            }
        }
        else if(lockState == Unlocked){
             if(checkBeamBreak){
                 checkBeamBreak = 0;
                 if(beams_Blocked()){
                     enterState(Locked);
                 }
                 else{
                     short_buzzes();
                 }
             }
        }

       if(isFullCircBuf((CircBuf_t *)RXBuf)) {
                for(i=0;i<17;i++) {
                    rfidData[i] = RXBuf->buffer[i];
                }
                if(rfidData[0] == 'W') {
                    short_buzzes();
                    loadToBuf((CircBuf_t *)TXBuf, rfidData, 17);
                    if(!(isEmpty((CircBuf_t *)TXBuf))) {
                        EUSCI_A3->IFG |= BIT1;
                    }
                    resetCircBuf((CircBuf_t *)RXBuf);
                }
                // if a heartbeat is received send it back
                //if((rfidData[0] == 'H') && (hbCheck == 17)) {
                  if(rfidData[0] == 'H') {
                    loadToBuf((CircBuf_t *)TXBuf, heartbeat, 17);
                    if(!(isEmpty((CircBuf_t *)TXBuf))) {
                        EUSCI_A3->IFG |= BIT1;
                    }
                    resetCircBuf((CircBuf_t *)RXBuf);
                }
                // if rfid data is received set mainUser to it and send it back
                if(rfidData[0] == 'R') {
                    for(i=0;i<16;i++) {
                        mainUser[i] = rfidData[i+1];
                    }
                    loadToBuf((CircBuf_t *)TXBuf, (uint8_t *)rfidData, 17);
                    if(!(isEmpty((CircBuf_t *)TXBuf))) {
                        EUSCI_A3->IFG |= BIT1;
                    }
                    resetCircBuf((CircBuf_t *)RXBuf);
                }
            }
    }
}
