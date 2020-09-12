    globals:
    global TargetFunci;
    externs:
    extern CurrTarget;
    extern our_scheduler;
    extern tmp; our value of  all(in our case cordinate)
    extern LFSR
    extern convertToBoardCord;
    extern resume;
    extern do_resume;

    section .rodata
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
%macro coordinate 1
    call LFSR
    call convertToBoardCord          ; [value] has new X coordinate
    mov edx, dword[tmp]
    mov dword[ebx + %1], edx 
    mov ecx, dword[tmp + 4]                     
    mov dword[ebx + %1 + 4], ecx ; saving both part of tmp(converted random x cordinate values) in their current place

%endmacro
    section .text

    TargetFunci:
        recreationOfTarg: ; taking fron our ass3 target init
            mov ebx, dword[CurrTarget]
            coordinate X_Targ
            coordinate Y_Targ
   
        afterRecreation:
            mov ebx, dword[our_scheduler]
            call resume
            jmp TargetFunci;        

    
        