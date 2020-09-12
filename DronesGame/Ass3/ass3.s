globals:
    global main;
    global LFSR;
    global arraydroneP;
    global N;
    global R;
    global K;
    global d;
    global seed;
    global CURR;
    global SPT;
    global SPMAIN;
    global CODEP;
    global SPP;
    global STKi;
    global X_Drone;
    global Y_Drone;
    global drone_HEADING;
    global drone_speed;
    global drone_score;
    global drone_id;
    global isAlive;
    global dronSize;
    global X_Targ;
    global Y_Targ;
    global targetSize;
    global STKSIZE;
    global printCosize;
    global schedCosize;
    global our_printer;
    global resume;
    global do_resume;
    global setToZeroRegs;
    global endmain;
    global CurrTarget;
    global our_scheduler;
    global  tmp; our value of all
    global convertToBoardCord;
    global convertToHeading:    ;  0 to 360            
    global convertToinitialSpeed:  ;0 to 100            
    global convertToAddAngle:  ; -60 to 60                
    global convertToAddSpeed:   ; -10 to 10   

externs:
    extern sscanf;
    extern printf;
    extern TargetFunci
    extern DroneFunci
    extern PrinterFunci
    extern SchedulerFunci
    extern malloc
    extern calloc   
    extern free
    


;mcaros start

%macro updatingStack 0
     ;;;;;; doing usual schema of courutine
    mov dword[SPT], esp ; keeping current esp position on regular stack of memory
    mov edx, dword[ebx + CODEP]  ; put in register oour target function
    mov esp, dword[ebx + SPP] ; put in esp the current position of our spp stack pointer in target stack
    push edx; pushing our target function to memory
    pushfd;putting flags in our stack target
    pushad; putting registers in our stack target
    mov dword[ebx + SPP], esp  ; changing our spp of stack target to be  in the current position(esp ran over it and has the current position)
    mov esp, dword[SPT] ; give esp back its regular stack adrees(computer stack not the one we allocated)
    ;;;;; end of usual schema of corutine

%endmacro
%macro coordinate 1
    call LFSR
    call convertToBoardCord          ; [value] has new X coordinate
    mov edx, dword[tmp]
    mov dword[ebx + %1], edx 
    mov ecx, dword[tmp + 4]                     
    mov dword[ebx + %1 + 4], ecx ; saving both part of tmp(converted random x cordinate values) in their current place

%endmacro
%macro allocating 1
                         
    push %1             ; target size
    push 1;
    call calloc
    add esp, 8
    
%endmacro
%macro init_target 0
    setToZeroRegs; 
    ;pushad                     
    allocating targetSize             ; target size
    
    ;popad
    mov dword [CurrTarget], eax
    allocating STKSIZE 
    
    mov edx, dword[CurrTarget]
    mov dword[edx + STKi],    eax         ;keep allocatiing stack target for later use
    add eax, STKSIZE
    mov dword[edx+SPP], eax
    mov dword[edx + CODEP], TargetFunci   ;keeping the adress of the function of target

    setToZeroRegs
    mov ebx, dword[CurrTarget]
    coordinate X_Targ
    coordinate Y_Targ
    updatingStack

%endmacro

