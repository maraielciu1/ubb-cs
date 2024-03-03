bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; a - byte, b - word, c - double word, d - qword - Unsigned representation
    a db 2
    b dw 4
    c dd 5
    d dq 7

; our code starts here
segment code use32 class=code
    start:
        ; ((a + b) + (a + c) + (b + c)) - d = (6+7+9) - 7 = 15
        mov eax, 0
        mov al, [a]
        mov ah,0 ; ax=a
        add ax,[b] ; ax=a+b    => eax = a+b
        
        mov ecx, [c]
        mov ebx,0
        mov bl, [a] ; ebx = a
        
        add ecx, ebx ; ecx = a+c
        
        mov ebx, 0
        mov bl, [b] ; ebx = b
        add ebx, [c] ; ebx = b+c
        
        add eax, ecx ; eax = (a + b) + (a + c)
        add eax, ebx ; eax = (a + b) + (a + c) + (b + c)
        
        mov edx, 0 ; edx:eax = (a + b) + (a + c) + (b + c)
        
        sub eax,[d]
        sbb edx, [d+4]  ; edx:eax = (a + b) + (a + c) + (b + c) - d
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
