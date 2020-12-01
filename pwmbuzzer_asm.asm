; pwmbuzzer_asm.asm: code for Lab 7, Fall 2020
; Created by Drew, Iraklis and James
; operation function to set PWM parameters

                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmbuzzer.h"

                .text
				.global	pwmBuzzerSet
;BUZZER_PERIPH			.field	SYSCTL_PERIPH_GPIOC
;WTIMER0_PERIPH			.field	SYSCTL_PERIPH_WTIMER0
WTIMER0					.field	WTIMER0_BASE
PORTC					.field	GPIO_PC5_WT0CCP1
BUZZER_WTIMER_PIN		.field	GPIO_PC5_WT0CCP1
PIN5					.equ	GPIO_PIN_5

; void pwmBuzzerSet(uint32_t pulseWidth, uint32_t period)

					.asmfunc
; used pwmled_asm as a template
pwmBuzzerSet		PUSH	{lr, r1, r0}

					LDR		r0,  WTIMER0	; Load the value of pulseWidth
					MOV		r1,  #TIMER_B
					LDR		r2,  [sp, #4]	; Load the value of period
					BL		TimerLoadSet

					LDR		r0,  WTIMER0	; Values are not perserved, load again
					MOV		r1,  #TIMER_B
					LDR		r2,  [sp, #0]	; r2 is pulseWidth
					BL		TimerMatchSet

					ADD		sp, #8		; Release stack for lr
					POP		{pc}

					.endasmfunc
