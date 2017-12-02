/*
*
* Wireless Bike Lock - Station
* register.c
*
* 11-20-17
*


#include <Lcd.h>
#include <Register.h>
#include "msp.h"
#include <stdint.h>
#include <stdlib.h>

//extern List_t * userList;
*/
/*
typedef struct {
    uint64_t data;
    uint16_t studentId;
} User_t;

typedef struct{
    User_t * buffer;
    volatile User * head;
    volatile uint32_t num_items;
    volatile uint32_t length; // needs to be 60 for 60 sample sizes
} List_t;
*/
/*
List_t * createList(uint32_t length) {
    if(length>0){
        List_t * newList = malloc(sizeof(List_t));
        if(!newList)
            return NULL;
        newList->buffer = malloc(sizeof(User_t)*length);
        if(!newList->buffer)
            return NULL;
        newList->length = length;
        //resetList(newList);
        return newList;
    }
    return NULL;
}

void deleteList(List_t * list) {
    if(list){
        free(list->buffer);
        free(list);
    }
}

// needs to be fixed
/*
void resetList(List_t * list){
    if(!list){
        return;
    }
    uint32_t i = 0;
    for(i=0; i < list->length; i++){
        list->buffer[i] = 0;
    }
    list->head = list->buffer;
    list->num_items = 0;
}
*/
/*
// Return 1 = Buffer is full, Return 0 = Buffer is not full
int16_t isFullList(List_t * list) {
    if(!list){
        return 0;
    }
    else if (list->num_items == list->length) {
        return 1;
    }
    else {
        return 0;
    }
}

void addItemList(List_t * list, User_t item) {
    if(!list){
        return;
    }
    else if(isFullList(list)){
        removeItem(list);
        P1->OUT |= BIT0;
    }
    *list->buffer = item;
    list->head++;
    list->num_items ++;
}

uint16_t isEmptyList(List_t * list){
    if(!list){
        return 1;
    }
    if(list->num_items == 0){
        return 1;
    }
    return 0;
}

// needs to be fixed
/*
void loadToList(List_t * list, uint8_t * string, uint16_t length){
    if(!list || !string ){
        return;
    }
    volatile uint16_t i;
    for(i = 0; i<length; i++){
        while(isFullList(list)==1);
        addItemList(list, string[i]);
    }

}
*/
/*
User_t removeItemList(List_t * list) {
    //if(!list){
    //    return 0xFF;
    //}
    //if(list->num_items == 0){
    //    return 0xFF;
    //}
    User_t data = *list->head;
    list->head = (list->head - (volatile User_t *)list->buffer + 1)%(list->length) + (volatile User_t *)list->buffer;
    list->num_items --;

    return data;
}

uint16_t idAtoI(uint8_t * studentID) {
    //takes a student id as a string and outputs it as an integer
    uint32_t idInt;
    // studentID is 9 characters long and a 32 bit integer
    //      can hold numbers 10 tens places long
}

uint8_t * getStudentID() {
    uint8_t studentID[9];
    uint8_t msgOne[17] = "Enter StudentID: ";
    //displayText(msgOne);
    // have each uint8_tacter entered fill a corresponding spot in the studentID string
    // instead of writing out the entire alphabet, just count how many times are clicked up or down with pattern
    //      123456789-0-123456789 with that zero being the starting point
    uint8_t msgTwo[11] = "ID accepted";
    return studentID;

}
*/
