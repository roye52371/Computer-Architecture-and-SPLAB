section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%s", 10, 0	; format string

section .bss			; we define (global) uninitialized variables in .bss section
	an: resb 12		; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]

section .text
	global convertor
	extern printf

convertor:
	push ebp
	mov ebp, esp	
	pushad			

    mov ecx, dword [ebp+8]	; get function argument (pointer to string)
    mov ebx, 10
    mov eax, 0


  

startLoop:
    ;turning String to Decimal integer(into eax)
    cmp byte[ecx],0;0x00 is null terminator in ascii
    je endLoop
    cmp byte[ecx],10
    je endLoop
    cmp0:
    cmp byte[ecx],48; cmp to 0
    jne cmp1
    add0:
    add eax,0
    jmp otheroperate
    cmp1:
    cmp byte[ecx],49; cmp to 1
    jne cmp2
    add1:
    add eax,1
    jmp otheroperate;
    cmp2:
    cmp byte[ecx],50
    jne cmp3
    add2:
    add eax,2;
    jmp otheroperate
    cmp3:
    cmp byte[ecx],51
    jne cmp4
    add3:
    add eax,3;
    jmp otheroperate
    cmp4:
    cmp byte[ecx],52
    jne cmp5
    add4:
    add eax,4;
    jmp otheroperate
    cmp5:
    cmp byte[ecx],53
    jne cmp6
    add5:
    add eax,5;
    jmp otheroperate
    cmp6:
    cmp byte[ecx],54
    jne cmp7
    add6:
    add eax,6;
    jmp otheroperate
    cmp7:
    cmp byte[ecx],55
    jne cmp8
    add7:
    add eax,7;
    jmp otheroperate
    cmp8:
    cmp byte[ecx],56
    jne cmp9
    add8:
    add eax,8;
    jmp otheroperate
    cmp9:
    cmp byte[ecx],57;
    jne endLoop; if not 0,1,2,3,4,5,6,7,8,9 then not valid number
    add9:
    add eax,9;
    jmp otheroperate
    otheroperate:
    cmp byte[ecx+ 1],0
    je endLoop
    cmp byte[ecx + 1], 10
    je endLoop
    mul ebx
    inc  ecx
    jmp startLoop
    endLoop:
    
    mov ebx, 16
    mov ecx,0; need counter find other register
    cmp eax, 0
    je iam0
    
    converthexaLOOP:

    cmp eax,0;// if the value is zero finished converting
    je endHexaLoop;
    mov edx, 0
    div ebx;
    push edx; pushing the reminder to the stack to prevent opposite direction to convert
    inc ecx;
    jmp converthexaLOOP
    endHexaLoop:
    
    
    mov ebx,0;
    poploop:
    cmp ecx,0;
    je endpooploop
    pop edx;
    cmp edx,9;
    jng restoperate
    add edx,7; so if it bigger than 9 to make it A B C in order
    restoperate:
    add edx,48;
    mov [an+ebx],edx;
    inc ebx;
    dec ecx;
    jmp poploop;
    endpooploop:
    jmp print
    

    iam0:
    add eax, 48
    mov [an], eax


    print:

	push an		; call printf with 2 arguments -  
	push format_string	; pointer to str and pointer to format string
	call printf
	add esp, 8		; clean up stack after call
	popad			
	mov esp, ebp	
	pop ebp
	ret
