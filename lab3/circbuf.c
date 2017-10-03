/*
 *
 * circbuf.c
 *
 * Created on 9-28-17
 * Author: Jack Danielski and Avery Anderson
 *
 */


#include "circbuf.h"
#include <stdlib.h>

CircBuf_t * createCircBuf(uint32_t length) {
    if(length>0){
        CircBuf_t * ourBuf = malloc(sizeof(CircBuf_t));
        ourBuf->buffer = malloc(sizeof(uint8_t)*length);

        ourBuf->length = length;
        resetCircBuf(ourBuf);
        return ourBuf;
    }
    return NULL;
}

void deleteCircBuf(CircBuf_t * buf) {
    if(buf){
        free(buf->buffer);
        free(buf);
    }
}

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
int8_t isFullCircBuff(CircBuf_t * buf) {
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

void addItemCircBuf(CircBuf_t * buf, uint8_t item) {
    if(!buf){
        return;
    }
    else if(isFullCircBuff(buf)){
        return;
    }

    *buf->tail = item;
    buf->tail = (buf->tail - (volatile uint8_t *)buf->buffer + 1)%((uint8_t)buf->length) + (volatile uint8_t *)buf->buffer;
    buf->num_items ++;
}

uint8_t isEmpty(CircBuf_t * buf){
    if(!buf){
        return 1;
    }
    if(buf->num_items == 0){
        return 1;
    }
    return 0;
}

uint8_t removeItem(CircBuf_t * buf) {
    if(!buf){
        return 0xFF;
    }
    if(buf->num_items == 0){
        return 0xFF;
    }
    uint8_t data = *buf->head;
    buf->head = (buf->head - (volatile uint8_t *)buf->buffer + 1)%((uint8_t)buf->length) + (volatile uint8_t *)buf->buffer;
    buf->num_items --;

    return data;
}

