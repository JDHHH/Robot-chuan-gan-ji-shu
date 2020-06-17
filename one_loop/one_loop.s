	AREA STACK,NOINIT,READWRITE,ALIGN=3
		
Stack_Mem SPACE 0x400
	
__initial_sp
	AREA RESET,DATA,READONLY

__Vectors
	DCD __initial_sp
	DCD Reset_handler
	
	AREA |.text|,CODE,READONLY
	THUMB
	REQUIRE8
	PRESERVE8
	ENTRY

Reset_handler
	mov		r0,#0
	mov		r1,#10

loop
	add		r0,r1
	subs	r1,#1
	bne		loop
	
	b		.
	end
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	