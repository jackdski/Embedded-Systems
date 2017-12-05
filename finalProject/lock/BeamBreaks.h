/*
 * beamBreaks.h
 *
 *  Created on: Oct 28, 2017
 *      Author: amabo
 */

#ifndef BEAMBREAKS_H_
#define BEAMBREAKS_H_
#include "msp.h"

//Configure the BeamBreak sensor
void configure_BeamBreaks();

//These functions enable and disable the interrupts on the BeamBreak sensor.
void enable_BeamBreaks();
void disable_BeamBreaks();
uint8_t beams_Blocked();


#endif /* BEAMBREAKS_H_ */
