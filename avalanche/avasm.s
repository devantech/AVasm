
;	A semicolon starts a comment. All characters until the end of the line are ignored


; A constant may be defined as follows. In this case, we are defining an I/O port address
.const	LedPortAddr		0x100


; The .data directive initialises or reserves space in the data RAM which starts at 0x200 
.data	initLCD			0, 4, 19, 12
.data	pixelBuffer		[900]
.data   lcdRow1			"    Hello World     "


; The .reg directive assigns a CPU register and optionally initialises it.
; All registers defined outside of a process are global and available to all processes
; The following three registers MUST be the first defined in all programs
.reg zero			0								; note: register 0 is read only and MUST be set to zero here.
.reg one			1								; Registers 1 and 2 define 1 and -1
.reg minusone	0xffff								; these are both useful and expected by the assembler


; A process is a stand-alone CPU, independent of all other CPU's
; Its timing is regular and jitter free.
; The minimum number of processes is 7. The maximum is 255.
; A 100MHz (100MIPs) system with 10 processes will get 10MIPS per process.
; i.e. an instruction will be executed every 100nS
; A register defined within a process is only visible to that process, even if another process has a register of the same name.
; Labels are unique to a process, other processes may use the same name without interference.
; All defined processes will run automatically so MUST contain some code, even if only an endless loop.

process myProcess
endless:	jmp endless
endprocess

; A primary process may be sub divided into any number of sub processes.
; Only the time assigned to that process is divided between sub processes
; Each sub process is still an independent CPU.
; Here the primary process "P2" is split into two processes, A and B
; Note the dot notation used to split the process. PrimaryName.SubName
; If the primary processes get 10MIPs each, then the split processes gets 5MIPs each

process P2.A
endless:	jmp endless
endprocess

process P2.B
endless:	jmp endless
endprocess

; Likewise, if a process is split into three, each process will get 1/3 of the time (3.33 MIPs in this case).
 
process P3.A
endless:	jmp endless
endprocess

process P3.B
endless:	jmp endless
endprocess

process P3.C
endless:	jmp endless
endprocess

; Make sure you have a minimum of 7 primary processes defined.

process P4
endless:	jmp endless
endprocess

process P5
endless:	jmp endless
endprocess

process P6
endless:	jmp endless
endprocess

; Instruction set examples
process P7
.reg A
.reg B
.reg X

myLabel:	add X, A, B						; add A to B and save result in X
			add (X), A, B					; add A to B and save result in memory location pointed to by X
			add X, A, (B)					; add A to memory location pointed to by B and save result in X
			add (X), A, (B)					; add A to memory location pointed to by B and save result in memory location pointed to by X
			
			and X, A, B						; logical and A with B and save result in X 
			and (X), A, B
			and X, A, (B)
			and (X), A, (B)
			
			or X, A, B						; logical or A with B and save result in X 
			or (X), A, B
			or X, A, (B)
			or (X), A, (B)
			
			xor X, A, B						; logical xor A with B and save result in X 
			xor (X), A, B
			xor X, A, (B)
			xor (X), A, (B)
			
			srl X, A						; shift right logical A and save result in X
			srl (X), A
			
			clrb X, 5, A					; clear bit 5 (0-15) in A and save result in X
			clrb (X), 5, A
			clrb X, 5, (A)
			clrb (X), 5, (A)

			setb X, 5, A					; set bit 5 (0-15) in A and save result in X
			setb (X), 5, A
			setb X, 5, (A)
			setb (X), 5, (A)
			
			ldi A, 0x100					; load 16 bit immediate value (0x0000-0xFFFF) into A
			jal	A, myLabel					; jump to address, store PC+1 in A to use as return address
			jz A, myLabel					; if A is zero, jump to address
			jnz A, myLabel					; if A is not zero, jump to address
			
			jalr A, B						; jump to A, store PC+1 in B to use as return address
			
; note the following expect X to be pre-loaded with the destination address
			jeqr X, A, B					; compare A with B, jump to X if equal
			jner X, A, B					; compare A with B, jump to X if not equal
			jltr X, A, B					; compare A with B, jump to X if A is less than B
			jgtr X, A, B					; compare A with B, jump to X if A is greater than B
			jler X, A, B					; compare A with B, jump to X if A is less or equal to B
			jger X, A, B					; compare A with B, jump to X if A is greater or equal to B
			jbsr X, 5, A					; test bit 5 (0-15) in A, jump to X if set
			jbcr X, 5, A					; test bit 5 (0-15) in A, jump to X if clear
	
; Macro Instructions. The assembler translates these into one or more of the native instructions above		

			nop								; (1) does nothing for one instruction cycle

			inc	A							; (1) increment A
			inc	(A)							; (1) increment memory location pointed to by A
			dec	A							; (1) decrement A
			dec	(A)							; (1) decrement memory location pointed to by A

			sub X, A, B						; (3) subtract B from A and save result in X
			sub (X), A, B					; (3)
			sub X, A, (B)					; (3)
			sub (X), A, (B)					; (3)

			sll X, A						; (1) shift left logical A and save result in X
			sll (X), A						; (1)

			djnz X, myLabel					; (2) decrement X and jump to label if not zero
			jmp myLabel						; (1) unconditional jump
			call X, myLabel					; (1) place PC+1 in X and jump to subroutine address
			ret X							; (1) place X into PC to return from subroutine
					
			jeq A, B, myLabel				; (2) compare A with B, jump if equal	
			jne A, B, myLabel				; (2) compare A with B, jump if not equal	
			jlt A, B, myLabel				; (2) compare A with B, jump if A is less than B	
			jgt A, B, myLabel				; (2) compare A with B, jump if A is greater than B	
			jle A, B, myLabel				; (2) compare A with B, jump if A is less or equal to B	
			jge A, B, myLabel				; (2) compare A with B, jump if A is greater or equal to B	
					
			jbc	A, 5, myLabel				; (2) test bit 5 (0-15) in A, jump if clear		
			jbs	A, 5, myLabel				; (2) test bit 5 (0-15) in A, jump if set	
			jbs	(A), 5, myLabel				; (2) test bit 5 (0-15) in A, jump if set	
					
endless:	jmp endless
endprocess












