globals:
    global SchedulerFunci
externs:
    extern sscanf;
    extern printf;
    extern TargetFunci
    extern DroneFunci
    extern PrinterFunci
    extern malloc
    extern calloc   
    extern free
    extern arraydroneP;
    extern resume;
    extern do_resume;
    extern main;
    extern endmain;
    extern LFSR;
    extern arraydroneP;
    extern N;
    extern R;
    extern K;
    extern d;
    extern seed;
    extern CURR;
    extern SPT;
    extern SPMAIN;
    extern setToZeroRegs;
    extern our_printer;
   
     %macro setToZeroRegs 0
    XOR eax, eax
    XOR ebx, ebx
    XOR ecx, ecx
    XOR edx, edx
    %endmacro

    section .rodata
    formatisAlive       db "isAlive? =%d",10,0
    format_inpiniteLopp db "inipinitLoop",10,0;
    format_theWinner    db "The WinnerId is:%d",10,0;
    CODEP       equ 0       ; point to to func core
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
    Y_Targ      equ	20
    targetSize  equ 28      ; total size of target struct 

    STKSIZE     equ 16*1024
    printCosize equ 12      ;print coiroutine size
    schedCosize equ 12      ;print coiroutine size

    section .data
    stepsnuum dd 0; use for num of stek(every time one drone play its a step)
    droncount dd 0;running over drone array
    numOfCycle  dd 0; check num of cycle passed
    minScore    dd 0;
    maxInt_32   dd 1024;    
    loserDroneRound dd 0; the one with most less score after cycle
    current_dron    dd 0; 
    current_ToKill  dd 0; drone to kill current
    countFullCycle  dd 0; use for filling full cycle each time and check  if full
    termloopDroneIndx    dd 0;
    numofdroneTerminated dd 0;
    winnerCounter        dd 0;


    section .text

    SchedulerFunci:
        mov eax,0;
        mov ebx,0;
        mov ecx,0;
        mov edx,0;
        cmp dword[N], 1
            je TheWinner
             
    runOverDrone:

        mov ecx, dword[arraydroneP]
        mov edx, dword[droncount];
        mov ebx,dword[ecx+4*edx]; ebx controler, hold current drone
        mov al,byte[ebx+isAlive];debuging
        cmp byte[ebx+isAlive],1 ; if current dron not terminated
        jne notAliveDrone;
    enterResume:
        call resume;
    notAliveDrone:
        inc dword[droncount];we resume dron so we can pass to next dron
        inc dword[stepsnuum]; we use resume on a drone so he did step so we need to inc step cpunter
        inc dword[countFullCycle]; one player played we got 1 drone of the full cycle(cycle of N drone)
        ;;;;check if passed threw all drones
        mov edx,dword[droncount];
        cmp edx, dword[N]; check if we run over all the drones
        jne checkifpassFullCycle;
        mov dword[droncount],0

    checkifpassFullCycle:

        mov edx,dword[N];
        sub edx,dword[numofdroneTerminated]; edx is current active drones(cycle is for active drone, change after each elemination)
        cmp dword[countFullCycle],edx; check if pass full cycle
        jne checkSteps;
        mov dword[countFullCycle],0;
        inc dword[numOfCycle];
        
    checkSteps:

        mov edx,dword[K];
        cmp dword[stepsnuum],edx; check if dine k steps of playing drones
        jne notTimePrintCheckR;
        mov dword[stepsnuum],0
        mov ebx,[our_printer];
        call resume;

    notTimePrintCheckR:

        mov edx,dword[R];
        cmp dword[numOfCycle],edx; check if pass R rounds
        je TerminateAfterRCycle;

    checkifWinnerStayed: ;; check if we have left only one drone alive
        mov edx, dword[N]
        sub edx, dword[numofdroneTerminated];
        cmp edx,1; check if only one drone left, if yes, it wins
        je TheWinner ;
            jmp runOverDrone



    TerminateAfterRCycle:
    
        mov ebx,dword[maxInt_32];
        mov dword[minScore],ebx; score can be int , so we put max int in it , to prevent when changing it, the situation that the start value will be the lowest one;
        mov ebx,0;

    runningoverArrayforTERM:
        mov ebx,dword[arraydroneP];
        mov edx,dword[termloopDroneIndx];
        mov ecx,dword[ebx+4*edx]; keep current drone in ecx
        cmp dword[ecx+isAlive],0; if allready dead doesnt count
        je incAndCheckIfLst
        mov edx,dword[ecx+drone_score]; the score of current drone
        cmp edx,dword[minScore]; check if current score is less than minscore(need to check if R is max int and all dronesget 1 score each round than we will not have any loser, and that a problem)
        jl changingminScore;
        jmp incAndCheckIfLst; 

    changingminScore:
        mov dword[minScore],edx; edx has the curremt drone min score
        mov dword[current_ToKill],ecx; ecx is the current drone with min current score

    incAndCheckIfLst:
        inc dword[termloopDroneIndx];
        mov ebx, dword[N];
        cmp dword[termloopDroneIndx],ebx; check if reach last drone
        je TerminateDrone;
        jmp runningoverArrayforTERM;

    TerminateDrone:
        mov edx, dword[current_ToKill];
        mov byte[edx+isAlive],0; terminated current drone with the lowest score o=in the cycle;
        inc dword[numofdroneTerminated];
        mov dword[numOfCycle],0;
        mov dword[termloopDroneIndx],0;
        jmp checkifWinnerStayed;





    TheWinner:
    
        setToZeroRegs;
        mov edx,dword[arraydroneP];
        mov ecx, dword[winnerCounter];
        mov ebx, dword[edx+4*ecx];
        cmp byte[ebx+isAlive],1; check if is alive than it is the winner
        je printing_the_winner;
        
        inc dword[winnerCounter];
        jmp TheWinner; run over the loop until find the winner

    printing_the_winner:
        push dword[ebx+drone_id];
        push format_theWinner;
        call printf;
        add esp,8;
        call endmain;
