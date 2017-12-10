/*
 * Bike.c
 *
 *  Created on: Dec 8, 2017
 *      Author: amabo
 */
#include "Bike.h"
#include <stdlib.h>

extern Bike_t * bikeList;


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
    newBike->isPresent = 0;     //Make the default state be that the bike is away
    newBike->inUse = 0;         //Bikes are initially not in use
    newBike->user = NULL;      //Initial user is a Null pointer
    newBike->bikeCounter  = 0;  //Checkout timer is initially zero
    newBike->checkOutTime = 0;  //Checkout time is initially zero

    //Add the new bike to the list of bikes
    newBike->nextBike = bikeList;
    bikeList = newBike;
}

///Go through each
inline void flagDeregister(){
    //Make a Bike pointer to go through the bike list and check the state of their timers
    Bike_t * runner = bikeList;
    while(runner){
        //If a bike is presently checkedOut and
        if((runner->inUse == 1)&& (runner->bikeCounter > runner->checkOutTime)){
            runner->inUse = 2;
        }

        runner = runner->nextBike;
    }

}
