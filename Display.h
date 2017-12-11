/*
 * Display.h
 *
 *  Created on: Dec 10, 2017
 *      Author: amabo
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

// sets up LCD for size and resolution
void LCD_configure();

// Defines the specifications of the slide-0 state
void clearSlide();

// Defines "Welcome" screen instructing RFID bump
void welcomeScreen();

// fun logo for slideshow during welcome
void universityScreen();

// Register studentID using joystick and buttons
void registerStudent();

// input timer configurations for duration of use
void checkout();

// thank you slide "Bike is buzzing"
void thankYou();

// no bikes available
// also an Incredibles reference!!
void noBikes();

// reset screen for blue background with white text
void goBlue();

// reset screen to (default) black background with white text
void goBlack();

// Error: 404 slide
void error();

// for users already registered, tryna get another bike
void noDoubleDipping();


#endif /* DISPLAY_H_ */
