; Assembly file for increment() function
; Lab 3, ECE 266, fall 2020
; Created by Zhao Zhang

; Function prototype: stopWatchIncrement(displayState_t *pDisplayState);
; This function increment the number shown on the stop watch display. It should be called once every one centisecond.
                .text
                .global timeIncrement	; make this symbol global
timeIncrement  .asmfunc

                ; Update c2, continue to c1 if c2 is reset to zero
                LDRB    r1, [r0, #0]        ; load c2 to r1, note c2's offset is zero which can be omitted
                ADD     r1, #1              ; increment c2 by one
                STRB    r1, [r0, #0]        ; store back c2
                CMP     r1, #10             ; compare c2 and 10
                BNE     return              ; if c2 != 10, jump to return
                MOV     r1, #0              ; reset c2 to zero
                STRB    r1, [r0, #0]        ; store back again

				LDRB    r1, [r0, #1]        ; load c2 to r1, note c2's offset is zero which can be omitted
                ADD     r1, #1              ; increment c2 by one
                STRB    r1, [r0, #1]        ; store back c2
                CMP     r1, #6             ; compare c2 and 10
                BNE     return              ; if c2 != 10, jump to return
                MOV     r1, #0              ; reset c2 to zero
                STRB    r1, [r0, #1]

                LDRB    r1, [r0, #2]
                LDRB	r2, [r0, #3]
                ADD		r1, #1
                CMP 	r2, #1
                BNE		continue
                CMP		r1, #2
                BNE		chk3
                LDR		r3, [r0, #4]
                EOR     r3, r3, #1
                STRB	r3, [r0, #4]
                CMP		r3, #1
                ITTEE	EQ
                MOVEQ	r1, #2
                MOVEQ	r2, #1
                MOVNE	r1, #0
                MOVNE	r2, #0
                STRB	r1, [r0, #2]
                STRB	r2, [r0, #3]
                B		return
chk3			CMP		r1, #3
				ITT		GE
                SUBGE	r1,r1,#2
                MOVGE	r2, #0
                STRB	r2, [r0,#3]
continue        STRB    r1, [r0, #2]
                CMP     r1, #10
                BNE     return
                MOV     r1, #0
                STRB    r1, [r0, #2]

                LDRB    r1, [r0, #3]        ; load c2 to r1, note c2's offset is zero which can be omitted
                ADD     r1, #1              ; increment c2 by one
                STRB    r1, [r0, #3]        ; store back c2
                CMP     r1, #2             ; compare c2 and 10
                BNE     return              ; if c2 != 10, jump to return
                MOV     r1, #0              ; reset c2 to zero
                STRB    r1, [r0, #3]
                ; YOUR CODE: update c1, s2, and s1

return          BX      LR                  ; return
                .endasmfunc

                .end
