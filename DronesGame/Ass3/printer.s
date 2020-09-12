
%macro setToZeroRegs 0
    XOR eax, eax
    XOR ebx, ebx
    XOR ecx, ecx
    XOR edx, edx
    %endmacro
;cor+4, cor,format
%macro printcordinateXY 3 
    pushad
    push %1
    push %2
    push %3;
    call printf;
    add esp,12;
    popad
%endmacro

%macro print2arguemets 2 
    pushad
    push %1
    push %2
    call printf;
    add esp,8;
    popad
%endmacro

%macro print1arguemets 1 
    pushad
    push %1
    call printf;
    add esp,4;
    popad
%endmacro



globals:
  global PrinterFunci;
   
externs:
    extern main;
    extern LFSR;
    extern arraydroneP;
    extern N;
    extern R;
    extern K;
    extern d;
    extern resume;
    extern do_resume;
    extern endmain;
    extern CurrTarget;
    extern our_scheduler;
    extern  tmp; our value of all
    extern convertToBoardCord;
    extern printf;


section .rodata
format_cordinateX: db "%.2f, ",0
format_cordinateY: db "%.2f",0
newline: db "",10,0
format_decimal: db "%d, ",0
format_decimalNonPsik: db "%d",0
CODEP      equ 0       ; point to to func core
SPP	        equ	4	    ; point to stack core
STKi        equ 8       ; saving the allocated stack core
X_Drone	    equ	12	    ; point to X_dron  
Y_Drone     equ	20	    ; point to Y_dron 
drone_HEADING     equ 28      ; point to HEADING of the drone
drone_speed       equ 36      ; point to speed of the drone
drone_score       equ 44      ; point to score of the drone
drone_id          equ 48      ; point to id of the drone
isAlive     equ 52      ; check if drone is alive or terminated(stil in the game)
dronSize    equ 53      ; size struct

;TargetStruct
;CODEP      equ 0       ; point to to func core
;SPP	     equ 4	    ; point to stack core
;STKi        equ 8       ; saving the allocated stack core
X_Targ	    equ	12	    ; offset of pointer to co-routine X value  
Y_Targ     equ	20
targetSize  equ 28      ; total size of target struct 

STKSIZE     equ 16*1024
printCosize equ 12      ;print coiroutine size
schedCosize equ 12      ;print coiroutine size

section .data
Counter dd 0;

section .text

PrinterFunci:
;;;print target
mov ecx, dword[CurrTarget];
printcordinateXY dword[ecx+X_Targ+4],dword[ecx+X_Targ],format_cordinateX
printcordinateXY dword[ecx+Y_Targ+4],dword[ecx+Y_Targ],format_cordinateY
print1arguemets newline;
;;;end print target
mov eax, 0
printallLivingDrones:
    mov edx,dword[arraydroneP];
    mov ebx, dword[edx+4*eax];

    cmp dword[ebx+isAlive],1; check if is alive than it is the winner
    je printing;
    jmp incAndcheckiflast

    printing: ;ebx is holding the current drone
        ;; id
        print2arguemets dword[ebx+drone_id],format_decimal
        ;; cordinates x,y
        printcordinateXY dword[ebx+X_Drone+4],dword[ebx+X_Drone],format_cordinateX
        printcordinateXY dword[ebx+Y_Drone+4],dword[ebx+Y_Drone],format_cordinateX ; beacuase in format Y there is o psik; and it is most of the same
        ;;heading
        printcordinateXY dword[ebx+drone_HEADING+4],dword[ebx+drone_HEADING],format_cordinateX
        ;;speed
        printcordinateXY dword[ebx+drone_speed+4],dword[ebx+drone_speed],format_cordinateX ; beacuase in format Y there is o psik; and it is most of the same
        ;;score
        print2arguemets dword[ebx+drone_score],format_decimalNonPsik
        ;;new line
        print1arguemets newline;


    incAndcheckiflast:
    inc eax
    cmp eax, dword[N]; check if reach the end
    jne printallLivingDrones;
    ; else ,reached the end


finishloop:
    mov ebx, [our_scheduler]
    call resume
    jmp PrinterFunci






    