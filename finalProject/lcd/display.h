/*
 * display.h
 *
 *  Created on: Dec 6, 2017
 *      Author: Stefan
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

// sets up LCD for size and resolution
void LCD_configure();

// Defines the specifications of the slide-0 state
void clearSlide();

// Defines slide-1 "Welcome"
void welcomeScreen();

// Register studentID using joystick and buttons
void registerStudent();

// input timer configurations for duration of use
void checkout();

// thank you slide "Bike is buzzing"
void thankYou();

// no bikes available
void noBikes();

// Error: 404 slide
void error();


#endif /* DISPLAY_H_ */
