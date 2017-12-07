#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <arm_math.h>
#include "arm_const_structs.h"
#include "ButtonConfig.h"
#include "display.h"

void Delay0(void);
void Delay1(void);
void Delay2(void);
void Delay3(void);
void Delay4(void);
void Delay5(void);


extern volatile uint8_t slideNum;
Graphics_Context g_sContext;

int main(void)
{
    WDT_A_holdTimer();
    Delay0();

    GPIO_configure();
    LCD_configure();
    __enable_irq();

    while(1)
    {
        if(slideNum == 1){
            welcomeScreen();
            while(slideNum == 1);
        }
        if(slideNum == 2){
            registerStudent();
            while(slideNum == 2);
        }
        if(slideNum == 3){
            checkout();
            while(slideNum == 3);
        }
        if(slideNum == 4){
            thankYou();
            while(slideNum == 4);
        }
        if(slideNum == 5){
            noBikes();
            while(slideNum == 5);
        }
        if(slideNum == 6){
            error();
            while(slideNum == 6);
        }
    }
}

void Delay0(void)
{
    volatile int i;
    for (i=0;i < 100 ; i ++);

}
void Delay1(void)
{
    volatile int i;
    for (i=0;i < 1700000 ; i ++);

}
void Delay2(void)
{
    volatile int i;
    for (i=0;i < 3400000 ; i ++);

}
void Delay3(void)
{
    volatile int i;
    for (i=0;i < 5100000 ; i ++);

}
void Delay4(void)
{
    volatile int i;
    for (i=0;i < 6800000 ; i ++);

}
void Delay5(void)
{
    volatile int i;
    for (i=0;i < 8500000 ; i ++);

}

