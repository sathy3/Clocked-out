;
; Lab 4, ECE 266, fall 2020
;
; Assembly functions related to buzzer operations.
;
; Created by james and drew
;

; Include names declared in C
				.cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/gpio.h","motion.h"

;So the important thing is that .cdecls has to be defined before everything else (because there is header for motion).
;It took me few minutes to realize why the program doesnt want to compile. However, the problem is now resolved.

MOTION_PORT     .field	GPIO_PORTC_BASE
MOTION_PIN      .equ	GPIO_PIN_4

				.text
				.global motionSensor

motionSensor	PUSH    {LR}

				LDR     r0, MOTION_PORT
                MOV     r1, #MOTION_PIN
                BL      GPIOPinRead

                POP     {PC}
