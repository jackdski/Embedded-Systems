/*
 * Student.c
 *
 *  Created on: Dec 6, 2017
 *      Author: amabo
 */
#include "Student.h"
#include "Circbuf.h"
#include <stdlib.h>
#include "RFID.h"

extern Student_t * registry;
extern CircBuf_t * RFIDBuf;

volatile uint8_t SIDReady;
extern uint8_t SIDIterator;
uint8_t newSID[4];

//Function takes an RFID number and finds its matching student.  If it cannot find said student, it will return null
Student_t * findStudent(uint8_t readRFID[16]){
    //If we have nothing on our list, we cannot find the student.  Return Null so we know to start the list
    if(!registry){
        return NULL;
    }

    //Now that we know we need to search a list, we need to start searching it.
    Student_t * runner = registry;

    while(runner->prevUser){
        //If we find our RFID tag in the list, return the student that it is tied to
        if(compare_RFID(runner->RFID, readRFID)){
            return runner;
        }

        //Otherwise iterate through the list
        runner = runner->prevUser;
    }
    if(compare_RFID(runner->RFID, readRFID) == 1){
        return runner;
    }

    //If I didn't find a new student, return NULL to add to the front of the list
    return NULL;
}

//This function will create a new student object and append it to the registry of students
Student_t * registerStudent(uint8_t newRFID[16]){

    while(!SIDReady);

    SIDReady = 0;

    //Once we have all of the student information, create the object and store the info
    Student_t * newStudent = (Student_t *)malloc(sizeof(Student_t));
    if(!newStudent)
        return NULL;

    stringCopy(newStudent->SID,newSID);
    stringCopy(newStudent->RFID,newRFID);
    newStudent->HASBIKE = 0;

    //Attach this new student as the head of the list of students, and change the head of students
    newStudent->prevUser = registry;
    registry = newStudent;

    return newStudent;
}

//Copies string two into string one
void stringCopy(uint8_t * s1, uint8_t * s2){
    while(*s1++ = *s2++);
}
