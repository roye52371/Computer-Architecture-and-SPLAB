section .bss
resValidity: resd 1

section .data
result  db '%d', 10

section .text
global assFunc
extern c_checkValidity
extern printf

assFunc:
push ebp;
mov ebp,esp;
sub esp,4;
mov ebx,[ebp+8];
mov ecx,[ebp+12];
push ecx;
push ebx;
call c_checkValidity; call the function → push return address
mov [resValidity], eax ; retrieve return value from EAX
cmp byte[resValidity], 1; 49 is the ascci value of '1'
jne sum;
sub ebx,ecx;
jmp doAllways;

sum:
add ebx,ecx;

doAllways:
mov[ebp-4],ebx;
mov eax,[ebp-4];
push eax;
push result
call printf
mov esp,ebp;
pop ebp;
ret


