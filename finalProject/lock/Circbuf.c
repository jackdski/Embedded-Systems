/*
 * Wireless Bikelock - Lock
 * Circbuf.c
 *
 * Created on 12-2-17
 *
 */

#include <stdlib.h>
#include "Circbuf.h"

// Function to create a circle buffer of a specified length to a prexisitng CircBuf
CircBuf_t * createCircBuf(uint32_t length) {
    if(length>0){
        CircBuf_t * ourBuf = malloc(sizeof(CircBuf_t));

        if(!ourBuf)
            return NULL;

        ourBuf->buffer = malloc(sizeof(uint8_t)*length);

        if(!ourBuf->buffer)
            return NULL;

        ourBuf->length = length;
        resetCircBuf(ourBuf);
        return ourBuf;
    }
    return NULL;
}

// Delete a CircBuf
void deleteCircBuf(CircBuf_t * buf) {
    if(buf){
        free(buf->buffer);
        free(buf);
    }
}
// Reset CircBuf by setting everything inside of
// the buffer equal to zero, moving the head and tail,
// and seting the number of items in the buffer to zero
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
inline int8_t isFullCircBuf(CircBuf_t * buf) {
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

// add an item to the tail of the circBuf
void addItemCircBuf(CircBuf_t * buf, uint8_t item) {
    if(!buf){
        return;
    }
    else if(isFullCircBuf(buf)){
        return;
    }

    *buf->tail = item;
    buf->tail = (buf->tail - (volatile uint8_t *)buf->buffer + 1)%(buf->length) + (volatile uint8_t *)buf->buffer;
    buf->num_items ++;
}

// check if the CircBuf has and items in it
uint8_t isEmpty(CircBuf_t * buf){
    if(!buf){
        return 1;
    }
    if(buf->num_items == 0){
        return 1;
    }
    return 0;
}

// add a string of a certain length to the buffer
void loadToBuf(CircBuf_t * buf, uint8_t * string, uint8_t length){
    if(!buf || !string ){
        return;
    }
    volatile uint8_t i;
    for(i = 0; i<length; i++){
        while(isFullCircBuf(buf)==1);
        addItemCircBuf(buf, string[i]);
    }

}

// remove and return a piece of data from the circBuf
uint8_t removeItem(CircBuf_t * buf) {
    if(!buf){
        return 0xFF;
    }
    if(buf->num_items == 0){
        return 0xFF;
    }
    uint8_t data = *buf->head;
    buf->head = (buf->head - (volatile uint8_t *)buf->buffer + 1)%(buf->length) + (volatile uint8_t *)buf->buffer;
    buf->num_items --;

    return data;
}
