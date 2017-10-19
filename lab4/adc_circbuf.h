/*
 *
 * acd_circbuf.h
 * Author: Jack Danielski and Avery Anderson
 * 10-12-17
 *
 */


#ifndef ACD_CIRCBUF_H_
#define ACD_CIRBUF_H_

#include <stdint.h>

typedef struct{
    uint16_t * buffer;
    volatile uint16_t * head;
    volatile uint16_t * tail;
    volatile uint32_t num_items;
    volatile uint32_t length;
}CircBuf_t;

CircBuf_t * createCircBuf(uint32_t length);
void        deleteCircBuf(CircBuf_t * buf);
void        resetCircBuf(CircBuf_t * buf);
int8_t      isFullCircBuf(CircBuf_t * buf);
void        addItemCircBuf(CircBuf_t * buf, uint16_t item);
uint8_t     isEmpty(CircBuf_t * buf);
void        loadToBuf(CircBuf_t * buf, uint8_t * string, uint8_t length);
uint8_t     removeItem(CircBuf_t * buf);


#endif
