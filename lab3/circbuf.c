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

typedef struct CircBuf_t {
    uint8_t * buffer;               // Pointer to the base of the buffer in heap
    volatile uint8_t * head;        // Pointer to first item
    volatile uint8_t * tail;        // Pointer to last item
    volatile uint32_t num_items;    // Number of items in buffer
    volatile uint32_t length;       // Max number of items the buffer can hold
} CircBuf_t;


CircBuf_t * createCircBuf(uint32_t length) {
    CircBuf_t * ourBuf = malloc(sizeof(CircBuf_t));
    ourBuff->buffer = malloc(sizeof(uint8_t));
    ourBuff->head = 0;
    ourBuff->tail = 0;
    ourBuf->length = length;

}

CircBuf_t clearCircBuf(CircBuf_t * buf) {
    free(buf);
}

// Return 1 = Buffer is full, Return 0 = Buffer is not full
int8_t isFullCircBuff(CircBuf_t * buf) {
    if (buf->num_items == buf->length) {
        return 1;
    }
    else {
        return 0;
    }
}

void addItemCircBuf(CircBuf_t * buf, uint8_t item) {
    int next = buf->head + 1;
    if (next >= buf->length) {
        next = 0;
    }

    buf->buffer[buf->head] = item;
    buf->head = next;
}

uint8_t removeItem(CircBuf_t * buf) {
    int next = buf->tail+1;
    if (next > buf->length) {
        next = 0;
    }
    uint8_t data = buf->buffer[buf->tail];
    buf->tail = next;
}
