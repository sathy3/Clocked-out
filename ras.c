#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.h>
#include <driverlib/pin_map.h>
#include "launchpad.h"
#include "ras.h"

//initiation
void rasInit()
{
    //enable ADC0 module function calling
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //waits for ADC0 module to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) {}

    //enable the sample sequence to capture the value of channel 7 when the processor
    //trigger occurs
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH7);
    ADCSequenceEnable(ADC0_BASE, 0);
}

void rasInit_2()
{
    //enable ADC1 module function calling
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    //waits for ADC1 module to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC1)) {}

    //enable the sample sequence to capture the value of channel 6 when the processor
    //trigger occurs
    ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH6);
    ADCSequenceEnable(ADC1_BASE, 0);
}
