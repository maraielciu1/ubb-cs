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
    a db 0xf3 ; f3
    b dw 0xA11B ; a11b
    c dd 15AC7D9Eh
    d dq 7E8D9012AA458F41h

; our code starts here
segment code use32 class=code
    start:
        ; a-d+b+b+c = 8172 6FED 6B68 3186 result is in ecx:ebx
        
        mov al, [a]
        cbw ; ax = a
        cwde ; eax = a
        cdq ; edx:eax = a
        
        sub eax,[d]
        sbb edx, [d+4] ; edx:eax = a-d
        
        mov ebx, eax
        mov ecx,edx ; ecx:ebx = a-d
        
        mov ax,[b]
        cwde ; eax = b
        cdq ; edx:eax = b
        
        add ebx, eax
        adc ecx, edx ; ecx:ebx = a-d+b
        
        add ebx, eax
        adc ecx, edx ; ecx:ebx = a-d+b+b 
        
        mov eax, [c]
        cdq ; edx:eax = c 
        
        add ebx,eax
        adc ecx,edx ; ecx:ebx = a-d+b+b+c 
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
