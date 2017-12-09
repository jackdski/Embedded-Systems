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

volatile uint8_t hbCheck = 0; // if this starts with H

/*
 * 0 = main screen
 * 1 = student ID has been entered
 * 2 = RFID tag data has been entered
 */
volatile uint8_t registerStage = 0;

// 1 if the station gets a HB message back
// 0 otherwise
volatile uint8_t receivedHB = 0;

void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    // configs
    configButtons();
    configure_SystemClock();
    configLED();
    configure_Bluetooth();


    // User List
    //userList = createList(300);
    TXBuf = createCircBuf(17);
    RXBuf = createCircBuf(17);
    RFIDBuf = createCircBuf(16);

    uint8_t heartbeat[17] = "HXXXXXXXXXXXXXXXX";

    P1->SEL0 &= ~(BIT0);
    P1->SEL1 &= ~(BIT0);
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;

    volatile uint8_t i;
    volatile uint8_t j;
    volatile uint8_t rfidData[17] = "00000000000000000";

    loadToBuf(RXBuf,"HXXXXXXXXXXXXXXXX",17);
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
            if((heartbeat[0] == 'H') && (hbCheck == 17)) {
                /*
                loadToBuf(TXBuf, "HXXXXXXXXXXXXXXXX", 17);
                while(!(isEmpty(TXBuf))) {
                    sendByte(removeItem(TXBuf));
                }
                */
                while(!(isEmpty(RFIDBuf))){
                    sendByte(removeItem(RFIDBuf));
                }
            }
            else{
                uint8_t * rfidString;
                for(j=1;j<17;j++){
                    rfidString[j-1] = RXBuf->buffer[j];
                }
                loadToBuf(RFIDBuf, rfidString, 16);
                //EUSCI_A2->IFG |= BIT1;
                sendByte(removeItem(TXBuf));
            }
            hbCheck = 0;
        }
        // registerStage == 2 initiates the station to send rfid data
        if((registerStage == 2) && (isFullCircBuf(RXBuf))) {
            if(receivedHB) {
                // assume RFIDBuf is already loaded
                EUSCI_A2->IFG |= BIT1;
                sendByte(removeItem(RFIDBuf));
            }
            registerStage == 0; // reset and return to main screen/loop
        }
    }
}
