/*
 * main.c
 *
 *  Created on: Dec 2, 2017
 *      Author: Stefan
 */
#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <stdint.h>
#include <arm_math.h>
#include "arm_const_structs.h"
#include "ButtonConfig.h"
#include "display.h"
#include "clock.h"

volatile uint32_t test = 1234;
volatile uint32_t SID = 0;
extern volatile uint8_t slideNum;
Graphics_Context g_sContext;
uint8_t result;
char string[4];


int main(void)
{
    WDT_A_holdTimer();

    GPIO_configure();
    LCD_configure();
    __enable_irq();

    setID();
    result = itoa(SID, string, 3, 10);


    while(1)
    {
        if(slideNum == 1){
            welcomeScreen();
            while(slideNum == 1);}
        if(slideNum == 2){
            registerStudent();
            while(slideNum == 2);}
        if(slideNum == 3){
            checkout();
            while(slideNum == 3);}
        if(slideNum == 4){
            thankYou();
            while(slideNum == 4);}
        if(slideNum == 5){
            goBlack();
            noBikes();
            while(slideNum == 5);}
        if(slideNum == 6){
            goBlue();
            error();
            while(slideNum == 6);}
        if(slideNum == 7){
            goBlack();
            noDoubleDipping();
            while(slideNum == 7);}
    }
}


