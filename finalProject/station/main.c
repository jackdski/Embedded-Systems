/*
* Wireless Bike Lock
* main.c
*
* 11-12-17
*/

#include "msp.h"
#include <stdint.h>
#include "Buttons.h"
#include "Bluetooth.h"
#include "Circbuf.h"
#include "Checkout.h"


/*
 * Wireless Bike Lock Bluetooth Packet Structure
 *
 * [    MSGTYPE     |   RFIDDATA    ]
 * [    1 BYTE      |   16 BYTES    ]
 *              136 BITS
 *              17 BYTES
 */
/*
volatile CircBuf_t * TXBuf;
volatile CircBuf_t * RXBuf;
volatile CircBuf_t * RFIDBuf;
*/

CircBuf_t * TXBuf;
CircBuf_t * RXBuf;
CircBuf_t * RFIDBuf;

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
volatile uint8_t hours = 0;
volatile uint8_t mins = 0;


void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    // configs
    configButtons();
    configure_SystemClock();
    configLED();
    configure_Bluetooth();
    configSystick();

    // test valuse
    uint8_t hours = 0;
    uint8_t mins = 1;
    // must get hours and mins input first
    checkoutTicks = checkoutTimerTicks(hours, mins);

    // Create buffers used
    TXBuf = createCircBuf(17);
    RXBuf = createCircBuf(17);
    RFIDBuf = createCircBuf(16);

    // Heartbeat message
    uint8_t heartbeat[17] = "HXXXXXXXXXXXXXXXX";

    // Cleared RFID message
    uint8_t clearRFID[17] = "00000000000000000";

    // Where the RFID data is stored
    volatile uint8_t rfidData[17] = "00000000000000000";

    volatile uint8_t i;
    volatile uint8_t j;
    uint8_t rfidChecks[16];

    // hardcoded RFID test card
    //loadToBuf(RXBuf,"HXXXXXXXXXXXXXXXX",17);
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
    for(j=0;j<16;j++) {
        addItemCircBuf(RFIDBuf, test[j]);
    }

    while(1) {
        // registerStage == 2 initiates the station to send rfid data
        if(checkoutStage == 2) {
            loadToBuf(TXBuf, heartbeat, 17);
            while(!(isEmpty(TXBuf))) {
                sendByte(removeItem(TXBuf));
            }
            checkoutStage = 0; // reset and return to main screen/loop
        }
        // if timer limit has been reached
        if(timerFull) {
            loadToBuf(TXBuf, clearRFID, 17);
            while(!(isEmpty(TXBuf))) {
                sendByte(removeItem(TXBuf));
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
                    rfidChecks[i] = RFIDBuf->buffer[i+1];
                }
                while(!(isEmpty(RFIDBuf))){
                    sendByte(removeItem(RFIDBuf));
                }
                loadToBuf(RFIDBuf, rfidChecks, 16);
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
                            sendByte(removeItem(RFIDBuf));
                        }
                        loadToBuf(RFIDBuf, rfidChecks, 16);
                    }
                }
                // reset RXBuf since it has to receive something again
                resetCircBuf(RXBuf);
            }
            hbCheck = 0;
        }
    }
}
