/*
 * Bike.c
 *
 *  Created on: Dec 8, 2017
 *      Author: amabo
 */
#include "Bike.h"
#include <stdlib.h>
#include "Student.h"

extern Bike_t * bikeList;
extern uint8_t  transmitFlag;


volatile uint8_t CTimeReady;
volatile uint8_t CTime;
volatile uint8_t CTimeSTR[3];

void makeBike(uint32_t newSN){
    //First make sure we don't have another bike with that SN in the list
    Bike_t * runner = bikeList;
    while(runner){
        if(runner->SN == newSN){
            return;
        }

        runner = runner->nextBike;
    }

    //Now that we have assured our SN is unique, we can construct the bike object
    Bike_t * newBike = (Bike_t *)malloc(sizeof(Bike_t));
    if(!newBike)
        return;

    //Populate the elements of the bike
    newBike->SN = newSN;        //Store the new SN
    newBike->isPresent = 1;     //Make the default state be that the bike is present
    newBike->inUse = 0;         //Bikes are initially not in use
    newBike->user = NULL;      //Initial user is a Null pointer
    newBike->bikeCounter  = 0;  //Checkout timer is initially zero
    newBike->checkOutTime = 0;  //Checkout time is initially zero

    //Add the new bike to the list of bikes
    newBike->nextBike = bikeList;
    bikeList = newBike;
}

void checkOutBike(Student_t * newStudent){

    //Wait until the user has finished entering their checkout time
    while(!CTimeReady);
    CTimeReady = 0;

    //Go through the bike list looking for bikes that are present and free
    Bike_t * runner = bikeList;
    while(runner){
        //If a bike is both present and free, begin checking it out
        if(!runner->inUse && runner->isPresent){

            //Store the passed user on the bike
            runner->user = newStudent;
            //Checkout the bike on the User's side
            runner->inUse = 2;
            //Store the checkout time and reset the counter
            runner->checkOutTime = CTime;
            runner->bikeCounter = 0;
            transmitFlag = 1;
            return;

        }
        runner = runner->nextBike;
    }
    //Throw error screen.  We didn't find you a bike.
}

///Go through each
inline void flagDeregister(){
    //Make a Bike pointer to go through the bike list and check the state of their timers
    Bike_t * runner = bikeList;
    while(runner){
        //If a bike is presently checkedOut and
        if((runner->inUse == 1)&& (runner->bikeCounter > runner->checkOutTime)){
            runner->inUse = 3;
        }

        runner = runner->nextBike;
    }

}
