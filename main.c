/**
 *
 *  Author: Jack Danielski, Adam Smrekar, Riley Hadjis
 *  Lab 1 ECEN 2240
 *  Due: 9/8/2017
 *
 */

#include "msp.h"
#include "lab1.h"
#include <stdint.h>

/*
void foo(uint8_t * ptr_a, uint32_t * ptr_b) {
    *ptr_a += 2;
    (*(uint8_t*)(ptr_b))++;
    ptr_b++;
}
*/

int main(void)
{
    volatile unsigned int count = 0;
    volatile unsigned int i = 0;

    // Stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    //<CONFIGURE_P1.0_GENERAL_IO>   P1.0 to General IOMode
    uint8_t * p1_in    = (uint8_t *)0x40004C00;
    uint8_t * p1_out   = (uint8_t *)0x40004C02;
    uint8_t * p1_dir   = (uint8_t *)0x40004C04;
    uint8_t * p1_sel0  = (uint8_t *)0x40004C0A;
    uint8_t * p1_sel1  = (uint8_t *)0x40004C0C;

    *p1_dir |= 0xFF;            // P1.0 set as output direction
    *p1_out &= 0x00;            // P1 Out set to low
    *p1_sel0 |= 0x00;
    *p1_sel1 |= 0x00;
    /** 4
     *
     * 0xA274 = 0b1010_0010_0111_0100
     * Set bits 0,4,5,14,15
     * Use bitwise OR on var1 using 0b1100_0000_0011_0001
     *
     *      0b1010_0010_0111_0100
     *      0b1100_0000_0011_0001
     *    |  _____________________
     *      0b1110_0010_0111_0101
     *
     */
    uint16_t var1 = 0xA274;
    var1 |= 0xC031;

    /*
     *0
     * Clear Bits 9,10,15
     * Use bitwise & on var1 using 0b0111_1001_1111_1111
     *  since only bits 9,10, and 11 need to be cleared.
     *  This will keep all set bits except 9,10,and 11
     *  without setting any bits not previously set.
     *
     *  0b0111_1001_1111_1111 = 0x7D83
     *
     *      0b1100_0000_0011_0001
     *      0b0111_1001_1111_1111
     *    & _____________________
     *      0b0100_0000_0011_0001
     *
     */

    var1 &= 0x7D83;

    /*
     *
     * Toggle middle 8 bits
     * Use bitwise ^ on var1 using 0b0000_1111_1111_0000
     *  since the 8 bits in the middle that are already 1
     *  will be turned off and the bits that are off will
     *  be turned on.n
     *
     * 0b0000_1111_1111_0000 = 0x0FF0
     *
     *      0b0100_0000_0011_0001
     *      0b0000_1111_1111_0000
     *    ^  _____________________
     *      0b0100_1111_1100_0001
     *
     *
     */

    var1 ^= 0x0FF0;

    report_types();
    report_pointer_types();

    // the make square wave is set to 100 ms period
    while(1) {
        make_square_wave();
    }


    while (1) {
        count++;
        *p1_out ^= 0x01; //Blink P1.0 LED

        for (i = 0; i < 30000; i++); // Delay
    }



    return 0;
}

