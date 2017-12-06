/*
 * Wireless Bike Lock - Lock
 * Bluetooth.c
 *
 */

#include "msp.h"
#include "Bluetooth.h"
#include "Circbuf.h"
#include <stdint.h>

extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;

/*
typedef struct {
    uint8_t direction;
    uint8_t cmd;
    uint8_t * data;
    uint8_t * checksum;
} BTCMD;
*/

void configure_SystemClock(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.
}

//Connect 3.3 to the RX
void configure_Bluetooth(){
    //Configure UART pins, set 2-UART pins to UART mode
    P3->SEL0 |=  (BIT2 | BIT3);
    P3->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A2->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A2->BRW = 78;                          //Set Baud Rate
    EUSCI_A2->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A2->IFG &= ~(BIT1 | BIT0);
    UCA2IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA2_IRQn);
}

void bluetooth_send_n(uint8_t * data, uint8_t length){
    //Code to iterate through the transmit data
    if(!data)
        return;
    volatile uint8_t n;
    for(n = 0; n<length; n++){
        sendByte(data[n]);
    }
}

void sendByte(uint8_t data){
    EUSCI_A2->TXBUF = data;
    //P1->OUT ^= BIT0;
}

void sendBTCMD(BTCMD cmd) {
    addItemCircBuf(TXBuf, cmd.direction);
    addItemCircBuf(TXBuf, cmd.cmd);
    // add every character of cmd.data
    volatile uint8_t n;
    for(n = 0; n < 16; n++) {
        addItemCircBuf(TXBuf, cmd.data[n]);
    }
    addItemCircBuf(TXBuf, cmd.checksum);
}

void sendHeartbeat() {
    BTCMD heartbeat;
    heartbeat.direction = STOL;
    heartbeat.cmd = HEARTBEAT;
    heartbeat.data = "0000000000000000";
    heartbeat.checksum = '152';
    sendBTCMD(heartbeat);
}

void sendRFIDData(uint8_t * rfiddata) {
    BTCMD rfidsend;
    rfidsend.direction = STOL;
    rfidsend.cmd = RFIDDATA;
    rfidsend.data = rfiddata;
    rfidsend.checksum = '152';
    sendBTCMD(rfidsend);
}

void sendDeleteRFID() {
    BTCMD delete;
    delete.direction = STOL;
    delete.cmd = DELETERFID;
    delete.data = "0000000000000000";
    delete.checksum = '152';
    sendBTCMD(delete); // send the command with the above data
}

void sendChecksumValid() {
    BTCMD checksumvalid;
    checksumvalid.direction = LTOS;
    checksumvalid.cmd = CHKSUMVALID;
    checksumvalid.data = "0000000000000000";
    checksumvalid.checksum = '152';
    sendBTCMD(checksumvalid);
}

void sendChecksumBad() {
    BTCMD checksumbad;
    checksumbad.direction = LTOS;
    checksumbad.cmd = CHKSUMBAD;
    checksumbad.data = "0000000000000000";
    checksumbad.checksum = '152';
    sendBTCMD(checksumbad);
}

void sendReturnHB() {
    BTCMD returnHB;
    heartbeat.direction = lTOS;
    heartbeat.cmd = RETURNHB;
    heartbeat.data = "0000000000000000";
    heartbeat.checksum = '152';
    sendBTCMD(returnHB);
}

void sendReturnRFID() {
    BTCMD returnRFID;
    returnRFID.direction = lTOS;
    returnRFID.cmd = RETURNRFID;
    returnRFID.data = "0000000000000000";
    returnRFID.checksum = '152';
    sendBTCMD(returnRFID);
}

// 1 == OK, 0 == NOT OK
void checksum(BTCMD * cmd) {
    uint8_t i;
    uint16_t calcChecksum = 0;
    uint8_t * ptr = (uint8_t *)cmd;
    for(i = 0; i < sizeof(BTCMD) - 1; i++) {
        calcChecksum += *ptr;
        ptr++;
    }
    if(cmd->checksum == calcChecksum) {
        //sendChecksumValid();
        return 1;
    }
    else {
        //sendChecksumBad();
        return 0;
    }
}
/*
 * Wireless Bike Lock Bluetooth Packet Structure
 *
 * [ DIRECTION | CMDTYPE | CMDDATA | CHKSUM  ]
 * [  8 bits   | 8 bits  | 128 bits| 24 bits ]
 *                  168 bits per message
 *                  21 bytes per message
 */
BTCMD parseBTCMD() {
    BTCMD parse;
    uint8_t i;
    parse.direction = RXBuf->buffer[0];
    parse.cmd = RXBuf->buffer[1];
    for(i=2;i<18;i++){
        parse.data[i] = RXBuf->buffer[i];
    }
    for(i=18;i<21;i++) {
        parse.checksum[i] = RXBuf->buffer[i];
    }
    return parse;
}

void EUSCIA2_IRQHandler(){
    if (EUSCI_A2->IFG & BIT0){
        addItemCircBuf(RXBuf, EUSCI_A2->RXBUF);
    }
    if (EUSCI_A2->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)) {
            EUSCI_A2->IFG &= ~BIT1;
            return;
        }
        EUSCI_A2->TXBUF = removeItem(TXBuf);
        P1->OUT ^= BIT0;
    }

}
