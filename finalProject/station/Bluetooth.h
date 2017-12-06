/*
 * Wireless Bike Lock - Station
 * Bluetooth.h
 *
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "msp.h"
#include "Circbuf.h"

// Direction stuff
#define STOL (0X00)
#define LTOS (0xFF)

// Command types so receiver knows what to do with this data
#define HEARTBEAT   (0x1) // ack command with dummy data
#define RFIDDATA    (0X2) // contains rfid data to give to lock
#define DELETERFID  (0X3) // delete the rfid data currently stored
#define CHKSUMVALID (0x4) // the checksum is correct
#define CHKSUMBAD   (0x5) // the checksum is incorrect
#define RETURNHB    (0x06) // ack return with dummy data
#define RETURNRFID  (0x07) // says it got RFID data correctly

extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;

typedef struct {
    uint8_t direction;
    uint8_t cmd;
    uint8_t * data;
    uint8_t * checksum;
} BTCMD;

void configure_SystemClock();

void configure_Bluetooth();

void sendByte(uint8_t data);

void sendBTCMD(BTCMD cmd);

void sendHeartbeat();

void sendRFIDData(uint8_t * rfiddata);

void sendDeleteRFID();

uint8_t checksum(BTCMD * cmd);

void bluetooth_send_n(uint8_t * data, uint8_t length);


#endif
