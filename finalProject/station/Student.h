/*
 * Student.h
 *
 *  Created on: Dec 6, 2017
 *      Author: amabo
 */

#ifndef STUDENT_H_
#define STUDENT_H_
#include "msp.h"
#include <stdint.h>

typedef struct Student{
    uint8_t SID[4];    //Unique SID
    uint8_t RFID[16];  //Unique RFID
    uint32_t HASBIKE;     //Boolean showing that an error has occurred with this student
    struct Student * prevUser;//Keep a linked list of the students who have used the bike
} Student_t;

//Function takes an RFID number and finds its matching student.  If it cannot find said student, it will return null
Student_t * findStudent(uint8_t readRFID[16]);

//This function will create a new student object and append it to the registry of students
Student_t * registerStudent(uint8_t newRFID[16]);

//In order to store our data, we need to copy strings into each other
//This function copies string two into string one
void stringCopy(uint8_t * s1, uint8_t * s2);

#endif /* STUDENT_H_ */
