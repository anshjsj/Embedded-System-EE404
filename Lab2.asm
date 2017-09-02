BLINK
-------------------------------------------------------------------------------
;   Turing-on LEDs 
;   This program turns on Red and Green LEDs, 

  	PRESERVE8			; 
      THUMB       
 
 	AREA    RESET, DATA, READONLY
	EXPORT  __Vectors
 
__Vectors 
		DCD  0x20001000     ; stack pointer value when stack is empty
		DCD  Reset_Handler  ; reset vector
		ALIGN
 
 		AREA   LEDON, CODE, READONLY   ;LEDON
   	  	ENTRY
   	  	EXPORT Reset_Handler
Reset_Handler

Clock	EQU	0x400FE608		;This is a clock register address RCGC
ClkBit5	EQU	0x20        ; Bit5 to turn on clock of PORTF
DEN	EQU	0x4002551C 		; Digital Enable register address
DIR	EQU	0x40025400 		; Direction register offset 0x0400
DATA	EQU	0x400253FC		; PORTF Data Address 

START ; Enable clock to PORTF
	LDR R1, = Clock	;RCGC - Clock register address
	LDR R2, [R1]
	ORR R2, ClkBit5 	;Set Bit5 - 00100000
	STR R2, [R1] 	;Set Bit5 in Clock (RCGC) register
	LDR R1, = DEN	;Digital Enable Register
	MOV R2, #0x08 	; Pins 1 and 3 = 00001000
	STR R2, [R1]	; Set up Pins 1 and 3 for digital function
	LDR R1, = DIR	; 
	MOV R2, #0x08 	;Pins 1 and 3 - 00001000
	STR R2, [R1] 	;Set up pins 1 and 3 as output
	LDR R1, = DATA	;PORTF Data Address
REPEAT	MOV R2, #0x08 	;Pins 1 and 3 = 00001000
	STR R2, [R1] 	;Turn on Red and Green LEDs
	BL DELAY
	MOV R2, #0x02 	;Pins 1 and 3 = 00000010
	STR R2, [R1] 	;Turn on Red and Green LEDs
	BL DELAY
	B REPEAT ;Stay here

	 
        
      
; This subroutine grants access to 
; floating point coprocessor.
; It is called by the startup code.
        EXPORT  SystemInit
SystemInit 
        LDR     R0, =0xE000ED88           
                ; Enable CP10,CP11
        LDR     R1,[R0]
        ORR     R1,R1,#(0xF << 20)
        STR     R1,[R0]
        BX      LR

    ; This variable is required for the 
    ; startup code though not used.
        EXPORT  __use_two_region_memory
__use_two_region_memory EQU 1

DELAY

COUNT EQU	100
COUNT1	EQU	53200			; Count for 10 ms delay – System clock – 16 MHz	
	
		;PUSH	{R0, R1}		;Save R0, R1 of the Caller on the stack
		LDR  	R3, = COUNT	;Load the count provided by the caller
NEXT		LDR 	R4, = COUNT1	;Load COUNT1 for the inner loop
AGAIN	SUBS	R4, #1			;Reduce count in R1 by one
		BNE	AGAIN		;Is R1 = 0, if not, go back to the next count
		SUBS	R3, #1			;Reduce COUNT by 1 in the outer loop	
		BNE	NEXT			;Is R0 = 0, if not go back to load inner loop count
		;POP  	{R3, R4}		; Retrieve R0, R1 of the caller from the stack 
		BX	LR			;Go back to the Caller 

		
		
		
		
		
		
		
		
		
		
		ALIGN
        END
