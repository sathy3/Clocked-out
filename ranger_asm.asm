;
; ranger_asm.asm
;
; Created on: Oct 30, 2020
; Author: Drew, Iraklis, and James
;

 .cdecls "stdint.h", "stdbool.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "ranger.h","driverlib/timer.h", "driverlib/uart.h"
			.text
;			.global rangerInit
;			.global returnVal
RANGER_PERIPH		.field 	SYSCTL_PERIPH_GPIOB
RANGER_PIN		.equ   	GPIO_PIN_3
RANGER_TIMER3BASE	.field 	TIMER3_BASE
RANGER_TIMER_PIN	.field 	GPIO_PB3_T3CCP1

; Preserve registers unchanged for the caller function

rangerDetect2		PUSH 	{r1, r2, r3, LR}

			BL	rangerDetect		; Function writen in C

; The code to handle the rising time

while			LDR	r0, RANGER_TIMER3BASE	; Load timer3
			MOV	r1, #0
			BL	TimerIntStatus
			CMP 	r0, #0
			BEQ 	while

			LDR	r0, RANGER_TIMER3BASE
			MOV	r1, #TIMER_B
			BL	TimerValueGet
			STR	r0, [sp]

			LDR	r0, RANGER_TIMER3BASE
			MOV	r1, #TIMER_CAPB_EVENT
			BL	TimerIntClear

; The code to handle the falling time

while2			LDR	r0, RANGER_TIMER3BASE
			MOV	r1, #0
			BL	TimerIntStatus
			CMP 	r0, #0
			BEQ 	while2

			LDR	r0, RANGER_TIMER3BASE
			MOV 	r1, #TIMER_B
			BL	TimerValueGet
			STR	r0, [sp, #4]

			LDR	r0, RANGER_TIMER3BASE
			MOV	r1, #TIMER_CAPB_EVENT
			BL	TimerIntClear

; Calculating the time difference

			LDR 	r1, [sp]
			LDR 	r0, [sp, #4]
			SUB 	r0, r0, r1

			POP 	{r1, r2, r3, PC}
			.endasmfunc
