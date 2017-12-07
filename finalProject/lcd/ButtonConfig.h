/*
 * ButtonConfig.h
 *
 *  Created on: Dec 2, 2017
 *      Author: Stefan
 */

#ifndef BUTTONCONFIG_H_
#define BUTTONCONFIG_H_

/*Purpose: General pin and LED configure
 *
 *
 * BoosterPack:
 *     T&B buttons(T:J4.33  B:J4.32)
 *          MSP mapping(T:P5.1  B:P3.5)
 *     Joystick axis(X:J1.2  Y:J3.26)
 *          MSP mapping(X:P6.0  Y:P4.4)
 *     Joystick button(J1.5)
 *          MSP mapping(P4.1)
 *
 */

// Configuration for Buttons and Joystick
void GPIO_configure(void);

// Interrupt service handler for Bottom(P3.5) button
void PORT3_IRQHandler();

// Interrupt service handler for Joystick Y-axis(P4.4) and Joystick button(P4.1)
//void PORT4_IRQHandler(void);

// Interrupt service handler for Top(P5.1) button
void PORT5_IRQHandler();

// Interrupt service handler for Joystick X-axis(P6.0)
//void PORT6_IRQHandler(void);


#endif /* BUTTONCONFIG_H_ */
