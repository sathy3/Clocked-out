/*
 * ras.c
 *
 *      Contributors: Siddhant Sathyan
 */
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/adc.h>
#include <driverlib/adc.c>
#include "launchpad.h"
#include "ras.h"

#define RAS_PERIPH  SYSCTL_PERIPH_ADC0
#define RAS_PORT    ADC0_BASE
#define SEQ_NUM     0
#define SEQ_NUM2    1

// Initialize ADC for RAS
void rasInit()
{
    // Enable the ADC0 peripheral.
    //
    SysCtlPeripheralEnable(RAS_PERIPH);

    while(!SysCtlPeripheralReady(RAS_PERIPH))
    {
    }
 //   SysCtlPeripheralEnable(MOT_PERIPH);

        // Configure the pin as output
     //GPIOPinTypeGPIOInput(MOT_PORT, MOT_PIN);

    //Configure ADC0, sequencer #0
    ADCSequenceConfigure(RAS_PORT, SEQ_NUM, ADC_TRIGGER_PROCESSOR, 0);

    //Configure the sequencer
    ADCSequenceStepConfigure(RAS_PORT, SEQ_NUM, 0,
    ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH7);
    //Enable the ADC sequencer
    ADCSequenceEnable(RAS_PORT, SEQ_NUM);
}
void ras2Init()
{
    // Enable the ADC0 peripheral.
    //
    SysCtlPeripheralEnable(RAS_PERIPH);

    while(!SysCtlPeripheralReady(RAS_PERIPH))
    {
    }

    ADCSequenceConfigure(RAS_PORT, SEQ_NUM2, ADC_TRIGGER_PROCESSOR, 0);


    //Configure ADC0, sequencer #1
        ADCSequenceConfigure(RAS_PORT, SEQ_NUM2, ADC_TRIGGER_PROCESSOR, 0);

        //Configure the sequencer
        ADCSequenceStepConfigure(RAS_PORT, SEQ_NUM2, 0,
        ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH6);

    //Enable the ADC sequencer
    ADCSequenceEnable(RAS_PORT, SEQ_NUM2);
}
