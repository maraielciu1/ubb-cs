bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; a - byte, b - word, c - double word, d - qword - Signed representation
    a db 3
    b dw 4
    c dd 5
    d dq 3
; our code starts here
segment code use32 class=code
    start:
        ; (a + b + c) - d + (b - c) = 12-3+(-1) = 8
        mov al, [a]
        cbw ; ax = a
        add ax, [b]
        
        cwde ; eax = a + b
        add eax, [c] ; eax = a+b+c
        
        cdq ; edx:eax = a+b+c
        sub eax, [d]
        sbb edx, [d+4] ; edx:eax = (a+b+c)-d
        
        mov ebx, eax
        mov ecx, edx ; ecx:ebx = (a+b+c)-d
        
        mov ax, [b]
        cwde ; eax = b
        sub eax, [c] ; eax = b-c 
        cdq ; edx:eax = b-c 
        
        add eax, ebx
        adc edx, ecx ; edx:eax = res
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
