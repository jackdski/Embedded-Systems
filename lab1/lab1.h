/*
 *
 * Author: Jack Danielski, Adam Smrekar, Riley Hadjis
 * Date Due: 9-8-2017
 *
 */

#ifndef LAB1_H
#define LAB1_H

/*
 * Lab Procedure Part 7
 *
 * Defined macros that access registers of Port 1 to
 * manipulate bits for the make_square_wave function.
 *
 */
#define P1_IN    (*(uint8_t *)(0x40004C00))
#define P1_OUT   (*(uint8_t *)(0x40004C02))
#define P1_DIR   (*(uint8_t *)(0x40004C04))
#define P1_SEL0  (*(uint8_t *)(0x40004C0A))
#define P1_SEL1  (*(uint8_t *)(0x40004C0C))

int * report_types();

int * report_pointer_types();

void make_square_wave();


#endif
