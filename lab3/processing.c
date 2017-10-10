/*
 * processing.c
 *
 *  Created on: Oct 3, 2017
 *      Author: amabo
 */
#include "processing.h"
#include "circbuf.h"
#include "uart.h"
#include "conversions.h"
#include "eScooter.h"

extern uint8_t transmit;
extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;

extern uint32_t alp; //Number of alphabetical chars
extern uint32_t pun; //Number of punctuation chars
extern uint32_t num; //Number of numerical chars
extern uint32_t whi; //Number of white chars
extern uint32_t ran; //Number of random chars

extern es_V * myScooter;
extern uint8_t updateDistance;

volatile uint32_t words = 0;
volatile uint8_t lastChr = ' ';

void configurePorts(){
    P1->SEL0 &= ~(BIT1 | BIT4);
    P1->SEL1 &= ~(BIT1 | BIT4);
    P1->DIR  &= ~(BIT1 | BIT4);
    P1->REN  |=  (BIT1 | BIT4);
    P1->OUT  |=  (BIT1 | BIT4);
    P1->IES  |=  (BIT1 | BIT4);

    P1->DIR |= BIT0;

    P1->IFG   = 0;
    P1->IE   |=  (BIT1 | BIT4);
    NVIC_EnableIRQ(PORT1_IRQn);
}

void PORT1_IRQHandler(){
    if(P1->IFG & BIT1){
        resetCircBuf(RXBuf);
    }
    else if(P1->IFG & BIT4){
        transmit = 1;
    }
    //P1->OUT ^= BIT0;
    updateDistance = 1;
    P1->IFG = 0;
}

void analyzeBuf(){
    if(isEmpty(RXBuf)){
        return;
    }
    volatile uint32_t length = RXBuf->num_items;
    volatile uint32_t i;
    uint8_t temp;

    for(i = 0; i < length; i++){
        temp = removeItem(RXBuf);
        addItemCircBuf(RXBuf, temp);
        analyzeChr(temp);
    }

    uint8_t * str1 = "We saw";
    uint8_t * str2 = " letters";
    uint8_t * str3 = " punctuation marks";
    uint8_t * str4 = " numbers";
    uint8_t * str5 = " white-space characters";
    uint8_t * str6 = " other characters";

    uint8_t alps[3];
    uint8_t nums[3];
    uint8_t puns[3];
    uint8_t whis[3];
    uint8_t rans[3];

    itoa(alp,3,alps);
    itoa(num,3,nums);
    itoa(pun,3,puns);
    itoa(whi,3,whis);
    itoa(ran,3,rans);

    alp = 0;
    num = 0;
    pun = 0;
    whi = 0;
    ran = 0;

    loadToBuf(TXBuf,str1, 6);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,alps, 3);

    loadToBuf(TXBuf,str2, 8);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,puns, 3);

    loadToBuf(TXBuf,str3, 18);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,nums, 3);

    loadToBuf(TXBuf,str4, 8);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,whis, 3);


    loadToBuf(TXBuf,str5, 23);
    addItemCircBuf(TXBuf, 0x0A);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,rans, 3);

    loadToBuf(TXBuf,str6, 17);
    addItemCircBuf(TXBuf, 0x0A);
    addItemCircBuf(TXBuf, 0x0D);

    addItemCircBuf(TXBuf, 0x0A);

    UART_send_byte(removeItem(TXBuf));


    //UART_send_byte(removeItem(TXBuf));
    while(!isEmpty(TXBuf));
    for(i = 0; i < length; i++){
        addItemCircBuf(TXBuf, removeItem(RXBuf));
    }
    UART_send_byte(removeItem(TXBuf));

}


void analyzeChr(uint8_t chr){
    if (chr >= 'A' && chr <= 'Z') {
        alp++;
    }
    else if (chr >= 'a' && chr <= 'z') {
        alp++;
    }
    else if (chr == 33 || chr == 46 || chr == 63 || chr == 44
                       || chr == 39 || chr == 45) {
        pun++;
    }
    else if (chr >= 48 && chr <= 57) {
        num++;
    }

    else if (chr == 32 || chr == 9) {
        whi++;
    }
    else {
        ran++;
    }
}

void analyzeChrEC(uint8_t chr){

    if(isWhiteSpace(lastChr) && (isWhiteSpace(chr)==0) ){
        words++;
    }
    lastChr = chr;

    if (chr >= 'A' && chr <= 'Z') {
        alp++;
    }
    else if (chr >= 'a' && chr <= 'z') {
        alp++;
    }
    else if (chr == 33 || chr == 46 || chr == 63 || chr == 44
                       || chr == 39 || chr == 45) {
        pun++;
    }
    else if (chr >= 48 && chr <= 57) {
        num++;
    }

    else if (isWhiteSpace(chr)) {
        whi++;
    }
    else {
        ran++;
    }
}

uint8_t isWhiteSpace(uint8_t chr){
    if (chr == 32 || chr == 9 || chr == 10 || chr == 13) {
        return 1;
    }
    return 0;
}


void transmitEC(){
    uint8_t * str1 = "We saw";
    uint8_t * str2 = " letters";
    uint8_t * str3 = " punctuation marks";
    uint8_t * str4 = " numbers";
    uint8_t * str5 = " white-space characters";
    uint8_t * str6 = " other characters";
    uint8_t * str7 = " words total";

    uint8_t alps[10];
    uint8_t nums[10];
    uint8_t puns[10];
    uint8_t whis[10];
    uint8_t rans[10];
    uint8_t wordss[10];

    itoa(alp,10,alps);
    itoa(num,10,nums);
    itoa(pun,10,puns);
    itoa(whi,10,whis);
    itoa(ran,10,rans);
    itoa(words,10,wordss);

    alp = 0;
    num = 0;
    pun = 0;
    whi = 0;
    ran = 0;
    words = 0;
    lastChr = ' ';

    loadToBuf(TXBuf,str1, 6);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,alps, 10);

    loadToBuf(TXBuf,str2, 8);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,puns, 10);

    loadToBuf(TXBuf,str3, 18);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,nums, 10);

    loadToBuf(TXBuf,str4, 8);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,whis, 10);


    loadToBuf(TXBuf,str5, 23);
    addItemCircBuf(TXBuf, 0x0A);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,rans, 10);

    loadToBuf(TXBuf,str6, 17);
    addItemCircBuf(TXBuf, 0x0A);
    addItemCircBuf(TXBuf, 0x0D);

    loadToBuf(TXBuf,wordss, 10);

    loadToBuf(TXBuf,str7, 12);
    addItemCircBuf(TXBuf, 0x0A);
    addItemCircBuf(TXBuf, 0x0D);

    addItemCircBuf(TXBuf, 0x0A);

    UART_send_byte(removeItem(TXBuf));
}
