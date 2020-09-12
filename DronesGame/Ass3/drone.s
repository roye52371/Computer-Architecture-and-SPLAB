globals:
    global DroneFunci;

externs:
    extern main;
    extern LFSR;
    extern arraydroneP;
    extern N;
    extern R;
    extern K;
    extern d;
    extern seed;
    extern CURR;
    extern resume;
    extern do_resume;
    extern setToZeroRegs;
    extern endmain;
    extern CurrTarget;
    extern our_scheduler;
    extern tmp; our value of all
    extern CURR
    extern convertToHeading;   ;  0 to 360        
    extern convertToBoardCord;  0 to 100     
    extern convertToinitialSpeed; 0 to 100            
    extern convertToAddAngle;  -60 to 60                
    extern convertToAddSpeed;  -10 to 10   
    extern printf

    ;;;;;;;;macros
    %macro convertToRadian 0
        mov ecx,0
        mov ecx,dword[CURR]; CURR holding the current drone (gaved in do_resume), ecx is now holding the current drone
        fild dword[ecx+drone_HEADING];loading current heading into x87 memory st 0
        fldpi; loading pi to st0, know heading now in st 1
        fmulp; st0 * st1
        fild dword[number180val]; loading 180 to sto
        fdivp; st1 div 180
    %endmacro

    %macro computeDistanceFromTarget 0
        ;;;current drone in ecx
        mov ebx,dword[CurrTarget];
        fld qword[ecx+X_Drone];
        fld qword[ebx+X_Targ];
        fsubp; xtarg-xdrone
        fld ST0; distance 
        fmulp; fistancex*distacex sqr

        fld qword[ecx+Y_Drone];
        fld qword[ebx+Y_Targ];
        fsubp; ytarg-ydrone
        fld ST0; distancey 
        fmulp; fistancey*distacey sqr

        faddp;distancey*distacey+ fistancex*distacex
        fsqrt; shoresh of the above number is the distance

    %endmacro

    ;;;;;end macros
    
section .rodata
    number180val: dd 180
    number100val: dd 100
    number360val: dd 360
    number20val   dd 20
    number10val   dd 10
    number0val    dd 0
    

section .data
    
    adding_Angle dq 0
    adding_Speed dq 0
    cleanx87Stack dq 0;
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


