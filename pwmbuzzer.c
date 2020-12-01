/*
 * pwmbuzzer.c: Starter code for Lab 7, Fall 2020
 *
 * Created by Zhao Zhang
 * Modified by: Drew, Iraklis and James
 */

#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "launchpad.h"
#include "pwmbuzzer.h"

void pwmBuzzerInit()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);

    // first arg = the base address of the timer module, second arg = is the configuration for the timer
    TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    // setting count time for Timer B
    // address of the timer of the timer module, specifies the timers to adjust (timer_a /b)
    // the load value
    // pwm pulse width in clock cycles
    TimerLoadSet(WTIMER0_BASE, TIMER_B, 1000);

    // configures the match value for a timer
    // pwm pulse width in clock cycles
    TimerMatchSet(WTIMER0_BASE, TIMER_B, 0);

    // enable PWM output inversion
    // the last argument determines if the output is made low
    TimerControlLevel(WTIMER0_BASE, TIMER_B, true);

    //enable Timer B
    TimerEnable(WTIMER0_BASE, TIMER_B);
}
