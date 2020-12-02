;
; Lab 6, ECE 266, fall 2020
;
; Assembly function to read ADC.
;
; Created by Siddhant Sathyan
;

; To include names declared in C
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/adc.h","ras.h"

                .text
                .global rasRead
                .global ras2Read

RAS_PORT     .field  ADC0_BASE
SEQ_NUM      .byte	 0
SEQ_NUM2     .byte	 1
;
;
;
rasRead		PUSH 	{LR}			;Reads the ADC signals.
			PUSH 	{r0}
			LDR     r0, RAS_PORT
            LDRB	r1, SEQ_NUM
            BL		ADCProcessorTrigger
tes_loop    LDR		r0, RAS_PORT
			LDRB	r1, SEQ_NUM
			MOV		r2, #0
			BL		ADCIntStatus
			CMP		r0, #0
			BNE     tes_loop
			LDR		r0, RAS_PORT
			LDRB	r1, SEQ_NUM
			POP		{r2}
			BL 		ADCSequenceDataGet
			LDR		r0, RAS_PORT
			LDRB	r1, SEQ_NUM
			BL		ADCIntClear
			POP		{PC}

ras2Read	PUSH 	{LR}
			PUSH 	{r0}
			LDR     r0, RAS_PORT
            LDRB	r1, SEQ_NUM2
            BL		ADCProcessorTrigger
tes2_loop   LDR		r0, RAS_PORT
			LDRB	r1, SEQ_NUM2
			MOV		r2, #0
			BL		ADCIntStatus
			CMP		r0, #0
			BNE     tes2_loop
			LDR		r0, RAS_PORT
			LDRB	r1, SEQ_NUM2
			POP		{r2}
			BL 		ADCSequenceDataGet
			LDR		r0, RAS_PORT
			LDRB	r1, SEQ_NUM2
			BL		ADCIntClear
			POP		{PC}
