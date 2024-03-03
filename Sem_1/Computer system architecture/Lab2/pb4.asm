bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    a db 50
    b db 3
    c db 10
    d db 7
    e db 0

; our code starts here
segment code use32 class=code
    start:
        ; (a-b-b-c)+(a-c-c-d) = 34+23=57
        mov ah, [a]
        sub ah, [b]
        sub ah, [b]
        sub ah, [c]
        
        mov bh, [a]
        sub bh, [c]
        sub bh, [c]
        sub bh, [d]
        mov byte [e], bh
        
        add ah, [e]
        
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
