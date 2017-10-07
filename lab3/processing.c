/*
 * processing.c
 *
 *  Created on: Oct 3, 2017
 *      Author: amabo
 */
#include "processing.h"
#include "circbuf.h"
#include "uart.h"

extern uint8_t transmit;
extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;

extern uint32_t alp; //Number of alphabetical chars
extern uint32_t pun; //Number of punctuation chars
extern uint32_t num; //Number of numerical chars
extern uint32_t whi; //Number of white chars
extern uint32_t ran; //Number of random chars



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
    P1->OUT ^= BIT0;
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

    itoa(alp,alps);
    itoa(num,nums);
    itoa(pun,puns);
    itoa(whi,whis);
    itoa(ran,rans);

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

    //COPY RX STILL

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

void itoa(uint32_t num, uint8_t * str){
    str[2] = (uint32_t)num%10;
    num = num/10;
    str[1] = (uint32_t)num%10;
    num = num/10;
    str[0] = (uint32_t)num%10;
    num = num/10;

    str[0] += 48;
    str[1] += 48;
    str[2] += 48;
}

