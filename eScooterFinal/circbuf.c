
/*
 *
 * circbuf.c
 * Author: Jack Danielski and Avery Anderson
 *
 */

#include "circbuf.h"
#include <stdlib.h>
#include "msp.h"

extern CircBuf_t * TXBuf;

//Create a circular buffer
CircBuf_t * createCircBuf(uint32_t length) {
    if(length>0){
        CircBuf_t * ourBuf = malloc(sizeof(CircBuf_t));

        if(!ourBuf)
            return NULL;

        ourBuf->buffer = malloc(sizeof(uint16_t)*length);

        if(!ourBuf->buffer)
            return NULL;

        ourBuf->length = length;
        resetCircBuf(ourBuf);
        return ourBuf;
    }
    return NULL;
}

//Delete a cirbuf
void deleteCircBuf(CircBuf_t * buf) {
    if(buf){
        free(buf->buffer);
        free(buf);
    }
}

//reset the pointers and set all of the buf's values to zero
void resetCircBuf(CircBuf_t * buf){
    if(!buf){
        return;
    }
    uint32_t i = 0;
    for(i=0; i < buf->length; i++){
        buf->buffer[i] = 0;
    }
    buf->head = buf->buffer;
    buf->tail = buf->buffer;
    buf->num_items = 0;
}

// Return 1 = Buffer is full, Return 0 = Buffer is not full
int16_t isFullCircBuf(CircBuf_t * buf) {
    if(!buf){
        return 0;
    }
    else if (buf->num_items == buf->length) {
        return 1;
    }
    else {
        return 0;
    }
}

//Add the individual item to the buffer.
void addItemCircBuf(CircBuf_t * buf, uint16_t item) {
    if(!buf){
        return;
    }
    else if(isFullCircBuf(buf)){
        removeItem(buf);
        P1->OUT |= BIT0;
    }

    *buf->tail = item;
    buf->tail = (buf->tail - (volatile uint16_t *)buf->buffer + 1)%(buf->length) + (volatile uint16_t *)buf->buffer;
    buf->num_items ++;
}

//Return 1 if the buffer has no items.
uint16_t isEmpty(CircBuf_t * buf){
    if(!buf){
        return 1;
    }
    if(buf->num_items == 0){
        return 1;
    }
    return 0;
}

//Load a string of information to the buffer
void loadToBuf(CircBuf_t * buf, uint8_t * string, uint16_t length){
    if(!buf || !string ){
        return;
    }
    volatile uint8_t i;
    for(i = 0; i<length; i++){
        while(isFullCircBuf(buf)==1);
        addItemCircBuf(buf, string[i]);
    }

}

//Pop an item from the buffer
uint16_t removeItem(CircBuf_t * buf) {
    if(!buf){
        return 0xFF;
    }
    if(buf->num_items == 0){
        return 0xFF;
    }
    uint16_t data = *buf->head;
    buf->head = (buf->head - (volatile uint16_t *)buf->buffer + 1)%(buf->length) + (volatile uint16_t *)buf->buffer;
    buf->num_items --;

    return data;
}