section .text

    mayDestroy:;; function
        computeDistanceFromTarget;
        fld qword[d]; fld command line distance(prevent new vairiable)
        fcomip ST0,ST1;
        jae enoughToTerminate;
        notEnough:
            mov eax,0; we cant kill we keep in eax wich return in function the false boolean val
            jmp knowReturn;
        enoughToTerminate:
            mov eax,1;we can kill we keep in eax wich return in function the true boolean val
        knowReturn:
            ret; eax allways is ret value holder

    ;;;; end of mayDestroy function
    DroneFunci:
        finit;
        call LFSR; ranodmized number
        call convertToAddAngle; convert the number into angle
        mov ecx,dword[tmp];
        mov edx,dword[tmp+4];
        mov dword[adding_Angle],ecx; putting first part in angle
        mov dword[adding_Angle+4],edx; putting sec part in angle+4
        call LFSR; ranodmized number
        call convertToAddSpeed; convert the number into angle
        mov ecx,dword[tmp];
        mov edx,dword[tmp+4];
        mov dword[adding_Speed],ecx; putting first part in angle
        mov dword[adding_Speed+4],edx; putting sec part in angle+4

       

        computelocation:
            
            ;;;first X
            convertToRadian;
             
            computingXcordinate:
                fcos; doing cos because computing x, and doing it with the radian of heading computed above
                fmul qword[ecx+drone_speed]; cos mul current dron speed
                fld qword[ecx+X_Drone];
                faddp; xdron plus    cos muls speed
                mov eax,0
                mov eax,100
                fild dword[number100val];
                fcomip ST0,ST1; comparing 100 to (xdron plus    cos muls speed)
                jb outof100BoundsX;jump if greater than 100(100 less than it)
                mov eax,0
                fild dword[number0val];
                pushfd
                fcomip ST0,ST1; comparing 0 to (xdron plus    cos muls speed)
                ja outof0BoundsX;jump if less thn 0(0 greater than it)
                jmp inBoundsX;
            outof100BoundsX:
            
                mov eax,0
                mov eax,100
                fisub dword[number100val]; sub 100 back to legal bound
                jmp inBoundsX;
            outof0BoundsX:
                
                mov eax,0
                mov eax,100
                fiadd dword[number100val]; add 100 back to legal bound
            inBoundsX:
                fstp qword[ecx+X_Drone]; putting the legal cordinate in X

                ;;;; now Y
                convertToRadian;
            computingYcordinate:
             
                fsin; doing sin because computing Y, and doing it with the radian of heading computed above
                fmul qword[ecx+drone_speed]; sin mul current dron speed
                fld qword[ecx+Y_Drone];
                faddp; ydron plus    sin muls speed
                mov eax,0
                mov eax,100
                fild dword[number100val];
                fcomip ST0,ST1; comparing 100 to (ydron plus    sin muls speed)
                jb outof100BoundsY;jump if greater than 100(100 less than it)
                mov eax,0
                fild dword[number0val];
                fcomip ST0,ST1; comparing 0 to (ydron plus    sin muls speed)
                ja outof0BoundsY;jump if less thn 0 (0 greater than it)
                jmp inBoundsY;
            outof100BoundsY:
                mov eax,0
                mov eax,100
                fisub dword[number100val]; sub 100 back to legal bound
                jmp inBoundsY;
            outof0BoundsY:
                mov eax,0
                mov eax,100
                fiadd dword[number100val]; add 100 back to legal bound
            inBoundsY:
                fstp qword[ecx+Y_Drone]; putting the legal cordinate in Y

           

            addingAngleTOHeadingAndKeepBound:
             
                fld qword[ecx+drone_HEADING];
                fld qword[adding_Angle];
                faddp; ydron plus    sin muls speed
                mov eax,0
                mov eax,360
                fild dword[number360val];
                fcomip ST0,ST1; comparing 360 to (ydron plus    sin muls speed)
                jb outof360BoundsH;jump if greater than 360(360 less than it)
                mov eax,0
                fild dword[number0val];
                fcomip ST0,ST1; comparing 0 to (ydron plus    sin muls speed)
                ja outof0BoundsH;jump if less thn 0 faddp; ydron plus    sin muls speed
                jmp inBoundsH;
            outof360BoundsH:
                mov eax,0
                mov eax,360
                fisub dword[number360val]; sub 100 back to legal bound
                jmp inBoundsH;
            outof0BoundsH:
                mov eax,0
                mov eax,360
                fiadd dword[number360val]; add 100 back to legal bound
            inBoundsH:
                fstp qword[ecx+drone_HEADING]; putting the legal cordinate in Y
            
            addingSpeedAndKeepBound:
                fld qword[ecx+drone_speed];
                fld qword[adding_Speed];
                faddp; ydron plus    sin muls speed
                mov eax,0
                mov eax,100
                fild dword[number100val];
                fcomip ST0,ST1; comparing 100 to (
                jb outof100BoundsS;jump if greater than 100(100 less than it)
                mov eax,0
                fild dword[number0val];
                fcomip ST0,ST1; comparing 0 to (ydron plus    sin muls speed)
                ja outof0BoundsS;jump if less thn 0 faddp; ydron plus    sin muls speed
                jmp inBoundsS;
            outof100BoundsS:
                mov eax,0
                mov eax,100
                fild dword[number100val]; we need to cut the extras and put 100, CHECK maybe we need to pop before the arguements in x87 stack
                jmp inBoundsS;
            outof0BoundsS:
                mov eax,0
                fild dword[number0val];we need to cut the extras and put 0
            inBoundsS:
                fstp qword[ecx+drone_speed]; putting the legal cordinate in Y

                fstp qword[cleanx87Stack]; maybe we need to clean stack , check if needed

                call mayDestroy;
                cmp eax,1;
                je ableToTerminate;

            notableToTerminate:
                mov ebx, dword[our_scheduler];
                call resume;
                jmp DroneFunci;

            ableToTerminate:
                inc dword[ecx+drone_score]; we killed target ,current drone get 1 score more
                mov ebx, dword[CurrTarget];
                call resume;
                jmp DroneFunci;



