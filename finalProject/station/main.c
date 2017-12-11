/*
* Wireless Bike Lock
* main.c
*
* 11-12-17
*/


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

/*
 * Wireless Bike Lock Bluetooth Packet Structure
 *
 * [    MSGTYPE     |   RFIDDATA    ]
 * [    1 BYTE      |   16 BYTES    ]
 *              136 BITS
 *              17 BYTES
 */

volatile Student_t * registry;
volatile Bike_t    * bikeList;

volatile CircBuf_t * TXBuf;
volatile CircBuf_t * RXBuf;
volatile CircBuf_t * RFIDBuf;
volatile CircBuf_t * LOGBuf;

volatile uint8_t LOGFlag;
volatile uint8_t transmitFlag = 0;

volatile uint8_t newRFID = 0;
volatile State stationState;


// counts similar characters between what was received
//      and the heartbeat message
volatile uint8_t hbCheck = 0;

/*
 * 0 = main screen
 * 1 = student ID has been entered
 * 2 = RFID tag data has been entered and saved
 */
volatile uint8_t registerStage = 0;

/*
 * 0 = main screen
 * 1 = RFID tag data has been entered and saved to RFIDBuf
 * 2 = Time has been entered and saved
 */
volatile uint8_t checkoutStage = 0;

// 1 if the station gets a HB message back
// 0 otherwise
volatile uint8_t receivedHB = 0;

// 1 if timer has hit its limit
volatile uint8_t timerFull = 0;

volatile uint32_t checkoutTicks;
//volatile uint32_t overtime;
volatile uint8_t systickcount = 0;


void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //Initialize the registry to have no students
    registry = NULL;

    // configs
    configure_RFID();
    configure_SystemClock();
    configure_Systick()
    configure_Buttons();
    configure_RTC();
    configure_TerminalUART();
    configure_Joystick_TA0();
    configureLED();
    configure_serial_port();
    configAltBT();

    __enable_irq();

    // Create buffers used
    makeBike(1234);

    TXBuf = createCircBuf(17);
    RXBuf = createCircBuf(17);
    RFIDBuf = createCircBuf(16);

    enterState(Standby);

    uint8_t heartbeat[17] = "HXXXXXXXXXXXXXXXX"; // Heartbeat message
    uint8_t clearRFID[17] = "00000000000000000"; // Cleared RFID message

    // Where the RFID data is stored
    volatile uint8_t rfidData[17] = "00000000000000000";

    volatile uint8_t i;
    volatile uint8_t j;
    uint8_t rfidChecks[16];

    /*
    // test for Bluetooth/UART
    loadToBuf(TXBuf, heartbeat, 17);
    if(!(isEmpty(TXBuf))){
        EUSCI_A0->IFG |= BIT1;
    }
    */

    while(1) {
        if(newRFID){
            newRFID = 0;
            //Create a string to hold our new data, and create an iterator to populate it
            uint8_t readRFID[16];
            volatile uint8_t i = 0;

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
        if(transmitFlag && (stationState == Standby)) {
            if(bikeList->inUse == (0 || 1)) {
                loadToBuf(TXBuf, heartbeat, 17);
                if(!(isEmpty(TXBuf))) {
                    EUSCI_A0->IFG |= BIT1;
                }
            }
            if(bikeList->inUse == 2) {
                if(send_registration(bikeList)) {
                    bikeList->isPresent = 1;
            }
            transmitFlag = 0;
        }
    }


        /*
    while(1) {
        // checkoutStage == 2 initiates the station to send rfid data
        if(checkoutStage == 2) {
            loadToBuf(TXBuf, heartbeat, 17);
            while(!(isEmpty(TXBuf))) {
                UART_send_byte(removeItem(TXBuf));
            }
            checkoutStage = 0; // reset and return to main screen/loop
        }
        // if timer limit has been reached
        if(timerFull) {
            loadToBuf(TXBuf, clearRFID, 17);
            while(!(isEmpty(TXBuf))) {
                UART_send_byte(removeItem(TXBuf));
            }
        }
        // If RX buffer is fill check what is in it
        if(isFullCircBuf(RXBuf)) {
            // load rfidData with what is in RXBuf
            for(i=0;i<17;i++) {
                rfidData[i] = RXBuf->buffer[i];
            }
            // count how many characters it has in common with the heartbeat msg
            for(i=0;i<17;i++){
                if(heartbeat[i] == rfidData[i]) {
                    hbCheck++;
                }
            }
            // if station receives heartbeat msg back, send RFID data
            if((rfidData[0] == 'H') && (hbCheck == 17)) {
                for(j=0;j<16;j++) {
                    // if characters do not match at any point resend rfid data
                    rfidChecks[j] = RFIDBuf->buffer[j+1];
                    P2->IFG |= BIT2;
                    P1->IFG |= BIT0;
                }
                while(!(isEmpty(RFIDBuf))){
                    UART_send_byte(removeItem(RFIDBuf));
                }
                loadToBuf(RFIDBuf, rfidChecks, 16);
                P1->OUT |= BIT1;
                P2->OUT |= BIT1;
                //resetCircBuf(RXBuf);
            }
            // if not a heartbeat msg, check if it is the correct RFID data
            // if not the correct RFID data, send the data again
            //else if (rfidData[0] == 'R') {
            if(rfidData[0] == 'R') {
                //uint8_t checkReturnRFID = 0;
                for(j=0;j<16;j++) {
                    rfidChecks[i] = RFIDBuf->buffer[i+1];
                }
                for(j=0;j<16;j++) {     // run through and check each character
                    // if characters do not match at any point resend rfid data
                    if(rfidData[j+1] != rfidChecks[j]) {
                        while(!(isEmpty(RFIDBuf))){
                            UART_send_byte(removeItem(RFIDBuf));
                        }
                        loadToBuf(RFIDBuf, rfidChecks, 16);
                    }
                }
                // reset RXBuf since it has to receive something again
                //resetCircBuf(RXBuf);
            }
            hbCheck = 0;
        }
        */
    }
}
