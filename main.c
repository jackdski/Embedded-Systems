//Adam Smrekar
//Riley Hadjis
//Jack Danielski
//Lab1

#include "msp.h"
#include <stdint.h>

void main(void)
{
    unsigned int count = 0;
    unsigned int i = 0;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;    // Stop watchdog timer

    uint16_t * var1 = (uint16_t *)0xA274;
    *var1 |= 0xC031;
    *var1 &= 0x79FF;
    *var1 ^= 0x0FF0;

    uint8_t * P1_SEL0 = (uint8_t *) 0x40004C0A;   // P1.0 to General IOMode
    uint8_t * P1_SEL1 = (uint8_t *) 0x40004C0C;
    //uint8_t * P1_IN = (uint8_t *) 0x40004C00;
    uint8_t * P1_OUT = (uint8_t *) 0x40004C02;
    uint8_t * P1_DIR = (uint8_t *) 0x40004C04;

    *P1_SEL1 = 0x0;     // P1.0 set as output direction
    *P1_SEL0 = 0x0;
    *P1_DIR |= 0x1;

    *P1_OUT = 0x0;      // P1.0 output low
    //*P1_IN = 0x0;
    while (1) {                    // continuous loop
            count++;
            *P1_OUT ^= 1;              // Blink P1.0 LED
            for (i = 0; i < 30000; i++);    // Delay
    }
}
