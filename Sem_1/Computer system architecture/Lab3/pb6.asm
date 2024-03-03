bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ;  a,b-byte; c-word; e-doubleword; x-qword
    a db 2
    b db 4
    c dw 3
    e dd 10
    x dq 5
    f dw 128
; our code starts here
segment code use32 class=code
    start:
    ; SIGNED 
        ; (a-b+c*128)/(a+b)+e-x = 382/6 + 10 - 5 = 63+10-5 = 68
        mov al, [a]
        sub al, [b] ; al = a-b
        cbw ; ax = a-b
        cwde ; eax = a-b
        
        mov ebx, eax ; ebx = a-b
        mov ax, [c]
        imul word [f] ; dx:ax = c*128
        
        push dx
        push ax
        pop ecx ; ecx = c*128
        
        add ecx, ebx ; ecx = (a-b+c*128)
        
        mov al, [a]
        add al, [b] ; al = a+b
        
        cbw ; ax = a+b 
        
        mov bx, ax
        mov eax, ecx
        idiv bx ; ax = (a-b+c*128)/(a+b)  dx = (a-b+c*128)%(a+b)
        
        cwde ; eax = (a-b+c*128)/(a+b)
        add eax, [e] ; eax = (a-b+c*128)/(a+b)+e 
        cdq ; edx:eax = (a-b+c*128)/(a+b)+e 
        sub eax, [x]
        sbb edx,[x+4]
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
