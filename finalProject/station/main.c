/*
* Wireless Bike Lock
* main.c
*
* 11-12-17
*/

#include "msp.h"
#include "Bluetooth.h"
#include "Borrow.h"
#include "Buttons.h"
#include "Lcd.h"
//#include "Register.h"
#include "Circbuf.h"


/*
 * Wireless Bike Lock Bluetooth Packet Structure
 *
 * [ DIRECTION | CMDTYPE | CMDDATA | CHKSUM  ]
 * [  8 bits   | 8 bits  | 128 bits| 24 bits ]
 *                  168 bits per message
 *                  21 bytes per message
 */
// Direction stuff
//#define STOL (0X00)
//#define LTOS (0x11)

// Command types so receiver knows what to do with this data
#define HEARTBEAT   (0x01) // ack request with dummy data
#define RFIDDATA    (0X02) // contains rfid data to give to lock
#define DELETERFID  (0X03) // delete the rfid data currently stored
#define CHKSUMVALID (0x04) // the checksum is correct
#define CHKSUMBAD   (0x05) // the checksum is incorrect
#define RETURNHB    (0x06) // ack return with dummy data
#define RETURNRFID  (0x07) // says it got RFID data correctly

volatile CircBuf_t * TXBuf;
volatile CircBuf_t * RXBuf;

volatile uint8_t checkPacket = 0;

//List_t * userList;

void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    // configs
    //configButtons();
    configure_SystemClock();
    configLED();
    configure_Bluetooth();

    // User List
    //userList = createList(300);
    TXBuf = createCircBuf(21);
    RXBuf = createCircBuf(21);

    P1->SEL0 &= ~(BIT0);
    P1->SEL1 &= ~(BIT0);
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;

    volatile uint8_t i;
    volatile uint8_t j;
    BTCMD parsed;
    uint8_t check;

    while(1) {
        // If buffer is not being filled check it
        if(!(EUSCI_A2->IFG |= BIT1)) {
            parsed = parseBTCMD();
            check = checksum(parsed);
            if(check){
                //do stuff with data

            }
            else {
                sendCheckSumBad();
            }
        }
        check = 0;
    }
}
