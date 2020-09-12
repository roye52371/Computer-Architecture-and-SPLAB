;section	.rodata			; we define (global) read-only variables in .rodata section
	;my_print: db "Hello, Infected File",10,0	; my string
section .bss			
	descriptionFile: resb 12		
	
section .text
global _start
global system_call
global infection
global infector
global code_start
global code_end
extern main

_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller
    
code_start:
    my_print: db "Hello, Infected File",10,0	; my string, changed place to here so i can see the string in the file i attached to(because rest code is written binary)
infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state

    mov     eax, 4    ;write mode 4       
    mov     ebx, 1   ; write to stdout-1
    mov     ecx, my_print   ; string to print
    mov     edx, 21  ; string size
    int     0x80
    popad                   ; Restore caller state (registers)
    ;mov     eax, [ebp-4]    ; place returned value where caller can see it
   ; add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller
    
infector:
    push ebp
	mov ebp, esp	
	pushad			

    mov ebx, dword [ebp+8]	; get function argument (pointer to string) 
    ;mov ebx,ecx;
    mov eax, 5;
    mov ecx,2|1024;
    mov edx,0777;
    int  0x80;
    mov [descriptionFile],eax; keep the decriptorfile number for closing at end
    mov ebx, eax   ; put return value descriptor in ebx for write operate need to save it some place for close
    mov eax, 4;write value
    mov ecx,code_start;
    mov edx,code_end;
    sub edx,ecx ;
    ;mov ecx,code_start;
    int  0x80;
    mov eax,6; close value
    mov ebx,[descriptionFile];// put descriptorfile number to close
    int  0x80;
    popad;
    pop ebp;
    ret   
code_end:

