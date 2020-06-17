GPIOA_BSRR	EQU	0x40010810
GPIOA_BRR	EQU	0x40010814
RCC_APB2ENR	EQU	0x40021018
GPIOA_CRL	EQU	0x40010800

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
	bl	led_init
main_loop
	bl	led_on
	bl	delay
	bl	led_off
	bl	delay
	b	main_loop

;;;;;;;;;;;led_init
led_init
	push {r0,r1,lr}
	LDR		r0,=RCC_APB2ENR
	ORR		r0,r0,#0x04
	LDR		r1,=RCC_APB2ENR
	STR		r0,[r1]
	
	LDR		r0,=GPIOA_CRL
	BIC		r0,r0,#0x0F0000
	LDR		r1,=GPIOA_CRL
	STR		r0,[r1]
	
	LDR		r0,=GPIOA_CRL
	ORR		r0,r0,#0x030000
	LDR		r1,=GPIOA_CRL
	STR		r0,[r1]
	
	pop		{r0,r1,pc}

led_on
	push 	{r0,r1,lr}
	LDR		r0,=GPIOA_BSRR
	ORR		r0,r0,#0x10
	LDR		r1,=GPIOA_BSRR
	STR		r0,[r1]
	pop		{r0,r1,pc}
	
led_off
	push 	{r0,r1,lr}
	LDR		r0,=GPIOA_BRR
	ORR		r0,r0,#0x10
	LDR		r1,=GPIOA_BRR
	STR		r0,[r1]
	pop		{r0,r1,pc}


;;;;;;;delay
delay
	push	{r0,r1,lr}
	mov		r0,#1000
loop1
	subs	r0,#1
	blne	delay1
	bne		loop1
	pop		{r0,r1,pc}
	
delay1
	;push	{lr}
	mov		r1,#1000
loop2
	subs	r1,#1
	bne		loop2
	msr		xPSR,r1
	
	bx		lr	;pop	{pc}
	
	END