%macro init_createDrones 0
    setToZeroRegs;
    mov eax,dword[N]; puutting num of drones in eax
    mov ebx,4; giving 4 bytes to each drone adrees
    mul ebx; eax is now N*4, creating pointers array
    allocating eax
    
    mov dword[arraydroneP],eax

    %%initEachDrone:
        allocating dronSize
        
        mov edx,dword[arraydroneP];
        mov ebx,[drone_indx];
        mov dword[edx+4*ebx],eax; putting the new allocated drone inside the current indx on arraydrone
        mov ecx, dword[edx+4*ebx]; putting current drone in ecx
        mov dword[CurrDrone],ecx; saving the cur drone
        allocating STKSIZE 
        
        mov edx, dword[CurrDrone]
        mov dword[edx + STKi],    eax         ;keep allocatiing stack Drone for later use
        add eax, STKSIZE
        mov dword[edx+SPP], eax
        mov dword[edx + CODEP], DroneFunci  ;keeping the adress of the function of Drone

        setToZeroRegs
        mov ebx, dword[CurrDrone]
        coordinate X_Drone
        coordinate Y_Drone
        
        ;; init score
        setToZeroRegs
        mov ebx, dword[CurrDrone];
        mov dword[ebx+drone_score],0;

    

        ;;init isAlive
        setToZeroRegs
        mov ebx, dword[CurrDrone];
        mov byte[ebx+isAlive],1; 1 is alive, 0 is terminated

        ;; init speed
        setToZeroRegs
        call LFSR                             ; lfsr Heading
        call convertToinitialSpeed
        mov ebx, dword[CurrDrone]
        mov edx, dword[tmp]
        mov dword[ebx + drone_speed], edx 
        mov ecx, dword[tmp+4]                     
        mov dword[ebx + drone_speed+4], ecx ; saving Heading in it current place


        ;; init drone id
        setToZeroRegs
        mov ebx, dword[CurrDrone];
        inc dword[drone_indx]; we want to start id from 1, and we need to inc for loop
        mov edx, dword[drone_indx];
        mov dword[ebx+drone_id],edx; first id

        ;;init HEADING
        setToZeroRegs
        call LFSR                             ; lfsr Heading
        call convertToHeading
        mov ebx, dword[CurrDrone]
        mov edx, dword[tmp]
        mov dword[ebx + drone_HEADING], edx 
        mov ecx, dword[tmp+4]                     
        mov dword[ebx + drone_HEADING+4], ecx ; saving Heading in it current place

        updatingStack
        mov ecx,dword[drone_indx];
        cmp ecx,dword[N]; comparing indx dron to if got to N(num of drones)
        jne %%initEachDrone
%endmacro

%macro init_print 0
    setToZeroRegs;
    allocating printCosize            ; printer size
    
    mov dword[our_printer], eax
    allocating STKSIZE      ; allocating stack to printer
    
    mov edx, dword[our_printer]
    mov dword[edx + STKi],    eax         ;keep allocatiing stack printer for later use
    add eax, STKSIZE
    mov dword[edx+SPP], eax
    mov dword[edx + CODEP], PrinterFunci   ;keeping the adress of the function of printer
    mov ebx, dword[our_printer]; putting in ebx the adrress of our printer
    updatingStack

%endmacro

%macro init_Schedular 0
    setToZeroRegs;
    allocating schedCosize             ; Schedular size
    
    mov dword[our_scheduler], eax
    allocating STKSIZE 
    
    mov edx, dword[our_scheduler]
    mov dword[edx + STKi],    eax         ;keep allocatiing stack Schedular for later use
    add eax, STKSIZE
    mov dword[edx+SPP], eax
    mov dword[edx + CODEP], SchedulerFunci   ;keeping the adress of the function of Schedular
    ;;;;;;;;;;;;;;;;
    mov ebx, dword[our_scheduler]; putting in ebx the adrress of our Schedular
    updatingStack

%endmacro

%macro setToZeroRegs 0
    XOR eax, eax
    XOR ebx, ebx
    XOR ecx, ecx
    XOR edx, edx
%endmacro

%macro scanner 3 
    ;%1- variable, %2- print format,
    pushad
    push %1
    push %2
    push %3;
    call sscanf;
    add esp,12
    popad

%endmacro
%macro convertFunc 3
    pushad
    fld   %1 
    fimul %2 
    fstp  %3 
    popad
    ret
%endmacro
%macro convertSubFunc 4
    pushad
    fld   %1
    fimul %2
    fisub %3
    fstp  %4
    popad
    ret
%endmacro
%macro free1 1
    pushad
    push %1
    call free
    add esp, 4
    popad
%endmacro
;mcaros end

section .rodata
    div_100inMax16int   dt  0.0015259021896         
    div_360inMax16int   dt  0.0054932478828         
    div_120inMax16int   dt  0.0018310826276         
    div_20inMax16int    dt  0.0003051804379         
    number360 dd 360
    number100 dd 100
    number120 dd 120
    number20  dd 20 
    number10  dd  10
    number60  dd  60
    max16Int  dd  65535
    ;DroneStruct
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

section .bss
    CURR:   resd 1
    SPT:    resd 1 ; temporary stack pointer
    SPMAIN: resd 1 ; stack pointer of main
    N: resb 4;int
    R: resb 4;int
    K: resb 4;int
    d: resb 8;float
    seed: resb 4;int
    lfsr: resb 2; reserve 16 bits
    bit:  resb 2; reserve 16 bits
    tmp:  resb 8; temporary value

