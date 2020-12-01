                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/gpio.h","ras.h","driverlib/adc.h", "launchpad.h"

                .text
                .global rasRead
                .global rasRead_2

ui32Base     	.field  ADC0_BASE
ui32Base2		.field	ADC1_BASE

rasRead         PUSH    {LR}
                LDR     r0, ui32Base			; load value from ras
                MOV     r1, #0
                BL      ADCProcessorTrigger		; call to initiate sample sequence

while_loop      LDR     r0, ui32Base			; load value from ras
                MOV     r1, #0
                MOV     r2, #0
                BL      ADCIntStatus			; check interrupt status
                CMP     r0, #0
                BEQ     while_loop				; repeat loop if r0 == 0

                SUB     sp, #4
                LDR     r0, ui32Base
                MOV     r1, #0
                MOV     r2, sp
                BL      ADCSequenceDataGet		; get data from sample sequence results

rasRead_2       PUSH	{LR}
                LDR     r0, ui32Base2
                MOV     r1, #0
                BL      ADCProcessorTrigger

while_loop2     LDR     r0, ui32Base2
                MOV     r1, #0
                MOV     r2, #0
                BL      ADCIntStatus
                CMP     r0, #0
                BEQ     while_loop2

                SUB     sp, #4
                LDR     r0, ui32Base2
                MOV     r1, #0
                MOV     r2, sp
                BL      ADCSequenceDataGet

                POP     {r0, pc}
