#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <arm_math.h>
#include "arm_const_structs.h"
#include "ButtonConfig.h"
#include "display.h"

void clockInit(void);
void boardInit(void);
void timerInit(void);
void Delay0(void);
void Delay1(void);
void Delay2(void);
void Delay3(void);
void Delay4(void);
void Delay5(void);


extern volatile uint8_t slideNum;

/* TimerA UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        65535-1,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,
        TIMER_A_SKIP_CLEAR
};

Graphics_Context g_sContext;

int main(void)
{
    WDT_A_holdTimer();
    Delay0();

    boardInit();
    clockInit();
    timerInit();

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

void clockInit(void)
{
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

}

void boardInit(void)
{
    // All GPIO output and low
    GPIO_setAsOutputPin(GPIO_PORT_P1,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P2,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P3,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P4,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P5,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P6,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6,PIN_ALL8);
}

void timerInit(void)
{
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);

    Timer_A_enableInterrupt(TIMER_A1_BASE);
    Interrupt_enableInterrupt(INT_TA1_0);

    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
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



//------------------------------------------------------------------------------
// Timer A0 Interrupt Service Routine
//------------------------------------------------------------------------------

void TA1_0_IRQHandler(void)
{
//    Sharp96x96_SendToggleVCOMCommand();

    /* Add Offset to TACCR0 */
    Timer_A_setCompareValue(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0,
    Timer_A_getCaptureCompareCount(TIMER_A1_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0)+(100-1));

    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_clearInterruptFlag(TIMER_A1_BASE);
}


