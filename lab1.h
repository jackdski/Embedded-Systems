/*
 *
 * Author: Jack Danielski
 * Date Due: 9-8-2017
 *
 */

#ifndef LAB1_H
#define LAB1_H


#define P1_IN    (*(uint32_t *)(0x40004C00))
#define P1_OUT   (*(uint32_t *)(0x40004C02))
#define P1_DIR   (*(uint32_t *)(0x40004C04))
#define P1_SEL0  (*(uint32_t *)(0x40004C0A))
#define P1_SEL1  (*(uint32_t *)(0x40004C0C))

void report_types();

void report_pointer_types();

void make_square_wave();


#endif
