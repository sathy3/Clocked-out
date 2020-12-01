/*
 * ECE 266, fall 2020, Lab 4 starter code
 *
 * motion.c: This file contains the initialization function for the motion detector.
 *
 * Created by james and drew
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "motion.h"

#define MOTION_PERIPH   SYSCTL_PERIPH_GPIOC
#define MOTION_PORT     GPIO_PORTC_BASE
#define MOTION_PIN      GPIO_PIN_4

void motionInit()
{
    SysCtlPeripheralEnable(MOTION_PERIPH);          //Enable the port peripheral used by the motion sensor

    GPIOPinTypeGPIOInput(MOTION_PORT, MOTION_PIN);  //Configure the pin as input
}
