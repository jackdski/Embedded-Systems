/*
*
* Wireless Bike Lock
* register.h
*
* 11-12-17
*
*/

#ifndef REGISTER_H_
#define REGISTER_H_

#include "msp.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t data;
    uint16_t studentId;
} User_t;

typedef struct{
    User_t * buffer;
    volatile User_t * head;
    volatile uint32_t num_items;
    volatile uint32_t length; // needs to be 60 for 60 sample sizes
} List_t;

List_t * createList(uint32_t length);
void deleteList(List_t * list);
//void resetList(List_t * list);

// Return 1 = Buffer is full, Return 0 = Buffer is not full
int16_t isFullList(List_t * list);
void addItemList(List_t * list, User_t item);
uint16_t isEmpty(List_t * list);
//void loadToList(List_t * list, uint8_t * string, uint16_t length);
User_t removeItem(List_t * list);


#endif
