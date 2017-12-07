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
 * [    RFIDDATA    ]
 * [    16 BYTES    ]
 *              152 BITS
 *              19 BYTES
 */

volatile CircBuf_t * TXBuf;
volatile CircBuf_t * RXBuf;

volatile uint8_t dummyRfidCheck;
volatile uint8_t dummyTimeCheck;

void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    // configs
    configButtons();
    configure_SystemClock();
    configLED();
    configure_Bluetooth();


    // User List
    //userList = createList(300);
    TXBuf = createCircBuf(200);
    RXBuf = createCircBuf(200);

    uint8_t * dummyRFID = "XXXXXXXXXXXXXXXX";
    uint8_t * dummyTime = "XXX";

    P1->SEL0 &= ~(BIT0);
    P1->SEL1 &= ~(BIT0);
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;

    volatile uint8_t i;
    volatile uint8_t j;
    uint8_t * rfidData; // [16] = "0000000000000000";
    uint8_t * time; // [3] = "000";

    while(1) {
        // If buffer is not being filled check it
        if(!(EUSCI_A2->IFG |= BIT1)) {
            for(i=0;i<16;i++) {
                rfidData[i] = RXBuf->buffer[i];
            }
            for(i=16;i<19;i++){
                time[i-16] = RXBuf->buffer[i];
            }
            dummyRfidCheck = 0;
            dummyTimeCheck = 0;
            for(i=0;i<16;i++){
                if(dummyRFID[i] == rfidData[i]) {
                    dummyRfidCheck++;
                }
            }
            for(i=16;i<19;i++) {
                if(dummyTime[i] == time[i]) {
                    dummyTimeCheck++;
                }
            }
        }
    }
}