section .data
    our_scheduler:  dd 0
    drone_indx:     dd 0
    format_int:     db "%d",0
    format_float:   db "%lf",0
    our_printer:    dd 0
    CurrTarget:     dd 0
    arraydroneP:    dd 0
    CurrDrone:      dd 0   
    counter:        dd 0

section .text

main:
    push ebp                           
    mov ebp, esp 


    ;;getting and setting the input from the user 
    mov ecx, dword[ebp+12];pointer to first arguemnet of argv*

    ;mov ebx,dword[ecx+4];first argument of argv
    scanner N, format_int, dword[ecx+4]
   
    scanner R, format_int, dword[ecx+8]
    
    scanner K, format_int, dword[ecx+12]

    scanner d, format_float, dword[ecx+16]
 
    scanner seed, format_int, dword[ecx+20]


;;initial all
initializing_coRoutines:
    finit ; afterward using function of x87
initializing_Target:
    init_target;
initAndCreationDrones:
    init_createDrones;
initPrint:
    init_print;
initSched:
    init_Schedular;

;; taking from the tirgul of co routines
startCo:
    pushad ; save registers of main ()
    mov [SPMAIN], ESP ; save ESP of main ()
    mov EBX,dword[our_scheduler];
    jmp do_resume ; resume a scheduler co-routine

resume: ; save state of current co-routine
    pushfd
    pushad
    mov EDX, [CURR]
    mov [EDX+SPP], ESP ; save current ESP

do_resume: ; load ESP for resumed co-routine
    mov ESP, [EBX+SPP]
    mov [CURR], EBX
    popad ; restore resumed co-routine state
    popfd
    ret ; "return" to resumed co-routine


endmain:
    mov esp,[SPMAIN]; or SPT need to check
    call allocateddelete;
    mov esp, ebp
    pop  ebp                                                         
    ret; 

LFSR:
;number generator
    pushad
     ;mov dword[counter], 0
    loop:
        
        mov ecx,[seed]
        mov [lfsr],ecx; put seed inside lfsr

        ;begindoWhile:
        mov ax,[lfsr];
        mov bx,[lfsr];
        mov cx,[lfsr];
        mov dx,[lfsr];

        shr ax, 0    ; shr lfsr 0
        shr bx, 2    ; shr lfsr 2
        shr cx, 3    ; shr lfsr 3
        shr dx, 5    ; shr lfsr 5

        XOR ax, bx;
        XOR ax, cx;
        XOR ax, dx;
        mov [bit],ax;

        mov ax,[lfsr];
        shr ax,1
        shl word[bit],15
        OR ax,[bit]
        mov [seed],ax ; now seed hold the psuedo random value

    ;    inc dword[counter]
   ;     cmp dword[counter], 16
  ;          jne loop
    popad
    ret

;end LFSR


convertToHeading:    ;  0 to 360
    convertFunc tword[div_360inMax16int], dword [seed], qword [tmp]      
    

convertToBoardCord:        ; 0 to 100   
    convertFunc tword[div_100inMax16int], dword [seed], qword [tmp]      


convertToinitialSpeed:  ;0 to 100
    convertFunc tword[div_100inMax16int], dword [seed], qword [tmp]      

convertToAddAngle:  ; -60 to 60
    convertSubFunc tword[div_120inMax16int], dword [seed], dword [number60], qword [tmp]      

convertToAddSpeed:   ; -10 to 10
    convertSubFunc tword[div_20inMax16int], dword [seed], dword [number10], qword [tmp]      

    allocateddelete:
        ;;free target
        mov edx, dword[CurrTarget]

        ;;free1
        
        free1 dword[edx + STKi]
        
        ;;free2
        free1 edx
        
        ;;free printer
        mov edx, dword[our_printer]
        ;;free1
        free1 dword[edx + STKi]
   
        ;;free2
        free1 edx

        ;;free scheduler
        mov edx, dword[our_scheduler]

        ;;free1
        free1 dword[edx + STKi]

        ;;free2
        free1 edx

        ;;free drones
        mov ecx, 0;counter till [N]
        freedrones:
            mov ebx, dword[arraydroneP]
            mov ebx, dword[ebx + 4 * ecx]
            ;;free1
            free1 dword[ebx + STKi]

            ;;free2
            free1 ebx
            inc ecx;
            cmp ecx, dword[N]
                jne freedrones

            ;;last free
            free1 dword[arraydroneP]; free the array itself            
            ret


