bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ;  a-doubleword; b,c,d-byte; x-qword
    a dd 10
    b db 5
    c db 4
    d db 2
    x dq 43

; our code starts here
segment code use32 class=code
    start:
    ; SIGNED
        ; a-(7+x)/(b*b-c/d+2) = 10 - 50/(25-2+2) = 8
        
        mov al, [b]
        imul byte [b] ; ax=b*b
        
        mov bx,ax ; bx = b*b
        mov al,[c]
        cbw ; ax = c
        idiv byte [d] ; al = c/d
        
        cbw ; ax = c/d 
        sub bx,ax ; bx = b*b-c/d
        add bx, 2 ; bx = b*b-c/d+2
        mov ax,bx
        cwde ; eax = b*b-c/d+2
        mov ebx, eax
        
        mov eax, [x]
        mov edx, [x+4]
        
        add eax, 7 
        adc edx,0 ; edx:eax = x+7
        
        idiv ebx ; eax=(7+x)/(b*b-c/d+2) edx=(7+x)%(b*b-c/d+2)
        
        mov ebx,eax
        mov eax, [a]
        sub eax,ebx ; eax=a-(7+x)/(b*b-c/d+2)
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
