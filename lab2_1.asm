
 ;Assembly Code
 
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
Bit5	EQU	0x20                    	; Bit5 to turn on clock for PORTF
DEN	EQU	0x4002551C 		; Digital Enable register address
DIR	EQU	0x40025400 		; Direction register offset 0x0400
DATA	EQU	0x400253FC		; PORTF Data Address 
PUR	EQU	0x40025510		; Pull Up Register Address

START ; Enable clock to PORTF
	LDR R1, = Clock	;RCGC - Clock register address
	LDR R2, [R1]
	ORR R2, Bit5 	;Set Bit5 - 00100000
	STR R2, [R1] 		;Set Bit5 in Clock (RCGC) register
	LDR R1, = DEN	;Digital Enable Register
	MOV R2, #0x1A 	; Pins 1, 3, and 4 = 00011010
	STR R2, [R1]		; Set up Pins 1, 3, and 4 for digital function
	LDR R1, = PUR	; Pull Up Resistor Register
	MOV R2, #0x10		;Pin 4 = 0001 0000
	STR R2, [R1]		; Set up pull-up register for SW2
	LDR R1, = DIR	; Direction Register
	MOV R2, #0x0A 	;Pins 1 and 3 as output and Pin 4 as input - 00001010
	STR R2, [R1] 		;Set up pins 1 and 3 as output and Pin 4 as input
REPEAT	LDR R1, = DATA	;PORTF Data Address
	LDR R2,  [R1]	; Read PORTF 
	AND R2,R2, #0x10
	CMP R2, #0x00; Check Pin 4 
	BNE SKIP		; If Pin4 = 0, jump to turn on Green LED 
	LDR  R1, =DATA	; Get Data PORTF address
	MOV R2, #0x02		;Pin 1 – Red LED
	STR R2, [R1]		; Turn on Red LED
	B REPEAT
SKIP  
	MOV R2, #0x08		; Pin 3		; 
	STR R2, [R1] 		; Turn on Green LED
	B REPEAT 	;Stay here

	EXPORT SystemInit
SystemInit
	LDR R0, =0xE000ED88
	LDR R1,[R0]
	ORR R1,R1,#(0xF << 20);???
	STR R1,[R0]
	BX LR
	EXPORT __use_two_region_memory
__use_two_region_memory EQU 1
	ALIGN
	END
-------------------------------------------------------------------------------------------------------------------------------------------------
 
 
 
 
 
 
 
 