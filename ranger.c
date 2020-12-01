/*
 *  ranger.c
 *
 *  Created on: Oct 30, 2020
 *  Author: Drew, Iraklis, and James
 */

#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include <driverlib/uart.h>
#include "launchpad.h"

#define TIMER3  TIMER3_BASE

void rangerInit() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3); 				//To enable GPIO peripheral
    TimerConfigure(TIMER3, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME_UP)); 	//To configure a timer and sub-timer(s)
    TimerControlEvent(TIMER3, TIMER_B, TIMER_EVENT_BOTH_EDGES); 		//To configure event (edge) type
    TimerEnable(TIMER3, TIMER_B); 						//To enable a timer and sub-timer(s)
}

/*
 * Configure the pin as GPIO output pin
 * Make output low for 2 microseconds
 * Make the output high for 5 microseconds
 * Configure the pin as Timer pin and route it the input channel of timer
 */

/*
 * 1. Configure the pin type to GPIO output pin
 * 2. Set the pin output to low
 * 3. Wait for a short time (microseconds)
 * 4. Set the pin output to high
 * 5. Wait for a short time (microseconds)
 * 6. Set the pin output to low
 * 7. Configure the pin type to Timer pin
 * 8. Route the pin to timer pin
 * 9. Clear timer interrupt status
 */

void rangerDetect()
{
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0); 		//Send the pulse (set it to low)
    waitUs(2); 							//Wait for 2 microseconds
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
    waitUs(5);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_3); 		//To configure GPIO pin to be compatible with timer input
    GPIOPinConfigure(GPIO_PB3_T3CCP1); 				//To route GPIO pin to timer's internal pin
    TimerIntClear(TIMER3, TIMER_CAPB_EVENT);
}

