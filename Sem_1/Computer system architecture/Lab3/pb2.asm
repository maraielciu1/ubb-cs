bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    a db 2
    b dw 4
    c dd 5
    d dq 10

; our code starts here
segment code use32 class=code
    start:
        ; (d+d-b)+(c-a)+d
        
        ; signed
        
        mov eax, [d]
        mov edx, [d+4]
        
        add eax, [d]
        adc edx, [d+4]
        
        mov ebx, eax
        mov ecx, edx
        
        mov ax, [b]
        cwde ; eax = b
        cdq ; edx:eax = b
        
        sub ebx, eax
        sbb ecx, edx ; ecx:ebx = (d+d-b)
        
        mov al, [a]
        cbw ; ax = a
        cwde ; eax = a
        mov edx, [c]
        sub edx, eax ; edx = c-a
        
        mov eax, edx
        cdq ; edx:eax = c-a
        
        add ebx, eax
        adc ecx, edx ; ecx:ebx = (d+d-b) + (c-a)
        
        add ebx, [d]
        adc ecx, [d+4] ; ecx:ebx = (d+d-b) + (c-a) + d
        
        
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
