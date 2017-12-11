/*
 * Bike.h
 *
 *  Created on: Dec 8, 2017
 *      Author: amabo
 */

#ifndef BIKE_H_
#define BIKE_H_
#include "msp.h"
#include <stdint.h>
#include "Student.h"

typedef struct Bike{
    uint32_t SN;          //Serial Number of the bike
    uint8_t isPresent;    //Boolean showing the bike is here
    //Use indicator.
    //0 not in use, 1 in use and checked out, 2 checked out at station, not at bike. 3 overtime
    uint8_t inUse;
    Student_t * user;     //Pointer to the student using the bike
    uint8_t bikeCounter;  //Counter that is incremented each minute.  Determines checkout time
    uint8_t checkOutTime; //The end time for each checkout. Compared to bikeCounter
    struct Bike *  nextBike;     //Bikes will form a linked list
}Bike_t;

void makeBike(uint32_t SN);

void checkOutBike(Student_t * newStudent);

void flagDeregister();

#endif /* BIKE_H_ */
