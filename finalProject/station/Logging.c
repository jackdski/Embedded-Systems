/*
 * Logging.c
 *
 *  Created on: Dec 10, 2017
 *      Author: amabo
 */
#include "Logging.h"
#include "Circbuf.h"
#include "Bike.h"

extern CircBuf_t * LOGBuf;
extern Student_t * registry;
extern Bike_t    * bikeList;


void configure_TerminalUART(){
    //Configure UART pins, set 2-UART pins to UART mode
    P1->SEL0 |=  (BIT2 | BIT3);
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A0->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A0->BRW = 78;                         //Set Baud Rate
    EUSCI_A0->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    //For logging we only care about TX, so only enable that interrupt
    EUSCI_A0->IFG &= ~BIT1;

    //Turn on interrupts for TX At the peripheral
    UCA0IE |= BIT1;

    //Enable interrupts at the NVIC
    NVIC_EnableIRQ(EUSCIA0_IRQn);
}

void itos(uint32_t num, int8_t size, uint8_t * str ) {
    int8_t i;
    i = size;
    while (i >= 0) {
        if ( i == size) {
            str[i] = '\0';
            i--;
        }
        else {
            str[i] = num % 10;
            str[i] += 48;
            num = num / 10;
            i--;
        }
    }
}

void newLine(CircBuf_t * Buf){
    addItemCircBuf(Buf, 0x0A);
    addItemCircBuf(Buf, 0x0D);//size 34
}


void sendLog(){
    newLine(LOGBuf);
    //Label the first block of information
    loadToBuf(LOGBuf,"REGISTERED STUDENTS:", 20);
    newLine(LOGBuf);

    //Begin the transmission.  Since it is slow, we can continue loading the Buf
    Terminal_send_byte(removeItem(LOGBuf));

    //Create a student runner to print the entire linked list
    Student_t * myStudent = registry;

    //Corner case, there are no students
    if(!myStudent){
        loadToBuf(LOGBuf,"There are no students registered", 32);
        newLine(LOGBuf);
    }
    //If we have students, go through the list printing of their information
    while(myStudent){
        newLine(LOGBuf);

        //Load SID info
        loadToBuf(LOGBuf,"SID: ", 5);
        loadToBuf(LOGBuf,myStudent->SID, 4);
        newLine(LOGBuf);

        //Load RFID info
        loadToBuf(LOGBuf,"RFID: ", 6);
        loadToBuf(LOGBuf,myStudent->RFID + 1, 12);
        newLine(LOGBuf);

        //Print which bike the student has if applicable
        loadToBuf(LOGBuf,"Student has ", 12);
        if(myStudent->HASBIKE){
            loadToBuf(LOGBuf,"bike no. ", 9);

            //Convert the SN to a string and load it
            uint8_t SNStr[5];
            itos(myStudent->HASBIKE,4,SNStr);
            loadToBuf(LOGBuf,SNStr, 4);
        }
        else{
            loadToBuf(LOGBuf,"no bike", 7);
        }
        newLine(LOGBuf);

        myStudent = myStudent->prevUser;
    }

    //Now we need to log all of the bike data
    newLine(LOGBuf);
    loadToBuf(LOGBuf,"BIKE INFORMATION:", 17);
    newLine(LOGBuf);

    //Make a bike runner to pull all of the bike data
    Bike_t * myBike = bikeList;

    //Corner case: There are no students
    if(!myBike){
        loadToBuf(LOGBuf,"There are no bikes", 20);
    }
   while(myBike){
        newLine(LOGBuf);

        loadToBuf(LOGBuf,"Bike SN. ", 9);
        //Convert the SN to a string and load it
        uint8_t SNStr[5];
        itos(myBike->SN,4,SNStr);
        loadToBuf(LOGBuf,SNStr, 4);
        newLine(LOGBuf);

        //Load the bike's presence
        loadToBuf(LOGBuf,"This bike is ", 13);
        if(myBike->isPresent == 1){
            loadToBuf(LOGBuf,"here", 4);
        }
        else{
            loadToBuf(LOGBuf,"gone", 4);
        }
        newLine(LOGBuf);

        //Load the bike's current use state
        loadToBuf(LOGBuf,"This bike is ", 13);
        if(myBike->inUse == 0){
            loadToBuf(LOGBuf,"free", 4);
        }
        else if(myBike->inUse == 1){
            loadToBuf(LOGBuf,"in use", 6);
        }
        else if(myBike->inUse == 2){
            loadToBuf(LOGBuf,"waiting to be registered", 25);
        }
        else if(myBike->inUse == 3){
            loadToBuf(LOGBuf,"overdue", 7);
        }
        else{
            loadToBuf(LOGBuf,"in an error state", 17);
        }
        newLine(LOGBuf);

        //Load the bike's last user
        loadToBuf(LOGBuf,"Bike's User Information ", 24);
        newLine(LOGBuf);

        if(!myBike->user){
            loadToBuf(LOGBuf,"This bike has never been checked out", 36);
        }
        else{
            loadToBuf(LOGBuf,"The SID of the last user of this bike was", 41);
            newLine(LOGBuf);
            loadToBuf(LOGBuf,myBike->user->SID, 4);
        }
        newLine(LOGBuf);


        myBike = myBike->nextBike;
    }

}

inline void Terminal_send_byte(uint8_t data){
    EUSCI_A0->TXBUF = data;
}

void EUSCIA0_IRQHandler(){

    if (EUSCI_A0->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(LOGBuf)){
            EUSCI_A0->IFG &= ~BIT1;
              return;
        }
        Terminal_send_byte(removeItem(LOGBuf));
    }

}
