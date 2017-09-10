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
 * Function from homework to check answer
 */

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

    /*
     * Lab Procedure Part 3
     *
     * We set unsigned 8-bit pointers to the addresses of
     * registers that control various aspects of Port 1
     * so we can use bitwise operators to turn on or off
     * bits needed for different functions.
     *
     * <CONFIGURE_P1.0_GENERAL_IO>   P1.0 to General IOMode
     */

    uint8_t * p1_in    = (uint8_t *)0x40004C00;
    uint8_t * p1_out   = (uint8_t *)0x40004C02;
    uint8_t * p1_dir   = (uint8_t *)0x40004C04;
    uint8_t * p1_sel0  = (uint8_t *)0x40004C0A;
    uint8_t * p1_sel1  = (uint8_t *)0x40004C0C;

    *p1_dir |= 0xFF;            // P1.0 set to output direction
    *p1_out &= 0x00;            // P1_OUT set low
    *p1_sel0 |= 0x00;           // P1_SEL0 set low
    *p1_sel1 |= 0x00;           // P1_SEL1 set low

    /** Lab Procedure Part 4:
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
     *
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

    /*
     * Lab Procedure Part 5:
     *
     * The report_types and report_pointer_types are written
     *  and described in the lab1.c file.
     *
     */
    report_types();
    report_pointer_types();


    /*
     * Lab Procedure Parts 8 and 9:
     *
     * The make_square_wave function is currently set to get a 100 ms period.
     * It is inside of a while(1) loop so that we could continuously see the waveform
     * on the oscilloscope.
     *
     */
    /*
    while(1) {
        make_square_wave();
    }
    */

    /*
     *
     * A while(1) loop that will continuously blink the LED on the board.
     * The LED is toggled using the XOR bitwise operator and the for-loop
     * is a delay to keep the LED on or off for a certain amount of time.
     *
     */
    while (1) {
        count++;
        *p1_out ^= 0x01; //Blink P1.0 LED

        for (i = 0; i < 300000; i++); // Delay
    }



    return 0;
}

